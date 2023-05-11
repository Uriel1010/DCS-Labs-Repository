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
  float presentege[] ={0,0.25,0.5,1};
  sysConfig();
  int SWS;
  
  while(1){
	switch(state){
	  case state0:
		 print2LEDs(0);
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
		PWMoff();
		disable_interrupts();
		int i=0;
		for(i=0; i <= 8; i++){ 
			print2LEDs(id[i]);
			delay(LEDs_SHOW_RATE);	// delay of 50 [ms] 
		}
		state = state0;
		enable_interrupts();
		break;
		 
	  case state2:
		disable_interrupts();
		PWMoff();
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
		state = state0;	
		enable_interrupts();
		break;
		
	  case state3:
		  PWMoff();
		  PWMonoff();
  		  delay(Duty75);
  		  PWMonoff();
  		  delay(Duty25);
		  break;
	  case state4:
		  SWS = readSWs();
		  if(SWS == 4){
			  PWMon();
		  }
		  else if(SWS == 2 || SWS == 3){	
			  PWMonoff();
			  delay(khz2*presentege[SWS-1]*2);
			  PWMonoff();
			  delay(khz2*(1-presentege[SWS-1])*2);
		  }
		  else{
			  PWMoff();
		  }
		  break;
	}
  }
}
  
  
  
  
  
  
