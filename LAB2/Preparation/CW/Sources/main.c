#include  "..\Project_Headers\api.h"    		// private library - API layer
#include  "..\Project_Headers\app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
int whereIwas = 0;

void main(void){
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  int id[] = {3,1,2,6,1,1,1,9,7};
  int ledmove[] = {1,2,4,8,16,32,64,128};
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
		 print2LEDs(0);
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
		disable_interrupts();
		int i=0;
		for(i=0; i <= 8; i++){ 
			print2LEDs(id[i]);
			delay(LEDs_SHOW_RATE);	// delay of 50 [ms] 
		}
		enable_interrupts();
		state = state0;
		break;
		 
	  case state2:
		disable_interrupts();
		int j = 0;
		while(1){ 
			print2LEDs(ledmove[whereIwas%8]);
			delay(LEDs_SHOW_RATE);	// delay of 50 [ms]
			j++;
			whereIwas++;
			if(j == 13){
				break;
			}
		}
		enable_interrupts();
		state = state0;
		break;
		
	  case state3:
  		  PWMon();
  		  delay(Duty75);
  		  PWMoff();
  		  delay(Duty25);
		  break;
		
	}
  }
}
  
  
  
  
  
  
