
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer2_Enable_OvfIntr( VOID (*isr)( VOID ) )
{
	gTimer2_OvfCallbackFunc = isr;		// register the user's callback with the real ISR
	sbi( TIMSK2, TOIE2 );				// sets the timer overflow interrupt enable bit
}

