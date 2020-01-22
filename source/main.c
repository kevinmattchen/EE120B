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
	
	unsigned char leds;
	unsigned char button;
    /* Insert your solution below */ 

    while (1) {
	button = PINA & 0x01;
	
	if (button) {
	    leds = 0x01;
	} else {
	    leds = 0x02;
	}

	PORTC = leds;	
    }
    return 1;
}
