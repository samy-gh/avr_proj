
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer0_Init( ULONG usec )
{
#ifdef power_timer0_enable
	power_timer0_enable();										// disable power save mode
#endif

	// set mode as fast pwm mode, stop the timer
	TCCR0A = _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(WGM02);
	// disable all interrupts
	TIMSK0 = 0;

	gTimer0_PwmPinEnable = 0;

	Timer0_Set_Period( usec );
}

