#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "..\Project_Headers\bsp.h"    		// private library - BSP layer
#include  "..\Project_Headers\app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);

void PWMon(void);
void PWMoff(void);

extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void toggleLEDs(char);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();

extern void PORTD_IRQHandler(void);

#endif







