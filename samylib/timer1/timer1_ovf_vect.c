
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

VOID (*gTimer1_OvfCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER1_OVF_vect)
{
	sleep_disable();

	if( gTimer1_OvfCallbackFunc ) {
		gTimer1_OvfCallbackFunc();
	}
}

