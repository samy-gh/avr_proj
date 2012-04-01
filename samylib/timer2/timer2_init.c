
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer2_Init( ULONG usec )
{
#ifdef power_timer2_enable
	power_timer2_enable();										// disable power save mode
#endif

	// set mode as fast pwm mode, stop the timer
	TCCR2A = _BV(WGM21) | _BV(WGM20);
	TCCR2B = _BV(WGM22);
	TIMSK2 = 0;					// disable all interrupts

	gTimer2_PwmPinAEnable = 0;
	gTimer2_PwmPinBEnable = 0;

	Timer2_Set_Period( usec );
}

