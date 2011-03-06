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
 *  Set font
 *--------------------------------------------------*/
VOID Lcd_SetFont( UINT ui_font_no, UCHAR pc_font[] )
{
	UINT ui_loopcnt;
	// DB=0000_0001, RS=0, RW=0

	ui_font_no &= 0x7;
	ui_font_no <<= 3;

	_Lcd_Write_Data( (ui_font_no >> 4) | 0x4 );	// 1st
	_Lcd_ToggleE();
	_Lcd_Write_Data( ui_font_no & 0x0F );		// 2nd
	_Lcd_ToggleE();
	_delay_us( 40 );				//wait 40us

	for( ui_loopcnt = 0; ui_loopcnt < 8; ui_loopcnt++ ) {
		Lcd_Write( pc_font[ui_loopcnt] );
	}

	Lcd_Goto( 0, 0 );		// DDRAMデータ設定モードに変更
}

