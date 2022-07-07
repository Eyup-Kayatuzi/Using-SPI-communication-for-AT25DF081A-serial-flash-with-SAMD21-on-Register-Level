/*
 * SERCOMforUSART.h
 *
 * Created: 15.06.2022 11:58:06
 *  Author: ARGE
 */ 


#ifndef SERCOMFORUSART_H_
#define SERCOMFORUSART_H_

///////////////////////// Needed Library
#include "sam.h"
#include "delay.h"

void uartInit(uint32_t fbaud);
void uartWrite(uint8_t *veri, uint8_t karakterAdedi);
void uartReadEnable(void);
void uartReadDisable(void);


#endif /* SERCOMFORUSART_H_ */