
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer1_OvfCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER1_OVF_vect)
{
	if( gTimer1_OvfCallbackFunc ) {
		gTimer1_OvfCallbackFunc();
	}

	sleep_disable();
}

