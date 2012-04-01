
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer2_CompbCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER2_COMPB_vect)
{
	if( gTimer2_CompbCallbackFunc ) {
		gTimer2_CompbCallbackFunc();
	}

	sleep_disable();
}

