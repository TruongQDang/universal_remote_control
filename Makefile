TARGET := myProject
MCU := atmega328p
F_CPU := 16000000UL
# 57600 for nano, 115200 for uno
BAUD := 57600

SOURCE_DIR := src core lib
BUILD_DIR := build
INCLUDE_DIR := lib/include core/include hardware tests

PROGRAMMER_TYPE := arduino
# /dev/ttyACM0 or /dev/ttyUSB0
PROGRAMMER_PORT := /dev/ttyUSB0
PROGRAMMER_ARGS := -p $(MCU) -c $(PROGRAMMER_TYPE) -P $(PROGRAMMER_PORT) -b $(BAUD)

CC := avr-gcc
OBJDUMP := avr-objdump
OBJCOPY = avr-objcopy
AVRDUDE := avrdude

CPPFLAGS := -DF_CPU=$(F_CPU) -mmcu=$(MCU) -MMD -MP $(addprefix -I,$(INCLUDE_DIR))
CFLAGS := -Os -g -Wall -std=c11

SOURCES := $(foreach dir, $(SOURCE_DIR), $(wildcard $(dir)/*.c))
OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean flash disasm

all: $(BUILD_DIR)/$(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.elf: $(OBJECTS)
	$(CC) -mmcu=$(MCU) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

disasm: $(BUILD_DIR)/$(TARGET).lst

%.lst: %.elf
	$(OBJDUMP) -S $< > $@

-include $(DEPS)

clean:
	rm -r $(BUILD_DIR)/*

flash: $(BUILD_DIR)/$(TARGET).hex 
	$(AVRDUDE) $(PROGRAMMER_ARGS) -U flash:w:$<