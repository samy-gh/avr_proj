
#include "timer1.h"
#include <avr/io.h>

VOID Timer1_Enable_OvfIntr( VOID (*isr)( VOID ) )
{
	gTimer1_OvfCallbackFunc = isr;		// register the user's callback with the real ISR
	TIMSK1 |= _BV(OCIE1A);				// sets the timer compare A interrupt enable bit
//	TIMSK1 |= _BV(TOIE1);				// sets the timer overflow interrupt enable bit
}

