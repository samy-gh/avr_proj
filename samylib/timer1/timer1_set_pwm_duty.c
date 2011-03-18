
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


// pin:  0:OC1A 1:OC1B
// duty: 0:0%  10000:100%
VOID Timer1_Set_PwmDuty( UCHAR pin, UINT duty )
{
	ULONG duty_period = (ULONG)duty * gTimer1_PwmPeriod / 10000;

	sbi( TCCR1A, WGM11 );

	if( pin == 0 ) {
		OCR1A = duty_period;
	}
	else {
		OCR1B = duty_period;
	}
}

