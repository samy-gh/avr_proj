
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


// pin:  0:OC1A 1:OC1B
VOID Timer1_Unset_PwmPin( UCHAR pin )
{
	if( !LCD_IS_BLOCK() ) {
		return;
	}

	if( pin == 0 ) {
		cbi( DDRB, DDB1 );
		cbi( TCCR1A, COM1A1 );
	}
	else {
		cbi( DDRB, DDB2 );
		cbi( TCCR1A, COM1B1 );
	}

	if( !Lcd_Set_Block( FALSE ) ) {
		return;
	}
}

