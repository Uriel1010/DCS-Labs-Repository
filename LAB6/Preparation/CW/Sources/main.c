/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


//#include "derivative.h" /* include peripheral declarations */
# include "TFC.h"
#define MUDULO_REGISTER  0x2EE0
#define ADC_MAX_CODE     4095

unsigned int LightIntensity = MUDULO_REGISTER/2;  // Global variable
enum FSMstate state;
enum FSMstate state_assist;
enum SYSmode lpm_mode;
int X = 0xB71B00;
int up_count = 0;
int down_count = 65535; 
char lcd_count[20];
int flag_4 = 0;
int multiply = 0;
int TempX = 0;
int Xarry[10];
float VSimple = 0;
float VSimple_old = 0;
float VtoC;
int VtoC_before_point;
int VtoC_after_point;
char VC[20];
int index = 0;
int Varry[100];
float V_sum;




int main(void){

	sysConfig();
	state = state0;
	state_assist = state0;

	while(1){
		switch(state){
		  case state0:
			 enterLPM(wait_mode);
			break;
			 
		  case state1:
			  state_assist = state1;
			  set_blue_light;
			  state = state0;
			break;
			 
		  case state2:
			  state_assist = state2;
			  turn_off_RGB;
			  LCD_clear_move_to_first_line();
			  sprintf(lcd_count, "up count: %d", up_count++);
			  lcd_puts(&lcd_count);
			  state = state0;
			  break;
			
		  case state3:
			  state_assist = state3;
			  turn_off_RGB;
			  LCD_clear_move_to_first_line();
			  sprintf(lcd_count, "downcount: %d", down_count--);
			  lcd_puts(&lcd_count);
			  state = state0;
			  break;
			  
		  case state4:
			  state_assist = state4;
			  disable_pit();
			  turn_off_RGB;
			  int i = 0;
			  if(flag_4 == 0){
				while(Xarry[i] != 10){
					TempX += ((Xarry[i]) * (pow(10,i)));
					i++;
				}
				set_X(TempX);
				state = state0;
			  }
			  break;
			  
		  case state5:
			  if(index == 100){
				  ADC0_off();
				  index = 0;
				  int i;
				  for(i=0; i<100; i++){
					  V_sum += Varry[i];
				  }
				  V_sum = V_sum/100;
				  if((V_sum > VSimple_old + 10) || (V_sum < VSimple_old - 10)){
					  VSimple_old = V_sum;
					  VtoC = (V_sum/4096)*3.37025;
					  VtoC_before_point = VtoC;
					  VtoC_after_point = (VtoC - VtoC_before_point)*1000;
					  
					  sprintf(VC, "V = %d.%.3d [V] ", VtoC_before_point, VtoC_after_point);
					  LCD_clear_move_to_first_line();
					  lcd_puts(&VC);
				  }
				  ADC0_on();
			  }
			  break;
			  
		  case state6:
			  disable_pit();
			  turn_off_RGB;
			  state_assist = state6;
			  LCD_clear_move_to_first_line();
			  up_count = 0;
			  down_count = 65535; 
			  state = state0;
		  	 break;
		  	 
		  case state7:
			 disable_pit();
			 turn_off_RGB;
			 state_assist = state7;
			 state = state0;
		  	 break;
		  			  
		  case state8:
			disable_pit();
			turn_off_RGB;
			state_assist = state8;
			state = state0;
		  	break;
		  	
		  case state9:
			  return 0;
			  break;
			
		}
	 }
}

void set_X(int x){
	X = x*24000;
}



