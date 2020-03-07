/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>
#include "io.c"

#define FOSC 8000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#if 1 //IR remote codes
	#define Power       'W'
	#define FuncStop    'F'
	#define VolUp       '+'
	#define VolDown     '-'
	#define Previous    'R'
	#define Next        'N'
	#define PlayPause   'P'
	#define Down        'D'
	#define Up          'U'
	#define Eq          'E'
	#define StRept      'S'
	#define Remote0     '0'
	#define Remote1     '1'
	#define Remote2     '2'
	#define Remote3     '3'
	#define Remote4     '4'
	#define Remote5     '5'
	#define Remote6     '6'
	#define Remote7     '7'
	#define Remote8     '8'
	#define Remote9     '9'
	#define hold        'H'
#endif

#if 1 //global variables
	unsigned char remote;
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
	TCCR2B = 0x0B;
	OCR2A = 125;
	TIMSK2 = 0x02;
	TCNT2 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR2B = 0x00;
}
void TimerISR() {
	TimerFlag = 1;
}
ISR(TIMER2_COMPA_vect) {
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

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}
void USART_Transmit( unsigned char data) {
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

#endif

#if 1 //tick function definitions
enum remote_states {receive};
int remote_tick(int state) {
	switch (state) {
		case receive:
		remote = USART_Receive();
		state = receive;
		break;
	}
	return state;
};

enum transmit_states {transmit};
int transmit_tick(int state) {
	switch (state) {
		case transmit:
			if (PORTD & 0x20) {
				UCSR0B |= (1<<TXEN0);
				USART_Transmit(remote);
			} else {
				UCSR0B = (1<<RXEN0) | !(1<<TXEN0);
			}
			state = transmit;
		break;
	}
	return state;
};

enum relay_states {flip};
int relay_tick(int state) {
	switch (state) {
		case flip:
			if ((remote == Power)) {
				if (PORTD & 0x20) {
					PORTD &= 0xDF;
				} else {
					PORTD |= 0x20;
				}
			}
			state = flip;
		break;

		default:
			state = flip;
		break;
	}
	return state;
};

#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xF0; PORTD = 0x00;

	unsigned char timerPeriod = 1;

	#if 1 //task assignment
	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);
	
	task1.state = 0;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &remote_tick;
	
	task2.state = 0;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &relay_tick;

	task3.state = 0;
	task3.period = 50;
	task3.elapsedTime = task3.period;
	task3.TickFct = &transmit_tick;

	#endif

	TimerSet(timerPeriod);
	TimerOn();
	USART_Init(MYUBRR);

	while(1) {
	#if 1 //task execution
		for (unsigned char i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += timerPeriod;
	#endif
	}

		while(!TimerFlag){};
		TimerFlag = 0;
	}
	

    return 1;
}
