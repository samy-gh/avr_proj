
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles )
{
	gTimer0_ClockSelectBits = clk_sel;

	OCR0A = OCR0B = gTimer0_PwmPeriod = ul_cycles;			// OCR0A is TOP in p & f correct pwm mode
	TCCR0B &= ~(_BV(CS00) | _BV(CS01) | _BV(CS02));
}

