#
# template of Makefile for GNU make
#
# Last Change: 05-Mar-2011.
#

##
## デバイス設定
##
DEVICE = atmega328
#DEVICE = attiny2313
#F_CPU = 12000000
F_CPU = 16000000

OSC=$(subst 0,,$(F_CPU))


##
## ソースコード設定
##
TARGET=main-$(OSC).hex
CSRCS+=main.c
CSRCS+=usbdrv/usbdrv.c
ASMSRCS+=usbdrv/usbdrvasm.S


##
## ライブラリ設定
##
include ../samylib/samylib.mak
# USART送信割り込みはUSB通信を阻害するので無効にする
CFLAGS+= -DD_USART_TX_BUFFER_SIZE=0


##
## コンパイラ設定
##
CC=avr-gcc
AR=avr-ar
LD=avr-ld


##
## コンパイルオプション設定
##
CFLAGS+= -Wall
CFLAGS+= -Os
CFLAGS+= -mmcu=$(DEVICE)
CFLAGS+= $(DEFINES) -I. -DDEBUG_LEVEL=0 -DF_CPU=$(F_CPU)
CFLAGS+= -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS+= -gdwarf-2 -std=gnu99
CFLAGS+= -fdata-sections -ffunction-sections
CFLAGS+= -ffreestanding
CFLAGS+= -mcall-prologues -fno-tree-scev-cprop -fno-split-wide-types


##
## リンクオプション設定
##
LDFLAGS= -Wl,--relax,--gc-sections



OBJS=$(patsubst %.c,%.o,$(CSRCS))
OBJS+=$(patsubst %.S,%.o,$(ASMSRCS))
DEPENDS=$(patsubst %.c,%.d,$(CSRCS))
DEPENDS+=$(patsubst %.S,%.d,$(ASMSRCS))
TARGET_ELF=$(patsubst %.hex,%.elf,$(TARGET))


.PHONY: all
all: $(TARGET)


%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET)
	avr-size --mcu=$(DEVICE) $(TARGET_ELF)


$(TARGET_ELF): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)


.PHONY: depend
depend:
	@echo '' > /dev/null


.PHONY: cleandepend
cleandepend:
	rm -f $(DEPENDS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


%.S: %.c
	$(CC) $(CFLAGS) -o $@ -S $<


%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -o $@ -c $<


%.o: %.s
	$(CC) $(CFLAGS) -x assembler-with-cpp -o $@ -c $<


%.o: %.asm
	$(CC) $(CFLAGS) -x assembler-with-cpp -o $@ -c $<


.PHONY: clean
clean:
	rm -rf $(OBJS) *.stackdump *.exe *.elf *.hex *.eep $(TARGET)


%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
	 | sed 's,$(notdir $*)\.o[ :]* \(.*\)\.c,\1.o $@ : \1.c,g' > $@; \
	 [ -s $@ ] || rm -f $@

-include $(DEPENDS)


