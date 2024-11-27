CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m0 -mthumb -nostdlib
CPPFLAGS=-DSTM32F072xB \
	 -Ivendor/CMSIS_5/Device/ST/STM32F0/Include \
	 -Ivendor/CMSIS_5/CMSIS/Core/Include

LINKER_FILE=linker_script.ld
LDFLAGS=-T $(LINKER_FILE) -lgcc
BINARY = blink.elf
PROGRAMMER = openocd
PROGRAMMER_FLAGS = -f interface/stlink.cfg -f target/stm32f0x.cfg

all: blink.elf

blink.elf: main.o startup.o system_stm32f0xx.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c

startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

system_stm32f0xx.o: vendor/CMSIS_5/Device/ST/STM32F0/Source/Templates/system_stm32f0xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o system_stm32f0xx.o

.PHONY:
clean:
	rm -f *.o *.elf

flash: $(BINARY)
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $(BINARY) verify reset exit"