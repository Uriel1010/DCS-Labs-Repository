/*
` * main implementation: use this 'C' sample to create your own application
 *
 */


//#include "derivative.h" /* include peripheral declarations */
#include "TFC.h"
#include  "..\Project_Headers\app.h"    		// private library - APP layer
#define MUDULO_REGISTER  0x2EE0

enum FSMstate state;
enum SYSmode lpm_mode;
unsigned int LightIntensity = MUDULO_REGISTER/2;  // Global variable
int index = 0;
int Varry[100];
int VSimple;
int V_change = 0;
int VMax_array;
int VMin_array;
float Vmax = 0;
float Vmin = 4096;
float VmaxtoC;
float VmintoC;
int VmaxtoC_before_point;
int VmaxtoC_after_point;
int VmintoC_before_point;
int VmintoC_after_point;
char VmaxC[20];
char VminC[20];

int main(void){
	
	sysConfig();

	while(1){
		switch(state){
		  case state0:
			 enterLPM(wait_mode);
			break;
			 
		  case state1:
			  if(index == 100){
				  ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;
				  index = 0;
				  VMax_array = 0;
				  VMin_array = 4096;
				  int i;
				  for(i=0; i<100; i++){
					  if(VMax_array < Varry[i]){
						  VMax_array = Varry[i];
					  }
					  if(VMin_array > Varry[i]){
						  VMin_array = Varry[i];
					  }
				  }
			  
				  if (!((VMax_array - 10 < Vmax) && (Vmax < VMax_array + 10))){
					  Vmax = VMax_array;
					  V_change = 1;
				  }
				  if (!((VMin_array - 10 < Vmin) && (Vmin < VMin_array + 10))){
					  Vmin = VMin_array;
					  V_change = 1;
				  }
				  ADC0_SC1A |= ADC_SC1_AIEN_MASK;
			  }
			  if(V_change == 1){
				  ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;
				  VmaxtoC = (Vmax/4096)*3.37025;
				  VmintoC = (Vmin/4096)*3.37025;
				  VmaxtoC_before_point = VmaxtoC;
				  VmaxtoC_after_point = (VmaxtoC - VmaxtoC_before_point)*100;
				  
				  VmintoC_before_point = VmintoC;
				  VmintoC_after_point = (VmintoC - VmintoC_before_point)*100;
				  
				  sprintf(VminC, "Vmin = %d.%.2d [V]    ", VmintoC_before_point, VmintoC_after_point);
				  sprintf(VmaxC, "Vmax = %d.%.2d [V]    ", VmaxtoC_before_point, VmaxtoC_after_point);
				  LCD_clear_move_to_first_line();
				  lcd_puts(&VminC);
			  	  lcd_begin_second_line;
			  	  lcd_puts(&VmaxC);
			  	  V_change = 0;
			  	  ADC0_SC1A |= ADC_SC1_AIEN_MASK;  //POT channel is SE0 , ADC interrupt is enabled.
			  }
			 		  
			break;
			 
		  case state2:
			  break;
			
		  case state3:
			  break;
			  
		  case state4:
			  return 0;
			  break;
		}
	 }
}


