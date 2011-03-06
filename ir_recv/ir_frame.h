
#ifndef __IR_FRAME_H__
#define __IR_FRAME_H__

// samylib
#include <my_typedef.h>


// フレームタイプ: E_IR_FRAME_TYPE
typedef enum {
	E_IR_FRAME_TYPE_UNKNOWN = 0,
	E_IR_FRAME_TYPE_NEC,
	E_IR_FRAME_TYPE_AEHA,
	E_IR_FRAME_TYPE_SONY,
	E_IR_FRAME_TYPE_MAX
} E_IR_FRAME_TYPE;

// フレームデータ: T_IR_FRAME
typedef struct {
	E_IR_FRAME_TYPE type;
	UCHAR data[32];
	UCHAR byte_idx;
	UCHAR bit_mask;
} T_IR_FRAME;

extern T_IR_FRAME volatile gIr_Frame;

extern VOID Ir_Frame_Init( VOID );
extern VOID Ir_Frame_Set_Type( E_IR_FRAME_TYPE type );
extern BOOL Ir_Frame_Store_Bit( UCHAR bit );
extern VOID Ir_Frame_Dump( VOID );

#endif //__IR_FRAME_H__
