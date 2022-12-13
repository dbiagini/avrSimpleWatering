TOOLCHAIN=/usr/local/bin

CC = $(TOOLCHAIN)/avr-gcc -Wall -Os -mmcu=atmega8
OBJCOPY = $(TOOLCHAIN)/avr-objcopy
AVRDUDE = avrdude

APP=blinky
OBJECTS= blinky.o timer.o

all: $(APP).elf

.c.o:
	$(CC) -c $< -o $@

.S.o:
	$(CC) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(CC) -S $< -o $@

$(APP).elf: $(OBJECTS)
	$(CC) -o $(APP).elf $(OBJECTS) 

$(APP).hex: $(APP).elf
	rm -f $(APP).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(APP).elf $(APP).hex
	avr-size --format=avr --mcu=atmega8 $(APP).elf

program: $(APP).hex
	avrdude -c avrispmkII -p atmega8 -P usb -e -U lfuse:w:0xb4:m -U hfuse:w:0xd9:m -U flash:w:$(APP).hex
clean:
	rm -f $(APP).elf $(OBJECTS)


