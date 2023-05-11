#include "TFC.h"




//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
		InitGPIO();
		lcd_init();
		InitADCs();
		InitDAC();
		GIE();
		state = state0;
}


void enterLPM(unsigned char LPM_level){
	if (LPM_level == stop_mode) 
		stop();    /* Enter Low Power Mode 0 */
    else if(LPM_level == wait_mode) 
    	wait();      /* Enter Low Power Mode 1 */
}


void setPIT1_timer(int a){
	disable_pit(); //disable PIT0 and its interrupt
	PIT_LDVAL1 = a; // setup timer a for a counting period
	PIT_TCTRL1 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	
}

void setTPM2_C0V(int a){
	TPM2_C0V  = a; // setup timer a for a counting period
	
}

void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}

void TPM0_off(){
    TPM0_C3SC = off_set_zero;
    TPM0_SC = off_set_zero;
}
void TPM2_off(){
    TPM2_C0SC = off_set_zero;
    TPM2_SC = off_set_zero;
}



void disable_pit(){
	disable_pit_module; // disable PIT0 and its interrupt
}

void start_TPM0(){
	  TPM0_SC |= TOF_TOIE_CMOD01_PS4;  //Start the TPM0 counter
	  TPM0_C3SC = Cflag_IE_inputcupture; //Start channel 3 as inputcupture interrupt enable
}

void start_TPM2(){
    TPM2_SC = TOF_TOIE_CMOD01; 
    TPM2_C0SC = Cflag_IE_outputcomper;
}
//-----------------------------------------------------------------
// PORTD = Interrupt Service Routine
//-----------------------------------------------------------------
void PORTD_IRQHandler(void){
   
	delay(debounceVal);
	if(PBsArrIntPend & PB0_LOC){
		 //setPIT1_timer(0x16e3600); //starting ther pit timer to 1 sec
		 ADC0_SC3 |= 0xC; // continus convertion
		 ADC0_SC2 &= ~0x20;
		 Vmax = 0;
		 Vmin = 4096;
		 ADC0_SC1A = ADC_SC1_AIEN_MASK; // start ADC0
		 state = state1;
		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
    	  LCD_clear_move_to_first_line();
    	  ADC0_SC3 |= 0xC; // continus convertion
    	  ADC0_SC1A = ADC_SC1_AIEN_MASK; // start ADC0
		  state = state2;
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){

    	 //LCD_clear_move_to_first_line();
		 state = state3;
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    	 state = state4;
    		 PBsArrIntPendClear(PB3_LOC);
    }
}


//-----------------------------------------------------------------
// ADC0 = Analog Too Digital 
//-----------------------------------------------------------------
void ADC0_IRQHandler(){
	VSimple = ADC0_RA;
	if (state == state1){
		Varry[index] = VSimple;
		index++;
	}
//		if(VSimple > Vmax + 12){
//			Vmax = VSimple;
//			V_change = 1;
//		}
//		else if (VSimple < Vmin - 12){
//			Vmin = VSimple;
//			V_change = 1;
//		}
	else if(state == state2){
		GPIOE_PDOR = 0x200000 ^ GPIOE_PDOR;
	}
	
}


