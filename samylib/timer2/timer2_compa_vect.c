
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer2_CompaCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER2_COMPA_vect)
{
	if( gTimer2_CompaCallbackFunc ) {
		gTimer2_CompaCallbackFunc();
	}

	sleep_disable();
}

