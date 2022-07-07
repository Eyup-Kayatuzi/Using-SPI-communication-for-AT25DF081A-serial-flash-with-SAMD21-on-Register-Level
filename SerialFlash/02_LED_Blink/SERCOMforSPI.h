/*
 * SERCOMforSPI.h
 *
 * Created: 29.06.2022 09:12:01
 *  Author: ARGE
 */ 
#ifndef SERCOMFORSPI_H_
#define SERCOMFORSPI_H_
//////////////////////////////////////// needed library
#include "sam.h"
#include "delay.h"
////////////////////////////////////////
#define SPI_CLK_FREQ	48000000
#define SPI_BAUD		12000000

/////////////////////////////////////// Functions

void SPIinit();
void gpio_ss_high();
void gpio_ss_low();
uint8_t spiSend(uint8_t data);
void readArray(uint8_t opcodeofread, uint32_t adress, uint8_t howmuchbyte, uint8_t *vallue); // for 0x1b 2 byte dummy, for 0x0b 1 byte dummy, for 0x03 0 byte dummy
void writeEnable();
void blockErase(uint8_t opcodeoferase, uint32_t adress); // 0x20 is for 4kbyte, 0x52 is for 32kbyte, 0xd8 is for 64kbyte
void readStatusRegister(uint8_t whichbyte, uint8_t *data);
void MakesectorUnprotected(uint32_t sectorAddress); //Before the Unprotect Sector command can be issued, 
												    //the Write Enable command must have been previously issued
void pageProgram(uint32_t startedaddress, uint8_t *data, uint8_t howmuchdata);
void globalUnprotected();
void globalProtected();
///////////////////////////////////////

#endif /* SERCOMFORSPI_H_ */