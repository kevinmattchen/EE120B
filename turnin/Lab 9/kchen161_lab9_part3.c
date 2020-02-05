/*	Author: kchen161
 *      Partner(s) Name: Kevin Chen
 *	Lab Section: 23
 *	Assignment: Lab #9  Exercise 2
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

short Melody1_1[]   = {Eb4/2, Ab4/2, Ab4, Eb4*2, Ab4, C4*2, Eb4*2, A_4/2, G4, Eb4*2, G4, Bb4, Eb4*2, Eb4/2, Ab4/2, Ab4, Eb4*2, Ab4, C4*2, Eb4*2, A_4/2, G4, Eb4*2, G4, Bb4, Eb4*2, Eb4/2, Eb4*2, E4/2, Ab4, B4, E4*2, Eb4/2, Eb4*2, Eb4/2, Eb4*2, E4/2, Ab4, B4, E4*2, Eb4/2, Eb4*2, Ab4/8, Ab4/4, B4/4, Ab4/2, Ab4/4, Ab4/2, B4/2, Ab4, Ab4/2, Ab4, B4, Ab4*2, Ab4, Ab4*2, B4*2, Ab4*4};
short notelengths1_1[] = {200,   100,  100,    100,  100, 100,   100,  100,  100,  100, 100, 100,  300, 200,   100,  100,    100,  100, 100,   100,  100,  100,  100, 100, 100,  300, 100,   100,   100,  100,100, 100,  100,   100,   100,    100,  100, 100, 100, 100,  100,   100,   100,  100,    100,  100,   100,    100,   100, 100, 100,   100,100, 100,   100,  100,  100,    100};
short restlengths1_1[] = {50,  25,      25,     25,   25, 25,     25,   25,   25,   25,  25,  25, 25*3, 50,  25,      25,     25,   25, 25,     25,   25,   25,   25,  25,  25, 25*3, 25,    25,    25,   25,  25,  25,   25,   25,   25,     25,    25,  25,   25,  25, 25, 25+250,     25,   25,    25,    25,   25,      25,   25,   25,  25,   25,  25,  25,    25,   25,   25,     25};

short Melody1_2[]   = {Ab4*4, Ab4*4, Ab4*4, Ab4*4, Ab4*4, C4*2, Eb4*4, F4*4, C4*4, Eb4*4, F4*4, C4*2, Ab4*2, B4, Bb4*2, B4*2, Ab4*2, Bb4*2, C4*4, C4*2, Ab4*2, C4*4, Ab4*2, Bb4*2, Ab4*2, Ab4*2, Ab4*2, Ab4*2, Ab4*2, Ab4*2, C4, Eb4*2, F4*2, C4*2, Eb4*2, F4*2, C4, Ab4, B4/2, Bb4, B4, Ab4, Bb4, C4*2, C4, Ab4, C4*2, Ab4, Bb4, Ab4};
short notelengths1_2[] = {200,   200,    200,    100, 100,   100,  100,   100,   100,  100,  100,  100,   100,  100, 100,    100,  100,  100,   100,  100, 100,   100,   100,  200,   200  ,200,   200,    200,    100, 100,   100,  100,   100,   100,  100,  100,  100,   100,  100, 100,    100,  100,  100,   100,  100, 100,   100,   100,  200,   200  };
short restlengths1_2[] = {50,    50,      50,     25,  25,   25,   25,     25,    25,   25,  25,   25,   25,    25,   25,    25,   25,   25,     25,   25,  25,   25,   25,    50,    300   ,50,  50,      50,     25,  25,   25,   25,     25,    25,   25,  25,   25,   25,    25,   25,    25,   25,   25,     25,   25,  25,   25,   25,    50,    50   };

short Melody2_1[]   = {A_4/2, Bb4/2, G4*2, Eb4*4, G4*2, Bb4*2, D4*4, Db4, G4*2, Db4*4, G4*2, Bb4*2, C4*4, Eb4/2, Eb4*2, Bb4*2, Eb4*2, Ab4/2, C4*2, Ab4*2, C4*2, Eb4*2, F4*2, C4, C4*2, Ab4*2, C4*2, Eb4*2, F4*2, Ab4/2, C4*2, F4*2};
short notelengths2_1[] = {200,   100,    100,  100,  100,   100,  100,  100, 100,   100,   100,  100,  100,  100,   100,  100,   100,     100,   100,  100,   100,   100,  100, 100,100,  100,  100,    100,  100,  100,   100,  200 };
short restlengths2_1[] = {50,    25,     25,    25,  25,  25,    25,   25,    25,   25,   25,    25,   25,    25,    25,   25,   25,       25,    25,    25,   25,   25,   25,   25, 25,  25,   25,    25,   25,  25,     25,    50  };

short Melody2_2[]   = {Bb4/2, Eb4*2, G4*2, Bb4, Db4*2, F4*2, Db4, Eb4*2, G4*2, Bb4, Db4*2, F4*2, Bb4/2, Db4*2, F4*2, C4, C4*2, Ab4*2, C4*2, Eb4*2, F4*2, C4, C4*2, Ab4*2, C4*2, Eb4*2, F4*2, Ab4/2, C4*2, F4*2};
short notelengths2_2[] = {100,     100,   100,  100,100,  100,   100,  100,  100,  100, 100,  100,  100,   100,   200, 100,  100,   100,   100,   100,  100,  100,100, 100,    100,  100,    100,   100,  100, 200 };
short restlengths2_2[] = {25,      25,    25,   25,   25,  25,  25,    25,   25,    25,  25,   25,   25,     25,  50,   25,   25,    25,    25,   25,  25,     25,   25,  25,    25,  25,    25,    25,   25,   50 };

short Melody2_3[]   = {Bb4/2, G4*2, Eb4*4, G4*2, Bb4*2, D4*4, Db4, G4*2, Db4*4, G4*2, Bb4*2, C4*4, Eb4/2, G4*2, Bb4*2, Eb4*2, Ab4/2, C4*2, Ab4*2, C4*2, Eb4*2, F4*2, C4, C4*2, Ab4*2, Ab4*2, G4*2, Gb4*2, F4/2, F4, A_4, C4*2, F4*2, C4*2, A_4, F4, Bb4/2, F4, Bb4, Db4*2, F4*2, Db4*2, C4*2, Bb4/2, C4*2, Eb4/2, Bb4, G4/2, Eb4};
short notelengths2_3[] = {100,    100,  100,   100,  100,  100,  100,  100,   100, 100,   100,  100,  100,    100,   100,   100,  100,   100,  100,   100,  100,  100, 100, 100,  200,  200,     200, 200,   100, 100, 100,  100,  100, 100,  100, 100, 100,100,  100,  100,  200,  200,   200,  100,  100,  100,   100,   100, 100};
short restlengths2_3[] = {25,       25,   25, 25,     25,   25,   25,  25,    25,  25,    25,   25,   25,     25,    25,      25,   25,    25,   25,  25,    25,    25,  25,   25, 50,  50,     50,  50,     25,   25, 25,    25,  25,   25,   25,  25,  25,  25,  25,  25,   50,  50,     50,     25,   25,  25,    25,   25,   25};

short Melody2_firstend[]  = {Ab4, Eb4*2, Eb4*2};
short notelengths2_firstend[] = {200,  200,    200};
short restlengths2_firstend[] = {50,    50,     50};

short Melody2_secondend[]  = {Ab4/2, Ab4, C4*2, Eb4*2, Ab4*2};
short notelengths2_secondend[] = {100,  100,  100,  100,    200};
short restlengths2_secondend[] = {25,    25,   25,   25,     50};

short Trio[]           = {Ab4*2, Gb4, Ab4*2, Eb4*2, Bb4, Ab4*2, Gb4, Ab4*2, Eb4*2, Bb4, Eb4*2, Ab4, Db4*2, Bb4, Db4*2, F4*2, Ab4, Db4*2, F4*2, Bb4, Db4*2, F4*2, Ab4, F4*2, Bb4, F4*2, Ab4*2, Gb4, Ab4*2, Eb4*2, Bb4, Ab4*2, Gb4, Ab4*2, Eb4*2, Bb4, Eb4*2, Ab4, Db4*2, Bb4, Db4*2, F4*2, Ab4, Db4*2, F4*2, Bb4, Db4*2, F4*2, Ab4, F4*2, A_4, F4*2, Bb4*2, Ab4, Bb4*2, F4*2, C4*2, Bb4*2, Ab4, Bb4*2, F4*2, C4*2, F4*2, Eb4/2, Eb4*2, Bb4*2, Eb4*2, Gb4*2, C4*4, Gb4, Eb4, Bb4*2, Eb4*2, Gb4*2, C4*4, Eb4*2, Bb4*2, Db4*4, Db4*4, C4*4, Bb4*2, F4*2, Db4*2, Eb4*2, Gb4*2, Bb4, F4*2, F4*2, Db4*2, Eb4*2, F4*2, C4*2, Eb4*2, Db4*2};
short notelengthsTrio[] = {200,  100, 200,    100, 200,  200,  100, 200,    100, 100,  100, 100,   100,   100,  100,  100,  100,  100,   100, 100,   100,   100,  200, 100,  100,  100, 200,  100, 200,    100,  200, 200,  100,  200,  100,  100,  100,   100, 100,  100,   100,   100, 100,  100,   100,  100,   100,   100,200, 100,   100,   100, 200,  100, 200,   100,  200,  200,   100,  200,  100,  100,   100,  100,  100,   100,    100,   100,  100, 100,100,  100,   100,   100, 200, 100,     200,   200,    200,200, 200,  100,  100,   100,       200,  100, 200,  100,   100,  100,   200,   100,  100,   200 };
short restlengthsTrio[] = {50,   25,  50,     25,  50,   50,   25,   50,     25,  25,   25, 25,  25,       25,  25,     25,   25,  25,   25,     25,  25,   25,   50,   25,   25,  25,   50,  25,  50,      25,   50,  50,  25,    50,   25,   25,   25,   25,   25,  25,    25,    25,   25,   25,   25,    25,    25,   25, 50,  25,     25,    25,  50,   25,  50,    25,  50,   50,    25,    50,  25,    25,   25,   25,   25,   25,     25,      25,  25,   25, 25,    25,   25,   25,  50,  25,     50,     50,     50,   50, 50,    25,  25,     25,       50,   25,  50,    25,   25,   25,     50,    25,   25,     50};

short TrioFirstend[]              = {Ab4*2, F4*2, Ab4*2, F4*2, Ab4*2, F4*2};
short notelengthsTrioFirstend[]  = {200, 100,  100,    100,  100,   100};
short restlengthsTrioFirstend[]  = {50,    25,   25,   25,   25,    25  };

short TrioSecondend[]               = {Db4*2, F4*2, Ab4*2, Db4*4, C4};
short notelengthsTrioSecondend[]   = {100,    100,  100,  200,  200};
short restlengthsTrioSecondend[]   = {25,     25,   25,   50,    50};

short Ending_1[]          = {Ab4*2, F4*2, Ab4*2, F4*2, Ab4*2, Bb4*2, C4*4, Bb4*2, Ab4*2, F4*2, Eb4*2, F4*2, Eb4, C4*2, Eb4/2, Ab4/2, Eb4*2, F4*2, C4*2, Eb4*2, F4*2, A_4/2, C4*2, Eb4*2, F4*2, Bb4};
short notelengthsEnding_1[] = {200,   200,  200,    200,  200,   100,  200,  100,   100,   100,   100,   100, 100, 300,  200,    100,   100,  100,  100,  100,  100,   100,   100,   200,   100, 300};
short restlengthsEnding_1[] = {50,    50,   50,      50,  50,     25,   50,   25,   25,     25,    25,    25,  25,  75,  50,    25,     25,    25,  25,   25,    25,    25,   25,   50,     25,  75 };

short Ending_2[]             = {A_4/2, Bb4/2, Db4*2, F4*2, Bb4, Db4*2, F4*2, G4/2, C4*2, Ab4/2, Eb4*2, F4*2, C4*2, Eb4*2, F4*2, C4, C4*2, Ab4/2, Eb4*2, F4*2, C4*2, Eb4*2, F4*2, C4, Eb4*2, Ab4*2, F4*2, Ab4*2, F4*2, Ab4*2, Bb4*2, C4*4, Bb4*2, Ab4*2, F4*2, Ab4*2, F4*2, Eb4*2, Ab4*2, Eb4/2, C4*2, Ab4/2, Eb4*2, F4*2, C4*2, Eb4*2, F4*2, C4, Ab4, Bb4, Ab4, Ab4, Bb4, Ab4, Bb4, C4*2, Ab4, Bb4, C4*2, E4/2, Ab4, Bb4, C4*2, Ab4, G4/2, Bb4, G4/2, Eb4};
short notelengthsEnding_2[] = {200,  100,     100,   100, 100,   100,   100, 100,  100, 100,   100,   100,   100,   100, 100, 100,  100, 100,   100,   100,   100,  100,  100,  100, 100,  200,    200,  200,   200, 200,   100,   200,  100,   100,   100,  200,   200,   100,  100,   100,  100,    100,   100,  100,   100,  100,   100,100, 200, 100, 200, 200, 100, 200, 100, 100, 100,  100, 100, 100,   200, 100, 100,  100,  100, 100, 100, 100 };
short restlengthsEnding_2[] = {50,    25,     25,    25,  25,   25,    25,     25,   25,  25,   25,   25,   25,       25,  25, 25,  25,   25,    25,   25,     25,  25,   25,    25,   25,  50,     50,  50,    50,  50,     25,    50,  25,    25,  25,     50,     50,    25,    25,   25,   25,   25,     25,    25,  25,    25,     25,  25, 50, 25,   50,  50, 25,   50,  25,  25,  25,    25,  25,  25,  50,  25,   25,  25,   25,   25,  25,   25};

short EndingFirstend[] = {Ab4, Eb4*2, Eb4*2, C4};
short notelengthsEndingFirstend[] = {200, 200, 200, 200};
short restlengthsEndingFirstend[] = {50, 50, 50, 50};

short EndingSecondend[] = {Ab4, Eb4*2, Ab4*2};
short notelengthsEndingSecondend[] = {200, 200, 200};
short restlengthsEndingSecondend[] = {50, 50, 50};
	
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
			for (i = 0; i < 2; i++) {
				elements = sizeof(Melody1_1) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Melody1_1[j]);
					delay(notelengths1_1[j]);
					noTone(buzzer);
					delay(restlengths1_1[j]);
				}
				
				elements = sizeof(Melody1_2) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Melody1_2[j]);
					delay(notelengths1_2[j]);
					noTone(buzzer);
					delay(restlengths1_2[j]);
				}
			}

			for (i = 0; i < 2; i++) {
				elements = sizeof(Melody2_1) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Melody2_1[j]);
					delay(notelengths2_1[j]);
					noTone(buzzer);
					delay(restlengths2_1[j]);
				}

				elements = sizeof(Melody2_2) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Melody2_2[j]);
					delay(notelengths2_2[j]);
					noTone(buzzer);
					delay(restlengths2_2[j]);
				}

				elements = sizeof(Melody2_3) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Melody2_3[j]);
					delay(notelengths2_3[j]);
					noTone(buzzer);
					delay(restlengths2_3[j]);
				}

				if (i == 0) {
					elements = sizeof(Melody2_firstend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, Melody2_firstend[j]);
						delay(notelengths2_firstend[j]);
						noTone(buzzer);
						delay(restlengths2_firstend[j]);
					}
					} else {
					elements = sizeof(Melody2_secondend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, Melody2_secondend[j]);
						delay(notelengths2_secondend[j]);
						noTone(buzzer);
						delay(restlengths2_secondend[j]);
					}
				}
			}

			elements = sizeof(Melody1_1) / sizeof(int);
			for (j = 0; j < elements; j++) {
				tone(buzzer, Melody1_1[j]);
				delay(notelengths1_1[j]);
				noTone(buzzer);
				delay(restlengths1_1[j]);
			}
			
			elements = sizeof(Melody1_2) / sizeof(int);
			for (j = 0; j < elements; j++) {
				tone(buzzer, Melody1_2[j]);
				delay(notelengths1_2[j]);
				noTone(buzzer);
				delay(restlengths1_2[j]);
			}

			tone(buzzer, Ab4*2);
			delay(200);
			noTone(buzzer);
			delay(50);

			for (int i = 0; i < 2; i++) {
				elements = sizeof(Trio) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Trio[j]);
					delay(notelengthsTrio[j]);
					noTone(buzzer);
					delay(restlengthsTrio[j]);
				}

				if (i == 0) {
					elements = sizeof(TrioFirstend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, TrioFirstend[j]);
						delay(notelengthsTrioFirstend[j]);
						noTone(buzzer);
						delay(restlengthsTrioFirstend[j]);
					}
					} else {
					elements = sizeof(TrioSecondend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, TrioSecondend[j]);
						delay(notelengthsTrioSecondend[j]);
						noTone(buzzer);
						delay(restlengthsTrioSecondend[j]);
					}
				}
			}

			for (int i = 0; i < 2; i++) {
				elements = sizeof(Ending_1) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Ending_1[j]);
					delay(notelengthsEnding_1[j]);
					noTone(buzzer);
					delay(restlengthsEnding_1[j]);
				}

				elements = sizeof(Ending_2) / sizeof(int);
				for (j = 0; j < elements; j++) {
					tone(buzzer, Ending_2[j]);
					delay(notelengthsEnding_2[j]);
					noTone(buzzer);
					delay(restlengthsEnding_2[j]);
				}

				if (i == 0) {
					elements = sizeof(EndingFirstend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, EndingFirstend[j]);
						delay(notelengthsEndingFirstend[j]);
						noTone(buzzer);
						delay(restlengthsEndingFirstend[j]);
					}
					} else {
					elements = sizeof(EndingSecondend) / sizeof(int);
					for (j = 0; j < elements; j++) {
						tone(buzzer, EndingSecondend[j]);
						delay(notelengthsEndingSecondend[j]);
						noTone(buzzer);
						delay(restlengthsEndingSecondend[j]);
					}
				}
			}
			
			sw = PINA & 0x01;
			while(sw) { sw = PINA & 0x01; }
		}
    }
    return 1;
}
