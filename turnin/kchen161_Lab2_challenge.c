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
    DDRC = 0x00; PORTC = 0x00;
    DDRD = 0XFF; PORTD = 0X00;
    
    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;
    unsigned char tempC = 0x00;
    unsigned char tempD = 0x00;
    unsigned char diff = 0x00;
    unsigned char overflow = 0x00;
    unsigned char total = 0x00;

    /* Insert your solution below */
    while (1) {
        tempA = PINA;
	tempB = PINB;
	tempC = PINC;
	
	total = tempA + tempB + tempC;
	if ( (total < tempA) | (total < tempB) | (total < tempC) ) {
		overflow = 0x01;
		tempD = tempD | 0x01;
	} else if (total >= 140) {
		overflow = 0x00;
		tempD = tempD | 0x01;
	} else {
		overflow = 0x00;
		tempD = tempD | 0x00;
	}	

	if (tempA > tempC) {
		diff = tempA - tempC;
	} else {
		diff = tempC - tempA;
	}

	if (diff >= 80) {
		tempD = tempD | 0x02;
	}

	


	PORTD = tempD;
	overflow = 0x00;
	tempD = 0x00;
    }
    return 1;
}
