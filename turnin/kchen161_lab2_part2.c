/*	Author: kchen161
 *  Partner(s) Name: Kevin Chen
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
    
    unsigned char tempA = 0x00;
    unsigned char ntempA = 0x00;
    unsigned char cntavail = 0x00;

    /* Insert your solution below */
    while (1) {
        tempA = PINA & 0x0F;
	ntempA = ~tempA;
	
	if (ntempA & 0x08) {
		cntavail++;
	} 
        if (ntempA & 0x04) {
		cntavail++;
	} 
        if (ntempA & 0x02) {
		cntavail++;
	} 
        if (ntempA & 0x01) {
		cntavail++;
	} 
        
	
	PORTC = cntavail;
	cntavail = 0x00;
    }
    return 1;
}
