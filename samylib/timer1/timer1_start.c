
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Start( VOID )
{
	TCCR1B |= gTimer1_ClockSelectBits;
	if( gTimer1_PwmPinAEnable != 0 ) {
		cbi( TCCR1A, COM1A0 );
		sbi( TCCR1A, COM1A1 );
	}
	if( gTimer1_PwmPinBEnable != 0 ) {
		cbi( TCCR1A, COM1B0 );
		sbi( TCCR1A, COM1B1 );
	}
}

