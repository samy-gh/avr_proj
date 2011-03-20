
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>

// STD-C
#include <stdio.h>


VOID (*gTimer0_CompbCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER0_COMPB_vect)
{
	if( gTimer0_CompbCallbackFunc ) {
		gTimer0_CompbCallbackFunc();
	}

	sleep_disable();
}

