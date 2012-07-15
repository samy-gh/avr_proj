
// プロジェクト固有
#include "ir_frame.h"
#include "ir_custom.h"

// samylib
#include <my_typedef.h>
#include <usart.h>

// STD-C
#include <stdio.h>
#include <string.h>


T_IR_FRAME volatile gIr_Frame;

VOID Ir_Frame_Init( VOID )
{
	gIr_Frame.byte_idx = 0;
	gIr_Frame.bit_mask = 0x80;
	memset( (void*)gIr_Frame.data, '\0', sizeof(gIr_Frame.data) );
}

VOID Ir_Frame_Set( const E_IR_FRAME_TYPE type, const UCHAR* const data, UCHAR data_bytes, const UCHAR last_bit_mask )
{
	gIr_Frame.type = type;
	memcpy( (void*)gIr_Frame.data, data, data_bytes );
	// 最終ビットの次の位置を格納
	if( last_bit_mask == 0x01 ) {
		gIr_Frame.byte_idx = data_bytes;
		gIr_Frame.bit_mask = 0x80;
	}
	else {
		gIr_Frame.byte_idx = data_bytes -1;
		gIr_Frame.bit_mask = last_bit_mask >> 1;
	}
}

VOID Ir_Frame_PgmSet( const E_IR_FRAME_TYPE type, PGM_P p_str, UCHAR data_bytes, const UCHAR last_bit_mask )
{
	UCHAR loopcnt;

	gIr_Frame.type = type;

	for( loopcnt = 0; loopcnt < data_bytes; loopcnt++ ) {
		gIr_Frame.data[loopcnt] = pgm_read_byte( p_str++ );
	}
	gIr_Frame.byte_idx = data_bytes -1;
	gIr_Frame.bit_mask = last_bit_mask;
}

VOID Ir_Frame_Set_Type( const E_IR_FRAME_TYPE type )
{
	gIr_Frame.type = type;
}

BOOL Ir_Frame_Store_Bit( UCHAR bit )
{
	if( bit ) {
		gIr_Frame.data[gIr_Frame.byte_idx] |= gIr_Frame.bit_mask;
	}

	if( gIr_Frame.bit_mask == 0x01 ) {
		gIr_Frame.byte_idx++;
		gIr_Frame.bit_mask = 0x80;
	}
	else {
		gIr_Frame.bit_mask >>= 1;
	}

	if( gIr_Frame.byte_idx >= sizeof(gIr_Frame.data) ) {
		return FALSE;
	}

	return TRUE;
}

VOID Ir_Frame_Dump( VOID )
{
	UCHAR loopcnt;
	UCHAR byte_idx;
	UCHAR bit_mask;

	if( (gIr_Frame.byte_idx == 0) && (gIr_Frame.bit_mask == 0x80) ) {
		Usart_Set_Stdout();
		printf_P( PSTR("empty.\n") );
		return;
	}

	byte_idx = gIr_Frame.byte_idx;
	if( gIr_Frame.bit_mask == 0x80 ) {
		bit_mask = 0x01;
	}
	else {
		bit_mask = gIr_Frame.bit_mask << 1;
		byte_idx++;
	}

	Usart_Set_Stdout();
	printf_P( PSTR("type=%u byte=%u bit=%x:"), gIr_Frame.type, byte_idx, bit_mask );
	for( loopcnt = 0; loopcnt < byte_idx; loopcnt++ ) {
		printf_P( PSTR(" %02X"), gIr_Frame.data[loopcnt] );
	}
	putchar( '\n' );
}

