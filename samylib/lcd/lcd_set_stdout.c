/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"

// samylib
#include "my_typedef.h"

// STD-C
#include <stdio.h>


static int lcd_putchar( char c, struct __file* dummy )
{
	Lcd_Write( c );
	return 0;
}

static FILE g_lcd_stdout = FDEV_SETUP_STREAM( lcd_putchar, NULL, _FDEV_SETUP_WRITE );

VOID Lcd_Set_Stdout( VOID )
{
	stdout = &g_lcd_stdout;
}

