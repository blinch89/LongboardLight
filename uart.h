#include <stm32f4xx_usart.h>

void init_USART1(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void USART_putc(USART_TypeDef* USARTx, volatile char c);
void USART1_IRQHandler();

extern volatile uint8_t control;
extern volatile uint8_t changeFlag;
