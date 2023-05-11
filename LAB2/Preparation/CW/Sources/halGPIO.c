#include  "..\Project_Headers\halGPIO.h"     // private library - HAL layer

//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
}
void PWMon(void){
	GPIOD_PDDR = 0x80;
	GPIOD_PDOR = 0x80;
}	
	
void PWMoff(void){
		GPIOD_PDDR = 0x00;
		GPIOD_PDOR = 0x00;
}
//--------------------------------------------------------------------
// 				Print Byte to 8-bit LEDs array 
//--------------------------------------------------------------------
void print2LEDs(unsigned char ch){
	GPIOC_PDOR = ch & 0xFF;
	// LEDsArrPortWrite(ch);
}    
//--------------------------------------------------------------------
//				Clear 8-bit LEDs array 
//--------------------------------------------------------------------
void clrLEDs(void){
	LEDsArrPortClear(0x00);
}  
//--------------------------------------------------------------------
//				Toggle 8-bit LEDs array 
//--------------------------------------------------------------------
void toggleLEDs(char ch){
	LEDsArrPortToggle(ch);
}
//--------------------------------------------------------------------
//				Set 8-bit LEDs array 
//--------------------------------------------------------------------
void setLEDs(char ch){
	LEDsArrPortSet(ch);
}
//--------------------------------------------------------------------
//				Read value of 4-bit SWs array 
//--------------------------------------------------------------------
unsigned char readSWs(void){
	unsigned char ch=0;
	
	ch = (GPIOD_PDIR>>4) & 0xF;
	// ch = SWsArrVal;  // mask the SWs 4-bit location
	     
	return ch;
}
//---------------------------------------------------------------------
//             Increment / decrement LEDs shown value 
//---------------------------------------------------------------------
void incLEDs(char val){
	
	GPIOC_PDOR = (GPIOC_PDOR + val) & 0xFF;
	// LEDsArrPort = (LEDsArrPort + val) & LEDsArr_LOC;
	
}
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
		stop();    /* Enter Low Power Mode 0 */
    else if(LPM_level == 0x01) 
    	wait();      /* Enter Low Power Mode 1 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
	GIE();        // Enable Interrupts
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
	GID();        // Disable Interrupts
}
//*********************************************************************
//            PORTD Interrupt Service Routine - uses for PBs array
//*********************************************************************
void PORTD_IRQHandler(void){
   
	delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	if(PBsArrIntPend & PB0_LOC){
		 state = state1;
		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
		 state = state2;
		 PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){	 
		 state = state0;
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    		 
    		 PBsArrIntPendClear(PB3_LOC);
    }
//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
    /* switch(lpm_mode){
     	 case mode0:
     	   exitSleep(); // must be called from ISR only
     	   break;
		 
		default: 
		  exitSleep(); // must be called from ISR only
		  break;
	}
     */   
}
 
