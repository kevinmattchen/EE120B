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
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA;
	unsigned char tempB;
	unsigned char cnt = 0;
	unsigned char i;
    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	tempB = PINB;
        for (i = 0; i < 8; i++) {
	    if (tempA & (0x01 << i)) {
		cnt++;
	    }
	    if (tempB & (0x01 << i)) {
		cnt++;
	    }
	}
	
	PORTC = cnt;
	cnt = 0;	
    }
    return 1;
}
