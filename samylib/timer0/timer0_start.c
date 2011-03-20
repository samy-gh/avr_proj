
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Start( VOID )
{
	TCCR0B |= gTimer0_ClockSelectBits;
	if( gTimer0_PwmPinEnable != 0 ) {
		sbi( TCCR0A, COM0B1 );
	}
}

