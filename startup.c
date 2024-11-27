#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (16U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)
#define ISR_VECTOR_SIZE_WORDS 48

void reset_handler(void);
void default_handler(void);

// Cortex-M system exceptions
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

// STM32F072RB interrupt handlers
void wwdg_handler(void) __attribute__((weak, alias("default_handler")));
void pvd_handler(void) __attribute__((weak, alias("default_handler")));
void rtc_handler(void) __attribute__((weak, alias("default_handler")));
void flash_handler(void) __attribute__((weak, alias("default_handler")));
void rcc_handler(void) __attribute__((weak, alias("default_handler")));
void exti0_1_handler(void) __attribute__((weak, alias("default_handler")));
void exti2_3_handler(void) __attribute__((weak, alias("default_handler")));
void exti4_15_handler(void) __attribute__((weak, alias("default_handler")));
void tsc_handler(void) __attribute__((weak, alias("default_handler")));
void dma_ch1_handler(void) __attribute__((weak, alias("default_handler")));
void dma_ch2_3_handler(void) __attribute__((weak, alias("default_handler")));
void dma_ch4_5_6_7_handler(void) __attribute__((weak, alias("default_handler")));
void adc_comp_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_brk_up_trg_com_handler(void) __attribute__((weak, alias("default_handler")));
void tim1_cc_handler(void) __attribute__((weak, alias("default_handler")));
void tim2_handler(void) __attribute__((weak, alias("default_handler")));
void tim3_handler(void) __attribute__((weak, alias("default_handler")));
void tim6_dac_handler(void) __attribute__((weak, alias("default_handler")));
void tim7_handler(void) __attribute__((weak, alias("default_handler")));
void tim14_handler(void) __attribute__((weak, alias("default_handler")));
void tim15_handler(void) __attribute__((weak, alias("default_handler")));
void tim16_handler(void) __attribute__((weak, alias("default_handler")));
void tim17_handler(void) __attribute__((weak, alias("default_handler")));
void i2c1_handler(void) __attribute__((weak, alias("default_handler")));
void i2c2_handler(void) __attribute__((weak, alias("default_handler")));
void spi1_handler(void) __attribute__((weak, alias("default_handler")));
void spi2_handler(void) __attribute__((weak, alias("default_handler")));
void usart1_handler(void) __attribute__((weak, alias("default_handler")));
void usart2_handler(void) __attribute__((weak, alias("default_handler")));
void usart3_4_5_6_7_8_handler(void) __attribute__((weak, alias("default_handler")));
void cec_can_handler(void) __attribute__((weak, alias("default_handler")));
void usb_handler(void) __attribute__((weak, alias("default_handler")));

uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
  STACK_POINTER_INIT_ADDRESS,              // Initial stack pointer
  (uint32_t)&reset_handler,                // Reset handler
  (uint32_t)&nmi_handler,                  // NMI handler
  (uint32_t)&hard_fault_handler,           // Hard Fault handler
  0, 
  0,
  0, 
  0, 
  0, 
  0, 
  0,                     // Reserved entries
  (uint32_t)&svcall_handler,               // SVCall handler
  0, 
  0,                                    // Reserved entries
  (uint32_t)&pendsv_handler,               // PendSV handler
  (uint32_t)&systick_handler,              // SysTick handler
  (uint32_t)&wwdg_handler,                 // Peripheral handlers
  (uint32_t)&pvd_handler,
  (uint32_t)&rtc_handler,
  (uint32_t)&flash_handler,
  (uint32_t)&rcc_handler,
  (uint32_t)&exti0_1_handler,
  (uint32_t)&exti2_3_handler,
  (uint32_t)&exti4_15_handler,
  (uint32_t)&tsc_handler,
  (uint32_t)&dma_ch1_handler,
  (uint32_t)&dma_ch2_3_handler,
  (uint32_t)&dma_ch4_5_6_7_handler,
  (uint32_t)&adc_comp_handler,
  (uint32_t)&tim1_brk_up_trg_com_handler,
  (uint32_t)&tim1_cc_handler,
  (uint32_t)&tim2_handler,
  (uint32_t)&tim3_handler,
  (uint32_t)&tim6_dac_handler,
  (uint32_t)&tim7_handler,
  (uint32_t)&tim14_handler,
  (uint32_t)&tim15_handler,
  (uint32_t)&tim16_handler,
  (uint32_t)&tim17_handler,
  (uint32_t)&i2c1_handler,
  (uint32_t)&i2c2_handler,
  (uint32_t)&spi1_handler,
  (uint32_t)&spi2_handler,
  (uint32_t)&usart1_handler,
  (uint32_t)&usart2_handler,
  (uint32_t)&usart3_4_5_6_7_8_handler,
  (uint32_t)&cec_can_handler,
  (uint32_t)&usb_handler
};

// These pointers to the memory sections are defined in the linker script
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _sidata;
void main(void);

void reset_handler(void)
{
  // Copy .data from FLASH to SRAM
  uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *flash_data = (uint8_t*) &_sidata; // Data load address (in flash)
  uint8_t *sram_data = (uint8_t*) &_sdata; // Data virtual address (in sram)
  
  for (uint32_t i = 0; i < data_size; i++)
  {
    sram_data[i] = flash_data[i];
  }

  // Zero-fill .bss section in SRAM
  uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  uint8_t *bss = (uint8_t*) &_sbss;

  for (uint32_t i = 0; i < bss_size; i++)
  {
    bss[i] = 0;
  }
  
  main();
}

void default_handler(void)
{
  while(1);
}