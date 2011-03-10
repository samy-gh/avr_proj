
// プロジェクト固有
#include "ir_frame.h"
#include "ir_frame_aeha.h"
#include "custom.h"

// STD-C
#include <stdio.h>
#include <string.h>


UCHAR ir_frame_aeha_calc_parity( T_IR_AEHA_FRAME* ir_frame_aeha )
{
	return  (ir_frame_aeha->customer & 0x000f)
		 ^ ((ir_frame_aeha->customer & 0x00f0) >> 4)
		 ^ ((ir_frame_aeha->customer & 0x0f00) >> 8)
		 ^ ((ir_frame_aeha->customer & 0xf000) >> 12);
}


UCHAR Ir_Frame_Aeha_Import_Frame( T_IR_AEHA_FRAME* ir_frame_aeha, T_IR_FRAME* ir_frame )
{
	UCHAR parity;

	if( ir_frame->type != E_IR_FRAME_TYPE_AEHA ) {
		return 0;
	}

	if( ir_frame->bit_mask != 0x01 ) {
		return 0;
	}

	ir_frame_aeha->customer = ir_frame->data[1] * 0x100 + ir_frame->data[0];

	ir_frame_aeha->data_bytes = ir_frame->byte_idx -2;
	memcpy( ir_frame_aeha->data, &ir_frame->data[2], ir_frame_aeha->data_bytes );
	ir_frame_aeha->data[0] &= 0x0f;

	parity = ir_frame_aeha_calc_parity( ir_frame_aeha );
	if( parity != ((ir_frame->data[2] & 0xf0) >> 4) ) {
		return 0;
	}

	return 1;
}


UCHAR Ir_Frame_Aeha_Export_Frame( T_IR_FRAME* ir_frame, T_IR_AEHA_FRAME* ir_frame_aeha )
{
	UCHAR parity;

	ir_frame->type = E_IR_FRAME_TYPE_AEHA;

	ir_frame->data[0] = (ir_frame_aeha->customer & 0xff);
	ir_frame->data[1] = (ir_frame_aeha->customer & 0xff) >> 8;
	parity = ir_frame_aeha_calc_parity( ir_frame_aeha );

	memcpy( &ir_frame->data[2], ir_frame_aeha->data, ir_frame_aeha->data_bytes );
	ir_frame->data[2] = (ir_frame->data[2] & 0x0f)
					  + (parity << 4);

	ir_frame->byte_idx = ir_frame_aeha->data_bytes +2;

	return 1;
}

