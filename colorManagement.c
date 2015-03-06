/*
 * colorManagement.c
 *
 *  Created on: 06.03.2015
 *      Author: blinch
 */

#include "light_ws2812_cortex.h"
#include "colorManagement.h"

const uint8_t rgb[]   = {0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF};

const uint8_t red[]   = {0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,
						 0x00,0xFF,0x00};

const uint8_t green[] = {0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,
						 0xFF,0x00,0x00};

const uint8_t blue[]  = {0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,
						 0x00,0x00,0xFF};

const uint8_t white[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
						 0xFF,0xFF,0xFF};

const uint8_t off[]   = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00};

const uint8_t purpl[] = {0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,
						 0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,
						 0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,
						 0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,
						 0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,
						 0x00,0xFF,0xFF};

const uint8_t cyan[]  = {0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,
						 0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,
						 0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,
						 0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,
						 0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,
						 0xFF,0x00,0xFF};

const uint8_t yello[] = {0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,
						 0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,
						 0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,
						 0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,
						 0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,0xFF,0xFF,0x00,
						 0xFF,0xFF,0x00};



void updateWS2812LEDs(enum colorState state)
{


	switch(state)
	{
	case RGB:
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		ws2812_sendarray((uint8_t*)rgb, sizeof(rgb));
		break;
	case RED:
		ws2812_sendarray((uint8_t*)red, sizeof(red));
		ws2812_sendarray((uint8_t*)red, sizeof(red));
		ws2812_sendarray((uint8_t*)red, sizeof(red));
		break;
	case GREEN:
		ws2812_sendarray((uint8_t*)green, sizeof(green));
		ws2812_sendarray((uint8_t*)green, sizeof(green));
		ws2812_sendarray((uint8_t*)green, sizeof(green));
		break;
	case BLUE:
		ws2812_sendarray((uint8_t*)blue, sizeof(blue));
		ws2812_sendarray((uint8_t*)blue, sizeof(blue));
		ws2812_sendarray((uint8_t*)blue, sizeof(blue));
		break;
	case WHITE:
		ws2812_sendarray((uint8_t*)white, sizeof(white));
		ws2812_sendarray((uint8_t*)white, sizeof(white));
		ws2812_sendarray((uint8_t*)white, sizeof(white));
		break;
	case YELLOW:
		ws2812_sendarray((uint8_t*)yello, sizeof(yello));
		ws2812_sendarray((uint8_t*)yello, sizeof(yello));
		ws2812_sendarray((uint8_t*)yello, sizeof(yello));
		break;
	case PURPLE:
		ws2812_sendarray((uint8_t*)purpl, sizeof(purpl));
		ws2812_sendarray((uint8_t*)purpl, sizeof(purpl));
		ws2812_sendarray((uint8_t*)purpl, sizeof(purpl));
		break;
	case CYAN:
		ws2812_sendarray((uint8_t*)cyan, sizeof(cyan));
		ws2812_sendarray((uint8_t*)cyan, sizeof(cyan));
		ws2812_sendarray((uint8_t*)cyan, sizeof(cyan));
		break;
	case OFF:
		ws2812_sendarray((uint8_t*)off, sizeof(off));
		break;
	default:
		break;
	}
}
