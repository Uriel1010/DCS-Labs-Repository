#ifndef _halGPIO_H_
#define _halGPIO_H_


#include "TFC.h"   		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable


extern short recorder[32];
extern short recorder2[32];
extern int index_rec;
extern short song1[];
extern short song2[];
extern short song3[];
extern char lcd_done_rec[20];
extern unsigned int screen;
extern short MOD_oc[13];
extern short song12[];
extern short song22[];
extern short song32[];
extern int size_of_song1;
extern int size_of_song2;
extern int size_of_song3;
extern int size_currnet;
extern int on;


extern void sysConfig(void);
extern void enterLPM(unsigned char LPM_level);

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







