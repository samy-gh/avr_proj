
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Init( ULONG usec )
{
	cbi( PRR, PRTIM1 );										// disable power save mode

	TCCR1A = 0;					// clear control register A
//	TCCR1B = _BV(WGM13);		// set mode as phase and frequency correct pwm, stop the timer
	TCCR1B = _BV(WGM13) | _BV(WGM12);	// set mode as ctc mode, stop the timer
	TIMSK1 = 0;					// disable all interrupts

	Timer1_Set_Period( usec );
}

