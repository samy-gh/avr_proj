/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"


VOID Lcd_PrintHex( UINT val )
{
	UINT print_lvl;

	// �ŏ�ʌ��܂ŃX�L�b�v
	for( print_lvl = 0x1000; print_lvl != 0; print_lvl /= 0x10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	Lcd_PrintHexN( val, print_lvl );
}

