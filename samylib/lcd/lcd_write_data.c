/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"
#include "my_typedef.h"
#include "common.h"

#include "usart.h"

VOID _Lcd_Write_Data( UCHAR data_4bit )
{
	if( data_4bit & 1 ) {
		sbi( REG_PORT(D_LCD_D4_PORT_NAME), BIT_PORT(D_LCD_D4_PORT_NAME, D_LCD_D4_BIT) );
	}
	else {
		cbi( REG_PORT(D_LCD_D4_PORT_NAME), BIT_PORT(D_LCD_D4_PORT_NAME, D_LCD_D4_BIT) );
	}
	data_4bit >>= 1;

	if( data_4bit & 1 ) {
		sbi( REG_PORT(D_LCD_D5_PORT_NAME), BIT_PORT(D_LCD_D5_PORT_NAME, D_LCD_D5_BIT) );
	}
	else {
		cbi( REG_PORT(D_LCD_D5_PORT_NAME), BIT_PORT(D_LCD_D5_PORT_NAME, D_LCD_D5_BIT) );
	}
	data_4bit >>= 1;

	if( data_4bit & 1 ) {
		sbi( REG_PORT(D_LCD_D6_PORT_NAME), BIT_PORT(D_LCD_D6_PORT_NAME, D_LCD_D6_BIT) );
	}
	else {
		cbi( REG_PORT(D_LCD_D6_PORT_NAME), BIT_PORT(D_LCD_D6_PORT_NAME, D_LCD_D6_BIT) );
	}
	data_4bit >>= 1;

	if( data_4bit & 1 ) {
		sbi( REG_PORT(D_LCD_D7_PORT_NAME), BIT_PORT(D_LCD_D7_PORT_NAME, D_LCD_D7_BIT) );
	}
	else {
		cbi( REG_PORT(D_LCD_D7_PORT_NAME), BIT_PORT(D_LCD_D7_PORT_NAME, D_LCD_D7_BIT) );
	}
}


