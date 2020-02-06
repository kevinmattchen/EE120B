/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #9  Exercise 3
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

#define C4 130.8*2
#define Db4 138.6*2
#define D4 146.8*2
#define Eb4 155.6*2
#define E4 164.8*2
#define F4 174.6*2
#define Gb4 185*2
#define G4 196*2
#define Ab4 207.7*2
#define A_4 220*2
#define Bb4 233.1*2
#define B4 246.9*2

short intro[]  = {G4*2, F4*2, Eb4*2, D4*2, Bb4, G4, D4, C4*2, D4*2, Eb4*2, F4*2, D4*2, G4, F4, D4};
short Lintro[] = {600,  300,  300,   300, 300,  300, 300,300,  300,  300,  300,   600, 300, 300,1000};
short Rintro[] = {150, 75,   75,    75,   75,  75,   75,75,   75,   75,   75,    150, 75,  75,  400};

short A[]  = {C4*2, G4/4, G4, C4/2, G4/4, G4, G4/4, G4, C4/2, G4/4, G4, F4, G4, G4/4, G4, C4/2, G4/4, G4, G4/4, G4, C4/2, G4/4, G4, F4, C4*4, G4/4, C4/2, G4*2, G4/4, C4/2, Eb4*2, C4/2, D4*2, G4/4, C4*2, Bb4/4, C4*2, D4*2, Eb4*2, C4*2, Bb4, C4*2, G4, G4/4, C4*2, G4/4, C4/2, G4, G4/4, C4/2, Eb4*2, C4/2, D4*2, G4/4, C4*2, C4, D4, Eb4, F4, D4, Bb4/2, C4, G4/4, C4*2, Bb4, C4*2, D4*2, Eb4*2, F4*2, G4*2, Bb4*2};
short LA[] = {300,  150, 150, 150,   150,300, 150, 150, 150,  150, 300, 300,300,150, 150, 150, 150,  300, 150, 150, 150,   150, 300,300,300,  150,  150,  150, 150,   300,  150, 150,  150,   150,  300,   300,   300,  300,   300,   300, 300, 300,  300,300,  300, 150,   150, 150,  150,  300, 150,  150,  150,   150, 300,  150,150,300, 300, 300, 300, 300, 300,  75,   75,  75,   75,   75,     75,   75,    75 };
short RA[] = {75, 75/2,75/2, 75/2, 75/2,75,75/2, 75/2,75/2,75/2, 75 ,75,   75,75/2,75/2,75/2,75/2, 75,  75/2,75/2,75/2,75/2,   75, 75, 75,   75/2,75/2,75/2,75/2,    75,  75/2,75/2,75/2,75/2,     75,    75,     75,   75,   75,    75,  75,  75,   75, 75,  75,   75/2, 75/2, 75/2,75/2,  75, 75/2, 75/2,75/2,  75/2,  75,  75/2,75/2,75, 75,  75, 75, 75,    75, 75/4, 75/4,75/4,75/4, 75/4,   75/4,  75/4, 75/4  };

