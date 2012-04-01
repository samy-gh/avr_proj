
// timer2
#include "timer2.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


// pin:  0:OC2A 1:OC2B
VOID Timer2_Unset_PwmPin( UCHAR pin )
{
	if( pin == 0 ) {
		cbi( DDRB, DDB3 );		// Input mode
		cbi( TCCR2A, COM2A1 );	// 
		gTimer2_PwmPinAEnable = 0;
	}
	else {
		cbi( DDRD, DDB3 );		// Input mode
		cbi( TCCR2A, COM2B1 );
		gTimer2_PwmPinBEnable = 0;
	}
}

