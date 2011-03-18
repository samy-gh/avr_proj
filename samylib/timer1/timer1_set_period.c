
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


UCHAR gTimer1_ClockSelectBits;
UINT gTimer1_PwmPeriod;

VOID Timer1_Set_Period( ULONG usec )
{
	ULONG ul_cycles = (ULONG)(((FLOAT)F_CPU) * usec / 1000000);		// the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
	if( ul_cycles < D_TIMER1_RESOLUTION ) {
		gTimer1_ClockSelectBits = _BV(CS10);				// no prescale, full xtal
	}
	else if( (ul_cycles >>= 3) < D_TIMER1_RESOLUTION ) {
		gTimer1_ClockSelectBits = _BV(CS11);				// prescale by /8
	}
	else if( (ul_cycles >>= 3) < D_TIMER1_RESOLUTION ) {
		gTimer1_ClockSelectBits = _BV(CS11) | _BV(CS10);	// prescale by /64
	}
	else if( (ul_cycles >>= 2) < D_TIMER1_RESOLUTION ) {
		gTimer1_ClockSelectBits = _BV(CS12);				// prescale by /256
	}
	else if( (ul_cycles >>= 2) < D_TIMER1_RESOLUTION ) {
		gTimer1_ClockSelectBits = _BV(CS12) | _BV(CS10);	// prescale by /1024
	}
	else {
		ul_cycles = D_TIMER1_RESOLUTION - 1;
		gTimer1_ClockSelectBits = _BV(CS12) | _BV(CS10);	// request was out of bounds, set as maximum
	}

	cbi( PRR, PRTIM1 );										// disable power save mode
	OCR1A = OCR1B = ICR1 = gTimer1_PwmPeriod = ul_cycles;			// ICR1 is TOP in p & f correct pwm mode
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}

