#ifndef _USART_H_
#define _USART_H_
#include "stm32f072xb.h"

void usart_init(USART_TypeDef *usart);
void usart_write(USART_TypeDef *usart, char c);

#endif