
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Start( VOID )
{
	cbi( PRR, PRTIM1 );										// disable power save mode
	TCCR1B |= gTimer1_ClockSelectBits;
}

