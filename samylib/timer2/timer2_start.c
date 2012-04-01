
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer2_Start( VOID )
{
	TCCR2B |= gTimer2_ClockSelectBits;
	if( gTimer2_PwmPinAEnable != 0 ) {
		cbi( TCCR2A, COM2A0 );
		sbi( TCCR2A, COM2A1 );
	}
	if( gTimer2_PwmPinBEnable != 0 ) {
		cbi( TCCR2A, COM2B0 );
		sbi( TCCR2A, COM2B1 );
	}
}

