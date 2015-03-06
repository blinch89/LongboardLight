# LongboardLight
A try to create LED-powered light effects under a longboard deck.


__Hardware__

LEDs:
a strip of WS2812 or WS2811, 30LEDs/meter

microcontroller:
XMC2GO (ARM Cortex M0)

power supply:
2,2Ah USB power bank, max. current 1A (not enough, when all 21 LEDs are white at full brightness)


__Firmware__

*   WS2812 driver library from CPLDCPU, link https://github.com/cpldcpu/light_ws2812
*   switching between different modes or color patterns via button


__TODO__

*   attach an inductive sensor to the truck and insert a piece of metal into one wheel
    to detect the actual speed for adjusting animations to it
*   build a boost converter, because 24V is needed for an industrial inductive sensor


THANKS!!! to CPLDCPU for his awesome WS2812 driver