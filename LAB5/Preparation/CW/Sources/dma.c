
#include "TFC.h"

void dma_init(char x){
	switch(x){
		case 0:
			// Enable clocks
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
		
			// Disable DMA Mux channel first
			DMAMUX0_CHCFG0 = 0x00;
			
			// Configure DMA
			//DMA_SAR0 = (uint32_t)&ADC0_RA;
			DMA_DAR0 = (uint32_t)&TPM2_MOD;
			// number of bytes yet to be transferred for a given block - 4 bytes(32 bits)
			DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(2); 
			
			DMA_DCR0 |= (//DMA_DCR_EINT_MASK|		// Enable interrupt
						 DMA_DCR_ERQ_MASK |		// Enable peripheral request
						 //DMA_DCR_CS_MASK  |
						 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
						 DMA_DCR_SINC_MASK |		// Set increments to source address
						 //DMA_DCR_DMOD(0)  |     // Destination address modulo of 32 Bytes
						 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits 
						 
			
			//Config DMA Mux for ADC0 operation, Enable DMA channel and source
			DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK; //| DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 as source
			
			// Enable interrupt
			enable_irq(INT_DMA0 - 16);
			break;
		case 1:
			// Enable clocks
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
		
			// Disable DMA Mux channel first
			DMAMUX0_CHCFG1 = 0x00;
			
			// Configure DMA
			//DMA_SAR0 = (uint32_t)&ADC0_RA;
			DMA_DAR1 = (uint32_t)&TPM2_C0V;
			// number of bytes yet to be transferred for a given block - 4 bytes(32 bits)
			DMA_DSR_BCR1 = DMA_DSR_BCR_BCR(2); 
			
			DMA_DCR1 |= (//DMA_DCR_EINT_MASK|		// Enable interrupt
						 DMA_DCR_ERQ_MASK |		// Enable peripheral request
						 //DMA_DCR_CS_MASK  |
						 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
						 DMA_DCR_SINC_MASK |		// Set increments to source address
						 //DMA_DCR_DMOD(0)  |     // Destination address modulo of 32 Bytes
						 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits 
						 
			
			//Config DMA Mux for ADC0 operation, Enable DMA channel and source
			DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK; //| DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 as source
			
			// Enable interrupt
			enable_irq(INT_DMA1 - 16);
			break;
		
	}
}

/*
 * Handles DMA0 interrupt
 * Resets the BCR register and clears the DONE flag
 * */
void DMA0_IRQHandler(void){
	/* Enable DMA0*/ 
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;	// Clear Done Flag
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(2);		// Set byte count register
}
