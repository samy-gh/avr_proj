/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"

// samylib
#include "my_typedef.h"
#include "common.h"


VOID Lcd_Close( BOOL en_set_low )
{
	if( !LCD_IS_OPEN() ) {
		return;
	}

	cbi( REG_DDR(D_LCD_D7_PORT_NAME), BIT_DDR(D_LCD_D7_PORT_NAME, D_LCD_D7_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D6_PORT_NAME), BIT_DDR(D_LCD_D6_PORT_NAME, D_LCD_D6_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D5_PORT_NAME), BIT_DDR(D_LCD_D5_PORT_NAME, D_LCD_D5_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_D4_PORT_NAME), BIT_DDR(D_LCD_D4_PORT_NAME, D_LCD_D4_BIT) );	// in port設定
	cbi( REG_DDR(D_LCD_RS_PORT_NAME), BIT_DDR(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );	// in port設定
	// LCDに電源供給をし続けるならEn=0にした方がが良い
	if( en_set_low ) {
		cbi( REG_PORT(D_LCD_EN_PORT_NAME), BIT_PORT(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );
	}
	else {
		cbi( REG_DDR(D_LCD_EN_PORT_NAME), BIT_DDR(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );	// in port設定
	}

	gLcd_Is_Open = E_LCD_IS_OPEN_FREE;
}


