
// プロジェクト固有
#include "ir_frame.h"
#include "custom.h"

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
	gIr_Frame.bit_mask = 1;
	memset( (void*)gIr_Frame.data, '\0', sizeof(gIr_Frame.data) );
}

VOID Ir_Frame_Set_Type( E_IR_FRAME_TYPE type )
{
	gIr_Frame.type = type;
}

BOOL Ir_Frame_Store_Bit( UCHAR bit )
{
	if( bit ) {
		gIr_Frame.data[gIr_Frame.byte_idx] |= gIr_Frame.bit_mask;
	}

	if( gIr_Frame.bit_mask == 0x80 ) {
		gIr_Frame.byte_idx++;
		gIr_Frame.bit_mask = 1;
	}
	else {
		gIr_Frame.bit_mask <<= 1;
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

	byte_idx = gIr_Frame.byte_idx;
	if( gIr_Frame.bit_mask != 0x01 ) {
		byte_idx++;
	}

	Usart_Set_Stdout();
	printf_P( PSTR("type=%u byte=%u bit=%x:"), gIr_Frame.type, gIr_Frame.byte_idx, gIr_Frame.bit_mask );
	for( loopcnt = 0; loopcnt < byte_idx; loopcnt++ ) {
		printf_P( PSTR(" %02X"), gIr_Frame.data[loopcnt] );
	}
	putchar( '\n' );
}

