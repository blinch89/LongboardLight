/*
 * light weight WS2812 lib - ARM Cortex M0/M0+ version
 *
 * Created: 07.07.2013
 *  Author: Tim (cpldcpu@gmail.com)
 *
 *  - modified for XMC2GO, Ulrich ter Horst (blinch89), 6th March 2015
 */

#include <stdint.h>

#ifndef LIGHT_WS2812_H_
#define LIGHT_WS2812_H_

///////////////////////////////////////////////////////////////////////
// User defined area: CPU specific CMSIS include
///////////////////////////////////////////////////////////////////////

#include "XMC1100.h"		// Change this to the include for your MCU


///////////////////////////////////////////////////////////////////////
// User defined area: Define I/O pin
///////////////////////////////////////////////////////////////////////

// Data port and pin definition for your CPU. Depending on the way
// port access is implemented in your Cortex CPU, the set and clr
// mask and addresses may be the same. This example is for the
// NXP LPC81X

//#define ws2812_port_set ((uint32_t*)&LPC_GPIO_PORT->SET0)	// Address of the data port register to set the pin
//#define ws2812_port_clr	((uint32_t*)&LPC_GPIO_PORT->CLR0)	// Address of the data port register to clear the pin
#define ws2812_port_set ((uint32_t*)0x40040004)	//output modification register for PORT0
#define ws2812_port_clr	((uint32_t*)0x40040004)

#define ws2812_mask_set  (0x00000020)	// Bitmask to set the data out pin 5
#define ws2812_mask_clr  (0x00200000)	// Bitmask to clear the data out pin 5

//port 1 pin 1 waere (1) und (0x00020000) und ein anderes OMR vermutlich!!!

///////////////////////////////////////////////////////////////////////
// User defined area: Define CPU clock speed
//
// The current implementation of the sendarray routine uses cycle accurate
// active waiting. The routine is automatically adjusted according to
// the clockspeed defined below. Only values between 8 Mhz and 60 Mhz
// are allowable.
//
// Important: The timing calculation assumes that there are no waitstates
// for code memory access. If there are waitstates you may have to reduce
// the value below until you get acceptable timing. It is highly recommended
// to use this library only on devices without flash waitstates and
// predictable code execution timing.
//
///////////////////////////////////////////////////////////////////////

#define ws2812_cpuclk 8000000

///////////////////////////////////////////////////////////////////////
// End user defined area
///////////////////////////////////////////////////////////////////////

#if (ws2812_cpuclk<8000000)
	#error "Minimum clockspeed for ARM ws2812 library is 8 Mhz!"
#endif

#if (ws2812_cpuclk>60000000)
	#error "Maximum clockspeed for ARM ws2812 library is 60 Mhz!"
#endif


///////////////////////////////////////////////////////////////////////
// Main function call
//
// Call with address to led color array (order is Green-Red-Blue)
// Number of bytes to be transmitted is leds*3
///////////////////////////////////////////////////////////////////////

void ws2812_sendarray(uint8_t *ledarray,int length);

#endif /* LIGHT_WS2812_H_ */
