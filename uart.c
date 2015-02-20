#include "uart.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include "stm32f4xx_rcc.h"
void init_USART1(uint32_t baudrate)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;    //GPIO pins used as TX and RX
    USART_InitTypeDef USART_InitStruct;  //USART1 initilization
    NVIC_InitTypeDef NVIC_InitStructure; //configure NVIC (nested vector interrupt controller)
    
    // enable APB2 peripheral clock for USART1 
    // note that only USART1 and USART6 are connected to APB2
    // the other USARTs are connected to APB1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // enable the peripheral clock for the pins used by 
    // USART1, PB6 for TX and PB7 for RX
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // This sequence sets up the TX and RX pins to be
    // used by USART1 peripheral
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;           // alternate function so the USART peripheral has access to them
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // this defines the IO speed and has nothing to do with the baudrate!
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;         // output type = push pull mode (as opposed to open drain)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;           // activate the pullup resistors
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // The RX and TX pins are now connected to their AF
    // so USART1 can take over control of the pins
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
    
    // USART_InitStruct is used to define the properties of USART1 
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b; // data frame size = 8 bits (standard)
    USART_InitStruct.USART_StopBits = USART_StopBits_1;      // 1 stop bit (standard)
    USART_InitStruct.USART_Parity = USART_Parity_No;         // no parity bit (standard)
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // no flow control (standard)
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enable transmitter & receiver
    USART_Init(USART1, &USART_InitStruct);
    
    
    // USART1 receive interrupt enable
    // interrupt controller config     
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable USART1 receive interrupt 
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;        // following param. are meant for USART1 interr.
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// set priority group of the USART1 interrupts
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       // set subpriority inside group
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          // USART1 interrupts are globally enabled
    NVIC_Init(&NVIC_InitStructure);   

    USART_Cmd(USART1, ENABLE); // finally this enables the complete USART1 peripheral
}





void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
    while(*s)
    {
        // wait until data register is empty
        while( !(USARTx->SR & 0x00000040) ); 
        USART_SendData(USARTx, *s++);
    }
}




void USART_putc(USART_TypeDef* USARTx, char c)
{
    // wait until data register is empty
    while( !(USARTx->SR & 0x00000040) ); 
    USART_SendData(USARTx, c);
}



// This method is called, if an USART1 interrupt occures.
void USART1_IRQHandler()
{
    // check if the USART1 receive interrupt flag was set
    if( USART_GetITStatus(USART1, USART_IT_RXNE) )
    {
        // USART1->DR   this line reads the received data
    }
}
