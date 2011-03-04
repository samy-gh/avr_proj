/*
 * HD44780 LCD driver
 */

#include "lcd_hd44780.h"
#include "my_typedef.h"


/*---------------------------------------------------
 *   Display 0-99
 * --------------------------------------------------*/
VOID Lcd_PrintDigitN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	for( ; print_lvl != 0; print_lvl /= 10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

//		if( digit_1 < 10 ) {
			Lcd_Write( '0' + digit_1 );
//		}
//		else {
//			Lcd_Write( '?' );
//		}
	}
}

