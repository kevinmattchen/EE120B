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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#if 1 //IR remote codes
	#define Power       16753245
	#define FuncStop    16769565
	#define VolUp       16736925
	#define VolDown     16769055
	#define Previous    16720605
	#define Next        16761405
	#define PlayPause   16712445
	#define Down        16769055
	#define Up          16748655
	#define Eq          16750695
	#define StRept      16756815
	#define Remote0     16738455
	#define Remote1     16724175
	#define Remote2     16718055
	#define Remote3     16743045
	#define Remote4     16716015
	#define Remote5     16726215
	#define Remote6     16734885
	#define Remote7     16728765
	#define Remote8     16730805
	#define Remote9     16732845
#endif

#if 1 //music

	#if 1 //note definitions
	#define C 130.8*2
	#define Db 138.6*2
	#define D 146.8*2
	#define Eb 155.6*2
	#define E 164.8*2
	#define F 174.6*2
	#define Gb 185*2
	#define G 196*2
	#define Ab 207.7*2
	#define A 220*2
	#define Bb 233.1*2
	#define B 246.9*2
	#define Cs Db
	#define Ds Eb
	#define Fs Gb
	#define Gs Ab
	#define As Bb
	#endif

	#if 1 //Melody note arrays
	short Mm1[]  = {D, D, D*2, A, Gs, G, F, D, F, G,                         C, C, D*2, A, Gs, G, F, D, F, G,                             B/2, B/2, D*2, A, Gs, G, F, D, F, G,                      Bb/2, Bb/2, D*2, A, Gs, G, F, D, F, G, -1};
	short Mm1_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Mm1_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25};

	short Mm5[]  = {D, D, D*2, A, Gs, G, F, D, F, G,                         C, C, D*2, A, Gs, G, F, D, F, G,                             B/2, B/2, D*2, A, Gs, G, F, D, F, G,                      Bb/2, Bb/2, D*2, A, Gs, G, F, D, F, G, -1};
	short Mm5_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Mm5_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25};

	short Mm9[]  = {D*2, D*2, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,       C*2, C*2, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,           B, B, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,            Bb, Bb, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2, -1};
	short Mm9_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Mm9_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25};

	short Mm13[]  = {D*2, D*2, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,       C*2, C*2, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,           B, B, D*4, A*2, Gs*2, G*2, F*2, D*2, F*2, G*2,            Bb, Bb, D*4, A*2, Gs*2, G*2, F*2, D*2, C*2, D*2, E*2, -1};
	short Mm13_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 40, 35};
	short Mm13_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 12, 13};

	short Mm17[]   = {E*2, F*2, F*2, F*2, F*2, E*2, F*2, D*2, D*2,        F*2, F*2, F*2, G*2, Ab*2, G*2, Ab*2, G*2, D*2, F*2, G*2,   F*2, F*2, F*2, G*2, Ab*2, A*2, C*4, A*2,              D*4, D*4, D*4, A*2, D*4, C*4, -1};
	short Mm17_L[] = {22, 200, 100, 100, 100, 22, 200, 100, 125+500-50,   200, 100, 100, 100, 200, 50, 50, 50, 100, 100, 100,        200, 100, 50, 100, 100, 100, 100, 125+250-50,         200, 200, 100, 100, 100, 125+500*2-50};
	short Mm17_R[] = {3, 50, 25, 25+125, 25+125-25, 3, 50, 25+125, 50,    50, 25, 25+125, 25+125, 50, 12, 13, 75, 25, 25, 25+250,    50, 25, 75+125, 25+125, 25+125, 25+125, 25+125, 50,   50, 50, 25, 25, 25, 50};

	short Mm21[]   = {A*2, A*2, A*2, A*2, Gs*2, A*2, G*2, G*2,        A*2, A*2, A*2, A*2, G*2, A*2, D*4, A*2, G*2,     D*4, A*2, G*2, F*2, C*4, G*2, F*2, E*2,    B, D*2, E*2, F*2, C*4, -1};
	short Mm21_L[] = {200, 100, 100, 100, 22, 200, 100, 125+500-50,   200, 200, 100, 50, 50, 50, 200, 100, 200,        200, 200, 200, 200, 200, 200, 200, 200,    200, 100, 100, 100, 125+2*500-50};
	short Mm21_R[] = {50, 25, 25+125, 25+125-25, 3, 50, 25+125, 50,   50, 50, 25, 75+125, 75+125, 75+125, 50, 25, 50,  50, 50, 50, 50, 50, 50, 50, 50,            50, 25, 150, 150, 50};

	short Mm25[]   = {0, F*2, D*2, F*2, G*2, Ab*2, G*2, F*2, D*2,    Ab*2, G*2, D*2, F*2, G*2, Gs*2, A*2,        C*4, A*2, Gs*2, G*2, F*2, D*2, E*2, F*2, G*2, A*2, C*4,    Db*4, Ab*2, Ab*2, G*2, F*2, G*2,         D*2, E*2, F*2, F*4, E*4, D*4,    E*4, F*4, G*4, E*4,   A*4, A*4, Gs*4, G*4, Fs*4, F*4, E*4, Ds*4, D*4,      Cs*4, Eb*4, -1};
	short Mm25_L[] = {0, 100, 100, 100, 100, 100, 100, 100, 100,     50, 50, 100, 200, 2*500+125-50, 200, 100,   100, 100, 100, 100, 100, 100, 100, 200, 200, 200, 200,     200, 100, 100, 100, 100, 125+2*500-50,   200, 200, 200, 200, 450, 450,    450, 450, 450, 450,   2*500-50, 100, 100, 100, 100, 100, 100, 100, 100,    2*500, 2*500-50};
	short Mm25_R[] = {2*500+100, 25, 25, 25, 25, 25, 25, 25, 25,     12, 13, 25, 50, 50, 50, 25,                 25+125, 25, 25, 25, 25, 25, 25, 50, 50, 50, 50,            50, 25+125, 25, 25, 25, 50,              50, 50, 50, 50, 50, 50,          50, 50, 50, 50,       50, 25, 25, 25, 25, 25, 25, 25, 25,                  0, 50};

	short Mm33[]   = {Bb/2, F,      E, D,               F,            0,           Bb/2, F,      E, D,               D, D/2, -1};
	short Mm33_L[] = {3*500, 450,   2*500, 2*500-50,    4*500-50,     0,           3*500, 450,   2*500, 2*500-50,    7*500, 450};
	short Mm33_R[] = {0, 50,        0, 50,              50,           4*500+125,   0, 50,        0, 50,              0, 50};

	short Mm41[]   = {Bb/2, F,       E, D,              F,          G*2, D*4, F*4, D*4, G*4, F*4, D*4, C*4, A*2, G*2, A*2, C*4,     Bb/2, F,       E, D,               D, F*2, E*2, C*2, E*2, D*2, G, A, C*2,                  F*2, E*2, C*2, E*2, D*2, G, A, C*2, -1};
	short Mm41_L[] = {3*500, 450,    2*500, 2*500-50,   4*500-50,   100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100,     3*500, 450,    2*500, 2*500-50,    200, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 200, 100, 100, 100};
	short Mm41_R[] = {0, 50,         0, 50,             50,         25, 25, 25, 25, 150, 150, 25, 25+125, 50, 25, 25, 25,           0, 50,         0, 50,              50, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25+250,   150, 150+125, 25+125, 25+125, 50, 25, 25, 25};

	short Mm49[]   = {0, -1};
	short Mm49_L[] = {0};
	short Mm49_R[] = {4*500*14+250+250};

	short Mm63[]   = {D, D, D*2, A, Gs, G, F, D, F, G,                         D, D, D*2, A, Gs, G, F, D, F, G,                            Bb/2, Bb/2, D*2, A, Gs, G, F, D, F, G,                    C, C, D*2, A, Gs, G, F, D, F, G,                           D, D, D*2, A, Gs, G, F, D, F, G,                         D, D, D*2, A, Gs, G, F, D, F, G,                            Bb/2, Bb/2, D*2, A, Gs, G, F, D, F, G,                    C, C, D*2, A, Gs, G, F, D, F, G, -1};
	short Mm63_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100,          100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Mm63_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,      25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25};
	#endif

	#if 1 //Harmony note arrays
	short Hm1[]   = {0, -1};
	short Hm1_L[] = {0};
	short Hm1_R[] = {4*500*4+250};

	short Hm5[]   = {D,                    C,                B/2,             Bb/2,          C, -1};
	short Hm5_L[] = {125+250+500*2+100,    250+500*3-50,     250+500*3-50,    250+250+100,   125+500*2-50};
	short Hm5_R[] = {25+3*125+250,         50+250,           50+250,          25,            50+100};

	short Hm9[]   = {D, D, D*2, A, Gs, G, F, D, F, G,                         C, C, D*2, A, Gs, G, F, D, F, G,                             B/2, B/2, D*2, A, Gs, G, F, D, F, G,                      Bb/2, Bb/2, D*2, A, Gs, G, F, D, F, G, -1};
	short Hm9_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Hm9_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25};

	short Hm13[]   = {D, D, D*2, A, Gs, G, F, D, F, G,                         C, C, D*2, A, Gs, G, F, D, F, G,                             B/2, B/2, D*2, A, Gs, G, F, D, F, G,                      Bb/2, Bb/2, D*2, A, Gs, G, F, D, -1};
	short Hm13_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100};
	short Hm13_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25+250};

	short Hm17[]   = {0, G*4, -1};
	short Hm17_L[] = {0, 125+500-50};
	short Hm17_R[] = {4*500*4-125-500, 50+125};

	short Hm21[]   = {F*2, F*2, F*2, F*2, E*2, F*2, D*2, D*2,         F*2, F*2, F*2, F*2, D*2, F*2, E*2, D*2, C*2,     F*2, A*2, G*2, F*2, E*2, G*2, F*2, E*2,    G, Bb, C*2, D*2, E*2, -1};
	short Hm21_L[] = {200, 100, 100, 100, 22, 200, 100, 125+500-50,   200, 200, 100, 50, 50, 50, 200, 100, 200,        200, 200, 200, 200, 200, 200, 200, 200,    200, 100, 100, 100, 125+2*500-50};
	short Hm21_R[] = {50, 25, 25+125, 25+125-25, 3, 50, 25+125, 50,   50, 50, 25, 75+125, 75+125, 75+125, 50, 25, 50,  50, 50, 50, 50, 50, 50, 50, 50,            50, 25, 150, 150, 50};

	short Hm25[]   = {0, D*2, B, D*2, D*2, D*2, D*2, D*2, B,         F*2, E*2, D*2, D*2, E*2, Gs*2, A*2,         C*4, A*2, Gs*2, G*2, F*2, D*2, E*2, F*2, G*2, A*2, C*4,    Db*4, Ab*2, Ab*2, G*2, F*2, G*2,         F, G, A, A*2, G*2, F*2,          G*2, A*2, C*4, A*2,   D*4, A*4, Gs*4, G*4, Fs*4, F*4, E*4, Ds*4, D*4,      Gs*2, B*2, -1};
	short Hm25_L[] = {0, 100, 100, 100, 100, 100, 100, 100, 100,     50, 50, 100, 200, 2*500+125-50, 200, 100,   100, 100, 100, 100, 100, 100, 100, 200, 200, 200, 200,     200, 100, 100, 100, 100, 125+2*500-50,   200, 200, 200, 200, 450, 450,    450, 450, 450, 450,   2*500-50, 100, 100, 100, 100, 100, 100, 100, 100,    2*500, 2*500-50};
	short Hm25_R[] = {2*500+100, 25, 25, 25, 25, 25, 25, 25, 25,     12, 13, 25, 50, 50, 50, 25,                 25+125, 25, 25, 25, 25, 25, 25, 50, 50, 50, 50,            50, 25+125, 25, 25, 25, 50,              50, 50, 50, 50, 50, 50,          50, 50, 50, 50,       50, 25, 25, 25, 25, 25, 25, 25, 25,                  0, 50};

	short Hm33[]   = {Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4,	     B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4,	               Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,	      D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, -1};
	short Hm33_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,                 100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Hm33_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,             150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Hm41[]   = {D/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,        C/2, C/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,             B/4, B/4, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,          B/4, B/4, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,               D/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,        C/2, C/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,        D, F, E, C, E, D, G, A, C*2,                            F, E, C, E, D, G, A, C*2, -1};
	short Hm41_L[] = {100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 100, 200, 100, 100, 100,              100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 50,       200, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 200, 100, 100, 100};
	short Hm41_R[] = {25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,  25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25+125,      25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,  25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 0,   50, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25+250,   150, 150+125, 25+125, 25+125, 50, 25, 25, 25+50};

	short Hm49[]   = {Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       D/2, D/2, D/2, D/2, Cs/2, Cs/2, Cs/2, Cs/2, Cs/2, Cs/2,	     C/2, C/2, C/2, C/2, B/4, B/4, B/4, B/4, B/4, B/4,      Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,	     D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,      Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       D/2, D/2, D/2, D/2, Cs/2, Cs/2, Cs/2, Cs/2, Cs/2, Cs/2,	     C/2, C/2, C/2, C/2, B/4, B/4, B/4, B/4, B/4, B/4,       Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4, Bb/4,   C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, -1};
	short Hm49_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,            100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,              100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Hm49_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,        150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,          150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Hm63[]   = {D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,	     D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,         0, -1};
	short Hm63_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,         0};
	short Hm63_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,     4*500*6};
	#endif

	#if 1 //Bass note arrays
	short Bm1[]   = {0, 0, -1};
	short Bm1_L[] = {0, 0};
	short Bm1_R[] = {4*500*4, 125};

	short Bm5[]   = {D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,        C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2,       B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4,        Bb/4, Bb/4, Bb/4, Bb/4, C/2, C/2, C/2, C/2, C/2, C/2, -1};
	short Bm5_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm5_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm9[]   = {D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,        C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8,        Bb/8, Bb/8, Bb/8, Bb/8, C/4, C/4, C/4, C/4, C/4, C/4, -1};
	short Bm9_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm9_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm13[]   = {D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,        C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8,        Bb/8, Bb/8, Bb/8, Bb/8, C/4, C/4, C/4, C/4, C/4, C/4, -1};
	short Bm13_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm13_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm17[]   = {D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2, D/2,        C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C/2, C, C/2,       B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4, B/4,        Bb/4, Bb/4, Bb/4, Bb/4, C/2, C/2, C/2, C/2, C/2, C, C/2, -1};
	short Bm17_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,     100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
	short Bm17_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 25, 25,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 25, 25};

	short Bm21[]   = {D/2, D/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,        C/2, C/2, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,             B/4, B/4, D, A/2, Ab/2, G/2, F/2, D/2, F/2, G/2,          Bb/4, Bb/4, D, A/2, G/2, F/2, D/2, F/2, G/2, -1};
	short Bm21_L[] = {100, 100, 100, 100, 100, 100, 200, 100, 100, 100,       100, 100, 100, 100, 100, 100, 200, 100, 100, 100,            100, 100, 100, 100, 100, 100, 200, 100, 100, 100,         100, 100, 100, 100, 100, 200, 100, 100, 100};
	short Bm21_R[] = {25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,   25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,        25, 25, 150, 150+125, 25+125, 25+125, 50, 25, 25, 25,     25, 25, 150, 150+250+125, 25+125, 50, 25, 25, 25};

	short Bm25[]   = {Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/4,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/2,       D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/2,	     Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/2,       Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/4,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/2,       D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/2,	      Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/2, -1};
	short Bm25_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,                 100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm25_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,             150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm33[]   = {Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8,	     B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8, B/8,                 Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,	      D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, -1};
	short Bm33_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200,                 100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,        100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm33_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,             150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,    150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm41[]   = {Bb/8,     C/4,       B/8,         B/8,       Bb/8,        C/4,       B/8,        B/8, -1};
	short Bm41_L[] = {4*500,    4*500,     4*500,       4*500,     4*500,       4*500,     4*500,      4*500-50};
	short Bm41_R[] = {0,        0,         0,           0,         0,           0,         0,          50+50};

	short Bm49[]   = {Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       D/4, D/4, D/4, D/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4,	     C/4, C/4, C/4, C/4, B/8, B/8, B/8, B/8, B/8, B/8,      Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,	     D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,      Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4,       D/4, D/4, D/4, D/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4, Cs/4,	     C/4, C/4, C/4, C/4, B/8, B/8, B/8, B/8, B/8, B/8,       Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8, Bb/8,   C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, C/4, -1};
	short Bm49_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,            100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,              100, 100, 100, 100, 100, 100, 100, 100, 100, 200,       100, 100, 100, 100, 100, 100, 100, 100, 100, 200,             100, 100, 100, 100, 100, 100, 100, 100, 100, 200};
	short Bm49_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,        150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,          150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,   150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,         150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50};

	short Bm63[]   = {D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,	     D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4, D/4,      0, -1};
	short Bm63_L[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      100, 100, 100, 100, 100, 100, 100, 100, 100, 200,      0};
	short Bm63_R[] = {150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  150, 150, 25, 25+125, 25+125, 25+125, 25, 25, 25, 50,  4*500*6};
	#endif

	short* Melody[]   = {Mm1, Mm5, Mm9, Mm13, Mm17, Mm21, Mm25, Mm25, Mm33, Mm41, Mm49, Mm63};
	short* Melody_L[] = {Mm1_L, Mm5_L, Mm9_L, Mm13_L, Mm17_L, Mm21_L, Mm25_L, Mm25_L, Mm33_L, Mm41_L, Mm49_L, Mm63_L};
	short* Melody_R[] = {Mm1_R, Mm5_R, Mm9_R, Mm13_R, Mm17_R, Mm21_R, Mm25_R, Mm25_R, Mm33_R, Mm41_R, Mm49_R, Mm63_R};

	short* Harmony[]   = {Hm1, Hm5, Hm9, Hm13, Hm17, Hm21, Hm25, Hm25, Hm33, Hm41, Hm49, Hm63};
	short* Harmony_L[] = {Hm1_L, Hm5_L, Hm9_L, Hm13_L, Hm17_L, Hm21_L, Hm25_L, Hm25_L, Hm33_L, Hm41_L, Hm49_L, Hm63_L};
	short* Harmony_R[] = {Hm1_R, Hm5_R, Hm9_R, Hm13_R, Hm17_R, Hm21_R, Hm25_R, Hm25_R, Hm33_R, Hm41_R, Hm49_R, Hm63_R};

	short* Bass[]   = {Bm1, Bm5, Bm9, Bm13, Bm17, Bm21, Bm25, Bm25, Bm33, Bm41, Bm49, Bm63};
	short* Bass_L[] = {Bm1_L, Bm5_L, Bm9_L, Bm13_L, Bm17_L, Bm21_L, Bm25_L, Bm25_L, Bm33_L, Bm41_L, Bm49_L, Bm63_L};
	short* Bass_R[] = {Bm1_R, Bm5_R, Bm9_R, Bm13_R, Bm17_R, Bm21_R, Bm25_R, Bm25_R, Bm33_R, Bm41_R, Bm49_R, Bm63_R};

