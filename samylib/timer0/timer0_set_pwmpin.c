
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


// duty: 0:0%  10000:100%
VOID Timer0_Set_PwmPin( UINT duty )
{
	if( !Lcd_Set_Block( TRUE ) ) {
		return;
	}

	Timer0_Set_PwmDuty( duty );

	sbi( DDRD, DDD5 );
	sbi( TCCR0A, COM0B1 );
}

