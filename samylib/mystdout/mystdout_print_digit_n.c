/*
 * HD44780 LCD driver
 */

#include "mystdout.h"
#include "my_typedef.h"
#include <stdio.h>


/*---------------------------------------------------
 *   Display 0-99
 * --------------------------------------------------*/
VOID Mystdout_PrintDigitN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	for( ; print_lvl != 0; print_lvl /= 10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

//		if( digit_1 < 10 ) {
			stdout->put( '0' + digit_1, NULL );
//		}
//		else {
//			Lcd_Write( '?' );
//		}
	}
}

