/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #7  Exercise 2
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

const unsigned char* winText = "You Won!";
const unsigned char* scoreText = "Score: 0";

unsigned char button;

unsigned char count = 0;
unsigned char val;

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

enum States {start, init, blink1, wait1, stay1, blink2, wait2, stay2, blink3, wait3, stay3, victoryWait, victory} state;
	
void tick() {
	button = PINA & 0x01;
	switch(state) {
		case start:
			state = init;
			LCD_ClearScreen();
			LCD_DisplayString(1, scoreText);
		break;
		
		case init:
			if (button) {
				state = init;
			} else {
				state = blink1;
			}
		break;
		
		case blink1:
			if (count < 30) {
				state = blink1;
				count++;
			} else {
				state = blink2;
				count = 0;
			}
			if (button) {
				state = wait1;
				count = 0;
				if (val > 0) {
					val--;
				}
				LCD_Cursor(8);
				LCD_WriteData(val + '0');
				break;
			}
		break;
		
		case wait1:
			if (button) {
				state = wait1;
			} else {
				state = stay1;
			}
		break;
		
		case stay1:
			if (button) {
				state = init;
			} else {
				state = stay1;
			}
		break;
		
		case blink2:
			if (count < 30) {
				state = blink2;
				count++;
			} else {
				state = blink3;
				count = 0;
			}
			if (button) {
				state = wait2;
				count = 0;
				if (val < 8) {
					val++;
					LCD_Cursor(8);
					LCD_WriteData(val + '0');
				} else {
					state = victoryWait;
					LCD_ClearScreen();
					LCD_DisplayString(1, winText);
				}
				break;
			}
		break;
		
		case wait2:
		if (button) {
			state = wait2;
			} else {
			state = stay2;
		}
		break;
		
		case stay2:
		if (button) {
			state = init;
			} else {
			state = stay2;
		}
		break;
		
		case blink3:
			if (count < 30) {
				state = blink3;
				count++;
			} else {
				state = blink1;
				count = 0;
			}
			if (button) {
				state = wait3;
				count = 0;
				if (val > 0) {
					val--;
					LCD_Cursor(8);
					LCD_WriteData(val + '0');
				}
				break;
			}
		break;
		
		case wait3:
		if (button) {
			state = wait3;
			} else {
			state = stay3;
		}
		break;
		
		case stay3:
		if (button) {
			state = init;
			} else {
			state = stay3;
		}
		break;
		
		case victoryWait:
			if (button) {
				state = victoryWait;
			} else {
				state = victory;
			}
		break;
		
		case victory:
			if (button) {
				state = init;
				LCD_ClearScreen();
				LCD_DisplayString(1, scoreText);
				val = 0;
			} else {
				state = victory;
			}
		break;
		
		default:
			state = start;
		break;
	}
	
	switch(state) {
		case start:
			val = 0;
		break;
		
		case init:
			PORTB = 0x00;
		break;
		
		case blink1:
			PORTB = 0x01;
		break;
		
		case wait1:
			PORTB = 0x01;
		break;
		
		case stay1:
			PORTB = 0x01;
		break;
		
		case blink2:
			PORTB = 0x02;
		break;
		
		case wait2:
		PORTB = 0x02;
		break;
		
		case stay2:
		PORTB = 0x02;
		break;
		
		case blink3:
			PORTB = 0x04;
		break;
		
		case wait3:
		PORTB = 0x04;
		break;
		
		case stay3:
		PORTB = 0x04;
		break;
		
		case victoryWait:
		break;
		
		case victory:
		break;
		
		default:
			PORTB = 0x07;
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	state = start;
    /* Insert your solution below */
	TimerSet(10);
	TimerOn();
	
	LCD_init();
	
    while (1) {
		tick();
		while(!TimerFlag) {}
		TimerFlag = 0;
    }
    return 1;
}
