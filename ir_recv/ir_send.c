
// プロジェクト固有
#include "ir_recv.h"
#include "ir_frame.h"
#include "custom.h"

// samylib
#include <my_typedef.h>
#include <common.h>
#include <timer0.h>
#include <timer1.h>
#include <usart.h>

// WinAVR
#include <avr/io.h>
#include <util/delay.h>

// STD-C
#include <stdio.h>


typedef enum {
	E_IR_SEND_STAT_IDLE = 0,
	E_IR_SEND_STAT_SONY_LEADER_0,		// Leader送信中(1/4)
	E_IR_SEND_STAT_SONY_LEADER_1,		// Leader送信中(2/4)
	E_IR_SEND_STAT_SONY_LEADER_2,		// Leader送信中(3/4)
	E_IR_SEND_STAT_SONY_LEADER_3,		// Leader送信中(4/4)
	E_IR_SEND_STAT_SONY_DATA_0_LO,		// データ'0' LO部送信中
	E_IR_SEND_STAT_SONY_DATA_0_HI_0,	// データ'0' HI部送信中
	E_IR_SEND_STAT_SONY_DATA_1_LO,		// データ'1' LO部送信中
	E_IR_SEND_STAT_SONY_DATA_1_HI_0,	// データ'1' HI部送信中 (1/2)
	E_IR_SEND_STAT_SONY_DATA_1_HI_1,	// データ'1' HI部送信中 (2/2)
	E_IR_SEND_STAT_SONY_GAP,			// フレーム間ギャップ
	E_IR_SEND_STAT_SONY_END,			// 送信完了待ち
	E_IR_SEND_STAT_ERR,					// エラー発生
	E_IR_SEND_STAT_MAX
} E_IR_SEND_STAT;


E_IR_SEND_STAT volatile gIr_Send_Stat = E_IR_SEND_STAT_IDLE;
UCHAR gIr_Send_CurrBit;
UCHAR gIr_Send_ByteIdx;
UCHAR gIr_Send_BitMask;
UCHAR gIr_Send_LoopCnt;
UCHAR gIr_Send_GapCnt;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// サブキャリア制御
//
static VOID ir_send_init_subcarrier( UINT fsc_hz, UINT duty )
{
	Timer0_Init( 1 );
	Timer0_Set_Period( 1000000UL / fsc_hz );
//	Timer0_Set_Period_Direct( 2, 49 );
	Timer0_Set_PwmPin( duty );

#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("OCR0A=%u, OCR0B=%u"), OCR0A, OCR0B );
	printf_P( PSTR(", TCCR0A=0x%x, TCCR0B=0x%x\n"), TCCR0A, TCCR0B );
	printf_P( PSTR("bit=0x%x\n"), gTimer0_ClockSelectBits );
#endif
}

static VOID ir_send_start_subcarrier( VOID )
{
	Timer0_Start();
}

static VOID ir_send_stop_subcarrier( VOID )
{
	Timer0_Stop();
}

