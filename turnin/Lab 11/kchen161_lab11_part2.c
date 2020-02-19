/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #11  Exercise 2
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
unsigned char text[100] = {"                CS120B is Legend... wait for it DARY!                "};
unsigned char space[100] = {"                "};
unsigned char length;
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
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('1'); }
	if (GetBit(PINC, 1) == 0) { return('4'); }
	if (GetBit(PINC, 2) == 0) { return('7'); }
	if (GetBit(PINC, 3) == 0) { return('*'); }

	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('2'); }
	if (GetBit(PINC, 1) == 0) { return('5'); }
	if (GetBit(PINC, 2) == 0) { return('8'); }
	if (GetBit(PINC, 3) == 0) { return('0'); }
	
	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('3'); }
	if (GetBit(PINC, 1) == 0) { return('6'); }
	if (GetBit(PINC, 2) == 0) { return('9'); }
	if (GetBit(PINC, 3) == 0) { return('#'); }
	
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('A'); }
	if (GetBit(PINC, 1) == 0) { return('B'); }
	if (GetBit(PINC, 2) == 0) { return('C'); }
	if (GetBit(PINC, 3) == 0) { return('D'); }

	return ('\0');
}
#endif
#if 1 //tick function definitions
enum scroll_states {scroll};
int scroll_tick(int state) {
	static unsigned char k = 0;
	unsigned char pos = 0;
	unsigned char* temp = space;
	switch (state) {
		case scroll:
			if (k < length - 16) {
				for (unsigned char j = 0; j < 16; j++) {
					temp[j] = text[j+k];
				}
				k++;
			} else {
				k = 0;
				for (unsigned char j = 0; j < 16; j++) {
					temp[j] = text[j];
				}
				k++;
			}
			state = scroll;
		break;

		default: 
			state = scroll; 
		break;
	}
	switch (state) {
		case scroll:
			LCD_DisplayString(1, temp);
		break;

		default: break;
	}
	return state;
};
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	//DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	//strcat(text, space);
	length = strlen(text);

#if 1 //task assignment
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);

	task1.state = scroll;
	task1.period = 400;
	task1.elapsedTime = task1.period;
	task1.TickFct = &scroll_tick;
#endif
	TimerSet(10);
	TimerOn();

	LCD_init();

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
