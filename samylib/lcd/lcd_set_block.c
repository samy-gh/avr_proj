/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"
#include "int_lock.h"


static UCHAR glcd_block_count = 0;

BOOL Lcd_Set_Block( BOOL block_stat )
{
	UCHAR const lockKey = Int_Lock();

	if( block_stat ) {
		if( gLcd_Is_Open == E_LCD_IS_OPEN_USING ) {
			Int_Unlock( lockKey );
			return FALSE;
		}

		if( glcd_block_count != 255 ) {
			glcd_block_count++;
		}
		gLcd_Is_Open = E_LCD_IS_OPEN_BLOCK;
	}
	else {
		if( gLcd_Is_Open != E_LCD_IS_OPEN_BLOCK ) {
			Int_Unlock( lockKey );
			return FALSE;
		}

		if( glcd_block_count != 0 ) {
			glcd_block_count--;
			if( glcd_block_count == 0 ) {
				gLcd_Is_Open = E_LCD_IS_OPEN_FREE;
			}
		}
	}
	Int_Unlock( lockKey );

	return TRUE;
}

