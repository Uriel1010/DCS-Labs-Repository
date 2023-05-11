#include "TFC.h"
#include "mcg.h"
#include  "..\Project_Headers\app.h"


#define MUDULO_REGISTER  0x2EE0

// set I/O for switches and LEDs
void InitGPIO()
{
	//enable Clocks to all ports - page 206, enable clock to Ports
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	//Setup Pins as Timer Output PWM
	//PORTD_PCR1 = PORT_PCR_MUX(4);  //PTD1 pin TPM0_CH1- ALT4, Blue = Edge Aligned PWM
	//PORTB_PCR18 = PORT_PCR_MUX(3);  //PTB18 pin TPM2_CH0- ALT3, Red = Edge Aligned PWM
	//PORTB_PCR19 = PORT_PCR_MUX(3);  //PTB19 pin TPM2_CH1- ALT3, Green = Edge Aligned PWM
	
	//--------------------LCD-------------------------------------
	PORTD_PCR4 = PORT_PCR_MUX(1); // assign PTD4 as GPIO
	PORTD_PCR5 = PORT_PCR_MUX(1); // assign PTD5 as GPIO
	PORTD_PCR6 = PORT_PCR_MUX(1); // assign PTD6 as GPIO
	PORTD_PCR7 = PORT_PCR_MUX(1); // assign PTD7 as GPIO
	GPIOD_PDDR = 0xF0;
	GPIOD_PDOR = 0x00;
	
	PORTE_PCR3 = PORT_PCR_MUX(1);
	PORTE_PCR4 = PORT_PCR_MUX(1);
	PORTE_PCR5 = PORT_PCR_MUX(1);
	PORTE_PCR29 = PORT_PCR_MUX(1); 
	GPIOE_PDDR = 0x38;
	GPIOE_PDOR = 0x00;
	//GPIOD_PDDR &= PORT_LOC(4);  // PTD4 is OUTPUT
	//GPIOD_PDDR &= PORT_LOC(5);  // PTD5 is OUTPUT
	//GPIOD_PDDR &= PORT_LOC(6);  // PTD6 is OUTPUT
	//GPIOD_PDDR &= PORT_LOC(7);  // PTD7 is OUTPUT
	//PORTD_PCR7 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0x0a);

	
	// PushButtons Setup
		PORTD_PCR0 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.0=PB0 + Interrupt request enable
		PORTD_PCR1 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.1=PB1 + Interrupt request enable
		PORTD_PCR2 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_DOWN); // PTD.2=PB2 + Interrupt request enable
		PORTD_PCR3 = PORT_PCR_MUX(4); // PTD.3=PB3 + Interrupt request enable
		PBsArrPortDir &= ~PBsArr_LOC; // port PTD 0-2 Input
		PBsArrIntPendClear(PBsArr_LOC);
		
		enable_irq(INT_PORTD-16);           // Enable PORTD Interrupts 
		set_irq_priority (INT_PORTD-16,0);  // PORTD Interrupt priority = 0 = max
		
		
	
}



//-----------------------------------------------------------------
// DipSwitch data reading
//-----------------------------------------------------------------
uint8_t TFC_GetDIP_Switch()
{
	uint8_t DIP_Val=0;
	
	DIP_Val = (GPIOC_PDIR>>4) & 0xF;

	return DIP_Val;
}
//-----------------------------------------------------------------
// TPMx - Initialization
//-----------------------------------------------------------------
void InitTPM(char x){  // x={0,1,2}
	switch(x){
	case 0:
		TPM0_SC = 0; // to ensure that the counter is not running
		//TPM0_SC = 0x7;
		//TPM0_SC =  TPM_SC_PS(3)+TPM_SC_TOIE_MASK; //Prescaler =128, up-mode, counter-disable
		//TPM0_MOD = MUDULO_REGISTER; // PWM frequency of 250Hz = 24MHz/(8x12,000)
		TPM0_MOD = 0xFFFF; // time for interrupt
		TPM0_C3SC = 0xC4;//TPM_CnSC_ELSA_MASK;//0x00000004;
		//TPM0_C1V = 0xFFFF;
		TPM0_CONF = 0;//0xC0; 
		enable_irq(INT_TPM0-16); //  //Enable TMP0 IRQ on the NVIC
		set_irq_priority(INT_TPM0-16,1);  // Interrupt priority = 1
		
		break;
	case 1:
		
		break;
	case 2: 
		TPM2_SC = 0; // to ensure that the counter is not running
		TPM2_SC |= TPM_SC_PS(3)+TPM_SC_TOIE_MASK; //Prescaler =128, up-mode, counter-disable
		TPM2_MOD = MUDULO_REGISTER; // PWM frequency of 250Hz = 24MHz/(8x12,000)
		TPM2_C0SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		TPM2_C0V = 0xFFFF; 
		TPM2_C1SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		TPM2_C1V = 0xFFFF;
		TPM2_CONF = 0;
		break;
	}
}
//-----------------------------------------------------------------
// TPMx - Clock Setup
//-----------------------------------------------------------------
void ClockSetupTPM(){
	    
	    pll_init(8000000, LOW_POWER, CRYSTAL,4,24,MCGOUT); //Core Clock is now at 48MHz using the 8MHZ Crystal
		
	    //Clock Setup for the TPM requires a couple steps.
	    //1st,  set the clock mux
	    //See Page 124 of f the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;// We Want MCGPLLCLK/2=24MHz (See Page 196 of the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); //We want the MCGPLLCLK/2 (See Page 196 of the KL25 Sub-Family Reference Manual
		//Enable the Clock to the TPM0 and PIT Modules
		//See Page 207 of f the KL25 Sub-Family Reference Manual
		SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK + SIM_SCGC6_TPM2_MASK;
	    // TPM_clock = 24MHz , PIT_clock = 48MHz
	    
}
//-----------------------------------------------------------------
// PIT - Initialisation
//-----------------------------------------------------------------
void InitPIT(){
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	// Timer 0
	//PIT_LDVAL0 = 0x02DC6C00; // setup timer 0 for maximum counting period
	//PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	// PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,1);  // Interrupt priority = 0 = max
}


//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	

void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
		stop();    /* Enter Low Power Mode 0 */
    else if(LPM_level == 0x01) 
    	wait();      /* Enter Low Power Mode 1 */
}

//int InitPITx(select_pit, Time_to_Interrupt){
//	PIT_LDVAL(select_pit) = 0x02DC6C00; // setup timer 0 for maximum counting period
//	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
//}

void setPIT1_timer(int a){
	PIT_TCTRL1 &= ~0x00000003; //disable PIT0 and its interrupt
	PIT_LDVAL1 = a; // setup timer a for a counting period
	PIT_TCTRL1 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	
}

void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}








