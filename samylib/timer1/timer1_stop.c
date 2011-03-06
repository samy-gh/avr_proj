
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
}


