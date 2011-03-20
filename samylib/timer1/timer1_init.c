
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer1_Init( ULONG usec )
{
#ifdef power_timer1_enable
	power_timer1_enable();										// disable power save mode
#endif

	TCCR1A = 0;					// clear control register A
//	TCCR1B = _BV(WGM13);		// set mode as phase and frequency correct pwm, stop the timer
	TCCR1B = _BV(WGM13) | _BV(WGM12);	// set mode as ctc mode, stop the timer
	TIMSK1 = 0;					// disable all interrupts

	gTimer1_PwmPinAEnable = 0;
	gTimer1_PwmPinBEnable = 0;

	Timer1_Set_Period( usec );
}

