/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #11  Exercise 5
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
	unsigned char cursorPos = 1;
	unsigned char topObPos = 100;
	unsigned char botObPos = 100;
	unsigned char pause = 0;
	unsigned char restart = 0;
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

enum cursor_states {cursorInit, up, down, cursorPause};
int cursor_tick(int state) {
	static int saveState;
	switch (state) {
		case cursorInit:
			if (restart) {
				state = 0;
				cursorPos = 1;
			} else {
				state = up;
			}
		break;

		case up:
			if (pause) {
				saveState = state;
				state = cursorPause;
				break;
			}
			if (restart) {
				state = 0;
				break;
			}
			if (key == '8') {
				state = down;
			} else if (key != 'B') {
				state = up;
			}
		break;

		case down:
			if (pause) {
				saveState = state;
				state = cursorPause;
				break;
			}
			if (restart) {
				state = 0;
				break;
			}
			if (key == '2') {
				state = up;
			} else if (key != 'B') {
				state = down;
			}
		break;

		case cursorPause:
			if (pause) {
				state = cursorPause;
			} else {
				state = saveState;
			}

		default: state = up; break;
	}
	switch (state) {
		case cursorInit: cursorPos = 1; break;
		case up: LCD_Cursor(1); cursorPos = 1; break;
		case down: LCD_Cursor(17); cursorPos = 17; break;
	}
	return state;
};

enum topOb_states {topInit, topScroll, topPause};
int topOb_tick(int state) {
	static int saveState;
	static unsigned char i = 20;
	switch (state) {
		case topInit:
			if (restart) {
				state = topInit;
				i = 20;
				topObPos = i;
			} else {
				state = topScroll;
				i = 20;
				topObPos = i;
			}
		break;

		case topScroll:
			if (pause) {
				saveState = state;
				state = topPause;
				break;
			}
			if (restart) {
				state = 0;
				break;
			}
			i--;
			if (i < 1) {
				i = 20;
				LCD_Cursor(1);
				LCD_WriteData(' ');
			}
			if (i < 17) {
				LCD_Cursor(i+1);
				LCD_WriteData(' ');
				LCD_Cursor(i);
				LCD_WriteData('#');
			}
			state = topScroll;
			if (i != 17) {
				topObPos = i;
			}
		break;

		case topPause:
			if (restart) {
				saveState = 0;
			}
			if (pause) {
				state = topPause;
			} else {
				state = saveState;
			}
		break;

		default: state = topScroll; break;
	}
	return state;
};

enum botOb_states {botInit, botScroll, botPause};
int botOb_tick(int state) {
	static int saveState;
	static unsigned char i = 30;
	switch (state) {
		case botInit:
			if (restart) {
				state = 0;
				i = 30;
				botObPos = i+16;
			} else {
				state = botScroll;
				i = 30;
				botObPos = i+16;
			}
		break;

		case botScroll:
			if (pause) {
				saveState = state;
				state = botPause;
				break;
			}
			if (restart) {
				state = 0;
				break;
			}
			i--;
			if (i < 1) {
				i = 30;
				LCD_Cursor(17);
				LCD_WriteData(' ');
			}
			if (i < 17) {
				LCD_Cursor(16+i+1);
				LCD_WriteData(' ');
				LCD_Cursor(16+i);
				LCD_WriteData('*');
			}
			state = botScroll;
			botObPos = i+16;
		break;

		case botPause:
			if (restart) {
				saveState = 0;
			}
			if (pause) {
				state = botPause;
			} else {
				state = saveState;
			}
		break;

		default: state = botScroll; break;
	}
	return state;
};

enum pause_states {play, pause_press, pause_release, gameOver, gameOver_reset};
int pause_tick(int state) {
	switch(state) {
		case play:
			if (key == 'B') {
				state = pause_press;
				pause = 1;
				LCD_ClearScreen(); 
				LCD_DisplayString(6, "Paused"); 
			} else if ((cursorPos == topObPos) || (cursorPos == botObPos)) {
				state = gameOver;
				restart = 1;
				LCD_ClearScreen(); 
				LCD_DisplayString(4, "Game Over"); 
			} else {
				state = play;
			}
		break;

		case pause_press:
			if (key == 'B') {
				state = pause_press;
			} else {
				state = pause_release;
			}
		break;

		case pause_release:
			if (key == 'B') {
				state = play;
				pause = 0;
				LCD_ClearScreen();
			} else {
				state = pause_release;
			}
		break;

		case gameOver:
			if (key == 'B') {
				state = gameOver_reset;
				restart = 0;
				LCD_ClearScreen();
			} else {
				state = gameOver;
			}
		break;

		case gameOver_reset:
			if (key == 'B') {
				state = gameOver_reset;
			} else {
				state = play;
			}
		break;

		default: state = gameOver; break;
	}
	switch(state) {
		case play: break;
		case pause_press: break;
		case pause_release: break;
		case gameOver: break;
		case gameOver_reset: break;
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
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);

	task1.state = 0;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypad_tick;
	
	task2.state = 0;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &cursor_tick;
	
	task3.state = 0;
	task3.period = 250;
	task3.elapsedTime = task3.period;
	task3.TickFct = &topOb_tick;
	
	task4.state = 0;
	task4.period = 250;
	task4.elapsedTime = task4.period;
	task4.TickFct = &botOb_tick;
	
	task5.state = 0;
	task5.period = 50;
	task5.elapsedTime = task5.period;
	task5.TickFct = &pause_tick;

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
