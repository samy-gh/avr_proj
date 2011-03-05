/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "my_typedef.h"


VOID Lcd_Init( VOID )
{
	Lcd_Open();
	Lcd_Control( 1, 0, 0 );
	Lcd_Cls();
}

