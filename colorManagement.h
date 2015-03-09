/*
 * colorManagement.h
 *
 *  Created on: 06.03.2015
 *      Author: blinch
 */

#ifndef COLORMANAGEMENT_H_
#define COLORMANAGEMENT_H_

#define WS2812_QUANTITY 21

enum colorState
{
	OFF,
	RGB,
	WHITE,
	RED,
	GREEN,
	BLUE,
	PURPLE,
	YELLOW,
	CYAN,
	REDDOT,
};

void updateWS2812LEDs(enum colorState state);
void redDotOnBlueLine(uint8_t *leds);

#endif /* COLORMANAGEMENT_H_ */