#endif

#if 1 //global variables
	unsigned char pause = 1;
	unsigned int remote;
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

void set_PWM1(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR1B &= 0x08; }
		else { TCCR1B |= 0x03; }
		
		if (frequency < 0.954) { OCR1A = 0xFFFF; }
		else if (frequency > 31250) {OCR1A = 0x0000; }
		else {OCR1A = (short)(8000000 / (128 * frequency)) + 1; }
		
		TCNT1 = 0;
		current_frequency = frequency;
	}
}
void PWM1_on() {
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
	set_PWM1(0);
}
void PWM1_off() {
	TCCR1A = 0x00;
	TCCR1B = 0x00;
}
void tone1(double frq) {
	set_PWM1(frq);
}
void noTone1() {
	set_PWM1(0);
}

void set_PWM3(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) {OCR3A = 0x0000; }
		else {OCR3A = (short)(8000000 / (128 * frequency)) + 1; }
		
		TCNT3 = 0;
		current_frequency = frequency;
	}
}
void PWM3_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM1(0);
}
void PWM3_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
void tone3(double frq) {
	set_PWM3(frq);
}
void noTone3() {
	set_PWM3(0);
}

void set_PWM0(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; TCCR0A |= 0x03;}
		else { TCCR0B |= 0x05; }
		
		if (frequency < 31) { OCR0A = 0xFF; }
		else if (frequency > 3906) {OCR0A = 0x00; }
		else {OCR0A = (char)(8000000 / (2 * 1024 * frequency)) + 0.5; }
		
		TCNT0 = 0;
		current_frequency = frequency;
	}
}
void PWM0_on() {
	TCCR0A = (1 << COM0A0);
	TCCR0B = (1 << WGM02) | (1 << WGM01) | (1 << WGM00) | (1 << CS02) | (1 << CS00);
	set_PWM0(0);
}
void PWM0_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
void tone0(double frq) {
	set_PWM0(frq);
}
void noTone0() {
	set_PWM0(0);
}

