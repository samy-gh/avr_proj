/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"
#include "my_typedef.h"
#include "common.h"


VOID Lcd_Close( VOID )
{
	cbi( REG_DDR(D_LCD_D7_PORT_NAME), BIT_DDR(D_LCD_D7_PORT_NAME, D_LCD_D7_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D6_PORT_NAME), BIT_DDR(D_LCD_D6_PORT_NAME, D_LCD_D6_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D5_PORT_NAME), BIT_DDR(D_LCD_D5_PORT_NAME, D_LCD_D5_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D4_PORT_NAME), BIT_DDR(D_LCD_D4_PORT_NAME, D_LCD_D4_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_RS_PORT_NAME), BIT_DDR(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );	// in port設定
	// LCDに電源供給をし続けるならEn=0にした方がが良い
	cbi( REG_DDR(D_LCD_EN_PORT_NAME), BIT_DDR(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );	// in port設定
}


