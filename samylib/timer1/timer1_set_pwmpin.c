
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


volatile UCHAR gTimer1_PwmPinAEnable = 0;
volatile UCHAR gTimer1_PwmPinBEnable = 0;

// pin:  0:OC1A 1:OC1B
// duty: 0:0%  10000:100%
VOID Timer1_Set_PwmPin( UCHAR pin, UINT duty )
{
	if( !Lcd_Set_Block( TRUE ) ) {
		return;
	}

	Timer1_Set_PwmDuty( pin, duty );

	if( pin == 0 ) {
		cbi( PORTB, PB1 );
		sbi( DDRB, DDB1 );
		gTimer1_PwmPinAEnable = 1;
	}
	else {
		cbi( PORTB, PB2 );
		sbi( DDRB, DDB2 );
		gTimer1_PwmPinBEnable = 1;
	}
}

