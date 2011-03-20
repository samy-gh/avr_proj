
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles )
{
	gTimer1_ClockSelectBits = clk_sel;

	cbi( PRR, PRTIM1 );										// disable power save mode
	OCR1A = OCR1B = ICR1 = gTimer1_PwmPeriod = ul_cycles;			// ICR1 is TOP in p & f correct pwm mode
	TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}

