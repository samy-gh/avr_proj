
// プロジェクト固有
#include "ir_frame.h"
#include "ir_frame_sony.h"
#include "ir_custom.h"

// STD-C
#include <stdio.h>
#include <string.h>


UCHAR Ir_Frame_Sony_Import_Frame( T_IR_SONY_FRAME* ir_frame_sony, T_IR_FRAME* ir_frame )
{
	if( ir_frame->type != E_IR_FRAME_TYPE_SONY ) {
		return 0;
	}

	ir_frame_sony->data = ir_frame->data[0] & 0x7f;

	// 5bit addr
	if( (ir_frame->byte_idx == 1) && (ir_frame->bit_mask == 0x10) ) {
		ir_frame_sony->addr = ((ir_frame->data[0] & 0x80) >> 7)
							+ ((ir_frame->data[1] & 0x04) << 1);
		ir_frame_sony->addr_bit = 5;
	}
	else
	// 8bit addr
	if( (ir_frame->byte_idx == 1) && (ir_frame->bit_mask == 0x80) ) {
		ir_frame_sony->addr = ((ir_frame->data[0] & 0x80) >> 7)
							+ ((ir_frame->data[1] & 0x7f) << 1);
		ir_frame_sony->addr_bit = 8;
	}
	else
	// 12bit addr
	if( (ir_frame->byte_idx == 2) && (ir_frame->bit_mask == 0x04) ) {
		ir_frame_sony->addr = (((UINT)ir_frame->data[0] & 0x80) >> 7)
							+  ((UINT)ir_frame->data[1] << 1)
							+ (((UINT)ir_frame->data[2] & 0x07) << 8);
		ir_frame_sony->addr_bit = 12;
	}
	else {
		return 0;
	}

	return 1;
}


UCHAR Ir_Frame_Sony_Export_Frame( T_IR_FRAME* ir_frame, T_IR_SONY_FRAME* ir_frame_sony )
{
	ir_frame->type = E_IR_FRAME_TYPE_SONY;

	ir_frame->data[0] = ir_frame_sony->data & 0x7f;

	switch( ir_frame_sony->addr_bit ) {
		// 5bit addr
		case 5:
			// data=xDDD DDDD, addr=xxxA AAAa
			// => aDDD DDDD  xxxx AAAA
			ir_frame->data[0] += (ir_frame_sony->addr & 0x01) << 7;
			ir_frame->data[1] =  (ir_frame_sony->addr & 0x1e) >> 1;
			ir_frame->byte_idx = 2;
			break;
		// 8bit addr
		case 8:
			// data=xDDD DDDD, addr=AAAA AAAa
			// => aDDD DDDD  xAAA AAAA
			ir_frame->data[0] += (ir_frame_sony->addr & 0x01) << 7;
			ir_frame->data[1] =  (ir_frame_sony->addr & 0xfe) >> 1;
			ir_frame->byte_idx = 2;
			break;
		// 12bit addr
		case 12:
			// data=xDDD DDDD, addr=xxxx AAAA  AAAA AAAa
			// => aDDD DDDD  AAAA AAAA  xxxx xAAA
			ir_frame->data[0] += (ir_frame_sony->addr & 0x001) << 7;
			ir_frame->data[1] =  (ir_frame_sony->addr & 0x1fe) >> 1;
			ir_frame->data[2] =  (ir_frame_sony->addr & 0xe00) >> 9;
			ir_frame->byte_idx = 3;
			break;
		default:
			return 0;
	}

	return 1;
}

