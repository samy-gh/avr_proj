
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


UCHAR volatile gTimer2_ClockSelectBits;
UINT volatile gTimer2_PwmPeriod;

VOID Timer2_Set_Period( ULONG usec )
{
	ULONG ul_cycles = (ULONG)(((FLOAT)F_CPU) * usec / 1000000);		// the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
	if( ul_cycles < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS20);				// no prescale, full xtal
	}
	else if( (ul_cycles >>= 3) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS21);				// prescale by /8
	}
	else if( (ul_cycles >>= 2) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS21) | _BV(CS20);	// prescale by /32
	}
	else if( (ul_cycles >>= 1) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS22);				// prescale by /64
	}
	else if( (ul_cycles >>= 1) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS22) | _BV(CS20);	// prescale by /128
	}
	else if( (ul_cycles >>= 2) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS22) | _BV(CS21);	// prescale by /256
	}
	else if( (ul_cycles >>= 2) < D_TIMER2_RESOLUTION ) {
		gTimer2_ClockSelectBits = _BV(CS22) | _BV(CS21) | _BV(CS20);	// prescale by /1024
	}
	else {
		ul_cycles = D_TIMER2_RESOLUTION - 1;
		gTimer2_ClockSelectBits = _BV(CS22) | _BV(CS21) | _BV(CS20);	// request was out of bounds, set as maximum
	}

	OCR2A = OCR2B = gTimer2_PwmPeriod = ul_cycles;
	TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));
}

