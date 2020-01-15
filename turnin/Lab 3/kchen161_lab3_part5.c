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
	DDRD = 0x00; PORTD = 0x00;
	DDRB = 0xFE; PORTB = 0x00;

	unsigned short weight = 0x00;
	unsigned char temp = 0x00;   
    /* Insert your solution below */
    while (1) {
	weight = (PINB & 0X01) + (PIND << 1);

	if (weight >= 70) {
	    temp = 0x02;
	} else if (weight > 5) {
	    temp = 0x04;
	} else {
	    temp = 0x00;
	}
	
	PORTB = (PORTB & 0x01) | (temp & 0xFE);
	temp = 0x00;
    }
    return 1;
}
