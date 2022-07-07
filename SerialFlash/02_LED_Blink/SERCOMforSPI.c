                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       /*
 * SERCOMforSPI.c
 *
 * Created: 29.06.2022 09:13:18
 *  Author: ARGE
 */ 
#include "SERCOMforSPI.h"
uint8_t valueofstatusreg = 0x00;

void SPIinit(){
	
	while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE);
	SERCOM5->SPI.CTRLA.bit.ENABLE = 0;
	SERCOM5->SPI.CTRLA.bit.MODE = 0x3; //0x3 is written for host mode, 0x2 is written for client mode
	SERCOM5->SPI.CTRLA.bit.CPOL = 0;
	SERCOM5->SPI.CTRLA.bit.CPHA = 0x0;
	SERCOM5->SPI.CTRLA.bit.FORM = 0x0; // for spi frame
	SERCOM5->SPI.CTRLA.bit.DIPO = 0x0; // pb16
	SERCOM5->SPI.CTRLB.bit.MSSEN = 0; // when CTRLB.MSSEN = 1, or by a GPIO driven by the application when CTRLB.MSSEN = 0.
	SERCOM5->SPI.CTRLA.bit.DOPO = 0x1; //In host operation, DO is MOSI.
	SERCOM5->SPI.CTRLB.bit.CHSIZE = 0x0; // Character Size is 8 bit
	SERCOM5->SPI.CTRLA.bit.DORD = 0x0; //MSB is transferred first
	
	uint16_t BAUD_REG = ((float)SPI_CLK_FREQ / (float)(2 * SPI_BAUD)) - 1;
	SERCOM5->SPI.BAUD.bit.BAUD = BAUD_REG;
	SERCOM5->SPI.CTRLB.bit.RXEN = 1; //Enable the receiver by writing the Receiver Enable bit in the CTRLB register 
	
	SERCOM5->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE);	
}

uint8_t spiSend(uint8_t data)
{
	while(0 == SERCOM5->SPI.INTFLAG.bit.DRE){}
	SERCOM5->SPI.DATA.bit.DATA = data;
	while(0 == SERCOM5->SPI.INTFLAG.bit.RXC){}	
	return (uint8_t)SERCOM5->SPI.DATA.bit.DATA;
}

void readArray(uint8_t opcodeofread, uint32_t adress, uint8_t howmuchbyte, uint8_t *vallue){
		
	volatile uint8_t byte1, byte2, byte3;
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);
	if(opcodeofread == 0x1b){
		
		gpio_ss_low();
		spiSend(0x1b);
		spiSend(byte1);
		spiSend(byte2);
		spiSend(byte3);
		spiSend(0xff); // 2 byte dummy
		spiSend(0xff); 
	}
	else if(opcodeofread == 0x0b){
		gpio_ss_low();
		spiSend(0x0b);
		spiSend(byte1);
		spiSend(byte2);
		spiSend(byte3);
		spiSend(0xff); // 1 byte dummy
	}
	else if(opcodeofread == 0x03){
		gpio_ss_low();
		spiSend(0x03);
		spiSend(byte1);
		spiSend(byte2);
		spiSend(byte3);
	}
	
	for (uint8_t a = 0; a < howmuchbyte; a++)
	{
		*(vallue + a) = spiSend(0xff);
	}
	
	gpio_ss_high();
	
}

