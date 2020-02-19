/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #11  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "io.c"
#include <string.h>
#include <avr/interrupt.h>

#if 1 //global variables
	unsigned char key;
#endif

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;
#if 1 //function definitions
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

unsigned char GetBit(unsigned char port, unsigned char number) {
	return (port & (0x01 << number));
}
unsigned char GetKeypadKey() {
	PORTA = 0xF7;
	asm("nop");
	if (GetBit(PINA, 7) == 0) { return('1'); }
	if (GetBit(PINA, 6) == 0) { return('4'); }
	if (GetBit(PINA, 5) == 0) { return('7'); }
	if (GetBit(PINA, 4) == 0) { return('*'); }

	PORTA = 0xFB;
	asm("nop");
	if (GetBit(PINA, 7) == 0) { return('2'); }
	if (GetBit(PINA, 6) == 0) { return('5'); }
	if (GetBit(PINA, 5) == 0) { return('8'); }
	if (GetBit(PINA, 4) == 0) { return('0'); }
	
	PORTA = 0xFD;
	asm("nop");
	if (GetBit(PINA, 7) == 0) { return('3'); }
	if (GetBit(PINA, 6) == 0) { return('6'); }
	if (GetBit(PINA, 5) == 0) { return('9'); }
	if (GetBit(PINA, 4) == 0) { return('#'); }
	
	PORTA = 0xFE;
	asm("nop");
	if (GetBit(PINA, 7) == 0) { return('A'); }
	if (GetBit(PINA, 6) == 0) { return('B'); }
	if (GetBit(PINA, 5) == 0) { return('C'); }
	if (GetBit(PINA, 4) == 0) { return('D'); }

	return ('\0');
}
#endif
#if 1 //tick function definitions
enum keypad_states {keypad_wait, keypad_press};
int keypad_tick(int state) {
	unsigned char x = GetKeypadKey();
	switch (state) {
		case keypad_wait:
			if (x != '\0') {
				state = keypad_press;
				key = x;
			} else {
				state = keypad_wait;
			}
		break;

		case keypad_press:
			key = '\0';
			if (x != '\0') {
				state = keypad_press;
			} else {
				state = keypad_wait;
			}
		break;

		default: state = keypad_wait; break;
	}
	return state;
};

enum display_states {display_wait, display};
int display_tick(int state) {
	static unsigned char prev = '\0';
	static unsigned char pos = 1;
	switch (state) {
		case display_wait:
			if (key == prev) {
				state = display_wait;
			} else if (key != '\0') {
				state = display;
			} else {
				state = display_wait;
			}
		break;

		case display:
			if (key == prev) {
				state = display_wait;
			} else if (key != '\0') {
				state = display;
			} else {
				state = display_wait;
			}
		break;

		default: state = display_wait; break;
	}
	switch (state) {
		case display_wait: break;
		case display: 
			LCD_Cursor(pos);
			LCD_WriteData(key);
			pos++;
			if (pos > 16) {
				pos = 1;
			}
		break;
	}
	prev = key;
	return state;
};
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x0F; PORTA = 0xF0;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

#if 1 //task assignment
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);

	task1.state = 0;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypad_tick;

	task2.state = 0;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &display_tick;
#endif
	TimerSet(10);
	TimerOn();

	LCD_init();
	LCD_DisplayString(1, "Congratulations!");

	while(1) {
#if 1 //task execution
		for (unsigned char i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}
#endif
		while(!TimerFlag){};
		TimerFlag = 0;
	}
	

    return 1;
}
