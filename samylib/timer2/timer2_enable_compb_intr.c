
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer2_Enable_CompbIntr( VOID (*isr)( VOID ) )
{
	gTimer2_CompbCallbackFunc = isr;	// register the user's callback with the real ISR
	cbi( PRR, PRTIM1 );										// disable power save mode
	sbi( TIMSK2, OCIE2B );				// sets the timer compare B interrupt enable bit
}

