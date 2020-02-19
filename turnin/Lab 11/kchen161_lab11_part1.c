/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #11  Exercise practice
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
#include <avr/interrupt.h>

unsigned char key = '\0';
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

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

enum keypad_States {keypad_wait, keypad_press, keypad_release};
int keypadTick(int state) {
	key = GetKeypadKey();
	switch (state) {
		case keypad_wait:
			if (key != '\0') {
				state = keypad_press;
			} else {
				state = keypad_wait;
			}
		break;

		case keypad_press:
			state = keypad_release;
		break;

		case keypad_release:
			if (key != '\0') {
				state = keypad_wait;
			} else {
				state = keypad_release;
			}
		break;

		default:
			state = keypad_wait;
		break;
	}
	switch (state) {
		case keypad_wait: break;
		case keypad_press: key = GetKeypadKey(); break;
		case keypad_release: break;
	}
	return state;
};

enum pause_States {pause_wait, pause_press, pause_release};
int pauseTick(int state) {
	unsigned char press = key;
	switch (state) {
		case pause_wait:
			state = press == '0'? pause_press : pause_wait; break;
		case pause_press:
			state = pause_release; break;
		case pause_release:
			state = press != '0'? pause_wait : pause_release; break;
		default: state = pause_wait; break;
	}
	switch (state) {
		case pause_wait: break;
		case pause_press: pause = (pause == 0) ? 1 : 0; break;
		case pause_release: break;
	}
	return state;
};

enum LED0_States {LED0_wait, LED0_blink};
int LED0Tick (int state) {
	switch (state) {
		case LED0_wait: state = !pause? LED0_blink : LED0_wait; break;
		case LED0_blink: state = pause? LED0_wait : LED0_blink; break;
		default: state = LED0_wait; break;
	}
	switch(state) {
		case LED0_wait: break;
		case LED0_blink:
		led0_output = (led0_output == 0x00) ? 0x01 : 0x00; break;
	}
	return state;
};

enum LED1_States {LED1_wait, LED1_blink};
int LED1Tick (int state) {
	switch (state) {
		case LED1_wait: state = !pause? LED1_blink : LED1_wait; break;
		case LED1_blink: state = pause? LED1_wait : LED1_blink; break;
		default: state = LED1_wait; break;
	}
	switch(state) {
		case LED1_wait: break;
		case LED1_blink:
		led1_output = (led1_output == 0x00) ? 0x01 : 0x00; break;
	}
	return state;
};

enum display_States {display};
int displayTick(int state) {
	unsigned char output;
	switch (state) {
		case display: state = display; break;
		default: state = display; break;
	}
	switch (state) {
		case display:
			output = led0_output | (led1_output << 1);
		break;
	}
	PORTB = output;
	return state;
};

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);

	task1.state = 0;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseTick;
	task2.state = 0;
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LED0Tick;
	task3.state = 0;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.TickFct = &LED1Tick;
	task4.state = 0;
	task4.period = 10;
	task4.elapsedTime = task4.period;
	task4.TickFct = &displayTick;
	task5.state = 0;
	task5.period = 10;
	task5.elapsedTime = task5.period;
	task5.TickFct = &keypadTick;

	TimerSet(10);
	TimerOn();
	
	unsigned short i;
	while(1) {
		for (i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}
		while(!TimerFlag){};
		TimerFlag = 0;
	}
	

    return 1;
}