unsigned char GetBit(unsigned char port, unsigned char number) {
	return (port & (0x01 << number));
}

void USART_Init( unsigned int ubrr) {
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
unsigned int USART_Receive( void )
{
	unsigned char status, resh, resl;
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get status and 9th bit, then data */
	/* from buffer */
	status = UCSR0A;
	resh = UCSR0B;
	resl = UDR0;
	/* If error, return -1 */
	if ( status & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)) )
	return -1;
	/* Filter the 9th bit, then return */
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | resl);
}
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
#endif
#if 1 //tick function definitions
enum States1 {init1, wait1, play1, length1, rest1, pause1};
int tickSM_play1(int state) {
	static unsigned short i, j, k;
	static unsigned char saveState;
	unsigned char rows = sizeof(Melody) / sizeof(Melody[0]);
	switch(state) {
		case init1:
		state= wait1;
		i = 0; j = 0; k = 0;
		break;

		case wait1:
		if (!pause) {
			state= play1;
			} else {
			state= wait1;
		}
		break;

		case play1:
			if (pause) {
				noTone3();
				saveState = state;
				state = pause1;
				break;
			}
		if (Melody[i][j] == -1) {
			i++;
			j = 0;
		}
		if (i == rows) {
			pause = 1;
			state = wait1;
			i = 0;
			j = 0;
			break;
		}
		tone3(Melody[i][j]);
		state = length1;
		break;

		case length1:
			if (pause) {
				noTone3();
				saveState = state;
				state = pause1;
				break;
			}
		if (k < Melody_L[i][j]) {
			k++;
			state = length1;
			} else {
			k = 0;
			noTone3();
			state = rest1;
		}
		break;

		case rest1:
			if (pause) {
				noTone3();
				saveState = state;
				state = pause1;
				break;
			}
		if (k < Melody_R[i][j]) {
			k++;
			state = rest1;
			} else {
			k = 0;
			j++;
			state = play1;
		}
		break;

		case pause1:
			if (pause) {
				state = pause1;
			} else {
				if (saveState == length1) {
					tone3(Melody[i][j]);
				}
				state = saveState;
			}
		break;
	}
	return state;
}

