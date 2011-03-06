/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"


VOID Lcd_PrintHexN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	if( print_lvl == 0 ) {
		return;
	}

	for( ; print_lvl != 0; print_lvl /= 0x10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

		if( digit_1 >= 0xa ) {
			Lcd_Write( 'A' + digit_1 - 0xa );
		}
		else {
			Lcd_Write( '0' + digit_1 );
		}
	}
}

