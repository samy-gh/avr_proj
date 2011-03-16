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
 *  Goto position   (0x00～ - line1)
 *                  (0x40～ - line2)
 *--------------------------------------------------*/
VOID Lcd_Goto( const UCHAR x, const UCHAR y )
{
	// DB=1xxx_xxxx, RS=0, RW=0
	UCHAR addr;

	switch( y ) {
		case 0:
			addr = D_LCD_ROW_1;
			break;
		case 1:
			addr = D_LCD_ROW_2;
			break;
		case 2:
			addr = D_LCD_ROW_3;
			break;
		case 3:
			addr = D_LCD_ROW_4;
			break;
		default:
			addr = 0;
			break;
	}
	addr += x;

	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );

	_Lcd_Write_Data( (addr>>4) | 0x8 );		//上位アドレス
	_Lcd_ToggleE();

	_Lcd_Write_Data( (addr & 0x0F) );		//下位アドレス
	_Lcd_ToggleE();

	_delay_us( LCD_WAIT_FACTOR(40) );		//wait 40us
}

