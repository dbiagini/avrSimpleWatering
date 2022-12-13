Simple Watering system based on ATMEGA8L

How to read flash
-avrdude -c avrispmkII -p atmega8 -P usb -U flash:w:flash.bin

avrdude -c avrispmkII -p atmega8 -P usb -e -U lfuse:w:0xb4:m -U hfuse:w:0xd9:m -U flash:w:$(APP).hex

Using Crosspack avr-gcc compiler on Mac

read Fuses:
-c usbasp -p m8 -U lfuse:r:low_fuse_val.hex:h -U hfuse:r:high_fuse_val.hex:h


