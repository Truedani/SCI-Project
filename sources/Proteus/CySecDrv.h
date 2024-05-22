/*
 * CySecDrv.h
 *  Description: header file for the Application driver
 *
 *  Created on: May 08, 2024
 *  Author: Daniel Calaver
 */

#ifndef CySecDrv_H_

#define CySecDrv_H_


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
	SECURED = 0u, 
	NOT_SECURED = 1u,
	UNDEFINED = 2u
} teCySecDrvStatus;

/*#################################*/
/*       Global ROM data           */
/*#################################*/
// extern const uint64_t CySecStaticHash[8] PROGMEM;

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name: CySecDrvInit
   Description: Initialize CySecDrv and calculates first secure state
*/
extern void CySecDrvInit();



/* Function name: CySecDrvGetSecurityState
   Description: returns the current security state of the system
   Function output: returns security status
*/
extern teCySecDrvStatus CySecDrvGetSecurityState ();

/* Function name: CySecDrvMain
   Description: Main loop function to periodically update the security state
*/
extern void CySecDrvMain();


#endif /* CySecDrv_H_ */

