/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #10  Exercise 3
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

unsigned char sw;
unsigned char threeLEDs, blinkingLED, buzzer;

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

enum States3 {init3, wait, buzzerOn, buzzerOff} state3;
void tick_buzzer() {
	switch (state3) {
		case init3:
			state3 = wait;
		break;

		case wait:
			if (sw) {
				state3 = buzzerOn;
			} else {
				state3 = wait;
			}
		break;

		case buzzerOn:
			if (sw) {
				state3 = buzzerOff;
			} else {
				state3 = wait;
			}
		break;

		case buzzerOff:
			if (sw) {
				state3 = buzzerOn;
			} else {
				state3 = buzzerOff;
			}
		break;
		
		default:
			state3 = init3;
		break;
	}

	switch (state3) {
		case init3:
			buzzer = 0;
		break;

		case wait:
		break;

		case buzzerOn:
			buzzer = 0x10;
		break;

		case buzzerOff:
			buzzer = 0;
		break;

		default:
			buzzer = 0x10;
		break;
	}
};

void tick_combine() {
	PORTB = threeLEDs | blinkingLED | buzzer;
};

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned short tickPeriod = 1;
	unsigned short threeLEDsPeriod = 300;
	unsigned short blinkingLEDPeriod = 1000;
	unsigned short buzzerPeriod = 2;
	unsigned short elapsedTime_threeLEDs = threeLEDsPeriod;
	unsigned short elapsedTime_blinkingLED = blinkingLEDPeriod;
	unsigned short elapsedTime_buzzer = buzzerPeriod;
	
    /* Insert your solution below */
	TimerSet(tickPeriod);
	TimerOn();
	
    while (1) {
		sw = PINA & 0x04;
		if (elapsedTime_threeLEDs >= threeLEDsPeriod) {
			tick_threeLEDs();
			elapsedTime_threeLEDs = 0;
		}
		if (elapsedTime_blinkingLED >= blinkingLEDPeriod) {
			tick_blinkingLED();
			elapsedTime_blinkingLED = 0;
		}
		if (elapsedTime_buzzer >= buzzerPeriod) {
			tick_buzzer();
			elapsedTime_buzzer = 0;
		}
		tick_combine();

		delay(1);

		elapsedTime_threeLEDs += tickPeriod;
		elapsedTime_blinkingLED += tickPeriod;
		elapsedTime_buzzer += tickPeriod;
		
    }
    return 1;
}
