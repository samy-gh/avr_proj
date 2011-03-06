
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
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

