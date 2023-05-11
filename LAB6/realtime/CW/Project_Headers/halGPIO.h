#ifndef _halGPIO_H_
#define _halGPIO_H_


#include "TFC.h"   		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable
extern enum FSMstate state_assist;


extern int X;
extern int up_count;
extern int down_count; 
extern char lcd_count[20];
extern int flag_4;
extern int multiply;
extern int TempX;
extern int Xarry[10];
extern float VSimple;
extern float VSimple_old;
extern int index;
extern int Varry[100];

extern void set_X(int x);
extern void sysConfig(void);
extern void enterLPM(unsigned char LPM_level);
extern void ADC0_on();
extern void ADC0_off();

extern void enterLPM(unsigned char LPM_level);
extern void setPIT1_timer(int a);
extern void setTPM2_C0V(int a);
extern void delay(unsigned int t);
extern void setPIT1_timer(int a);
extern int keyboard1to4();
extern void keyboard();


extern void PORTD_IRQHandler(void);
extern void PORTA_IRQHandler(void);
extern void PIT_IRQHandler();
extern void FTM0_IRQHandler();
extern void FTM2_IRQHandler();
extern void ADC0_IRQHandler();

#endif







