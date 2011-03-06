/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"


VOID Lcd_PgmPuts( PGM_P p_str )
{
	UCHAR c;

	while( 1 ) {
		c = pgm_read_byte( p_str++ );
		if( c == '\0' ) {
			return;
		}
		Lcd_Write( c );
	}
}

