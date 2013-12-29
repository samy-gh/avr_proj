#
# template of Makefile for GNU make
#
# Last Change: 16-Jul-2012.
#

##
## �f�o�C�X�ݒ�
##
DEVICE = atmega328p
#DEVICE = attiny2313
#F_CPU = 12000000
F_CPU = 16000000

OSC=$(subst 0,,$(F_CPU))

FUSE_LO=0xFF
FUSE_HI=0xDA
FUSE_EX=0xFF


##
## �\�[�X�R�[�h�ݒ�
##
TARGET=main-$(OSC).hex
CSRCS+=main.c
CSRCS+=framework_hal.c
CSRCS+=event_handle.c
CSRCS+=usb.c

CSRCS+=usbdrv/usbdrv.c
ASMSRCS+=usbdrv/usbdrvasm.S


##
## ���C�u�����ݒ�
##
include ../samylib/samylib.mak
# LCD����wait���Ԃ̔{�� (3.3V�쓮����2�ɂ���)
CFLAGS+= -DD_LCD_WAIT_FACTOR=2
# �{�^���C�x���g����
CFLAGS+= -DCO_ENABLE_PCINT10_BTN
CFLAGS+= -DCO_ENABLE_PCINT11_BTN
# �X���[�v���[�h�L��
#CFLAGS+= -DCO_SLEEP_ENABLE
# samylib �ăR���p�C�����K�v
#CFLAGS+= -DCO_ENABLE_EVENT_TRACE_LOG
CFLAGS+= -DCO_ENABLE_SERIAL_CMD
CFLAGS+= -DCO_ENABLE_SOFTTIMER
CFLAGS+= -DE_TIMER_SYSTEM_MAX=3
# USART���M���荞�ݖ���
CFLAGS+= -DD_USART_TX_BUFFER_SIZE=0
# I2C slave read�C�x���g�L��
CFLAGS+= -DCO_ENABLE_I2C_SLAVE_READ
# IR ��M�C�x���g�L��
CFLAGS+= -DCO_ENABLE_IR_RECV_EVENT
# IR ���M�C�x���g�L��
CFLAGS+= -DCO_ENABLE_IR_SEND_EVENT


##
## �R���p�C���ݒ�
##
CC=avr-gcc
AR=avr-ar
LD=avr-ld


##
## �R���p�C���I�v�V�����ݒ�
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
## �����N�I�v�V�����ݒ�
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

