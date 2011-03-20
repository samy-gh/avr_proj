
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Enable_CompbIntr( VOID (*isr)( VOID ) )
{
	gTimer0_CompbCallbackFunc = isr;	// register the user's callback with the real ISR
	cbi( PRR, PRTIM0 );					// disable power save mode
	sbi( TIMSK0, OCIE0B );				// sets the timer compare A interrupt enable bit
}

