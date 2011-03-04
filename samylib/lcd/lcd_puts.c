/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "my_typedef.h"


VOID Lcd_Puts( const CHAR* str )
{
	while( *str ) {
		Lcd_Write( *str++ );
	}
}

