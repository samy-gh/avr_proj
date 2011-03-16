/*
 * HD44780 LCD driver header
 *		nak@pluto.dti.ne.jp
 *
 *
 * SVNID: $Revision$ $Date::                            $
 */

#ifndef __LCD_HD44780_CUSTOM_H__
#define __LCD_HD44780_CUSTOM_H__

// SC1602BS or SC2004CS 互換アドレス
#ifndef D_LCD_ROW_1
#define		D_LCD_ROW_1		0x00	//１行目先頭アドレス
#endif
#ifndef D_LCD_ROW_2
#define		D_LCD_ROW_2		0x40	//２行目先頭アドレス
#endif
#ifndef D_LCD_ROW_3
#define		D_LCD_ROW_3		0x14	//３行目先頭アドレス
#endif
#ifndef D_LCD_ROW_4
#define		D_LCD_ROW_4		0x54	//４行目先頭アドレス
#endif

// 制御ポート設定
#ifndef D_LCD_RS_PORT_NAME
#define		D_LCD_RS_PORT_NAME	B
#endif
#ifndef D_LCD_RS_BIT
#define		D_LCD_RS_BIT		1
#endif
#ifndef D_LCD_EN_PORT_NAME
#define		D_LCD_EN_PORT_NAME	B
#endif
#ifndef D_LCD_EN_BIT
#define		D_LCD_EN_BIT		2
#endif
#ifndef D_LCD_D4_PORT_NAME
#define		D_LCD_D4_PORT_NAME	D
#endif
#ifndef D_LCD_D4_BIT
#define		D_LCD_D4_BIT		5
#endif
#ifndef D_LCD_D5_PORT_NAME
#define		D_LCD_D5_PORT_NAME	D
#endif
#ifndef D_LCD_D5_BIT
#define		D_LCD_D5_BIT		6
#endif
#ifndef D_LCD_D6_PORT_NAME
#define		D_LCD_D6_PORT_NAME	D
#endif
#ifndef D_LCD_D6_BIT
#define		D_LCD_D6_BIT		7
#endif
#ifndef D_LCD_D7_PORT_NAME
#define		D_LCD_D7_PORT_NAME	B
#endif
#ifndef D_LCD_D7_BIT
#define		D_LCD_D7_BIT		0
#endif


// 待機時間の調整
// 時間 x D_LCD_WAIT_FACTORになる。D_LCD_WAIT_FACTORは整数。
#ifdef		D_LCD_WAIT_FACTOR
#define		LCD_WAIT_FACTOR( time )	((time) * (D_LCD_WAIT_FACTOR))
#else
#define		LCD_WAIT_FACTOR( time )	(time)
#endif


#define		REG_CONCAT( a, b )		a ## b
#define		BIT_CONCAT( a, b, c )	a ## b ## c

#define		REG_DDR( name )			REG_CONCAT( DDR, name )
#define		BIT_DDR( name, bit )	BIT_CONCAT( DD, name, bit )
#define		REG_PORT( name )		REG_CONCAT( PORT, name )
#define		BIT_PORT( name, bit )	BIT_CONCAT( P, name, bit )

#endif	/* __LCD_HD44780_CUSTOM_H__ */

