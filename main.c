#include <stdint.h>

#define LED_PIN 5

#define PERIPHERAL_BASE (0x40000000U)
#define AHB1_BASE (PERIPHERAL_BASE + 0x20000U)
#define AHB2_BASE (PERIPHERAL_BASE + 0x8000000U)

#define RCC_BASE (AHB1_BASE + 0x1000U)
#define RCC_AHBENR_OFFSET (0x14U)
#define RCC_APB1ENR ((volatile uint32_t*) (RCC_BASE + RCC_APB1ENR_OFFSET))
#define RCC_AHBENR ((volatile uint32_t*) (RCC_BASE + RCC_AHBENR_OFFSET))
#define RCC_AHBENR_GPIOAEN (17)

#define GPIOA_BASE (AHB2_BASE)
#define GPIO_MODER_OFFSET (0x00U)
#define GPIOA_MODER ((volatile uint32_t*) (GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER5 (10U)
#define GPIO_OTYPER_OFFSET (0x04U)
#define GPIO_ODR_OFFSET (0x14U)
#define GPIOA_ODR ((volatile uint32_t*) (GPIOA_BASE + GPIO_ODR_OFFSET))

void main(void)
{
    //
    *RCC_AHBENR |= (1 << RCC_AHBENR_GPIOAEN);

    // do two dummy reads after enabling the peripheral clock, as per the errata
    volatile uint32_t dummy;
    dummy = *(RCC_AHBENR);
    dummy = *(RCC_AHBENR);

    *GPIOA_MODER |= (1 << GPIO_MODER_MODER5);

    while(1)
    {
        *GPIOA_ODR ^= (1 << LED_PIN);
        for (uint32_t i = 0; i < 100000; i++); // wait for some time
    }
}