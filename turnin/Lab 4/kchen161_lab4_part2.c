/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #2  Exercise Challenge
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, init, waitPress, inc, waitReleaseA0, dec, waitReleaseA1, reset} state;

void tick() {
    switch (state) {
	case start:
	    state = init;
	break;
	
	case init:
	    state = waitPress;
	break;

	case waitPress:
	    switch(PINA & 0x03) {
		case 0:
		    state = waitPress;
		break;

		case 1:
		    state = inc;
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

	case inc:
	    switch(PINA & 0x03) {
		case 0:
		    state = waitPress;
		break;

		case 1:
		    state = waitReleaseA0;
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

	case waitReleaseA0:
	    switch(PINA & 0x03) {
		
	    }
	break;
int main(void) {
    /* Insert DDR and PORT initializations */
	
    /* Insert your solution below */
    while (1) {
		
    }
    return 1;
}
