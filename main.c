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
#include "core_cmFunc.h"

void updateOnboardLEDs();
void buttonRead(void *ptr);

handle_t animationTimer;

int main(void)
{
	uint8_t stripe[WS2812_QUANTITY*3];
	enum colorState state = OFF;	//this should be volatile, I think
//	status_t status;		// Declaration of return variable for DAVE3 APIs (toggle comment if required)

	//init stripe array for an animation to blue color, all leds
	for(uint8_t i=0 ; i<(sizeof(stripe)/3) ; i++)
	{
		stripe[3*i]   = 0x00;
		stripe[3*i+1] = 0x00;
		stripe[3*i+2] = BRIGHTNESS;
	}

	DAVE_Init();			// Initialization of DAVE Apps
	handle_t timer;
	P0_5_set_mode(OUTPUT_PP_GP); 	//WS2812 data pin
	P0_6_set_mode(INPUT_PU);     	//button;
	timer=SYSTM001_CreateTimer(400,SYSTM001_PERIODIC,buttonRead,&state);
	SYSTM001_StartTimer(timer);
	animationTimer=SYSTM001_CreateTimer(10,SYSTM001_PERIODIC,redDotOnBlueLine,stripe);
	while(1){}
	return 0;
}

void buttonRead(void *ptr)
{
	if(!P0_6_read()) //button pressed?
	{
		enum colorState *lightState = ptr;

		if(*lightState == REDDOT)
		{
			SYSTM001_StopTimer(animationTimer);
			*lightState = OFF; //REDDOT is last Value of enum
		}
		else (*lightState)++;

		if(*lightState == REDDOT)
			SYSTM001_StartTimer(animationTimer);
		else
		{
			__disable_irq();
			updateWS2812LEDs(*lightState);
			__enable_irq();
		}
	}
}
