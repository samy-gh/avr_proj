
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Stop( VOID )
{
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));	// clears all clock selects bits
}


