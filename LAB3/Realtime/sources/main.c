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
int counter_overflow_2 = 0;
int counter_simple = 0;
long c = 0;
long c1 = 0;
float a;
char buffer[20];

int main(void){
	
	sysConfig();

	while(1){
		switch(state){
		  case state0:
			 enterLPM(wait_mode);
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
				  TPM0_off();
				  c1 = c - counter;
				  if(counter_overflow != 0){
						c1 += 65536*counter_overflow;
					}
				  a = 6000000/c1;
				  c1 = a;
				  counter_overflow = 0;
				  counter_simple = 0;
				  LCD_WAIT;
				  sprintf(buffer, "%d", c1);
				  lcd_begin_second_line; // move to the begin of second line
				  lcd_puts(&buffer);
				  lcd_puts("[Hz]    ");
				  delay(0xFFFFF);
				  delay(0xFFFFF);
				  start_TPM0(); //Start channel 3 as inputcupture interrupt enable
			  }
			  break;
			
		  case state3:
			  if(counter_overflow_2 < 1000){
				  setTPM2_C0V(0x1770);
			  }
			  else if(counter_overflow_2 >= 1000 && counter_overflow_2 < 2000){
				  setTPM2_C0V(0x2EE0);
			  }
			  else if(counter_overflow_2 >= 2000 && counter_overflow_2 < 3000){
				  setTPM2_C0V(0x4650);
			  }
			  else{
				  counter_overflow_2 = 0;
			  }
			  
			  	  
			  break;
			  
		  case state4:
			  return 0;
			  break;
		}
	 }
}


