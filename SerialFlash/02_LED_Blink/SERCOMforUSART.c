/*
 * SERCOMforUSART.c
 *
 * Created: 15.06.2022 11:58:28
 *  Author: ARGE
 */ 
#include "SERCOMforUSART.h"
uint32_t fref = 48000000;
uint64_t baudRate = 0;
void uartInit(uint32_t fbaud){
	
	NVIC_EnableIRQ(SERCOM3_IRQn); // global interrupt has been enabled.
	
	SERCOM3->USART.CTRLA.bit.ENABLE = 0;
	SERCOM3->USART.CTRLA.bit.MODE = 0x1; // for usart with internal clock
	SERCOM3->USART.CTRLA.bit.CMODE = 0; // for async communication
	SERCOM3->USART.CTRLA.bit.RXPO = 0x1; // for pad[1] for rx pin pa23 as pad[1]
	SERCOM3->USART.CTRLA.bit.TXPO = 0x0;
	SERCOM3->USART.CTRLB.bit.CHSIZE = 0x0; // for 8 bits
	SERCOM3->USART.CTRLA.bit.DORD = 1;// most significant bit first
	SERCOM3->USART.CTRLA.bit.FORM = 0x0; // only frame, so without party bit
	SERCOM3->USART.CTRLB.bit.SBMODE = 0; // for one stop bit sbmode is 0, 2 stop bit sbmod is 1

	uint64_t baudRate = (uint64_t)65536 * (fref - 16 * fbaud) / fref; 
	SERCOM3->USART.BAUD.bit.BAUD = baudRate;
	SERCOM3->USART.CTRLB.bit.RXEN = 1; // receiver is enabled
	SERCOM3->USART.CTRLB.bit.TXEN = 1;
	SERCOM3->USART.INTENSET.bit.RXC = 1; // receive compleate interrupt is enabled
	SERCOM3->USART.CTRLA.bit.ENABLE = 1;
	
}	
void uartWrite(uint8_t *veri, uint8_t karakterAdedi){
	
	if(SERCOM3->USART.INTFLAG.bit.DRE == 1){
		for(uint8_t a = 0; a < karakterAdedi; a++){
			
			SERCOM3->USART.DATA.bit.DATA = *(veri + a);
			while(!SERCOM3->USART.INTFLAG.bit.TXC){}
			
			}
	}

}
void uartReadEnable(){
	
	SERCOM3->USART.CTRLB.bit.RXEN = 1;
	
}

void uartReadDisable(){
	
	SERCOM3->USART.CTRLB.bit.RXEN = 0;
	
}