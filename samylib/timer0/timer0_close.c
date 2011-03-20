
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer0_Close( VOID )
{
	cbi( TIMSK0, OCIE0A );
	cbi( TIMSK0, OCIE0B );
	cbi( TIMSK0, TOIE0 );

	Timer0_Unset_PwmPin();

#ifdef power_timer0_disable
	power_timer0_disable();								// enable power-save mode
#endif
}

