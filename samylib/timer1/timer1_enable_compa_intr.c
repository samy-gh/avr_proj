
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Enable_CompaIntr( VOID (*isr)( VOID ) )
{
	gTimer1_CompaCallbackFunc = isr;	// register the user's callback with the real ISR
	cbi( PRR, PRTIM1 );										// disable power save mode
	sbi( TIMSK1, OCIE1A );				// sets the timer compare A interrupt enable bit
}

