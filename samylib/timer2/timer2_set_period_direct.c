
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer2_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles )
{
	gTimer2_ClockSelectBits = clk_sel;

	cbi( PRR, PRTIM2 );										// disable power save mode
	OCR2A = OCR2B = gTimer2_PwmPeriod = ul_cycles;
	TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));
}