static VOID ir_send_close_subcarrier( VOID )
{
	Timer0_Close();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ
//

static VOID ir_send__event_send_sony_data_lo( VOID );

static VOID ir_send__event_send_sony_gap( VOID )
{
	if( gIr_Send_GapCnt == 75 ) {
		gIr_Send_GapCnt = 0;
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_0;
		ir_send_start_subcarrier();
	}
}

static VOID ir_send__event_send_sony_data_lo( VOID )
{
	ir_send_stop_subcarrier();	// 発信停止

	// 終了判定
	if( (gIr_Send_ByteIdx == gIr_Frame.byte_idx)
	 && (gIr_Send_BitMask == gIr_Frame.bit_mask)
	 ) {
		gIr_Send_LoopCnt++;
		if( gIr_Send_LoopCnt == 3 ) {
			gIr_Send_Stat = E_IR_SEND_STAT_SONY_END;
			ir_send_close_subcarrier();
			return;
		}
		gIr_Send_ByteIdx = 0;
		gIr_Send_BitMask = 0x01;
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_GAP;
		ir_send__event_send_sony_gap();
		return;
	}


	// 送信中のビット0 or 1を判定
	gIr_Send_CurrBit = ((gIr_Frame.data[gIr_Send_ByteIdx] & gIr_Send_BitMask) != 0)?(1):(0);

	if( gIr_Send_BitMask == 0x80 ) {
		gIr_Send_ByteIdx++;
		gIr_Send_BitMask = 0x01;
	}
	else {
		gIr_Send_BitMask <<= 1;
	}

	// 送信中のビット0 or 1を判定し状態遷移
	if( gIr_Send_CurrBit ) {
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_DATA_1_LO;
	}
	else {
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_DATA_0_LO;
	}
}

static VOID ir_send__event_send_sony_data_0_hi( VOID )
{
	ir_send_start_subcarrier();

	gIr_Send_Stat = E_IR_SEND_STAT_SONY_DATA_0_HI_0;
}

static VOID ir_send__event_send_sony_data_1_hi_0( VOID )
{
	ir_send_start_subcarrier();

	gIr_Send_Stat = E_IR_SEND_STAT_SONY_DATA_1_HI_0;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// 送信制御タイマハンドラ

static VOID ir_send_ovf_inthdl( VOID )
{
	PORTC ^= _BV(PC3);
	gIr_Send_GapCnt++;

	if( gIr_Frame.type == E_IR_FRAME_TYPE_SONY ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_SONY_LEADER_0:
				gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_1;
				break;
			case E_IR_SEND_STAT_SONY_LEADER_1:
				gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_2;
				break;
			case E_IR_SEND_STAT_SONY_LEADER_2:
				gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_3;
				break;
			case E_IR_SEND_STAT_SONY_LEADER_3:
				ir_send__event_send_sony_data_lo();
				break;
			case E_IR_SEND_STAT_SONY_DATA_0_LO:
				ir_send__event_send_sony_data_0_hi();
				break;
			case E_IR_SEND_STAT_SONY_DATA_0_HI_0:
				ir_send__event_send_sony_data_lo();
				break;
			case E_IR_SEND_STAT_SONY_DATA_1_LO:
				ir_send__event_send_sony_data_1_hi_0();
				break;
			case E_IR_SEND_STAT_SONY_DATA_1_HI_0:
				gIr_Send_Stat = E_IR_SEND_STAT_SONY_DATA_1_HI_1;
				break;
			case E_IR_SEND_STAT_SONY_DATA_1_HI_1:
				ir_send__event_send_sony_data_lo();
				break;
			case E_IR_SEND_STAT_SONY_GAP:
				ir_send__event_send_sony_gap();
				break;
			default:
				gIr_Send_Stat = E_IR_SEND_STAT_ERR;
				return;
		}
	}
	else {
		gIr_Send_Stat = E_IR_SEND_STAT_ERR;
		return;
		// エラー
	}
}

VOID Ir_Send( VOID )
{
	UINT const fsc_hz  = 40000;	// サブキャリア周波数 40kHz
	UINT const duty_sc = 3333;	// サブキャリアDuty 33% (1/3)
	UINT const t_usec  = 600;	// フレーム単位時間 600usec
	BOOL send_complete = FALSE;

	// デバッグ用
	sbi( DDRC, DDC3 );

	// デバッグ用 送信フレーム
	gIr_Frame.type = E_IR_FRAME_TYPE_SONY;
	gIr_Frame.data[0] = 0x95;
	gIr_Frame.data[1] = 0x07;
	gIr_Frame.byte_idx = 1;
	gIr_Frame.bit_mask = 0x10;


	// サブキャリア発信タイマ初期化
	ir_send_init_subcarrier( fsc_hz, duty_sc );	// fsc=40kHz duty=33%

	// フレーム制御タイマ初期化
	Timer1_Init( 1 );
	Timer1_Set_Period( t_usec );	// 600usec
//	Timer1_Set_Period_Direct( 1, 9550 );	// 600usec
	Timer1_Enable_CompaIntr( ir_send_ovf_inthdl );

	PORTC ^= _BV(PC3);

	gIr_Send_LoopCnt = 0;
	gIr_Send_ByteIdx = 0;
	gIr_Send_BitMask = 0x01;
	gIr_Send_GapCnt = 0;


	// フレーム送信開始
	gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_0;
	ir_send_start_subcarrier();
	Timer1_Start();


#if 1
	Usart_Set_Stdout();
	printf_P( PSTR("ICR1=%u, OCR1B=%u"), ICR1, OCR1B );
	printf_P( PSTR(", TCCR1A=0x%x, TCCR1B=0x%x\n"), TCCR1A, TCCR1B );
	printf_P( PSTR("bit=0x%x\n"), gTimer1_ClockSelectBits );
#endif


	while( send_complete == FALSE ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_SONY_END:
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_IDLE:
				send_complete = TRUE;
				break;
			default:
				printf_P( PSTR("stat=%u\n"), gIr_Send_Stat );
				_delay_ms( 1000 );
				break;
		}
	}

	ir_send_stop_subcarrier();
	ir_send_close_subcarrier();
	Timer1_Stop();
	Timer1_Close();

	cbi( DDRC, DDC3 );
}

