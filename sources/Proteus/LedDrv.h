/*
 * LedDrv.h
 *  Description: header file for the LedDrv driver
 *
 *  Created on: April 03, 2024
 *  Author: Alex Costache
 */

#ifndef LedDrv_H_
#define LedDrv_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

/*#################################*/
/*      Global data types          */
/*#################################*/
typedef enum
{
	LED_LEFT = 0u,
	LED_RIGHT = 1u,
	MAX_NO_LEDS = 2u
} teLedDrvID;

typedef enum
{
	ON = 0u, 
	OFF = 1u
} teLedDrvState;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void LedDrvInit();

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void LedDrvToggleLedState(teLedDrvID eLedID);

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void LedDrvSetLedState(teLedDrvID eLedID, teLedDrvState eLedState);

#endif /* LedDrv_H_ */

