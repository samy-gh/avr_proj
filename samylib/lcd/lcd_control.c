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
 *  LCD & Cursor control (Blinking, ON/OFF,...)
 *--------------------------------------------------*/
VOID Lcd_Control( const UCHAR disonoff, const UCHAR curonoff, const UCHAR curblink )
{
	UCHAR	data;
	// DB=0000_1xxx, RS=0, RW=0

	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	// 1st
	_Lcd_Write_Data( 0 );	// Hi
	_Lcd_ToggleE();
	// 2nd
	data = 0x8;
	if( disonoff == 1 ) {
		data |= 0x04;	//	1:表示on     / 0:表示off
	}
	if( curonoff == 1 ) {
		data |= 0x02;	//	1:カーソルon / 0:カーソルoff
	}
	if( curblink == 1 ) {
		data |= 0x01;	//	1:ブリンクon / 0:ブリンクoff
	}
	_Lcd_Write_Data( data );
	_Lcd_ToggleE();
	_delay_us( LCD_WAIT_FACTOR(40) );	//wait 40us
}

