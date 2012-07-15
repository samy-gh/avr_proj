
#ifndef __IR_FRAME_SONY_H__
#define __IR_FRAME_SONY_H__

// プロジェクト固有
#include "ir_frame.h"

// samylib
#include <my_typedef.h>


// SONY形式フレームデータ: T_IR_SONY_FRAME
typedef struct {
	UCHAR data;		// 7bit
	UCHAR addr_bit;
	UINT  addr;		// 5/8/12bit
} T_IR_SONY_FRAME;


extern UCHAR Ir_Frame_Sony_Import_Frame( T_IR_SONY_FRAME* ir_frame_sony, T_IR_FRAME* ir_frame );
extern UCHAR Ir_Frame_Sony_Export_Frame( T_IR_FRAME* ir_frame, T_IR_SONY_FRAME* ir_frame_sony );

#endif //__IR_FRAME_SONY_H__

