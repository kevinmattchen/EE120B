/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #6  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char count = 0;

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

enum States {start, init, blink1, wait1, stay1, blink2, wait2, stay2, blink3, wait3, stay3} state;
	
void tick() {
	switch(state) {
		case start:
			state = init;
		break;
		
		case init:
			if (PINA & 0x01) {
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
			if (PINA & 0x01) {
				state = wait1;
				count = 0;
				break;
			}
		break;
		
		case wait1:
			if (PINA & 0x01) {
				state = wait1;
			} else {
				state = stay1;
			}
		break;
		
		case stay1:
			if (PINA & 0X01) {
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
			if (PINA & 0x01) {
				state = wait2;
				count = 0;
				break;
			}
		break;
		
		case wait2:
		if (PINA & 0x01) {
			state = wait2;
			} else {
			state = stay2;
		}
		break;
		
		case stay2:
		if (PINA & 0X01) {
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
			if (PINA & 0x01) {
				state = wait3;
				count = 0;
				break;
			}
		break;
		
		case wait3:
		if (PINA & 0x01) {
			state = wait3;
			} else {
			state = stay3;
		}
		break;
		
		case stay3:
		if (PINA & 0X01) {
			state = init;
			} else {
			state = stay3;
		}
		break;
		
		default:
			state = start;
		break;
	}
	
	switch(state) {
		case start:
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
		
		default:
			PORTB = 0x07;
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	state = start;
    /* Insert your solution below */
	TimerSet(10);
	TimerOn();
	
    while (1) {
		tick();
		while(!TimerFlag) {}
		TimerFlag = 0;
    }
    return 1;
}