short B[]   = {C4*4, G4/4, C4/2, G4*2, G4/4, C4/2, Eb4*4, C4/2, D4*4, G4/4, C4*4, Bb4/4, C4*2, D4*2, Eb4*2, C4*2, Bb4, C4*2, G4, G4*2, Ab4*2, Bb4*2, B4*2, C4*4, G4/4, C4/2, G4*2, G4/4, C4/2, Eb4*4, C4/2, D4*4, G4/4, C4*4, C4, D4};
short LB[]  = {300,  150,  150,  150,  150,   300, 150,   150,  150,   150, 300,  300,   300,  300, 300,   300,  300,   300,300, 75,   75,    75,    75,   300,  150,  150,  150,  150,   300, 150,   150,  150,   150, 300, 150, 150};
short RB[] = {75, 75/2,  75/2, 75/2, 75/2,  75,75/2, 75/2, 75/2,  75/2,   75,  75,   75,  75,     75,    75,  75,    75,75,75/4,  75/4,  75/4,  75/4,   75,  75/2,  75/2,  75/2,75/2,75,75/2,75/2,    75/2,  75/2,  75,75/2,75/2};
/*
short NotCoda[]  = {Eb4, F4, D4, Bb4/2, C4, C4/2, G4/4};
short LNotCoda[] = {300,300,300, 300,  600, 300,  300 };
short RNotCoda[] = {75, 75, 75, 75,   150, 75,    75 };
*/
short C[]   = {Ab4, Ab4, F4/4, Ab4, Ab4, Ab4, C4/2, Ab4, G4, G4, D4/2, G4, G4, G4, G4/4, G4, Ab4, Ab4, F4/4, Ab4, Ab4, Ab4, C4/2, Ab4, G4, G4, F4/2, G4, G4, G4/2};
short LC[]  = {150, 150, 150, 150,  150, 150, 150, 150, 150, 150,150, 150, 150,150,150,150, 150,150, 150,  150,  150, 150,  150,  150,  150,150,150,150,250, 250};
short RC[] = {75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,75/2,125, 125};

short C_2[] = {Ab4*2, G4*2, F4*2, Ab4/2, Eb4*2, F4*2, G4*2, C4*2, D4*2, C4*2, G4/2, Bb4, G4/2, F4*2, F4*2, F4*2, Eb4*2, D4*2, F4/2, Eb4*2, F4*2, Eb4*2, Ab4/2, F4*2, Ab4/2, G4*2, Ab4/2, Eb4*2, Ab4/2};
short LC_2[] = {450,   150, 150,  150,   150,   150,  300,  150, 150,    150,   150, 150,150,  225, 75,   150,  150,   150,   150,   150,  150,  150,   150,  150,   150,   150,  150,  150,   150   };
short RC_2[] = {75+75/2,75/2,75/2,75/2,75/2,75/2,75, 75/2,75/2,  75/2,  75/2,75/2,75/2,75/2+75/4,75/4,75/2,75/2,75/2,75/2,75/2,     75/2,75/2,75/2,   75/2,75/2,75/2,      75/2,75/2,75/2      };

short D_2[]   = {Ab4*2, G4*2, F4*2, Ab4/2, G4*2, Ab4*2, C4*4, G4*2, D4*2, Eb4*2, G4/2, C4*2, G4/2, D4*2, D4*2, D4*2, F4*2, D4*2, Bb4, G4, Bb4, C4*2, G4/2, G4/2, Bb4};
short LD_2[]  = {450,   150,  150,   150,  150,  150,   300,   150, 150,  150,   150,  150,  150,  225,  75,   150,   150, 150,   150,150,150, 450,  150,  300,  300};
short RD_2[] = {75+75/2,75/2,75/2,75/2,75/2,   75/2,  75, 75/2,  75/2,75/2,  75/2,  75/2, 75/2,75/2+75/4,75/4,75/2,75/2, 75/2,75/2,75/2,75/2,75+75/2,75/2,75,75};

short E[]   = {Eb4*2, Eb4*2, Eb4*2, F4*2, G4*2, G4*2, F4*2, Eb4*2, D4*2, D4*2, D4*2, Eb4*2, D4*2, C4*2, C4*2, C4*2, D4*2, Eb4*2, Eb4*2, D4*2, C4*2, Bb4, Bb4, Bb4, C4*2, D4*2, Ab4, Ab4, Ab4, Bb4, C4*2, C4*2, Bb4, Ab4, G4, Bb4, Eb4*2, F4*2, Eb4*2, Bb4, Eb4*2, Eb4*2, Eb4*2, F4*2, Gb4*2, Gb4*2, Ab4*2, Gb4*2, F4*2, C4, D4, Bb4/2};
short LE[]  = {225,    75,    150,   150,  150,  150, 150,  150,    225, 75,   150,  150,   600,  225,  75,  150,    150,  150, 150,   150,   150,  225,  75, 150,150, 600,  225, 75,  150,150, 150,   150,  150,  150,225,75,  150,  150,   300,   300,  225, 75,      150,  150, 150,   150,     150,  150,   300, 300, 300, 300 };
short RE[] = {75/2+75/4,75/4,75/2,75/2, 75/2,75/2,75/2,75/2,75/2+75/4,75/4,75/2,75/2, 150,75/2+75/4,75/4,75/2,75/2,  75/2, 75/2, 75/2,75/2,75/2+75/4,75/4,75/2,75/2,150,75/2+75/4,75/4,75/2,75/2,75/2,75/2,75/2,75/2,75/2+75/4,75/4,75/2,75/2,75,75,75/2+75/4,75/4,75/2,   75/2,75/2,75/2,   75/2,    75/2,   75,    75, 75,   75 };

