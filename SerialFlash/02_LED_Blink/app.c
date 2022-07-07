#include "app.h"
#include "clock.h"
#include "delay.h"

void AppInit(void)
{

	ClocksInit();
	
	// Assign LED0 as OUTPUT
	REG_PORT_DIR0 = LED0_PIN_MASK;
	
	// Set LED0 OFF
	REG_PORT_OUTCLR0 = LED0_PIN_MASK;
	
	PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXE = 0x2; // pa22 is config as sercom3 pad[0] for uart
	
	PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[11].bit.PMUXO = 0x2; // pa23 is config as sercom3 pad[1] for uart
	
	PORT->Group[1].PINCFG[16].bit.PMUXEN = 1; // pb16, sercom5, pad[0]
	PORT->Group[1].PMUX[8].bit.PMUXE = 0x2;
	
	PORT->Group[1].PINCFG[22].bit.PMUXEN = 1; // pb22 sercom5, pad[2] 
	PORT->Group[1].PMUX[11].bit.PMUXE = 0x3;
	
	PORT->Group[1].PINCFG[23].bit.PMUXEN = 1; //pb23, sercom5, pad[3]
	PORT->Group[1].PMUX[11].bit.PMUXO = 0x3;
	
	//PORT->Group[0].PINCFG[13].bit.PMUXEN = 1; // pa13 as selection(ss) pin,
	//PORT->Group[0].PMUX[6].bit.PMUXO = 0x2;
	PORT->Group[0].DIRSET.bit.DIRSET |= 0x00002000; // pa13 as output
	
	
} // AppInit()


/*******************************************************************************
 * Function:        void AppRun(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function contains your main application
 *                  
 *
 * Note:
 *
 ******************************************************************************/
void AppRun(void)
{
	
	// Set the drive strength to strong
	PORT->Group[LED0_PORT].PINCFG[LED0_PIN_NUMBER].bit.DRVSTR = 1;
	
	while(1)
	{	
		// Turn the LED on PA17 ON

	}

} // Apprun()

