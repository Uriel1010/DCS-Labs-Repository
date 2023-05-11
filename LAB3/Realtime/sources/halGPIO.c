#include "TFC.h"




//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
		InitGPIO();
		lcd_init();
		ClockSetupTPM();
		InitTPM(0); // Blue LED initialisation 
		InitTPM(2); // Red and Green LEDs initialisation
		InitPIT();
		PIT_MCR &= ~PIT_MCR_MDIS_MASK; //Enable the PIT module
		GIE();
		state = state0;
		MCG_C1 |= 0x2;
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
		 TPM0_off();
		 TPM2_off();
		 LCD_clear_move_to_first_line();
		 state = state1;
		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
    	  disable_pit();
    	  TPM2_off();
    	  LCD_clear_move_to_first_line();
		  lcd_puts("Measured frequency:");
		  start_TPM0();
		  state = state2;
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){
    	 disable_pit();
    	 TPM0_off();
    	 LCD_clear_move_to_first_line();
    	 start_TPM2();
		 state = state3;
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    	 state = state4;
    		 PBsArrIntPendClear(PB3_LOC);
    }
}


//-----------------------------------------------------------------
// PIT - ISR = Interrupt Service Routine
//-----------------------------------------------------------------
void PIT_IRQHandler(){
	PIT_TFLG1 = PIT_TFLG_TIF_MASK; //Turn off the Pit 0 Irq flag
	char string[5];
	if(timer_down >= 10){
		sprintf(string, "00:%d", timer_down);
		lcd_puts(&string);
		lcd_begin_first_line; // move to the begin of first line
		timer_down--;
	}
	else if(timer_down < 10){
			sprintf(string, "00:0%d", timer_down);
			lcd_puts(&string);
			lcd_begin_first_line; // move to the begin of first line
			if (timer_down != 0) {
				timer_down--;
			}
			else{
				disable_pit();  //PIT off
			}
	}
}

//-----------------------------------------------------------------
// PTM0 - FTM0 = Timer & PWM Module 
//-----------------------------------------------------------------
void FTM0_IRQHandler() {
	//TPM0_SC = 0; // to ensure that the counter is not running

	if(TPM0_SC & interupt_flag && counter_simple == 1){
		counter_overflow++;
		TPM0_SC |= interupt_flag;//overflow TPM0 flag down
	}
	  if(TPM0_C3SC & interupt_flag){
		  counter = c; //sampling
		  c = TPM0_CNT;
		  counter_simple++;
		  TPM0_C3SC |= interupt_flag; // interrupt flag of channel down
	  }
	
	TPM0_STATUS |= 0x1FF;
}

//-----------------------------------------------------------------
// PTM2 - FTM2 = Timer & PWM Module 
//-----------------------------------------------------------------
void FTM2_IRQHandler() {
	if(TPM2_SC & interupt_flag){
		    GPIOB_PDOR = off_set_zero;
			counter_overflow_2++;
			TPM2_SC |= interupt_flag;//overflow TPM0 flag down
		}
	if(TPM2_C0SC & interupt_flag){
			  GPIOB_PDOR = Leg_4;
			  TPM2_C0SC |= interupt_flag; // interrupt flag of channel down
		  }
	TPM2_STATUS |= 0x1FF;
}




