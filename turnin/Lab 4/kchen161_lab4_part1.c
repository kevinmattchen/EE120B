/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

	enum States {start, init, waitA0, flip, waitrelease} state;

    void tick() {
	switch(state) {
	    case start:
		state = init;
	    break;

	    case init:
		state = waitA0;
	    break;
	
	    case waitA0:
		if (PINA & 0x01) {
		    state = flip;
		} else {
		    state = waitA0;
		}
	    break;

	    case flip:
		if (PINA & 0x01) {
		    state = waitrelease;
		} else {
		    state = waitA0;
		}
	    break;

	    case waitrelease:
		if (!PINA & 0x01) {
		    state = waitA0;
		} else {
		    state = waitrelease;
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
		PORTB = 0x01;
	    break;

	    case waitA0:
	    break;

	    case flip:
		PORTB = ~PORTB & 0x03;
	    break;

	    case waitrelease:
	    break;
	
	    default: 
		PORTB = 0x01;
	    break;
	}
    }


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	
	PORTB = 0x01;
	state = start;

    while (1) {
	tick();	
    }
    return 1;
}
