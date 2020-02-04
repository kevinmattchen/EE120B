/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #8  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

const unsigned short MAX = 927; // UPDATE MAX

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned short x;
	unsigned char tempB = 0;
	ADC_init();
	
    /* Insert your solution below */
	
    while (1) {
		x = ADC;
		for (unsigned char i = 0; i < 8; i++) {
			if (x > (MAX / 8 * i)) {
				tempB |= (0x01 << i);
			}
		}
		
		PORTB = tempB;
		tempB = 0;
    }
    return 1;
}
