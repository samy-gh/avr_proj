
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Start( VOID )
{
	cbi( PRR, PRTIM1 );										// disable power save mode
	TCCR1B |= gTimer1_ClockSelectBits;
}

