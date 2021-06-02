/*
 * colorManagement.c
 *
 *  Created on: 06.03.2015
 *      Author: blinch
 */

#include "light_ws2812_cortex.h"
#include "colorManagement.h"

const uint8_t rgb[]   = {0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS};

const uint8_t red[]   = {0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,
						 0x00,BRIGHTNESS,0x00};

const uint8_t green[] = {BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,
						 BRIGHTNESS,0x00,0x00};

const uint8_t blue[]  = {0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,0x00,0x00,BRIGHTNESS,
						 0x00,0x00,BRIGHTNESS};

const uint8_t white[] = {BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,
						 BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,
						 BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,
						 BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,
						 BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,BRIGHTNESS,
						 BRIGHTNESS,BRIGHTNESS,BRIGHTNESS};

const uint8_t off[]   = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00};

const uint8_t purpl[] = {0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,
						 0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,
						 0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,
						 0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,
						 0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,
						 0x00,BRIGHTNESS,BRIGHTNESS};

const uint8_t cyan[]  = {BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,
						 BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,
						 BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,
						 BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,
						 BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,
						 BRIGHTNESS,0x00,BRIGHTNESS};

const uint8_t yello[] = {BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,
						 BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,
						 BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,
						 BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,
						 BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,BRIGHTNESS,BRIGHTNESS,0x00,
						 BRIGHTNESS,BRIGHTNESS,0x00};



void updateWS2812LEDs(enum colorState state)
{
	switch(state)
	{
	case RGB:
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		break;
	case RED:
		ws2812_sendarray((uint8_t*)red, sizeof(red));
		break;
	case GREEN:
		ws2812_sendarray((uint8_t*)green, sizeof(green));
		break;
	case BLUE:
		ws2812_sendarray((uint8_t*)blue, sizeof(blue));
		break;
	case WHITE:
		ws2812_sendarray((uint8_t*)white, sizeof(white));
		break;
	case YELLOW:
		ws2812_sendarray((uint8_t*)yello, sizeof(yello));
		break;
	case PURPLE:
		ws2812_sendarray((uint8_t*)purpl, sizeof(purpl));
		break;
	case CYAN:
		ws2812_sendarray((uint8_t*)cyan, sizeof(cyan));;
		break;
	case OFF:
		ws2812_sendarray((uint8_t*)off, sizeof(off));
		break;
	default:
		break;
	}
}




void redDotOnBlueLine(void *ptr)
{
	uint8_t *leds = ptr;
	static uint8_t redPosition = 0;
	static uint8_t bluePosition = 0;

	if(redPosition >= 3 || redPosition < bluePosition)
	{
		//delete oldest red dot, turn him blue
		leds[3*bluePosition  ] = 0x00;
		leds[3*bluePosition+1] = 0x00;
		leds[3*bluePosition+2] = BRIGHTNESS;
		bluePosition++;
	}

	//make new red dot
	leds[3*redPosition  ] = 0x00;
	leds[3*redPosition+1] = BRIGHTNESS;
	leds[3*redPosition+2] = 0x00;

	if(++redPosition >= WS2812_QUANTITY) redPosition = 0;
	if(bluePosition >= WS2812_QUANTITY) bluePosition = 0;
	ws2812_sendarray(leds,63); //write new colors to to LEDS:
}