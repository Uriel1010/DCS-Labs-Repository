#ifndef _bsp_H_
#define _bsp_H_

#include "derivative.h"  // contains the inclusion of <MKL25Z4.h>
// #include  <msp430g2553.h>          // MSP430x2xx
// #include  <msp430xG46x.h>          // MSP430x4xx

#define   debounceVal      250
#define   LEDs_SHOW_RATE   0xFFFFF  // 21_8ms
#define   Dutycycle        0x0020C
#define   Duty75           0x00189
#define   Duty25           0x00083
#define   PORT_LOC(x)        ((uint32_t)(1<<x))
//------------------------------------------------------------------
//  					LEDs abstraction
//------------------------------------------------------------------
#define LED0_LOC				PORT_LOC(0)
#define LED1_LOC				PORT_LOC(1)
#define LED2_LOC				PORT_LOC(2)
#define LED3_LOC				PORT_LOC(3)
#define LED4_LOC				PORT_LOC(4)
#define LED5_LOC				PORT_LOC(5)
#define LED6_LOC				PORT_LOC(6)
#define LED7_LOC				PORT_LOC(7)
#define LEDsArr_LOC             LED0_LOC|LED1_LOC|LED2_LOC|LED3_LOC|LED4_LOC|LED5_LOC|LED6_LOC|LED7_LOC

 // there is need to mask the LEDs array location in 32-bit PORT space
#define LEDsArrPort             GPIOC_PDOR
#define LEDsArrPortSet(x)		GPIOC_PSOR = x & LEDsArr_LOC
#define LEDsArrPortClear(x)	    GPIOC_PCOR = x & LEDsArr_LOC
#define LEDsArrPortToggle(x)	GPIOC_PTOR = x & LEDsArr_LOC
#define LEDsArrPortWrite(x)	    LEDsArrPort = x & LEDsArr_LOC 
// #define LEDsArrPortWrite(x)	    GPIOC_PDOR = LEDsArrPortSet(x),GPIOC_PDOR = LEDsArrPortSet(~x)

#define LEDsArrPortDir     		GPIOC_PDDR
#define LEDsArrPortSel     		PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK | PORT_PCR_SRE_MASK
//-----------------------------------------------------------------------
//                    Switches abstraction
//-----------------------------------------------------------------------
#define SWsArrPort         GPIOD_PDIR 
#define SWsArrPortDir      GPIOD_PDDR
#define SWsArrPortSel      PORT_PCR_MUX(1)+ PORT_PCR_IRQC(0x00)
#define SWIsAnSOUT         GPIOD_PDOR

#define SW0_LOC			PORT_LOC(4)
#define SW1_LOC			PORT_LOC(5)
#define SW2_LOC			PORT_LOC(6)
#define SW3_LOC			PORT_LOC(7)
#define SWsArr_LOC      SW0_LOC|SW1_LOC|SW2_LOC|SW3_LOC
#define SWsArrReadShift 4
#define SWsArrVal       (SWsArrPort >> SWsArrReadShift) & SWsArr_LOC

//------------------------------------------------------------------------
//					 PushButtons abstraction
//-------------------------------------------------------------------------
#define PB0_LOC		PORT_LOC(0)
#define PB1_LOC		PORT_LOC(1)	
#define PB2_LOC		PORT_LOC(2)
#define PB3_LOC		PORT_LOC(3)
#define PBsArr_LOC  PB0_LOC|PB1_LOC|PB2_LOC|PB3_LOC


#define PBsArrPort	       		GPIOD_PDOR 
#define PBsArrIntPend	    	PORTD_ISFR   
#define PBsArrIntPendClear(x)	PORTD_ISFR |= x  // using clear with PBi_LOC 
// #define PBsArrIntEn	       		
#define PBsArrIntEdgeSel(x) 	PORT_PCR_IRQC(x)
#define PULL_UP            		0x0a
#define PULL_DOWN           	0x09
#define PBsArrPortSel      		PORT_PCR_MUX(1)| PORT_PCR_PFE_MASK 
#define PBsArrPortDir      		GPIOD_PDDR 
//---------------------------------------------------------------------------

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);

#endif /* TFC_bspGPIO_H_ */
