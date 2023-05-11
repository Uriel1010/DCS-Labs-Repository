#include "TFC.h"




//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
		InitGPIO();
		lcd_init();
		InitADCs();
		//InitDAC();
		InitPIT();
		ClockSetupTPM();
		InitTPM(2);
		InitUARTs();
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

void enable_PortD_interapt(){
	enable_irq(INT_PORTD-16);
}

void disable_PortD_interapt(){
	disable_irq(INT_PORTD-16);
}

void ADC0_on(){
	 ADC0_SC3 |= 0xC; // continus convertion
	 ADC0_SC2 &= ~0x20;
	 ADC0_SC1A = ADC_SC1_AIEN_MASK; // start ADC0
}

void ADC0_off(){
	ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;
}

void disable_pit(){
	disable_pit_module; // disable PIT0 and its interrupt
}

void start_TPM0(){
	  TPM0_SC |= TOF_TOIE_CMOD01_PS4;  //Start the TPM0 counter
	  TPM0_C3SC = Cflag_IE_inputcupture; //Start channel 3 as inputcupture interrupt enable
}

void start_TPM2(){
    TPM2_SC = CMOD01_DMA; 
    TPM2_C0SC = Cflag_PWM_cflag;
}


void PIT_IRQHandler(void){
	if(state_assist == state1){
		if(GPIOB_PDOR & 0x2){
			set_blue_light;
		}
		else{
			shift_b_for_next_light;
		}
	}
	else if(state_assist == state2) {
		state = state2;
	}
	else{
		state = state3;
	}
	clear_pit_flag;
}

//-----------------------------------------------------------------
//  UART0 - ISR
//-----------------------------------------------------------------
void UART0_IRQHandler(){
	
	uint8_t Temp;
		
	if(UART0_S1 & UART_S1_RDRF_MASK){ // RX buffer is full and ready for reading
		Temp = UART0_D;
		if(flag_4 == 0){
			switch(Temp){
				case '1':
					disable_PortD_interapt();
					ADC0_off();
					state = state1;
					setPIT1_timer(X);
					break;
				case '2':
					disable_PortD_interapt();
					ADC0_off();
					turn_off_RGB;
					state = state2;
					setPIT1_timer(X);
					break;
				case '3':
					disable_PortD_interapt();
					ADC0_off();
					turn_off_RGB;
					state = state3;
					setPIT1_timer(X); 
					break;
				case '4':
					disable_PortD_interapt();
					disable_pit();
					ADC0_off();
					turn_off_RGB;
					TempX = 0;
					flag_4 = 1;
					state = state4;
					break;
				case '5':
					disable_PortD_interapt();
					disable_pit();
					turn_off_RGB;
					ADC0_on();
					state = state5;
					break;
				case '6':
					disable_PortD_interapt();
					ADC0_off();
					disable_pit();
					turn_off_RGB;
					state = state6;
					break;
				case '7':
					disable_PortD_interapt();
					ADC0_off();
					disable_pit();
					turn_off_RGB;
					state = state7;
					break;
				case '8':
					disable_PortD_interapt();
					ADC0_off();
					disable_pit();
					turn_off_RGB;
					state = state8;
					break;
				case '9':
					ADC0_off();
					disable_pit();
					turn_off_RGB;
					enable_PortD_interapt();
					state = state0;
					//UART0_C2 |= UART_C2_TIE_MASK;
			}
		}
		else{
			if(Temp != 10){
				Xarry[multiply] = Temp - 48; // -48 for 0 in ascii
				multiply++;
					
			}
			else{
				Xarry[multiply] = Temp;
				flag_4 = 0;
				multiply = 0;
			}
		}
	
	}
//	else if(UART0_S1 & UART_S1_TDRE_MASK){ // TX buffer is empty and ready for sending
//		
//		UART0_D = Temp;
//	}
}

//-----------------------------------------------------------------
// ADC0 = Analog Too Digital 
//-----------------------------------------------------------------
void ADC0_IRQHandler(){
	VSimple = ADC0_RA;
	Varry[index] = VSimple;
	index++;
}

void PORTD_IRQHandler(void){
   
	delay(7000);
	if(PBsArrIntPend & PB0_LOC){

		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
    	  UARTprintf(UART0_BASE_PTR,"I love my Negev \n");
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){
      	 
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    	
    		 PBsArrIntPendClear(PB3_LOC);
    }
}
	


