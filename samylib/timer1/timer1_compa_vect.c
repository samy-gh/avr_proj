
#include "timer1.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

VOID (*gTimer1_CompaCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER1_COMPA_vect)
{
	if( gTimer1_CompaCallbackFunc ) {
		gTimer1_CompaCallbackFunc();
	}

	sleep_disable();
}

