#ifndef _halGPIO_H_
#define _halGPIO_H_


#include "TFC.h"   		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable


extern float Vmax;
extern float Vmin;
extern int VSimple;
extern int V_change;
extern float VmaxtoC;
extern float VmintoC;
extern char VmaxC[20];
extern char VminC[20];
extern int index;
extern int Varry[100];

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
extern void ADC0_IRQHandler();

#endif







