
// timer1
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer2_Close( VOID )
{
	cbi( TIMSK2, OCIE2A );
	cbi( TIMSK2, OCIE2B );
	cbi( TIMSK2, TOIE2 );

	Timer2_Unset_PwmPin( 0 );
	Timer2_Unset_PwmPin( 1 );

#ifdef power_timer2_disable
	power_timer2_disable();								// enable power-save mode
#endif
}

