#ifndef _halGPIO_H_
#define _halGPIO_H_


#include "TFC.h"   		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern int timer_down;
extern int counter;
extern int counter_overflow;
extern int counter_overflow_2;
extern int counter_simple;
extern long c;
extern long c1;
extern float a;
extern char buffer[20];

extern void sysConfig(void);
extern void enterLPM(unsigned char LPM_level);

extern void enterLPM(unsigned char LPM_level);
extern void setPIT1_timer(int a);
extern void setTPM2_C0V(int a);
extern void delay(unsigned int t);
extern void setPIT1_timer(int a);

extern void PORTD_IRQHandler(void);
extern void PIT_IRQHandler();
extern void FTM0_IRQHandler();
extern void FTM2_IRQHandler();

#endif