enum States2 {init2, wait2, play2, length2, rest2, pause2};
int tickSM_play2(int state) {
	static unsigned short i, j, k;
	static unsigned char saveState;
	unsigned char rows = sizeof(Harmony) / sizeof(Harmony[0]);
	switch(state) {
		case init2:
		state= wait2;
		i = 0; j = 0; k = 0;
		break;

		case wait2:
		if (!pause) {
			state= play2;
			} else {
			state= wait2;
		}
		break;

		case play2:
		if (pause) {
			noTone1();
			saveState = state;
			state = pause2;
			break;
		}
		if (Harmony[i][j] == -1) {
			i++;
			j = 0;
		}
		if (i == rows) {
			state = wait2;
			i = 0;
			j = 0;
			break;
		}
		tone1(Harmony[i][j]);
		state = length2;
		break;

		case length2:
		if (pause) {
			noTone1();
			saveState = state;
			state = pause2;
			break;
		}
		if (k < Harmony_L[i][j]) {
			k++;
			state = length2;
			} else {
			k = 0;
			noTone1();
			state = rest2;
		}
		break;

		case rest2:
		if (pause) {
			noTone1();
			saveState = state;
			state = pause2;
			break;
		}
		if (k < Harmony_R[i][j]) {
			k++;
			state = rest2;
			} else {
			k = 0;
			j++;
			state = play2;
		}
		break;

		case pause2:
			if (pause) {
				state = pause2;
			} else {
				if (saveState == length2) {
					tone1(Harmony[i][j]);
				}
				state = saveState;
			}
		break;
	}
	return state;
}

