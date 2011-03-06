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
 *  Put character on LCD
 *--------------------------------------------------*/
VOID Lcd_Write( const UCHAR c )
{
	// DB=xxxx_xxxx, RS=1, RW=0
	//
	sbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );	// in port設定
	_Lcd_Write_Data( c >> 4 );				//上位データ
	_Lcd_ToggleE();
	_Lcd_Write_Data( c & 0x0F );			// 下位データ
	_Lcd_ToggleE();
	_delay_us( 40 );	//wait 40us
}

