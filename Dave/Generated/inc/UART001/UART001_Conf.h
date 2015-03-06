/*******************************************************************************
**  DAVE App Name : UART001       App Version: 1.0.24               
**  This file is generated by DAVE, User modification to this file will be    **
**  overwritten at the next code generation.                                  **
*******************************************************************************/


/*CODE_BLOCK_BEGIN[UART001_Conf.h]*/
/*******************************************************************************
**                                                                            **
** Copyright (C) 2013 Infineon Technologies AG. All rights reserved.          **
**                                                                            **
** Infineon Technologies AG (Infineon) is supplying this software for use     **
** with Infineon's microcontrollers.                                          **
** This file can be freely distributed within development tools that are      **
** supporting such microcontrollers.                                          **
**                                                                            **
** THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS,        **
** IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF **
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS         **
** SOFTWARE. INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, ** 
** INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.           **
**                                                                            **
********************************************************************************
**                                                                            **
** PLATFORM : Infineon XMC4000/XMC1000 Series                                 **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR   : App Developer                                                   **
**                                                                            **
** NOTE     : Any Changes made to this file will be overwritten during Code   **
**            Generation                                                      **
**                                                                            **
********************************************************************************
** VERSION HISTORY:
********************************************************************************
** 29 Nov 2013 v1.0.20  1. Added global macro definitions required by UART001 
**                      App.
**                      2. Removed DAVE3.h inclusion to fix the MISR warning for
**                      multiple includes.     
*******************************************************************************/
/**
 * @file   UART001_Conf.h
 * 
 * @App Version UART001 <1.0.24>
 *
 * @brief  Configuration file generated based on UI settings 
 *         of UART001 App
 *
 */
#ifndef _CONF_UART001_H_
#define _CONF_UART001_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** @Defines
*******************************************************************************/   
/*System Core clock frequency in MHz */
#define UART001_CLOCK    8.0F 
/*Base Address of mapped USIC Module for App Instance 0 */
#define UART001_0_USIC 	  USIC0
/*Base Address of mapped USIC Channel for App Instance 0 */
#define UART001_0_USIC_CH  USIC0_CH1
/*USIC0 peripheral module has been configured */
#define UART001_PER0_USIC0_ENABLED
/*Macro specifying the transmit FIFO buffer has been enabled */
#define UART001_TXFIFO_ENABLE  
/*Macro specifying the receive FIFO buffer has been enabled */
#define UART001_RXFIFO_ENABLE 
/*Macro specifying the number of UART001 handles */
#define UART001_NUM_HANDLES   1

#ifdef __cplusplus
}
#endif   

#endif /* End of _CONF_UART001_H_ */
/*CODE_BLOCK_END*/

