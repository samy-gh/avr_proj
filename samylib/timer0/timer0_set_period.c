
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


UCHAR gTimer0_ClockSelectBits;
UCHAR gTimer0_PwmPeriod;

VOID Timer0_Set_Period( ULONG usec )
{
	ULONG ul_cycles = (ULONG)(((FLOAT)F_CPU) * usec / 1000000);		// the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
	if( ul_cycles < D_TIMER0_RESOLUTION ) {
		gTimer0_ClockSelectBits = _BV(CS00);				// no prescale, full xtal
	}
	else if( (ul_cycles >>= 3) < D_TIMER0_RESOLUTION ) {
		gTimer0_ClockSelectBits = _BV(CS01);				// prescale by /8
	}
	else if( (ul_cycles >>= 3) < D_TIMER0_RESOLUTION ) {
		gTimer0_ClockSelectBits = _BV(CS01) | _BV(CS00);	// prescale by /64
	}
	else if( (ul_cycles >>= 2) < D_TIMER0_RESOLUTION ) {
		gTimer0_ClockSelectBits = _BV(CS02);				// prescale by /256
	}
	else if( (ul_cycles >>= 2) < D_TIMER0_RESOLUTION ) {
		gTimer0_ClockSelectBits = _BV(CS02) | _BV(CS00);	// prescale by /1024
	}
	else {
		ul_cycles = D_TIMER0_RESOLUTION - 1;
		gTimer0_ClockSelectBits = _BV(CS02) | _BV(CS00);	// request was out of bounds, set as maximum
	}

	OCR0A = OCR0B = gTimer0_PwmPeriod = ul_cycles;			// ICR0 is TOP in p & f correct pwm mode
	TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
}

