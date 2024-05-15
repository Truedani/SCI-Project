/*
 * Application.c
 *  Description: Application module
 *
 *  Created on: May 08, 2024
 *  Author: Daniel Calaver
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "Application.h"
#include "BtnDrv.h"
#include "CySecDrv.h"
#include "LedDrv.h"


#include <avr/io.h>
#include <stdio.h>

/*#################################*/
/*         Local defines           */
/*#################################*/


/*#################################*/
/*        Local data types         */
/*#################################*/
teBtnDrvState last_left_btn_state;
teBtnDrvState last_right_btn_state;

/*#################################*/
/*        Global ROM data          */
/*#################################*/


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

void ApplicationInit()
{
	last_left_btn_state = BtnDrvGetButtonState(BTN_LEFT);
	last_right_btn_state = BtnDrvGetButtonState(BTN_RIGHT);
	LedDrvSetLedState(LED_LEFT, OFF);
	LedDrvSetLedState(LED_RIGHT, OFF);
}

void ApplicationMain()
{
	if (SECURED == CySecDrvGetSecurityState())
	{
		if (NOT_PRESSED == BtnDrvGetButtonState(BTN_LEFT) && PRESSED == last_left_btn_state) {
			LedDrvToggleLedState(LED_RIGHT);
		}
		if (NOT_PRESSED == BtnDrvGetButtonState(BTN_RIGHT) && PRESSED == last_right_btn_state) {
			LedDrvToggleLedState(LED_LEFT);
		}
	} else {
		LedDrvSetLedState(LED_LEFT, OFF);
		LedDrvSetLedState(LED_RIGHT, OFF);
	}

	last_left_btn_state = BtnDrvGetButtonState(BTN_LEFT);
	last_right_btn_state = BtnDrvGetButtonState(BTN_RIGHT);
	
}

/*#################################*/
/*  Local function implementation  */
/*#################################*/
