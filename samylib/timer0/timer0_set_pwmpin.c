
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


volatile UCHAR gTimer0_PwmPinEnable = 0;

// duty: 0:0%  10000:100%
VOID Timer0_Set_PwmPin( UINT duty )
{
	if( !Lcd_Set_Block( TRUE ) ) {
		return;
	}

	Timer0_Set_PwmDuty( duty );

	cbi( PORTD, PD5 );
	sbi( DDRD, DDD5 );
	gTimer0_PwmPinEnable = 1;
}

