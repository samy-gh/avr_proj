/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"
#include <util/delay.h>
#include "my_typedef.h"
#include "common.h"


VOID Lcd_Open( VOID )
{
	cbi( REG_PORT(D_LCD_EN_PORT_NAME), BIT_PORT(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );	// EN=0

	// out port 設定
	sbi( REG_DDR(D_LCD_D7_PORT_NAME), BIT_DDR(D_LCD_D7_PORT_NAME, D_LCD_D7_BIT) );
	sbi( REG_DDR(D_LCD_D6_PORT_NAME), BIT_DDR(D_LCD_D6_PORT_NAME, D_LCD_D6_BIT) );
	sbi( REG_DDR(D_LCD_D5_PORT_NAME), BIT_DDR(D_LCD_D5_PORT_NAME, D_LCD_D5_BIT) );
	sbi( REG_DDR(D_LCD_D4_PORT_NAME), BIT_DDR(D_LCD_D4_PORT_NAME, D_LCD_D4_BIT) );
	sbi( REG_DDR(D_LCD_RS_PORT_NAME), BIT_DDR(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	sbi( REG_DDR(D_LCD_EN_PORT_NAME), BIT_DDR(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );


	_delay_ms( 15 );					//15ms以上wait
	// デフォルト設定 8bitモード
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x3 );
	_Lcd_ToggleE();
	_delay_ms( 5 );						//5ms以上wait
	// デフォルト設定 8bitモード
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x3 );
	_Lcd_ToggleE();
	_delay_us( 100 );					//100us以上wait
	// デフォルト設定 8bitモード
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x3 );
	_Lcd_ToggleE();
	_delay_us( 40 );					//wait 40us
	// デフォルト設定 8bit->4bitモード
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x2 );
	_Lcd_ToggleE();
	_delay_us( 40 );					//wait 40us
	// ファンクションセット
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x2 );		// Hi
	_Lcd_ToggleE();
	_Lcd_Write_Data( 0x8 );		// Lo
	_Lcd_ToggleE();
	_delay_us( 40 );					//wait 40us
	// 表示on/off初期化
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x0 );		// Hi
	_Lcd_ToggleE();
	_Lcd_Write_Data( 0x8 );		// Lo
	_Lcd_ToggleE();
	_delay_us( 40 );					//wait 40us
	// エントリーモードセット
	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0x0 );		// Hi
	_Lcd_ToggleE();
	_Lcd_Write_Data( 0x6 );		// Hi
	_Lcd_ToggleE();
	_delay_us( 40 );					//wait 40us
	Lcd_Cls();							// 表示クリア
}


