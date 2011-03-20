
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Stop( VOID )
{
	TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));	// clears all clock selects bits
	TCNT0 = 0;
	cbi( TCCR0A, COM0B1 );
}