enum States3 {init3, wait3, play3, length3, rest3, pause3};
int tickSM_play3(int state) {
	static unsigned short i, j, k;
	static unsigned char saveState;
	unsigned char rows = sizeof(Bass) / sizeof(Bass[0]);
	switch(state) {
		case init3:
		state= wait3;
		i = 0; j = 0; k = 0;
		break;

		case wait3:
		if (!pause) {
			state= play3;
			} else {
			state= wait3;
		}
		break;

		case play3:
		if (pause) {
			noTone0();
			saveState = state;
			state = pause3;
			break;
		}
		if (Bass[i][j] == -1) {
			i++;
			j = 0;
		}
		if (i == rows) {
			state = wait3;
			i = 0;
			j = 0;
			break;
		}
		tone0(Bass[i][j]);
		state = length3;
		break;

		case length3:
		if (pause) {
			noTone0();
			saveState = state;
			state = pause3;
			break;
		}
		if (k < Bass_L[i][j]) {
			k++;
			state = length3;
			} else {
			k = 0;
			noTone0();
			state = rest3;
		}
		break;

		case rest3:
		if (pause) {
			noTone0();
			saveState = state;
			state = pause3;
			break;
		}
		if (k < Bass_R[i][j]) {
			k++;
			state = rest3;
			} else {
			k = 0;
			j++;
			state = play3;
		}
		break;

		case pause3:
			if (pause) {
				state = pause3;
			} else {
				if (saveState == length3) {
					tone0(Bass[i][j]);
				}
				state = saveState;
			}
		break;
	}
	return state;
}

