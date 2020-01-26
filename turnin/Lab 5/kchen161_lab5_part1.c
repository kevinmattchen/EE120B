/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #5  Exercise 1
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
	
	unsigned char tempAfuel;
	unsigned char tempA;
	unsigned char tempC = 0x00;
		
    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	tempAfuel = tempA & 0x0F;
	if (tempAfuel > 0) {
	    tempC = tempC | 0x20;
	}
	if (tempAfuel > 2) {
	    tempC = tempC | 0x10;
	}
	if (tempAfuel > 4) {
	    tempC = tempC | 0x08;
	}
	if (tempAfuel > 6) {
	    tempC = tempC | 0x04;
	}
	if (tempAfuel > 9) {
	    tempC = tempC | 0x02;
	}
	if (tempAfuel > 12) {
	    tempC = tempC | 0x01;
	}
	
	if (tempAfuel <= 4) {
	    tempC = tempC | 0x40;
	}

	if ((tempA & 0x10) && (tempA & 0x20) && !(tempA & 0x40)) {
	    tempC = tempC | 0x80;
	}
	    

	PORTC = tempC;
	tempC = 0x00;	
    }
    return 1;
}
