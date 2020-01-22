/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, init, wait1, start1, next1, temp1, wait2, start2, next2, temp2} state;
	
void tick(){
	switch(state) {
		case start:
			state = init;
		break;
		
		case init:
			state = wait1;
			PORTB = 0x00;
		break;
		
		case wait1:
			if (!PINA & 0x01) {
				state = wait1;
			} else {
				if (PORTB == 0) {
					state = start1;
					PORTB = 0x01;
				} else if (PORTB < 32) {
					state = next1;
					PORTB = (PORTB << 1) | 0x01;
				} else {
					state = temp1;
					PORTB = 0;
				}
			}
		break;
		
		case start1:
			if (PINA & 0x01) {
				state = start1;
			} else {
				state = wait1;
			}
		break;
		
		case next1:
			if (PINA & 0x01) {
				state = next1;
				} else {
				state = wait1;
			}
		break;
		
		case temp1:
		if (PINA & 0x01) {
			state = temp1;
			} else {
			state = wait2;
		}
		break;
		
		case wait2:
			if (!PINA & 0x01) {
				state = wait2;
			} else {
				if (PORTB == 0) {
					state = start2;
					PORTB = 0x20;
				} else if (PORTB < 63) {
					state = next2;
					PORTB = (PORTB >> 1) | 0x20;
				} else {
					state = temp2;
					PORTB = 0;
				}
			}
		break;
		
		case start2:
		if (PINA & 0x01) {
			state = start2;
			} else {
			state = wait2;
		}
		break;
		
		case next2:
		if (PINA & 0x01) {
			state = next2;
			} else {
			state = wait2;
		}
		break;
		
		case temp2:
		if (PINA & 0x01) {
			state = temp2;
			} else {
			state = wait1;
		}
		break;
		
		default:
			state = start;
			PORTB = 0x00;
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	PORTB = 0x00;
	state = start;
    /* Insert your solution below */
	
    while (1) {
		tick();
    }
    return 1;
}
