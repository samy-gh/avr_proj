
// timer0
#include "timer0.h"

// samylib
#include "my_typedef.h"
#include "common.h"
#include "lcd_hd44780.h"

// WinAVR
#include <avr/io.h>


VOID Timer0_Unset_PwmPin( VOID )
{
	if( !LCD_IS_BLOCK() ) {
		return;
	}

	cbi( DDRD, DDD5 );
	cbi( TCCR0A, COM0B1 );

	if( !Lcd_Set_Block( FALSE ) ) {
		return;
	}
}

