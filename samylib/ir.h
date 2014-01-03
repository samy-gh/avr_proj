
#ifndef __IR_H__
#define __IR_H__

// samylib
#include <my_typedef.h>

// WinAVR
#include <avr/pgmspace.h>


// フレームワーク用イベント通知
#ifdef CO_ENABLE_IR_RECV_EVENT
extern volatile UCHAR _gui_event_ir_recv_end;
extern volatile UCHAR _gui_event_ir_recv_err;
#endif
#ifdef CO_ENABLE_IR_SEND_EVENT
extern volatile UCHAR _gui_event_ir_send_end;
#endif


// フレームタイプ: E_IR_FRAME_TYPE
typedef enum {
	E_IR_FRAME_TYPE_UNKNOWN = 0,
	E_IR_FRAME_TYPE_NEC,
	E_IR_FRAME_TYPE_AEHA,
	E_IR_FRAME_TYPE_SONY,
	E_IR_FRAME_TYPE_TOSHIBA,
	E_IR_FRAME_TYPE_MAX
} E_IR_FRAME_TYPE;

// フレームデータ: T_IR_FRAME
typedef struct {
	E_IR_FRAME_TYPE type;
	UCHAR data[32];
	UCHAR byte_idx;
	UCHAR bit_mask;
} T_IR_FRAME;


// E_IR_RECV_STAT
typedef enum {
	E_IR_RECV_STAT_IDLE = 0,				// 0
	E_IR_RECV_STAT_LEADER1_WAIT,
	E_IR_RECV_STAT_LEADER1_MEASURE,
	E_IR_RECV_STAT_LEADER0_NEC_MEASURE,
	E_IR_RECV_STAT_LEADER0_AEHA_MEASURE,
	E_IR_RECV_STAT_SONY_FRAME_WAIT,			// 5
	E_IR_RECV_STAT_SONY_FRAME_MEASURE,
	E_IR_RECV_STAT_NEC_FRAME_WAIT,
	E_IR_RECV_STAT_NEC_FRAME_MEASURE,
	E_IR_RECV_STAT_AEHA_FRAME_WAIT,
	E_IR_RECV_STAT_AEHA_FRAME_MEASURE,		// 10
	E_IR_RECV_STAT_LEADER0_TOSHIBA_MEASURE,
	E_IR_RECV_STAT_TOSHIBA_FRAME_WAIT,
	E_IR_RECV_STAT_TOSHIBA_FRAME_MEASURE,
	E_IR_RECV_STAT_END,
	E_IR_RECV_STAT_ERR,						// 15
	E_IR_RECV_STAT_ABORT,
	E_IR_RECV_STAT_MAX
} E_IR_RECV_STAT;

// E_IR_RECV_ERR
typedef enum {
	E_IR_RECV_ERR_NONE = 0,						// 0
	E_IR_RECV_ERR_LEADER1_WAIT,
	E_IR_RECV_ERR_LEADER1_MEASURE,
	E_IR_RECV_ERR_LEADER1_MEASURE_TO,
	E_IR_RECV_ERR_LEADER0_NEC_MEASURE,
	E_IR_RECV_ERR_LEADER0_NEC_MEASURE_TO,		// 5
	E_IR_RECV_ERR_LEADER0_AEHA_MEASURE,
	E_IR_RECV_ERR_LEADER0_AEHA_MEASURE_TO,
	E_IR_RECV_ERR_UNKNOWN_LEADER,
	E_IR_RECV_ERR_FRAME_DATA_1_NULL,
	E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH,	// 10
	E_IR_RECV_ERR_FRAME_DATA_0_NULL,
	E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH,
	E_IR_RECV_ERR_BUF_OVERFLOW,
	E_IR_RECV_ERR_ILLEGAL_STAT,
	E_IR_RECV_ERR_UNKNOWN_TO,					// 15
	E_IR_RECV_ERR_SONY_FRAME,
	E_IR_RECV_ERR_SONY_FRAME_MEASURE_TO,
	E_IR_RECV_ERR_NEC_FRAME_WAIT_TO,
	E_IR_RECV_ERR_AEHA_FRAME_WAIT_TO,
	E_IR_RECV_ERR_LEADER0_TOSHIBA_MEASURE,		// 20
	E_IR_RECV_ERR_LEADER0_TOSHIBA_MEASURE_TO,
	E_IR_RECV_ERR_TOSHIBA_FRAME_WAIT_TO,
	E_IR_RECV_ERR_MAX
} E_IR_RECV_ERR;

extern E_IR_RECV_STAT volatile gIr_Recv_Stat;
extern E_IR_RECV_ERR volatile gIr_Recv_Err;
extern T_IR_FRAME volatile gIr_Frame;


#define IR_RECV_PCINT8_HDL() \
	do { \
		if( (gIr_Recv_Stat != E_IR_RECV_STAT_IDLE) \
		 && (gIr_Recv_Stat != E_IR_RECV_STAT_END) \
		 && (gIr_Recv_Stat != E_IR_RECV_STAT_ERR) \
			) { \
			if( bit_is_set( PCMSK1, PCINT8 ) ) {	\
				Ir_Recv_Pcint8Hdl(); \
			} \
		} \
	} while( 0 )

extern VOID Ir_Frame_Set( const E_IR_FRAME_TYPE type, const UCHAR* const data, UCHAR data_bytes, const UCHAR last_bit_mask );
extern VOID Ir_Frame_PgmSet( const E_IR_FRAME_TYPE type, PGM_P p_str, UCHAR data_bytes, const UCHAR last_bit_mask );
extern VOID Ir_Frame_Dump( VOID );

extern VOID Ir_Recv_EventHistoryDump( VOID );
extern VOID Ir_Recv_Pcint8Hdl( VOID );
extern VOID Ir_Recv_Stop( VOID );
extern VOID Ir_Recv_Start( VOID );
extern BOOL Ir_Recv_WaitEnd( VOID );

extern INT Ir_Send_Start( VOID );
extern VOID Ir_Send_Stop( VOID );
extern BOOL Ir_Send_WaitEnd( VOID );

extern BOOL Ir_Write_Eeprom( UINT addr );
extern BOOL Ir_Read_Eeprom( UINT addr );


#endif //__IR_H__
