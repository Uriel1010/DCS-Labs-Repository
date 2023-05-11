#include  "..\Project_Headers\bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
	//enable Clocks to all ports
	
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	// LEDs 8-bit Array Port configuration
	PORTC_PCR0 = LEDsArrPortSel;  // PTB.0=LED0
	PORTC_PCR1 = LEDsArrPortSel;  // PTB.1=LED1
	PORTC_PCR2 = LEDsArrPortSel;  // PTB.2=LED2
	PORTC_PCR3 = LEDsArrPortSel;  // PTB.3=LED3
	PORTC_PCR4 = LEDsArrPortSel;  // PTB.8=LED4
	PORTC_PCR5 = LEDsArrPortSel;  // PTB.9=LED5
	PORTC_PCR6 = LEDsArrPortSel;  // PTB.10=LED6
	PORTC_PCR7 = LEDsArrPortSel;  // PTB.11=LED7
	
	LEDsArrPortDir |= LEDsArr_LOC;  //Setup PB.0-PB.3 and PB.8-PB.11 as GPIO output
	
	// Switches SW3-SW0 Setup
	PORTD_PCR4 = SWsArrPortSel; // PTD.4=SW0
	PORTD_PCR5 = SWsArrPortSel; // PTD.5=SW1
	PORTD_PCR6 = SWsArrPortSel; // PTD.6=SW2
	PORTD_PCR7 = SWsArrPortSel; // PTD.7=SW3
	SWsArrPortDir &= ~SWsArr_LOC; 
	SWIsAnSOUT |= SW3_LOC;
	
	// PushButtons Setup
	PORTD_PCR0 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.0=PB0 + Interrupt request enable
	PORTD_PCR1 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.1=PB1 + Interrupt request enable
	PORTD_PCR2 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_DOWN); // PTD.2=PB2 + Interrupt request enable
	PORTD_PCR3 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_DOWN); // PTD.3=PB3 + Interrupt request enable
	PBsArrPortDir &= ~PBsArr_LOC; 
	PBsArrIntPendClear(PBsArr_LOC);
	
	enable_irq(INT_PORTD-16);           // Enable PORTD Interrupts 
	set_irq_priority (INT_PORTD-16,0);  // PORTD Interrupt priority = 0 = max
	
}
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	
	//write here timers congiguration code
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	//write here ADC congiguration code
}              

