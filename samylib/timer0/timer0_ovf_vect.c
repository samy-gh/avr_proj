
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer0_OvfCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER0_OVF_vect)
{
	if( gTimer0_OvfCallbackFunc ) {
		gTimer0_OvfCallbackFunc();
	}

	sleep_disable();
}