void pageProgram(uint32_t startedaddress, uint8_t *data, uint8_t howmuchdata){

	writeEnable();
	readStatusRegister(1, &valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	} // well bit is 1 and device is ready
	volatile uint8_t max = 0xff, difference = 0x00;
	volatile uint16_t remainingplusone = 0x00;
	volatile uint8_t byte1, byte2, byte3;	
	byte3 = startedaddress;
	byte2 = (startedaddress >> 8);
	byte1 = (startedaddress >> 16);
	remainingplusone = (max - byte3) + 1; // remaining + current value
	gpio_ss_low();
	spiSend(0x02);
	spiSend(byte1);
	spiSend(byte2);
	spiSend(byte3);
	if (remainingplusone >= howmuchdata){
		
		for (volatile uint8_t a = 0; a < howmuchdata; a++){
			spiSend(*(data + a));
		}
		gpio_ss_high();	
		readStatusRegister(1, &valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg))){ // wait until writing is completed
			readStatusRegister(1, &valueofstatusreg);
		}
	}
	else{	
		for (volatile uint8_t b = 0; b < remainingplusone; b++){
			spiSend(*(data + b));
		}
		gpio_ss_high();
		// send new page address
		readStatusRegister(1, &valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg))){ // wait until writing is completed
			readStatusRegister(1, &valueofstatusreg);
		}
		writeEnable();
		readStatusRegister(1, &valueofstatusreg);
		while (!(0x02 == (0x03 & valueofstatusreg))){
			readStatusRegister(1, &valueofstatusreg);
		} // well bit is 1 and device is ready
		
		startedaddress += (remainingplusone); 
		byte3 = startedaddress;
		byte2 = (startedaddress >> 8);
		byte1 = (startedaddress >> 16);
		gpio_ss_low();
		spiSend(0x02);
		spiSend(byte1);
		spiSend(byte2);
		spiSend(byte3);
		difference = howmuchdata - remainingplusone;
		for (volatile uint8_t c = 0; c < difference; c++){
			spiSend(*(data + (c + remainingplusone)));
		}
		gpio_ss_high();
		readStatusRegister(1, &valueofstatusreg);
		while ((0x01 == (0x01 & valueofstatusreg))){ // wait until writing is completed
			readStatusRegister(1, &valueofstatusreg);
		}
		}
}

void writeEnable(){
	gpio_ss_low();
	spiSend(0x06);
	gpio_ss_high();
}

void MakesectorUnprotected(uint32_t sectorAddress){
	
	writeEnable();
	readStatusRegister(1, &valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);	
	} // well bit is 1 and device is ready
	volatile uint8_t byte1, byte2, byte3;
	byte3 = sectorAddress;
	byte2 = (sectorAddress >> 8);
	byte1 = (sectorAddress >> 16);
	gpio_ss_low();
	spiSend(0x39);
	spiSend(byte1);
	spiSend(byte2);
	spiSend(byte3);
	gpio_ss_high();
}

void blockErase(uint8_t opcodeoferase, uint32_t adress){
	
	writeEnable();
	readStatusRegister(1, &valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);	
	} // well bit is 1 and device is ready	
	volatile uint8_t byte1, byte2, byte3;
	byte3 = adress;
	byte2 = (adress >> 8);
	byte1 = (adress >> 16);
	gpio_ss_low();
	spiSend(opcodeoferase); 
	spiSend(byte1);
	spiSend(byte2);
	spiSend(byte3);
	gpio_ss_high();
	readStatusRegister(1, &valueofstatusreg);
	while ((0x20 == (0x20 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	}
	readStatusRegister(1, &valueofstatusreg);
	while ((0x01 == (0x01 & valueofstatusreg))){ // wait until blok erasing is completed
		readStatusRegister(1, &valueofstatusreg);
	}
}

void readStatusRegister(uint8_t whichbyte, uint8_t *data){
	
	gpio_ss_low();
	if(whichbyte == 1){
		spiSend(0x05);
		*data = spiSend(0xff);
		spiSend(0xff);
		gpio_ss_high();
	}
	else if(whichbyte == 2){
		spiSend(0x05);
		spiSend(0xff);
		*data = spiSend(0xff);
		gpio_ss_high();
	}
}

void globalUnprotected(){
	
	writeEnable();
	readStatusRegister(1, &valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	} // well bit is 1 and device is ready
	gpio_ss_low();
	spiSend(0x01);
	spiSend(0x00);
	gpio_ss_high();
	
}
void globalProtected(){
	
	writeEnable();
	readStatusRegister(1, &valueofstatusreg);
	while (!(0x02 == (0x03 & valueofstatusreg))){
		readStatusRegister(1, &valueofstatusreg);
	} // well bit is 1 and device is ready
	gpio_ss_low();
	spiSend(0x01);
	spiSend(0x7f);
	gpio_ss_high();
}

void gpio_ss_high(){
	
	PORT->Group[0].OUTSET.reg = PORT_PA13;
}

void gpio_ss_low(){
	
	PORT->Group[0].OUTCLR.reg = PORT_PA13;
}                                                                                                                                                                                                                                                  