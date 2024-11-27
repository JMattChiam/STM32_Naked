#include <stdint.h>
#include "stm32f072xb.h"

#define LED_PIN 5

void main(void)
{
    //
    RCC->AHBENR |= (1 << RCC_AHBENR_GPIOAEN_Pos);

    // do two dummy reads after enabling the peripheral clock, as per the errata
    volatile uint32_t dummy;
    dummy = RCC->AHBENR;
    dummy = RCC->AHBENR;

    GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);

    while(1)
    {
        GPIOA->ODR ^= (1 << LED_PIN);
        for (uint32_t i = 0; i < 100000; i++); // wait for some time
    }
}