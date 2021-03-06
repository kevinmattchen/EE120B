/*	Author: Kevin Chen
 *      Partner(s) Name:
 *	Lab Section: 23
 *	Assignment: Lab #1  Exercise #1
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
    DDRB = 0xFF; PORTB = 0x00;
    
    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;

    /* Insert your solution below */
    while (1) {
        tempA = PINA & 0x03;
        
	if (tempA == 0x01) {
		tempB = (tempB & 0xFC) | 0x01;
	} else {
		tempB = (tempB & 0xFC) | 0x00;
	}

	PORTB = tempB;
    }
    return 1;
}
