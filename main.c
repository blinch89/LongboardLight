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
#include "light_ws2812_cortex.h"

void updateOnboardLEDs();
void buttonRead(void *ptr);
void allowRedDotUpdate(void *flag);

int main(void)
{
	DAVE_Init();			// Initialization of DAVE Apps
	uint8_t stripe[WS2812_QUANTITY*3];
	uint8_t redDotFlag = 0;
	enum colorState state = OFF;	//this should be volatile, I think
	handle_t timer;
//	status_t status;		// Declaration of return variable for DAVE3 APIs (toggle comment if required)


	IO004_SetPin(IO004_Handle0);
	P0_5_set_mode(OUTPUT_PP_GP); 	//WS2812 data pin
	P0_6_set_mode(INPUT_PU);     	//button
	timer=SYSTM001_CreateTimer(500,SYSTM001_PERIODIC,updateOnboardLEDs,NULL);
	SYSTM001_StartTimer(timer);
	timer=SYSTM001_CreateTimer(400,SYSTM001_PERIODIC,buttonRead,&state);
	SYSTM001_StartTimer(timer);
	timer=SYSTM001_CreateTimer(50,SYSTM001_PERIODIC,allowRedDotUpdate,&redDotFlag);
	SYSTM001_StartTimer(timer);


	for(uint8_t i=0 ; i<sizeof(stripe)/3 ; i+=3)
	{
		stripe[i]   = 0x00;
		stripe[i+1] = 0x00;
		stripe[i+2] = 0xFF;
	}

	while(1)
	{
		if(state == REDDOT && redDotFlag)
		{
			redDotOnBlueLine(stripe);
			redDotFlag = 0;
		}
	}
	return 0;
}



void allowRedDotUpdate(void *flag)
{
	if(!(*(uint8_t *)flag)) *(uint8_t *)flag = 1;
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
		if(*lightState == REDDOT) *lightState = OFF; //REDDOT is last Value of enum
		else (*lightState)++;
		updateWS2812LEDs(*lightState);
	}
}
