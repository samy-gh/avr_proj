
#ifndef __IR_FRAME_NEC_H__
#define __IR_FRAME_NEC_H__

// プロジェクト固有
#include "ir_frame.h"

// samylib
#include <my_typedef.h>


// NEC形式フレームデータ: T_IR_NEC_FRAME
typedef struct {
	UINT	customer;
	UCHAR	data;
} T_IR_NEC_FRAME;


extern UCHAR Ir_Frame_Nec_Import_Frame( T_IR_NEC_FRAME* ir_frame_nec, T_IR_FRAME* ir_frame );
extern UCHAR Ir_Frame_Nec_Export_Frame( T_IR_FRAME* ir_frame, T_IR_NEC_FRAME* ir_frame_nec );

#endif //__IR_FRAME_NEC_H__

