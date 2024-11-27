#include <stdint.h>
#include <stdio.h>
#include "usart.h"
#include "stm32f072xb.h"

#define LED_PIN 5

void clock_init(); 
void delay_ms(uint32_t milliseconds);

volatile uint32_t ticks;


void main(void)
{
    clock_init();
    usart_init(USART2);
    SystemCoreClockUpdate();

    RCC->AHBENR |= (1 << RCC_AHBENR_GPIOAEN_Pos);

    // do two dummy reads after enabling the peripheral clock, as per the errata
    volatile uint32_t dummy;
    dummy = RCC->AHBENR;
    dummy = RCC->AHBENR;

    GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);

    SysTick_Config(48000); // Configure the timer reload value
    __enable_irq(); //systick is an interrupt that calls systick_handler()

    while(1)
    {
        GPIOA->ODR ^= (1 << LED_PIN);
        printf("[%.3f] Hello, World!\r\n", (float)ticks/1000.0);
        delay_ms(500);
    }
}


// Initialize the clock. Use external HSE clock from the ST-Link MCU at 8MHz
void clock_init() 
{   
    RCC->CR |= RCC_CR_HSEBYP_Msk | RCC_CR_HSEON_Msk;
    while (!(RCC->CR & RCC_CR_HSERDY_Msk));

    FLASH->ACR |= FLASH_ACR_LATENCY; 

    // Configure the PLL
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV; 
    RCC->CFGR |= RCC_CFGR_PLLMUL6;   

    // Enable PLL and wait for ready
    RCC->CR |= RCC_CR_PLLON_Msk;
    while (! (RCC->CR & RCC_CR_PLLRDY_Msk));

    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (! (RCC->CFGR & RCC_CFGR_SW_PLL));
}


void delay_ms(uint32_t milliseconds)
{
  uint32_t start = ticks;
  uint32_t end = start + milliseconds;

  if (end < start) // handle overflow
  {
    while (ticks > start); // wait for ticks to wrap around to zero
  }

  while (ticks < end);
}


// Redefinition of the handler specified in startup.c 
void systick_handler()
{
  ticks++;
}