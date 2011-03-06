/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <util/delay.h>


/*--------------------------------------------------
 *  Cursor shift on LCD
 *--------------------------------------------------*/
VOID Lcd_Shift( const UCHAR data )
{
	// DB=0001_0x00, RS=0, RW=0

	UCHAR	reg_val;

	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0 );	// Hi
	_Lcd_ToggleE();
	if( data ) {
		reg_val = 0x04;		// 右シフト
	}
	else {
		reg_val = 0;		// 左シフト
	}
	_Lcd_Write_Data( reg_val );	// Lo
	_Lcd_ToggleE();
	_delay_us( 40 );	//wait 40us
}

