/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <util/delay.h>


//
// LCD Drive TEST
//
// SII model M1632-0A (èHåéìdéq)
// ------------------
// |0123456789ABCDEF| 16ï∂éöÅ~2çs
// |0123456789ABCDEF| HD44780ì‡ëü
// ------------------
//
VOID Lcd_Test( VOID )
{
	UCHAR	x;
	UCHAR	y;
	UCHAR	ch;

	Lcd_Open();
	Lcd_Control( 1, 1, 0 );
	Lcd_Goto( 0, 0 );
	//lcd_putstr((byte*)"0123456789ABCDEF");
	ch = 0x21;

	while(1){
		for( y = 0; y < 2; y++ ) {
			for( x = 0; x < 16; x++ ) {
				Lcd_Goto( x, y );
				Lcd_Write( ch );
				if( ++ch > 0x7F ) {
					ch = 0x21;
				}

				_delay_ms( 1000 );

			}
		}
		Lcd_Cls();
	}
}
