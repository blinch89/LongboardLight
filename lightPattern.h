//--------------------------------------------------------------
// File     : lightPattern.h
//--------------------------------------------------------------

#ifndef LIGHTPATERN_H
#define LIGHTPATERN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_ws2812.h"



//--------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------
void dots_circular(char n_dotlines, WS2812_HSV_t *buf);
void rainbow_spin(WS2812_HSV_t *buf);
void initFrame(WS2812_HSV_t *buf);
void frame_white(WS2812_HSV_t *buf);
void frame_one_color(WS2812_HSV_t *buf, WS2812_HSV_t);


#endif