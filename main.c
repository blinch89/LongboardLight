/*
 * Main.c
 *
 *  Created on: 06.03.2015
 *      Author: blinch89
 */

#include <DAVE3.h>			//Declarations from DAVE3 Code Generation (includes SFR declaration)
#include "colorManagement.h"
#include "XMC1100.h"
#include "GPIO.h"

void updateOnboardLEDs();
void buttonRead(void *ptr);

int main(void)
{
//	status_t status;		// Declaration of return variable for DAVE3 APIs (toggle comment if required)
	DAVE_Init();			// Initialization of DAVE Apps
	IO004_SetPin(IO004_Handle0);
	P0_5_set_mode(OUTPUT_PP_GP); 	//WS2812 data pin
	P0_6_set_mode(INPUT_PU);     	//button
	enum colorState state = OFF;
	handle_t timer;
	timer=SYSTM001_CreateTimer(500,SYSTM001_PERIODIC,updateOnboardLEDs,NULL);
	SYSTM001_StartTimer(timer);
	timer=SYSTM001_CreateTimer(400,SYSTM001_PERIODIC,buttonRead,&state);
	SYSTM001_StartTimer(timer);
	while(1);
	return 0;
}


void updateOnboardLEDs()
{
	IO004_TogglePin(IO004_Handle0);
	IO004_TogglePin(IO004_Handle1);
}


void buttonRead(void *ptr)
{
	if(!P0_6_read()) //button pressed?
	{
		enum colorState *lightState = ptr;
		if(*lightState == CYAN) *lightState = OFF; //CYAN is last Value of enum
		else (*lightState)++;
		updateWS2812LEDs(*lightState);
	}
}
