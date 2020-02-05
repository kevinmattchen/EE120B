/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define C 261.63
#define D 293.66
#define E 329.63
#define F 349.23
#define G 392.00
#define A 440.00
#define B 493.88


void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
			
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
			
		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char tempA;
    /* Insert your solution below */
	
	PWM_on();
	
    while (1) {
		tempA = PINA & 0x07;
		if (tempA == 0x01) {
			set_PWM(C);
		} else if (tempA == 0x02) {
			set_PWM(D);
		} else if (tempA == 0x04) {
			set_PWM(E);
		} else {
			set_PWM(0);
		}
		
    }
    return 1;
}
