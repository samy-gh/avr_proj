#
# template of Makefile for GNU make
#
# Last Change: 02-Mar-2011.
#

include ../config.mk


TARGET=test.exe
SRCS=test.c report_id.c

#CC=gcc -pipe

CFLAGS=-g -Wall
CFLAGS+=-I..
CFLAGS+= -O2

LDFLAGS=-L..
LIBS=-lhid
LIBS_PATH=../libhid.a


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



