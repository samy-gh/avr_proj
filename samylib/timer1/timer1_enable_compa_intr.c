
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Enable_CompaIntr( VOID (*isr)( VOID ) )
{
	gTimer1_CompaCallbackFunc = isr;	// register the user's callback with the real ISR
	cbi( PRR, PRTIM1 );										// disable power save mode
	TIMSK1 |= _BV(OCIE1A);				// sets the timer compare A interrupt enable bit
}

