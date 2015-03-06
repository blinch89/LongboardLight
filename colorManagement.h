/*
 * colorManagement.h
 *
 *  Created on: 06.03.2015
 *      Author: blinch
 */

#ifndef COLORMANAGEMENT_H_
#define COLORMANAGEMENT_H_

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
	CYAN
};

void updateWS2812LEDs(enum colorState state);

#endif /* COLORMANAGEMENT_H_ */
