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
short song1[] = {2,7,7,7,2,4,4,2,11,11,9,9,7,2,7,7,7,2,4,4,2,11,11,9,9,7,2,7,7,7,2,7,7,7,7,7,7,2,7,7,7,2,7,7,7,2,4,4,2,11,11,9,9,7};
short song2[] = {0,5,9,0,5,9,9,7,7,7,7,9,10,10,10,12,10,9,7,9,9,9,9,5,7,9,10,10,10,12,10,10,9,5,9,12,12,0,4,7,10,9,7,5,5,5,5};
short song3[] = {4,7,4,4,7,4,12,11,9,7,5,2,5,9,7,7,5,4,2,0,4,7,4,4,7,4,12,11,9,7,5,2,5,9,7,7,5,4,2,0};
int size_of_song1 = sizeof(song1);
int size_of_song2 = sizeof(song2);
int size_of_song3 = sizeof(song3);
int size_currnet = 0;
short song12[sizeof(song1)/sizeof(song1[0])];
short song22[sizeof(song2)/sizeof(song2[0])];
short song32[sizeof(song3)/sizeof(song3[0])];
short recorder[32];
short recorder2[32];
short MOD_oc[13];
int octav6[] = {1046,1108,1174,1244,1318,1397,1480,1568,1661,1760,1864,1975,2093};
int index_rec = 0;
int on = 0;
char lcd_done_rec[20];
unsigned int screen = 0x0;  


int main(void){
	
	sysConfig();
	convert_6octav();
	translate_song_mod();
	state = state0;

	while(1){
		switch(state){
		  case state0:
			 enterLPM(wait_mode);
			break;
			 
		  case state1:
			break;
			 
		  case state2:
			  if((screen == 0x0) && (on == 0)){
				  LCD_clear_move_to_first_line();
				  lcd_puts("1: song1");
				  lcd_begin_second_line;
			  	  lcd_puts("2: song2");
			  }
			  if((screen == 0x1) && (on == 0)){
				  LCD_clear_move_to_first_line();
				  lcd_puts("3: song3");
				  lcd_begin_second_line;
				  lcd_puts("4: recorder");
			  }
			  on = 1;
			  break;
			
		  case state3:
			  break;
			  
		  case state4:
	    		LCD_clear_move_to_first_line();
	    		lcd_puts("recording done");
	    		lcd_begin_second_line;
	    		sprintf(lcd_done_rec, "sum of rec: %d", index_rec);
	    		lcd_puts(&lcd_done_rec);
	    		state = state0;
			  break;
		   case state5:
			   return 0;
			   break;   
		}
	 }
}

void convert_6octav(){
	int i = 0;
	for(i=0; i < 13; i++){
		MOD_oc[i] = 24000000/octav6[i] - 1;
	}
}

void translate_song_mod(){
	int size = 0;
	int j = 0;
	short temp = 0;
	size = sizeof(song1)/sizeof(song1[0]);
	for(j=0; j < size; j++){
		song1[j] = MOD_oc[song1[j]];
		temp = song1[j]/2;
		song12[j] = temp;
	}
	size = sizeof(song2)/sizeof(song2[0]);
	for(j=0; j < size; j++){
		song2[j] = MOD_oc[song2[j]];
		song22[j] = song2[j]/2;
	}
	size = sizeof(song3)/sizeof(song3[0]);
	for(j=0; j < size; j++){
		song3[j] = MOD_oc[song3[j]];
		song32[j] = song3[j]/2;
	}
	
}




