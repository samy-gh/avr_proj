/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"
#include "int_lock.h"


BOOL Lcd_Set_Block( BOOL block_stat )
{
	UCHAR const lockKey = Int_Lock();

	if( block_stat ) {
		if( gLcd_Is_Open != E_LCD_IS_OPEN_FREE ) {
			Int_Unlock( lockKey );
			return FALSE;
		}

		gLcd_Is_Open = E_LCD_IS_OPEN_BLOCK;
	}
	else {
		if( gLcd_Is_Open != E_LCD_IS_OPEN_BLOCK ) {
			Int_Unlock( lockKey );
			return FALSE;
		}

		gLcd_Is_Open = E_LCD_IS_OPEN_FREE;
	}
	Int_Unlock( lockKey );

	return TRUE;
}

