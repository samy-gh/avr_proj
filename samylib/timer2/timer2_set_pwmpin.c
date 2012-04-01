
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


volatile UCHAR gTimer2_PwmPinAEnable = 0;
volatile UCHAR gTimer2_PwmPinBEnable = 0;

// pin:  0:OC2A 1:OC2B
// duty: 0:0%  10000:100%
VOID Timer2_Set_PwmPin( UCHAR pin, UINT duty )
{
	Timer2_Set_PwmDuty( pin, duty );

	if( pin == 0 ) {
		cbi( PORTB, PB3 );
		sbi( DDRB, DDB1 );
		gTimer2_PwmPinAEnable = 1;
	}
	else {
		cbi( PORTD, PD3 );
		sbi( DDRD, DDB3 );
		gTimer2_PwmPinBEnable = 1;
	}
}

