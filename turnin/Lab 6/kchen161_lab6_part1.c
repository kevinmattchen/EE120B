/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #6  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum States {start, init, blink1, blink2, blink3} state;
	
void tick() {
	switch(state) {
		case start:
			state = init;
		break;
		
		case init:
			state = blink1;
		break;
		
		case blink1:
			state = blink2;
		break;
		
		case blink2:
			state = blink3;
		break;
		
		case blink3:
			state = blink1;
		break;
		
		default:
			state = start;
		break;
	}
	
	switch(state) {
		case start:
		break;
		
		case init:
			PORTB = 0x00;
		break;
		
		case blink1:
			PORTB = 0x01;
		break;
		
		case blink2:
			PORTB = 0x02;
		break;
		
		case blink3:
			PORTB = 0x04;
		break;
		
		default:
			PORTB = 0x07;
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	state = start;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	
    while (1) {
		tick();
		while(!TimerFlag) {}
		TimerFlag = 0;
    }
    return 1;
}
