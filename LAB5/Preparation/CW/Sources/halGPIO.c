#include "TFC.h"




//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
		InitGPIO();
		lcd_init();
		//InitADCs();
		//InitDAC();
		InitPIT();
		ClockSetupTPM();
		InitTPM(2);
		dma_init(0);
		dma_init(1);
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
    TPM2_SC = CMOD01_DMA; 
    TPM2_C0SC = Cflag_PWM_cflag;
}
void keyboard(){
	int buttom = -1;
	keyboard_out_(row1);
	delay(1);
	if((keyboard_out_check_column_(column1)) == 0) buttom = 1;
	else if((keyboard_out_check_column_(column2))== 0) buttom = 2;
	else if((keyboard_out_check_column_(column3))== 0) buttom = 3;
	else if((keyboard_out_check_column_(column4))== 0) buttom = 10;
	
	keyboard_out_(row2);
	delay(1);
	if((keyboard_out_check_column_(column1)) == 0) buttom = 4;
	else if((keyboard_out_check_column_(column2)) == 0) buttom = 5;
	else if((keyboard_out_check_column_(column3)) == 0) buttom = 6;
	else if((keyboard_out_check_column_(column4)) == 0) buttom = 11;
	
	keyboard_out_(row3);
	delay(1);
	if((keyboard_out_check_column_(column1)) == 0) buttom = 7;
	else if((keyboard_out_check_column_(column2)) == 0) buttom = 8;
	else if((keyboard_out_check_column_(column3)) == 0) buttom = 9;
	else if((keyboard_out_check_column_(column4)) == 0) buttom = 12;
	
	keyboard_out_(row4);
	delay(1);
	if((keyboard_out_check_column_(column1)) == 0) buttom = 15;
	else if((keyboard_out_check_column_(column2)) == 0) buttom = 0;
	else if((keyboard_out_check_column_(column3)) == 0) buttom = 14;
	else if((keyboard_out_check_column_(column4)) == 0) buttom = 13;
	if(buttom == -1){
		lcd_begin_second_line;
		lcd_puts("Error bottm");
	}
	else if(buttom < 13){
		recorder[index_rec] = MOD_oc[buttom];
		recorder2[index_rec] = MOD_oc[buttom]/2;
		index_rec++;
		keyboard_out_off;
	}
}

int keyboard1to4(){
	int buttom1to4 = -1;
		keyboard_out_(row1);
		delay(1);
		if((keyboard_out_check_column_(column1)) == 0) buttom1to4 = 1;
		else if((keyboard_out_check_column_(column2))== 0) buttom1to4 = 2;
		else if((keyboard_out_check_column_(column3))== 0) buttom1to4 = 3;
		else if((keyboard_out_check_column_(column4))== 0) buttom1to4 = 10;
		
		keyboard_out_(row2);
		delay(1);
		if((keyboard_out_check_column_(column1)) == 0) buttom1to4 = 4;
		else if((keyboard_out_check_column_(column2)) == 0) buttom1to4 = 5;
		else if((keyboard_out_check_column_(column3)) == 0) buttom1to4 = 6;
		else if((keyboard_out_check_column_(column4)) == 0) buttom1to4 = 11;
		
		keyboard_out_(row3);
		delay(1);
		if((keyboard_out_check_column_(column1)) == 0) buttom1to4 = 7;
		else if((keyboard_out_check_column_(column2)) == 0) buttom1to4 = 8;
		else if((keyboard_out_check_column_(column3)) == 0) buttom1to4 = 9;
		else if((keyboard_out_check_column_(column4)) == 0) buttom1to4 = 12;
		
		keyboard_out_(row4);
		delay(1);
		if((keyboard_out_check_column_(column1)) == 0) buttom1to4 = 15;
		else if((keyboard_out_check_column_(column2)) == 0) buttom1to4 = 0;
		else if((keyboard_out_check_column_(column3)) == 0) buttom1to4 = 14;
		else if((keyboard_out_check_column_(column4)) == 0) buttom1to4 = 13;
		keyboard_out_off;
		return buttom1to4;
	
}
//-----------------------------------------------------------------
// PORTD = Interrupt Service Routine
//-----------------------------------------------------------------
void PORTD_IRQHandler(void){
   
	delay(debounceVal);
	if(PBsArrIntPend & PB0_LOC){
		 TPM2_off();
		 index_rec = 0;
		 LCD_clear_move_to_first_line();
		 lcd_puts("now recording");
		 state = state1;
		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
    	  on = 0;
		  state = state2;
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){
    	 screen = screen ^ 0x1;
    	 on = 0;
		 //state = state3;
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    		state = state4;
    		PBsArrIntPendClear(PB3_LOC);
    }
}


void PORTA_IRQHandler(void){
	int to4 = 0;
	delay(debounceVal);
	if(state == state1){
		if(index_rec < 32){
			keyboard();
		}
		else{
			state = state4;
		}
	}
	else if(state == state2){
		to4 = keyboard1to4();
		switch(to4){
		case 1:
			DMA_SAR0 = (uint32_t)&song1;
			DMA_SAR1 = (uint32_t)&song12;
			size_currnet = (uint32_t)&song1 + size_of_song1;
			start_TPM2();
			setPIT1_timer(0x7E57C0); // set pit timer to inturrupt every 345ms 
			break;
		case 2:
			DMA_SAR0 = (uint32_t)&song2;
			DMA_SAR1 = (uint32_t)&song22;
			size_currnet = (uint32_t)&song2 + size_of_song2;
			start_TPM2();
			setPIT1_timer(0x7E57C0); // set pit timer to inturrupt every 345ms 
			break;
		case 3:
			DMA_SAR0 = (uint32_t)&song3;
			DMA_SAR1 = (uint32_t)&song32;
			size_currnet = (uint32_t)&song3 + size_of_song3;
			start_TPM2();
			setPIT1_timer(0x7E57C0); // set pit timer to inturrupt every 345ms 
			break;
		case 4:
			DMA_SAR0 = (uint32_t)&recorder;
			DMA_SAR1 = (uint32_t)&recorder2;
			size_currnet = (uint32_t)&recorder + index_rec*sizeof(short);
			start_TPM2();
			setPIT1_timer(0x7E57C0); // set pit timer to inturrupt every 345ms 
			break;
		}
	}
	
	clear_key_board_flag;

}
void PIT_IRQHandler(void){
	clear_DMA_BCR0_flag;
	clear_DMA_BCR1_flag;
	if(!(size_currnet == DMA_SAR0)){
		add_2_for_DMA_BCR0_transfer;
		add_2_for_DMA_BCR1_transfer;
	}
	else{
		TPM2_off();
	}
	DMA_DCR0 |= DMA_DCR_START_MASK; //start transfer data
	DMA_DCR1 |= DMA_DCR_START_MASK; //start transfer data
	clear_pit_flag;
	
	
}


//-----------------------------------------------------------------
// ADC0 = Analog Too Digital 
//-----------------------------------------------------------------
//void ADC0_IRQHandler(){
//	return;
//}


