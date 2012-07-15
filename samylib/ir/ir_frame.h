
#ifndef __IR_FRAME_H__
#define __IR_FRAME_H__

// ir
#include "ir.h"

// samylib
#include <my_typedef.h>


extern T_IR_FRAME volatile gIr_Frame;

extern VOID Ir_Frame_Init( VOID );
extern VOID Ir_Frame_Set_Type( E_IR_FRAME_TYPE type );
extern BOOL Ir_Frame_Store_Bit( UCHAR bit );
extern VOID Ir_Frame_Dump( VOID );

#endif //__IR_FRAME_H__
