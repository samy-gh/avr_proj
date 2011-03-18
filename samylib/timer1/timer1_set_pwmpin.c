
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


// pin:  0:OC1A 1:OC1B
// duty: 0:0%  10000:100%
VOID Timer1_Set_PwmPin( UCHAR pin, UINT duty )
{
	if( !Lcd_Set_Block( TRUE ) ) {
		return;
	}

	Timer1_Set_PwmDuty( pin, duty );

	if( pin == 0 ) {
		sbi( DDRB, DDB1 );
		cbi( TCCR1A, COM1A0 );
		sbi( TCCR1A, COM1A1 );
	}
	else {
		sbi( DDRB, DDB2 );
		cbi( TCCR1A, COM1B0 );
		sbi( TCCR1A, COM1B1 );
	}
}

