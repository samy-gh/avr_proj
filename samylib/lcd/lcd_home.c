/*
 * HD44780 LCD driver
 */

// samylib
#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <util/delay.h>


/*--------------------------------------------------
 *  Home position
 *--------------------------------------------------*/
VOID Lcd_Home( VOID )
{
	if( !LCD_IS_OPEN() ) {
		return;
	}

	// DB=0x02, RS=0, RW=0

	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0 );	// Hi
	_Lcd_ToggleE();
	_Lcd_Write_Data( 2 );	// Lo
	_Lcd_ToggleE();
	_delay_us( LCD_WAIT_FACTOR(1600) );	// 1.6ms wait
}

