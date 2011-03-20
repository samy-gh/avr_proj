
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Enable_OvfIntr( VOID (*isr)( VOID ) )
{
	gTimer0_OvfCallbackFunc = isr;		// register the user's callback with the real ISR
	sbi( TIMSK0, TOIE0 );				// sets the timer overflow interrupt enable bit
}

