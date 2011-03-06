/*
 * HD44780 LCD driver header
 */

#ifndef __LCD_HD44780_H__
#define __LCD_HD44780_H__

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/pgmspace.h>


/****************************************************************************/
/* 定数宣言																	*/
/****************************************************************************/
#define LCD_BITMAP_00000	0x00
#define LCD_BITMAP_00001	0x01
#define LCD_BITMAP_00010	0x02
#define LCD_BITMAP_00011	0x03
#define LCD_BITMAP_00100	0x04
#define LCD_BITMAP_00101	0x05
#define LCD_BITMAP_00110	0x06
#define LCD_BITMAP_00111	0x07
#define LCD_BITMAP_01000	0x08
#define LCD_BITMAP_01001	0x09
#define LCD_BITMAP_01010	0x0A
#define LCD_BITMAP_01011	0x0B
#define LCD_BITMAP_01100	0x0C
#define LCD_BITMAP_01101	0x0D
#define LCD_BITMAP_01110	0x0E
#define LCD_BITMAP_01111	0x0F

#define LCD_BITMAP_10000	0x10
#define LCD_BITMAP_10001	0x11
#define LCD_BITMAP_10010	0x12
#define LCD_BITMAP_10011	0x13
#define LCD_BITMAP_10100	0x14
#define LCD_BITMAP_10101	0x15
#define LCD_BITMAP_10110	0x16
#define LCD_BITMAP_10111	0x17
#define LCD_BITMAP_11000	0x18
#define LCD_BITMAP_11001	0x19
#define LCD_BITMAP_11010	0x1A
#define LCD_BITMAP_11011	0x1B
#define LCD_BITMAP_11100	0x1C
#define LCD_BITMAP_11101	0x1D
#define LCD_BITMAP_11110	0x1E
#define LCD_BITMAP_11111	0x1F


/****************************************************************************/
/* データ型宣言																*/
/****************************************************************************/
typedef unsigned char byte;


/****************************************************************************/
/* マクロ宣言																*/
/****************************************************************************/
#define LCD_BITMAP(a) (LCD_BITMAP_ ## a)
#define LCD_PGM_PUTS( str )	Lcd_PgmPuts( PSTR str );


/****************************************************************************/
/* プロトタイプ宣言															*/
/****************************************************************************/
extern VOID Lcd_Init( VOID );
extern VOID Lcd_Cls( VOID );
extern VOID Lcd_Home( VOID );
extern VOID Lcd_Control( const UCHAR disonoff, const UCHAR curonoff, const UCHAR curblink );
extern VOID Lcd_Goto( const UCHAR x, const UCHAR y );
extern VOID Lcd_Shift( const UCHAR data );
extern VOID Lcd_Write( const UCHAR c );
extern VOID Lcd_Open( VOID );
extern VOID Lcd_Close( VOID );
extern VOID Lcd_SetFont( UINT ui_font_no, UCHAR pc_font[] );

extern VOID Lcd_Set_Stdout( VOID );

extern VOID Lcd_Test( VOID );
extern VOID Lcd_Puts( const CHAR* str );
extern VOID Lcd_PgmPuts( PGM_P p_str );
extern VOID Lcd_PrintDigit( UINT val );
extern VOID Lcd_PrintDigitN( UINT val, UINT print_lvl );
extern VOID Lcd_PrintHexN( UINT val, UINT print_lvl );


// ここから内部用
extern VOID _Lcd_ToggleE( VOID );
extern VOID _Lcd_Write_Data( UCHAR data_4bit );

#endif	// __LCD_HD44780_H__