short F[]   = {G4*2, Eb4, Eb4, F4, G4, G4*2, F4, Eb4,                  Bb4*2, D4, D4, Eb4, D4, Bb4*2, Ab4*2, C4, C4, D4, Eb4, Ab4*2, D4, C4,               G4*2, Bb4/2, Bb4/2, C4, D4, G4*2, Ab4*2, Ab4/2, Ab4/2, Bb4/2, C4, Ab4*2, Eb4, F4, Bb4*2, G4, G4, Bb4, G4, Bb4*2, F4, Eb4, G4*2, G4, G4, G4, G4*2, G4, G4, G4};
short LF[]  = {225,  75, 150, 150, 150,150,150, 150,                   225,  75,  150,150, 250, 250,   225,  75, 150,150,150, 150,  150,150,               225,  75,    150,  150,250, 250,  225,   75,   150,    150,  150, 150,  150,150,  225,  75, 150,150, 150, 150,   150,150, 250, 150, 75, 75,   250, 150, 75, 75};
short RF[] = {75/2+75/4,75/4,75/2,75/2,75/2,75/2,75/2,75/2,    75/2+75/4, 75/4, 75/2,75/2,125, 125,75/2+75/4,75/4,75/2,75/2,75/2,75/2,75/2,75/2,    75/2+75/4, 75/4,  75/2,  75/2, 125,  125,75/2+75/4,75/4,75/2,  75/2,  75/2,75/2,75/2,75/2,75/2+75/4,75/4,75/2,75/2,75/2,75/2,75/2,75/2,125,75/2,75/4,75/4,125, 75/2, 75/4,75/4};

short G[]   = {C4*2, B4/2, C4, B4/2, C4, D4, Eb4, F4, G4, Ab4, G4, F4, Eb4, F4, Eb4, D4, Eb4, B4/2, C4, B4/2, D4, D4, Eb4, F4, C4, Ab4, G4, F4, Eb4, F4, Eb4, D4, C4, C4, B4/2, C4, D4, C4, D4, Eb4, Eb4, G4, Ab4, G4, C4, Bb4, C4*2, D4*2, Bb4/2, D4*2, C4*2, Bb4, C4, Bb4, Ab4, G4, G4/2, G4, F4, Eb4, F4, Eb4, D4, Bb4/2, C4*2, B4/2, C4, B4/2, C4, D4, Eb4, F4, G4, Ab4, G4, F4, Eb4, F4, Eb4, D4, Eb4, B4/2, C4, B4/2, D4, D4, Eb4, F4, C4, Ab4, G4, F4, C4, F4, D4, D4, Eb4, C4, Bb4/2, C4, F4, C4, D4, Eb4, D4, G4, Ab4, G4, Bb4/2, Bb4, C4*2, D4*2, C4, D4*2, Eb4*2, F4*2, G4*2, Ab4*2, Bb4*2, B4*2, C4*4};
short LG[] = {75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 150};
short RG[] = {75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/4,75/2};
	
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

void tone(char a, short frq) {
	a = 0; //dud
	set_PWM(frq);
}
void delay(short t) {
	for (short i = 0; i < t; i++) {
		while(!TimerFlag) {}
		TimerFlag = 0;
	}
}
void noTone(char a) {
	a = 0; //dud
	set_PWM(0);
}

