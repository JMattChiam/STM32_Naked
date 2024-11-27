#include "usart.h"

void usart_init(USART_TypeDef *usart)
{
  /*
  Enable USART clock
  Enable GPIO clock (PA2 and PA3?)
  Set alternate GPIO function
  Optional: Enable RX interrupt (and NVIC controller)

  Set:
  baudrate
  hardware flow control
  start bit
  parity bit
  stop bit
  data word
  */

  /*
  

  Baud rate register - mantissa and fraction, basically fixed point 12Q4 format, check if oversampling is set to 8 or 16 bits
  115200 baud (bits/sec)
  
  Control register 1:
  - USART enable
  - Word length
  - Parity control enable (use parity bit)
  - Parity selection (odd/even)

  CR2:
  - STOP bits
  - CPOL (clock polarity)
  - CPHA (clock phase)
  - 
  

  */

  /* Enable USART2 clock */
  RCC->APB1ENR |= (1 << RCC_APB1ENR_USART2EN_Pos);
  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile uint32_t dummy;
  dummy = RCC->APB1ENR;
  dummy = RCC->APB1ENR;

  /* Enable GPIOA clock*/
  RCC->AHBENR |= (1 << RCC_AHBENR_GPIOAEN_Pos);
  // do two dummy reads after enabling the peripheral clock, as per the errata
  dummy = RCC->AHBENR;
  dummy = RCC->AHBENR;

  /* Set PA2 and PA3 to alternate function */
  GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODER2_Pos) | (0b10 << GPIO_MODER_MODER3_Pos);

  /* USART2 is AF1 (found in datasheet) */
  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
  GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL2_Pos) | (1 << GPIO_AFRL_AFSEL3_Pos);
  
  /* Configure and enable USART2 */
  USART2->BRR = 434; // 115200 baud @ 50 MHz APB1 clock and 16x oversampling
  USART2->CR1 |= USART_CR1_UE | USART_CR1_TE; // USART enable and transmitter enable

  // Dummy write, because the first byte seems to always be dropped
  USART2->TDR = 0;
  while (!(USART2->ISR & USART_ISR_TC));
}


void usart_write(USART_TypeDef *usart, char c)
{
    usart->TDR = c;
    while (!(usart->ISR & USART_ISR_TC));
}