/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


//#include "derivative.h" /* include peripheral declarations */
#include "TFC.h"
#include  "..\Project_Headers\app.h"    		// private library - APP layer
#define MUDULO_REGISTER  0x2EE0

enum FSMstate state;
enum SYSmode lpm_mode;
unsigned int LightIntensity = MUDULO_REGISTER/2;  // Global variable
int timer_down = 59;
int counter = 0;
int counter_overflow = 0;
int counter_simple = 0;
long c = 0;
long c1 = 0;
float a;
char buffer[20];

int main(void){
	
	InitGPIO();
	lcd_init();
	ClockSetupTPM();
	InitTPM(0); // Blue LED initialisation 
	//InitTPM(2); // Red and Green LEDs initialisation
	InitPIT();
	
	//TPM2_SC |= TPM_SC_CMOD(1); //Start the TPM2 counter
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; //Enable the PIT module
	GIE();
	state = state0;
	MCG_C1 |= 0x2;



	while(1){
		switch(state){
		  case state0:
			 enterLPM(0x01);
			break;
			 
		  case state1:
			  timer_down = 59;
			  state = state0;
			  setPIT1_timer(0x16e3600); //starting ther pit timer
			break;
			 
		  case state2:
			  if(counter_simple > 2){
				  counter_simple = 0;
			  }
			  else if(counter_simple != 2){//checking if there are 2 samples exacly
				  continue;
			  }
			  else{
				  TPM0_C3SC = 0;  //turn off ch 3 interrupts
				  TPM0_SC = 0;  //turn off TPM interrupts and clock
				  
				  c1 = c - counter;
				  if(counter_overflow != 0){
						c1 += 65535*counter_overflow;
					}
				  a = 3000000/c1;
				  c1 = a;
				  counter_overflow = 0;
				  counter_simple = 0;
				  LCD_WAIT;
				  sprintf(buffer, "%d", c1);
				  lcd_cmd(0xC0); // move to the begin of second line
				  lcd_puts(&buffer);
				  lcd_puts("[Hz]    ");
				  delay(0xFFFFF);
				  delay(0xFFFFF);
				  TPM0_SC |= 0xCB;  //Start the TPM0 counter and interrupts
				  TPM0_C3SC = 0xC4; //Start channel 3 as inputcupture interrupt enable
			  }

			  
			  //TPM0_SC |= TPM_SC_CMOD(1);
			  //state = state2;
			  break;
			
		  case state3:
			  break;
			  
		  case state4:
			  return 0;
			  break;
		}
	 }
}


void PORTD_IRQHandler(void){
   
	delay(debounceVal);
if(PBsArrIntPend & PB0_LOC){
	     TPM0_C3SC = 0;
	     TPM0_SC = 0;
	   	 lcd_cmd(0x1); //clear scrren
	  	 lcd_cmd(0x80); // move to the begin of first line
		 state = state1;
		 PBsArrIntPendClear(PB0_LOC);
	}
    else if (PBsArrIntPend & PB1_LOC){
          PIT_TCTRL1 &= ~0x00000003; // disable PIT0 and its interrupt
		  lcd_cmd(0x1); //clear scrren
		  lcd_cmd(0x80); // move to the begin of first line
		  lcd_puts("Measured frequency:");
		  TPM0_SC |= 0xCB;  //Start the TPM0 counter
		  TPM0_C3SC = 0xC4; //Start channel 3 as inputcupture interrupt enable
		  state = state2;
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){	 
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
	sprintf(string, "00:%d", timer_down);
	lcd_puts(&string);
	lcd_cmd(0x80); // move to the begin of first line
	if (timer_down != 0) {
		timer_down--;
	}
	else{
		PIT_TCTRL1 &= ~0x00000003;  //PIT off
	}
}


void FTM0_IRQHandler() {
	//TPM0_SC = 0; // to ensure that the counter is not running

	if(TPM0_SC & 0x80 && counter_simple == 1){
		counter_overflow++;
		TPM0_SC |= 0x80;//overflow TPM0 flag down
	}
	  if(TPM0_C3SC & 0x80){
		  counter = c; //sampling
		  c = TPM0_CNT;
		  counter_simple++;
		  TPM0_C3SC |= 0x80; // interrupt flag of channel down
	  }
	
	TPM0_STATUS |= 0x1FF;
	}
	

