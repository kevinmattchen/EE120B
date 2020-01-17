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

	enum States {start, init, wait, inc, dec, reset} state;

    void tick() {
	switch (state) {
	    case start:
		state = init;
	    break;

	    case init:
		state = wait;
	    break;
	
	    case wait:
		switch (PINA & 0x03) {
		    case 0:
			state = wait;
		    break;

		    case 1:
			if (PORTC < 9) {
			    PORTC++;
			}
			state = inc;
		    break;

		    case 2:
			if (PORTC > 0) {
			    PORTC--;
			}
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
		switch (PINA & 0x03) {
		    case 0:
			state = wait;
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
	    
	    case dec:
		switch (PINA & 0x03) {
		    case 0:
			state = wait;
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

	    case reset:
		state = wait;
	    break;

	    default:
		state = start;
	    break;
	}

	switch (state) {
	    case start:
	    break;

	    case init:
		PORTC = 7;
	    break;

	    case wait:
	    break;

	    case inc:
	    break;

	    case dec:
	    break;

	    case reset:
		PORTC = 0;
  	    break;

	    default:
		PORTC = 0;
	    break;
	}
    }
		

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
	
	PORTC = 0x00;
	state = start;

    while (1) {
	tick();	
    }
    return 1;
}
