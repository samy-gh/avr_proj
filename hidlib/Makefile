#
# template of Makefile for GNU make
#
# Last Change: 02-Mar-2011.
#

include config.mk

TARGET=libhid.a
SRCS=libhid.c

# for Windows(MinGW)
ifeq "$(OSTYPE)" "msys"
ifeq "$(USE_LIBUSB)" "1"
SRCS+=libusb_dyn.c
endif
endif

#CC=gcc -pipe

#CFLAGS=-g
CFLAGS=-Wall
CFLAGS+=-O2

ifeq "$(USE_LIBUSB)" "1"
CFLAGS+=-Ilibusb/include -DLIBUSB
endif

OBJS=$(patsubst %.c,%.o,$(SRCS))
DEPENDS=$(patsubst %.c,%.d,$(SRCS))


.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS)
	ar r $@ $(OBJS)


.PHONY: depend
depend:
	@echo '' > /dev/null


.PHONY: cleandepend
cleandepend:
	rm -f $(DEPENDS)


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY: clean
clean:
	rm -rf $(OBJS) *.stackdump *.exe $(TARGET)


%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
	 | sed 's,$(notdir $*)\.o[ :]* \(.*\)\.c,\1.o $@ : \1.c,g' > $@; \
	 [ -s $@ ] || rm -f $@

-include $(DEPENDS)



