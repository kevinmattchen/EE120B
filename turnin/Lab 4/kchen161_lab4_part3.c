/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

	enum States {start, init, locked, first, unlocked} state;

    void tick() {
	switch (state) {
	    case start:
		state = init;
	    break;

	    case init:
		state = locked;
	    break;

	    case locked:
		if (PINA & 0x07 == 0x04) {
		    state = first;
		} else {
		    state = locked;
		}
	    break;

	    case first:
		if (PINA & 0x07 == 0x02) {
		    state = unlocked;
		} else {
		    state = locked;
		}
	    break;

	    case unlocked:
		if (PINA & 0x80 == 0x80) {
		    state = locked;
		} else {
		    state = unlocked;
		}
	    break;

	    default:
		state = locked;
	    break;
	}

	switch (state) {
	    case start:
	    break;

	    case init:
		PORTB = 0x00;
	    break;

	    case locked:
		PORTB = 0x00;
	    break;

	    case first:
	    break;

	    case unlocked:
		PORTB = 0x01;
	    break;

	    default:
		PORTB = 0x00;
	    break;
	}
	}
    }
		

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	
	PORTB = 0x00;
	state = start;

    while (1) {
	tick();	
    }
    return 1;
}
