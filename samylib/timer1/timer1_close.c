
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer1_Close( VOID )
{
	cbi( TIMSK1, OCIE1A );
	cbi( TIMSK1, OCIE1B );
	cbi( TIMSK1, TOIE1 );

	Timer1_Unset_PwmPin( 0 );
	Timer1_Unset_PwmPin( 1 );

#ifdef power_timer1_disable
	power_timer1_disable();								// enable power-save mode
#endif
}

