
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer2_OvfCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER2_OVF_vect)
{
	if( gTimer2_OvfCallbackFunc ) {
		gTimer2_OvfCallbackFunc();
	}

	sleep_disable();
}

