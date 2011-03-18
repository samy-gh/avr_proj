
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/interrupt.h>
#include <avr/sleep.h>


VOID (*gTimer1_CompbCallbackFunc)( VOID ) = 0;

SIGNAL(TIMER1_COMPB_vect)
{
	if( gTimer1_CompbCallbackFunc ) {
		gTimer1_CompbCallbackFunc();
	}

	sleep_disable();
}

