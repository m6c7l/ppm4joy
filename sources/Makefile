# Copyright (c) 2018, Manfred Constapel
# This file is licensed under the terms of the MIT license.

PROG=siprog
#PROG=atmelice_isp
#PROG=jtag3isp
#PORT=-P /dev/ttyUSB0
#PORT=-P usb
#BAUD=-B 115200

TARGET=ppm4joy
PART=t2313

ASM=$(wildcard usb/usbdrv/*.S)
SRC=$(wildcard usb/usbdrv/*.c) $(wildcard ppm/*.c) $(wildcard *.c)

MCU=attiny2313
FCPU=12000000UL
# div clk by 8, no clk out, start-up 14 ck + 4,1 ms, use *external* osc
LFUSE=0b11111110
EFUSE=0b11111111
HFUSE=0b10011111

LIBAVR=-I /usr/lib/avr/include
RM:=rm -rf

CFLAGS=-w -Os -mmcu=$(MCU) -DF_CPU=$(FCPU) -MMD -MP -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields
AFLAGS=-x assembler-with-cpp -mmcu=$(MCU) -DF_CPU=$(FCPU) -MMD -MP
LFLAGS=-Wl,-Map,$(TARGET).map -mmcu=$(MCU)
OCHFLAGS=-R .eeprom -R .fuse -R .lock -R .signature
OCEFLAGS=-j .eeprom --no-change-warnings --change-section-lma .eeprom=0

OBJS=$(SRC:%.c=%.o)
DEPS=$(ASM:%.S=%.o)

list:
	@grep '^[a-z].*:' $(lastword $(MAKEFILE_LIST)) | cut -d':' -f1 | grep -w -v '^list'

$(TARGET).elf: $(OBJS) $(DEPS) 
	@echo '* Invoking AVR C Linker: $@'
	avr-gcc $(LFLAGS) -o $(TARGET).elf $(OBJS) $(DEPS)
	@$(RM) $(OBJS) $(DEPS)

$(TARGET).hex: $(TARGET).elf
	@echo '* Create flash image (ihex format): $@'
	-avr-objcopy $(OCHFLAGS) -O ihex $(TARGET).elf $(TARGET).hex
	-avr-size --format=avr --mcu=$(MCU) -C $(TARGET).elf

$(TARGET).eep: $(TARGET).elf
	@echo '* Create eeprom image (ihex format): $@'
	-avr-objcopy $(OCEFLAGS) -O ihex $(TARGET).elf $(TARGET).eep

$(TARGET).lss: $(TARGET).elf
	@echo '* Create extended listing'
	-avr-objdump -h -S $(TARGET).elf > "$(TARGET).lss"

.c.o:
	@echo '* Invoking AVR Compiler: $@'
	@$(RM) $@
	avr-gcc $(LIBAVR) $(CFLAGS) -c -o $@ $<
	@$(RM) $(@:.o=.d)

.S.o:
	@echo '* Invoking AVR Assembler: $@'
	@$(RM) $@
	avr-gcc $(LIBAVR) $(AFLAGS) -c -o $@ $<
	@$(RM) $(@:.o=.d)

flash: $(TARGET).hex 

eeprom: $(TARGET).eep

listing: $(TARGET).lss

upload:
	@echo '* Upload flash memory image'
	-avrdude -p $(PART) -c $(PROG) $(PORT) $(BAUD) -U lfuse:w:${LFUSE}:m -U hfuse:w:${HFUSE}:m -U efuse:w:${EFUSE}:m -U flash:w:${TARGET}.hex

clean:
	@$(RM) $(OBJS) $(DEPS) $(TARGET).elf $(TARGET).eep $(TARGET).hex $(TARGET).lss $(TARGET).map
