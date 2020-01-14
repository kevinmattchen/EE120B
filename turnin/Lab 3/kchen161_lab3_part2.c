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

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA;
	unsigned char tempC = 0x00;
		
    /* Insert your solution below */
    while (1) {
	tempA = PINA & 0x0F;
	if (tempA > 0) {
	    tempC = tempC | 0x20;
	}
	if (tempA > 2) {
	    tempC = tempC | 0x10;
	}
	if (tempA > 4) {
	    tempC = tempC | 0x08;
	}
	if (tempA > 6) {
	    tempC = tempC | 0x04;
	}
	if (tempA > 9) {
	    tempC = tempC | 0x02;
	}
	if (tempA > 12) {
	    tempC = tempC | 0x01;
	}
	
	if (tempA <= 4) {
	    tempC = tempC | 0x40;
	}

	PORTC = tempC;
	tempC = 0x00;	
    }
    return 1;
}
