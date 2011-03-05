
#include "timer1.h"
#include <avr/io.h>

VOID Timer1_Enable_OvfIntr( VOID (*isr)( VOID ) )
{
	gTimer1_CallbackFunc = isr;			// register the user's callback with the real ISR
	cbi( PRR, PRTIM1 );										// disable power save mode
	TIMSK1 |= _BV(TOIE1);				// sets the timer overflow interrupt enable bit
}

