/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"


VOID Lcd_PrintDigit( const UINT val )
{
	UINT print_lvl;

	// 最上位桁までスキップ
	for( print_lvl = 10000; print_lvl != 0; print_lvl /= 10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	Lcd_PrintDigitN( val, print_lvl );
}

