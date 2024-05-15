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
extern void CySecDrvInit();



/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern teCySecDrvStatus CySecDrvGetSecurityState ();

/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
extern void CySecDrvMain();


#endif /* CySecDrv_H_ */

