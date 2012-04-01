
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer2_Stop( VOID )
{
	TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));	// clears all clock selects bits
	TCNT2 = 0;
	cbi( TCCR2A, COM2A0 );
	cbi( TCCR2A, COM2A1 );
	cbi( TCCR2A, COM2B0 );
	cbi( TCCR2A, COM2B1 );
}


