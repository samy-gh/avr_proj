
#ifndef __IR_CTRL_H__
#define __IR_CTRL_H__

// プロジェクト固有
#include "usb_proto.h"

// samylib
#include <my_typedef.h>


extern VOID Ir_Ctrl_Start_Recv_KeyEventHdl( VOID );
extern VOID Ir_Ctrl_Abort_Recv_KeyEventHdl( VOID );
extern BOOL Ir_Ctrl_Start_Send_KeyEventHdl( VOID );
extern VOID Ir_Ctrl_Recv_EventHdl( VOID );
extern BOOL Ir_Ctrl_Send_IrCode( T_USB_PROTO_IR_CODE* const buf );
extern INT Ir_Ctrl_Get_IrCodeEeprom( T_USB_PROTO_IR_CODE* const buf );
extern INT Ir_Ctrl_Get_IrCode( T_USB_PROTO_IR_CODE* const buf );
extern BOOL Ir_Ctrl_Set_Eeprom( T_USB_PROTO_IR_CODE* const buf );
 

#endif //__IR_CTRL_H__

