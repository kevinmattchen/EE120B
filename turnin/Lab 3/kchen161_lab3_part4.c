/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #3  Exercise 4
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
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char tempA;
	unsigned char tempB;
	unsigned char tempC;	
    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	tempB = tempA >> 4;
	tempC = tempA << 4;
	
	PORTB = tempB;
	PORTC = tempC;
    }
    return 1;
}
