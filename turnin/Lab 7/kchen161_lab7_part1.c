/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #7  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char* text;

unsigned char count = 0;
unsigned char val = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

	enum States {start, init, wait, inc, dec, reset} state;
		
	unsigned char buttons;

    void tick() {
		
	buttons = PINA & 0x03;
	
	switch (state) {
	    case start:
		state = init;
	    break;

	    case init:
		state = wait;
	    break;
	
	    case wait:
		count = 0;
		switch (buttons) {
		    case 0:
			state = wait;
		    break;

		    case 1:
			state = inc;
			if (val < 9) {
				val++;
			}
		    break;

		    case 2:
			state = dec;
			if (val > 0) {
				val--;
			}
		    break;

 		    case 3:
			state = reset;
		    break;
		
		    default:
		  	state = start;
		    break;
		}
	    break;

	    case inc:
		switch (buttons) {
		    case 0:
			state = wait;
		    break;

		    case 1:
				if (count < 9) {
					state = inc;
					count++;
				} else {
					state = inc;
					if (val < 9) {
						val++;
					}
					count = 0;
				}
		    break;

		    case 2:
			state = dec;
		    break;

		    case 3:
			state = reset;
		    break;

		    default:
			state = start;
		    break;
		}
	    break;
	    
	    case dec:
		switch (buttons) {
		    case 0:
			state = wait;
		    break;
		
		    case 1:
			state = inc;
		    break;
		
		    case 2:
				if (count < 9) {
					state = dec;
					count++;
				} else {
					state = dec;
					if (val > 0) {
						val--;
					}
					count = 0;
				}
		    break;
	
		    case 3:
		  	state = reset;
		    break;
	 	    
		    default:
			state = start;
		    break;
		}
	    break;

	    case reset:
			switch (buttons) {
				case 0:
					state = wait;
				break;
				
				default:
					state = reset;
				break;
			}
	    break;

	    default:
		state = start;
	    break;
	}

	switch (state) {
	    case start:
	    break;

	    case init:
		val = 0;
	    break;

	    case wait:
	    break;

	    case inc:
	    break;

	    case dec:
	    break;

	    case reset:
		val = 0;
  	    break;

	    default:
		val = 0;
	    break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0X00;
    /* Insert your solution below */
	
	val = 0x00;
	state = start;
	TimerSet(100);
	TimerOn();
	
	LCD_init();

    while (1) {
		tick();
		LCD_Cursor(1);
		LCD_WriteData(val + '0');
		while(!TimerFlag){}
		TimerFlag = 0;
    }
    return 1;
}
