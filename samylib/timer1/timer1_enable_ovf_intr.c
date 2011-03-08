
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Enable_OvfIntr( VOID (*isr)( VOID ) )
{
	gTimer1_OvfCallbackFunc = isr;		// register the user's callback with the real ISR
	sbi( TIMSK1, OCIE1A );				// sets the timer compare A interrupt enable bit
//	sbi( TIMSK1, TOIE1 );				// sets the timer overflow interrupt enable bit
}

