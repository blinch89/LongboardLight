# LongboardLight
A try to create LED-powered light effects under a longboard deck.

__Hardware__
LEDs:               a strip of WS2812 or WS2811, 30LEDs/meter
microcontroller:    STM32F4 Discovery Board (will may be replaced by a smaller custom board or a smaller NXP board)
power supply:       2,2Ah USB power bank, max. current 1A (not enough, when all 22 LEDs are white at full brightness)
housing:            a custom made plywood housing, hold by the 4 screws of the front truck (axle)
miscellaneous:      * a small perfboard pcb to distribute 5V and GND
                    * a small perfboard pcb with SN74HCT245N for level shifting from 3V to 5V (STM32 -> LED strip).

__Firmware__
*   WS2812 driver library "" from "", link .............. TODO!!! enter sources here
*   the LED-signal is generated completely CPU-independend by a DMA-controller
*   only changes in the memory area of the LEds color pattern or their brightess require cpu activity
*   switching between different modes or color patterns via the STM32F4s onboard user button


__TODO__
*   maybe a better housing, small as possible
*   try to port all features to a smaller microcontrollerboard (NXP?), because the most
    of the STM boards capacity is not used
*   attach an inductive sensor to the truck and insert a piece of metal into one wheel
    to detect the actual speed for adjusting animations to it
*   build a boost converter, because 24V is needed for an industrial inductive sensor
*   Thanks to line needs to be inserted here in this file!!!!!!!!!!!!!!!!!!!!!!!
