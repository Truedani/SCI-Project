/*
 * LedDrv.c
 *  Description: Low level driver for controlling the LED's
 *
 *  Created on: April 03, 2024
 *  Author: Alex Costache
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "LedDrv.h"

#include <avr/io.h>
#include <stdio.h>

/*#################################*/
/*         Local defines           */
/*#################################*/


/*#################################*/
/*        Local data types         */
/*#################################*/
typedef struct
{
	teLedDrvID 			tLedID;
	volatile uint8_t * 	DataDirReg;
	volatile uint8_t * 	PortReg;
	uint8_t 			PinNo;
} tsLedDrvConfig;

/*#################################*/
/*        Global ROM data          */
/*#################################*/
static const tsLedDrvConfig LedDrvConfigArray[MAX_NO_LEDS] =
{
	/*Left LED configuration*/
	{
		LED_LEFT,
		&DDRC,
		&PORTC,
		PC1
	},
	/*Right LED configuration*/
	{
		LED_RIGHT,
		&DDRC,
		&PORTC,
		PC4
	}
};

/*#################################*/
/*        Global RAM data          */
/*#################################*/


/*#################################*/
/*        Local ROM data           */
/*#################################*/


/*#################################*/
/*        Local RAM data           */
/*#################################*/


/*#################################*/
/*    Local function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/


/*#################################*/
/*  Global function implementation */
/*#################################*/
void LedDrvInit()
{
	uint8_t	ucIdx;

	/*Set DDR register as output for all LED's and switch off all LED's*/
	for (ucIdx=0; ucIdx<MAX_NO_LEDS; ucIdx++)
	{
		* LedDrvConfigArray[ucIdx].DataDirReg 	|= 1 << LedDrvConfigArray[ucIdx].PinNo;
		* LedDrvConfigArray[ucIdx].PortReg 		&= ~(_BV(LedDrvConfigArray[ucIdx].PinNo));
	}
	
}

void LedDrvToggleLedState(teLedDrvID eLedID)
{
	* LedDrvConfigArray[eLedID].PortReg ^= (1 << LedDrvConfigArray[eLedID].PinNo);
}

void LedDrvSetLedState(teLedDrvID eLedID, teLedDrvState eLedState)
{
	if (ON == eLedState)
	{
		* LedDrvConfigArray[eLedID].PortReg |= (1 << LedDrvConfigArray[eLedID].PinNo);
	}
	else
	{
		* LedDrvConfigArray[eLedID].PortReg &= ~(1 << LedDrvConfigArray[eLedID].PinNo);
	}
}
/*#################################*/
/*  Local function implementation  */
/*#################################*/

