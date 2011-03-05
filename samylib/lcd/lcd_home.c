/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"
#include <util/delay.h>
#include "my_typedef.h"
#include "common.h"


/*--------------------------------------------------
 *  Home position
 *--------------------------------------------------*/
VOID Lcd_Home( VOID )
{
	// DB=0x02, RS=0, RW=0

	cbi( REG_PORT(D_LCD_RS_PORT_NAME), BIT_PORT(D_LCD_RS_PORT_NAME, D_LCD_RS_BIT) );
	_Lcd_Write_Data( 0 );	// Hi
	_Lcd_ToggleE();
	_Lcd_Write_Data( 2 );	// Lo
	_Lcd_ToggleE();
	_delay_us( 1600 );
}