enum pause_states {pause_init, toggle_press, toggle_rel};
int pause_tick(int state) {
	unsigned char button = PINA & 0x01;
	switch (state) {
		case pause_init:
			pause = 1;
			state = toggle_press;
		break;

		case toggle_press:
			if (remote == PlayPause) {
				state = toggle_rel;
				pause = !pause;
			} else {
				state = toggle_press;
			}
		break;
		
		case toggle_rel:
			if (remote == PlayPause) {
				state = toggle_rel;
			} else {
				state = toggle_press;
			}
		break;

		default: state = pause_init; break;
	}
	return state;
};

enum remote_states {receive};
int remote_tick(int state) {
	switch (state) {
		case receive:
			remote = USART_Receive();
			state = receive;
		break;
	}
};
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x0F; PORTD = 0x00;

#if 1 //task assignment
	unsigned char timerPeriod = 1;
	static task task1, task2, task3, task4, task5;
	task *tasks[] = { &task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks) / sizeof(*tasks);
	
	task1.state = 0;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &tickSM_play1;
	
	task2.state = 0;
	task2.period = 1;
	task2.elapsedTime = task2.period;
	task2.TickFct = &tickSM_play2;
	
	task3.state = 0;
	task3.period = 1;
	task3.elapsedTime = task3.period;
	task3.TickFct = &tickSM_play3;
	
	task4.state = 0;
	task4.period = 50;
	task4.elapsedTime = task4.period;
	task4.TickFct = &remote_tick;
	
	task5.state = 0;
	task5.period = 50;
	task5.elapsedTime = task5.period;
	task5.TickFct = &pause_tick;
	
#endif
	TimerSet(timerPeriod);
	TimerOn();
	PWM3_on(); PWM1_on(); PWM0_on();
	USART_Init(MYUBRR);

	while(1) {
#if 1 //task execution
		for (unsigned char i = 0; i < numTasks; i++) {
			if ( tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += timerPeriod;
		}
#endif
		while(!TimerFlag){};
		TimerFlag = 0;
	}
	

    return 1;
}
