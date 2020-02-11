/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #10  Exercise 1
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
void delay(unsigned short time) {
	for (unsigned short i = 0; i < time; i++) {
		while(!TimerFlag) {}
		TimerFlag = 0;
	}
}

unsigned char threeLEDs, blinkingLED;

enum States1 {init, light1, light2, light3} state1;
void tick_threeLEDs() {
	switch(state1) {
		case init:
			state1 = light1;
		break;

		case light1:
			state1 = light2;
		break;

		case light2:
			state1 = light3;
		break;

		case light3:
			state1 = light1;
		break;

		default:
			state1 = init;
		break;
	}

	switch(state1) {
		case init:
			threeLEDs = 0;
		break;

		case light1:
			threeLEDs = 0x01;
		break;

		case light2:
			threeLEDs = 0x02;
		break;

		case light3:
			threeLEDs = 0x04;
		break;

		default:
			threeLEDs = 0x07;
		break;
	}
}

enum States2 {init2, on, off} state2;
void tick_blinkingLED() {
	switch(state2) {
		case init2:
			state2 = on;
		break;

		case on:
			state2 = off;
		break;

		case off:
			state2 = on;
		break;

		default:
			state2 = init;
		break;
	}

	switch(state2) {
		case init2:
			blinkingLED = 0;
		break;

		case on:
			blinkingLED = 0x08;
		break;

		case off:
			blinkingLED = 0;
		break;

		default:
			blinkingLED = 0x08;
		break;
	}
};

void tick_combineLEDs() {
	PORTB = threeLEDs | blinkingLED;
};

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned short tickPeriod = 1;
	unsigned short threeLEDsPeriod = 1000;
	unsigned short blinkingLEDPeriod = 1000;
	unsigned short elapsedTime_threeLEDs = 1000;
	unsigned short elapsedTime_blinkingLED = 1000;
	
    /* Insert your solution below */
	TimerSet(tickPeriod);
	TimerOn();
	
    while (1) {
		if (elapsedTime_threeLEDs >= threeLEDsPeriod) {
			tick_threeLEDs();
			elapsedTime_threeLEDs = 0;
		}
		if (elapsedTime_blinkingLED >= blinkingLEDPeriod) {
			tick_blinkingLED();
			elapsedTime_blinkingLED = 0;
		}
		tick_combineLEDs();

		delay(1);

		elapsedTime_threeLEDs += tickPeriod;
		elapsedTime_blinkingLED += tickPeriod;
		
    }
    return 1;
}
