
#ifndef __IR_FRAME_AEHA_H__
#define __IR_FRAME_AEHA_H__

// プロジェクト固有
#include "ir_frame.h"

// samylib
#include <my_typedef.h>


// AEHA形式フレームデータ: T_IR_AEHA_FRAME
typedef struct {
	UINT	customer;
	UCHAR	data[32];
	UCHAR   data_bytes;
} T_IR_AEHA_FRAME;


extern UCHAR Ir_Frame_Aeha_Import_Frame( T_IR_AEHA_FRAME* ir_frame_aeha, T_IR_FRAME* ir_frame );
extern UCHAR Ir_Frame_Aeha_Export_Frame( T_IR_FRAME* ir_frame, T_IR_AEHA_FRAME* ir_frame_aeha );

#endif //__IR_FRAME_AEHA_H__

