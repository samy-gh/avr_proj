
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


// duty: 0:0%  10000:100%
VOID Timer0_Set_PwmDuty( UINT duty )
{
	ULONG duty_period = (ULONG)duty * gTimer0_PwmPeriod / 10000;

	OCR0B = duty_period;
}

