/*
 * HD44780 LCD driver
 */

// lcd
#include "lcd_hd44780.h"
#include "lcd_hd44780_custom.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <util/delay.h>


/*--------------------------------------------------
 *  Clock(E)
 *--------------------------------------------------*/

// 時間稼ぎしなくても動作するようだ。
// 動かない時にタイミング調整する。
#if 0
static VOID dummy( VOID )
{
	volatile UCHAR i;
	// 時間稼ぎ
	i = 0;
}
#endif

VOID _Lcd_ToggleE( VOID )
{
	// tAS MIN=140ns
//	_delay_us( 1 );
	sbi( REG_PORT(D_LCD_EN_PORT_NAME), BIT_PORT(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );	// E=1
	// PW_EH MIN=450ns
//	_delay_us( 1 );
//	dummy();
	cbi( REG_PORT(D_LCD_EN_PORT_NAME), BIT_PORT(D_LCD_EN_PORT_NAME, D_LCD_EN_BIT) );	// E=0
	// tAH MIN=10ns
//	_delay_us( 1 );
}

