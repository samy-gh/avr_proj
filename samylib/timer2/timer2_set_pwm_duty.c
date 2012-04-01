
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


// pin:  0:OC2A 1:OC2B
// duty: 0:0%  10000:100%
VOID Timer2_Set_PwmDuty( UCHAR pin, UINT duty )
{
	ULONG duty_period = (ULONG)duty * gTimer2_PwmPeriod / 10000;

	sbi( TCCR2A, WGM20 );

	if( pin == 0 ) {
		OCR2A = duty_period;
	}
	else {
		OCR2B = duty_period;
	}
}

