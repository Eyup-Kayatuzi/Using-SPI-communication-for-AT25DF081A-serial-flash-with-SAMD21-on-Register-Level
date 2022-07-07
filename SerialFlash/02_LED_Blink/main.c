//////////////////////////////////////////////////////////////////////////
// Include and defines
//////////////////////////////////////////////////////////////////////////

#include "sam.h"
#include "definitions.h"
#include "delay.h"
#include "app.h"
#include "SERCOMforUSART.h"
#include "SERCOMforSPI.h"
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////////
void ClocksInit(void);	// Configure Clock, Wait States and synch, bus clocks for 48MHz operation

uint8_t ReceivedData[256]; // from uart
uint8_t buffread[256] = {}; // read from sdcard
uint8_t buffwrite[256] = {};// write to sdcard
uint8_t i = 0; // for uart

/*******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This is our main function
 *
 * Note:
 *
 ******************************************************************************/
int main(void){
	AppInit(); // clock has been started and port which is needed is configed.
	uartInit(9600);
	SPIinit();
	globalUnprotected();
	blockErase(0x20, 0x000fb000);
	readArray(0x1b, 0x000fb000, 5, buffread);
	while(1)
	{
		
	}
	
} // main()

void SERCOM3_Handler(){
	
		ReceivedData[i] = SERCOM3->USART.DATA.bit.DATA; // This flag is cleared by reading the Data register (DATA) or by disabling the receiver.
		if (ReceivedData[i] == '\n')
		{	
			pageProgram(0x000000, ReceivedData, (i + 1)); // 0x0000fc
			readArray(0x1b, 0x000000, (i + 1), buffread);
			memset(ReceivedData, 0x00, (i + 1) *sizeof(char));
			uartWrite(buffread, i + 1);
			memset(buffread, 0xff, (i + 1) *sizeof(char));
			i = 0;
			blockErase(0x20, 0x000000);
		}
		else{	
			
			i += 1;
		}
			
}



