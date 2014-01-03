#
# template of Makefile for GNU make
#
# Last Change: 03-Jan-2014.
#

include ../../hidlib/config.mk


TARGET=ir_aircon.exe
SRCS+=report_id.c
SRCS+=ir_frame.c
SRCS+=ir_usb.c

#CC=gcc -pipe

CFLAGS=-g -Wall
CFLAGS+=-I..
CFLAGS+=-I../../hidlib
CFLAGS+= -O2

LDFLAGS=-L../../hidlib
LIBS=-lhid
LIBS_PATH=../../hidlib/libhid.a


# for MinGW
ifeq "$(OSTYPE)" "msys"
LDFLAGS+=-mwindows -s -Wl,--subsystem,console
ifneq "$(USE_LIBUSB)" "1"
# without libusb
LIBS+=-lsetupapi
endif
endif

# for cygwin
ifeq "$(OSTYPE)" "cygwin"
LIBS+=-lusb
endif
# for unix
ifeq "$(OSTYPE)" "unix"
LIBS+=-lusb
endif



OBJS=$(patsubst %.c,%.o,$(SRCS))
DEPENDS=$(patsubst %.c,%.d,$(SRCS))


.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS) $(LIBS_PATH)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)


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



