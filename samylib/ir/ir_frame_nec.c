
// プロジェクト固有
#include "ir_frame.h"
#include "ir_frame_nec.h"
#include "ir_custom.h"

// STD-C
#include <stdio.h>
#include <string.h>


UCHAR Ir_Frame_Nec_Import_Frame( T_IR_NEC_FRAME* ir_frame_nec, T_IR_FRAME* ir_frame )
{
	if( ir_frame->type != E_IR_FRAME_TYPE_NEC ) {
		return 0;
	}

	if( ir_frame->data[2] != ~ir_frame->data[3] ) {
		return 0;
	}

	ir_frame_nec->customer = ir_frame->data[1] * 0x100 + ir_frame->data[0];
	ir_frame_nec->data = ir_frame->data[2];

	return 1;
}


UCHAR Ir_Frame_Nec_Export_Frame( T_IR_FRAME* ir_frame, T_IR_NEC_FRAME* ir_frame_nec )
{
	ir_frame->type = E_IR_FRAME_TYPE_NEC;

	ir_frame->data[0] = (ir_frame_nec->customer & 0xff);
	ir_frame->data[1] = (ir_frame_nec->customer & 0xff) >> 8;
	ir_frame->data[2] = ir_frame_nec->data;
	ir_frame->data[3] = ~ir_frame_nec->data;

	ir_frame->byte_idx = 4;

	return 1;
}

