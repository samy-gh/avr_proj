
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Stop( VOID )
{
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));	// clears all clock selects bits
	TCNT1 = 0;
	cbi( TCCR1A, COM1A0 );
	cbi( TCCR1A, COM1A1 );
	cbi( TCCR1A, COM1B0 );
	cbi( TCCR1A, COM1B1 );
}