unsigned char button;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned char elements;
	unsigned char sw;
	unsigned short i, j;
	char buzzer = 0;
	
    /* Insert your solution below */
	TimerSet(1);
	TimerOn();
	
	PWM_on();
	
    while (1) {
		sw = PINA & 0x01;
		
		if (sw) {
			elements = sizeof(intro) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, intro[i]);
			  delay(Lintro[i]);
			  noTone(buzzer);
			  delay(Rintro[i]);
			}

			elements = sizeof(A) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, A[i]);
			  delay(LA[i]);
			  noTone(buzzer);
			  delay(RA[i]);
			}

			elements = sizeof(B) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, B[i]);
			  delay(LB[i]);
			  noTone(buzzer);
			  delay(RB[i]);
			}
		/*
			elements = sizeof(NotCoda) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, NotCoda[i]);
			  delay(LNotCoda[i]);
			  noTone(buzzer);
			  delay(RNotCoda[i]);
			}
		*/

			tone(buzzer, Eb4);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, F4);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, D4);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, Bb4/2);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, C4);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, Eb4/2);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, C4/2);
			delay(300);
			noTone(buzzer);
			delay(75);

			tone(buzzer, G4/4);
			delay(300);
			noTone(buzzer);
			delay(75);
    
			for (j = 0; j < 2; j++) {
			  elements = sizeof(C) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, C[i]);
				delay(LC[i]);
				noTone(buzzer);
				delay(RC[i]);
			  }
  
			  elements = sizeof(C_2) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, C_2[i]);
				delay(LC_2[i]);
				noTone(buzzer);
				delay(RC_2[i]);
			  }
  
			  elements = sizeof(C) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, C[i]);
				delay(LC[i]);
				noTone(buzzer);
				delay(RC[i]);
			  }
  
			  elements = sizeof(D_2) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, D_2[i]);
				delay(LD_2[i]);
				noTone(buzzer);
				delay(RD_2[i]);
			  }
  
			  elements = sizeof(E) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, E[i]);
				delay(LE[i]);
				noTone(buzzer);
				delay(RE[i]);
			  }
  
			  elements = sizeof(F) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, F[i]);
				delay(LF[i]);
				noTone(buzzer);
				delay(RF[i]);
			  }
			  tone(buzzer, G4*2);
			  delay(600);
			  noTone(buzzer);
			  delay(900);
  
			  elements = sizeof(G) / sizeof(int);
			  for (i = 0; i < elements; i++) {
				tone(buzzer, G[i]);
				delay(LG[i]);
				noTone(buzzer);
				delay(RG[i]);
			  }
			  noTone(buzzer);
			  delay(375*3/2);
			}

			elements = sizeof(A) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, A[i]);
			  delay(LA[i]);
			  noTone(buzzer);
			  delay(RA[i]);
			}

			elements = sizeof(B) / sizeof(int);
			for (i = 0; i < elements; i++) {
			  tone(buzzer, B[i]);
			  delay(LB[i]);
			  noTone(buzzer);
			  delay(RB[i]);
			}

			tone(buzzer, C4);
			delay(300);
			noTone(buzzer);
			delay(75+4*375);

			tone(buzzer, G4*2);
			delay(1100);
			noTone(buzzer);
			delay(100);

			tone(buzzer, F4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, Eb4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, D4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, Bb4);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, G4);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer,D4);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, C4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, D4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, Eb4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, F4*2);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, D4*2);
			delay(1100);
			noTone(buzzer);
			delay(100);

			tone(buzzer, G4);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, F4);
			delay(550);
			noTone(buzzer);
			delay(50);

			tone(buzzer, D4);
			delay(2400);
			noTone(buzzer);
			delay(1100);

			tone(buzzer, C4*4);
			delay(400);
			noTone(buzzer);
			delay(50);
		}

		sw = PINA & 0x01;
		while (sw) { sw = PINA & 0x01; }
    }
    return 1;
}
