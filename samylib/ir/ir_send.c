
// プロジェクト固有
#include "ir.h"
#include "ir_frame.h"
#include "ir_custom.h"

// samylib
#include <my_typedef.h>
#include <common.h>
#include <timer0.h>
#include <timer1.h>
#include <usart.h>
#include <test_led.h>

// WinAVR
#include <util/delay.h>

// STD-C
#include <stdio.h>


#ifdef CO_IR_SEND_INDICATOR_ENABLE
#ifndef CO_TEST_LED_ENABLE
#define CO_TEST_LED_ENABLE
#endif
#else
#undef CO_TEST_LED_ENABLE
#endif

typedef enum {
	// [0]
	E_IR_SEND_STAT_IDLE = 0,
	// [1-11] SONY
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
	// [12-23] NEC
	E_IR_SEND_STAT_NEC_LEADER_HI_0,		// Leader HI部送信中(1/16)
	E_IR_SEND_STAT_NEC_LEADER_HI_1,		// Leader HI部送信中(2～16/16)
	E_IR_SEND_STAT_NEC_LEADER_LO,		// Leader LO部送信中(1～8/8)
	E_IR_SEND_STAT_NEC_DATA_HI,			// データ HI部送信中
	E_IR_SEND_STAT_NEC_DATA_0_LO_0,		// データ'0' LO部送信中
	E_IR_SEND_STAT_NEC_DATA_1_LO_0,		// データ'1' LO部送信中 (1/3)
	E_IR_SEND_STAT_NEC_DATA_1_LO_1,		// データ'1' LO部送信中 (2/3)
	E_IR_SEND_STAT_NEC_DATA_1_LO_2,		// データ'1' LO部送信中 (3/3)
	E_IR_SEND_STAT_NEC_LEADOUT,			// Leadout部送信中 (次フレームあり)
	E_IR_SEND_STAT_NEC_GAP,				// フレーム間ギャップ
	E_IR_SEND_STAT_NEC_LAST_LEADOUT,	// Leadout部送信中
	E_IR_SEND_STAT_NEC_END,				// 送信完了待ち
	// [24-35] AEHA
	E_IR_SEND_STAT_AEHA_LEADER_HI_0,	// Leader HI部送信中(1/8)
	E_IR_SEND_STAT_AEHA_LEADER_HI_1,	// Leader HI部送信中(2～8/8)
	E_IR_SEND_STAT_AEHA_LEADER_LO,		// Leader LO部送信中(1～4/4)
	E_IR_SEND_STAT_AEHA_DATA_HI,		// データ HI部送信中
	E_IR_SEND_STAT_AEHA_DATA_0_LO_0,	// データ'0' LO部送信中
	E_IR_SEND_STAT_AEHA_DATA_1_LO_0,	// データ'1' LO部送信中 (1/3)
	E_IR_SEND_STAT_AEHA_DATA_1_LO_1,	// データ'1' LO部送信中 (2/3)
	E_IR_SEND_STAT_AEHA_DATA_1_LO_2,	// データ'1' LO部送信中 (3/3)
	E_IR_SEND_STAT_AEHA_LEADOUT,		// Leadout部送信中 (次フレームあり)
	E_IR_SEND_STAT_AEHA_GAP,			// フレーム間ギャップ
	E_IR_SEND_STAT_AEHA_LAST_LEADOUT,	// Leadout部送信中
	E_IR_SEND_STAT_AEHA_END,			// 送信完了待ち
	// [36-47] Toshiba
	E_IR_SEND_STAT_TOSHIBA_LEADER_HI_0,	// Leader HI部送信中(1/8)
	E_IR_SEND_STAT_TOSHIBA_LEADER_HI_1,	// Leader HI部送信中(2～8/8)
	E_IR_SEND_STAT_TOSHIBA_LEADER_LO,	// Leader LO部送信中(1～8/8)
	E_IR_SEND_STAT_TOSHIBA_DATA_HI,		// データ HI部送信中
	E_IR_SEND_STAT_TOSHIBA_DATA_0_LO_0,	// データ'0' LO部送信中
	E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_0,	// データ'1' LO部送信中 (1/3)
	E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_1,	// データ'1' LO部送信中 (2/3)
	E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_2,	// データ'1' LO部送信中 (3/3)
	E_IR_SEND_STAT_TOSHIBA_LEADOUT,		// Leadout部送信中 (次フレームあり)
	E_IR_SEND_STAT_TOSHIBA_GAP,			// フレーム間ギャップ
	E_IR_SEND_STAT_TOSHIBA_LAST_LEADOUT,	// Leadout部送信中
	E_IR_SEND_STAT_TOSHIBA_END,			// 送信完了待ち
	// [48-52] その他
	E_IR_SEND_STAT_ERR,					// エラー発生
	E_IR_SEND_STAT_ERR2,				// エラー発生
	E_IR_SEND_STAT_ERR_INIT,			// 初期化エラー
	E_IR_SEND_STAT_ERR_EMPTY,			// データなしエラー
	E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE,	// 不明なフレーム形式エラー
	E_IR_SEND_STAT_MAX
} E_IR_SEND_STAT;


E_IR_SEND_STAT volatile gIr_Send_Stat = E_IR_SEND_STAT_IDLE;
UINT volatile gIr_Send_ErrData;
UCHAR gIr_Send_CurrBit;
UCHAR gIr_Send_ByteIdx;
UCHAR gIr_Send_BitMask;
UCHAR gIr_Send_LoopCnt;
UCHAR gIr_Send_LoopMax;
UCHAR gIr_Send_LeaderCount;
UINT gIr_Send_GapCnt;
UINT gIr_Send_GapCntMax;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// フレームワーク用イベント
#ifdef CO_ENABLE_IR_SEND_EVENT
volatile UCHAR _gui_event_ir_send_end = 0;
#endif


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// サブキャリア制御
//

static BOOL ir_send_init_subcarrier( const UINT fsc_hz, UINT duty )
{
#ifdef CO_IR_SEND_NEG_LOGIC
	duty = 10000 - duty;
#endif

#ifdef CO_TEST_LED_ENABLE
	TEST_LED1_OFF();
#endif

	Timer0_Init( 1 );
	Timer0_Set_Period( 1000000UL / fsc_hz );
//	Timer0_Set_Period_Direct( 2, 49 );
	if( Timer0_Set_PwmPin( duty ) == FALSE ) {
		return FALSE;
	}

#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("OCR0A=%u, OCR0B=%u"), OCR0A, OCR0B );
	printf_P( PSTR(", TCCR0A=0x%x, TCCR0B=0x%x\n"), TCCR0A, TCCR0B );
	printf_P( PSTR("bit=0x%x\n"), gTimer0_ClockSelectBits );
#endif

	return TRUE;
}

static VOID ir_send_start_subcarrier( VOID )
{
	Timer0_Start();
#ifdef CO_TEST_LED_ENABLE
	TEST_LED1_ON();
#endif
}

static VOID ir_send_stop_subcarrier( VOID )
{
	Timer0_Stop();
#ifdef CO_TEST_LED_ENABLE
	TEST_LED1_OFF();
#endif
}

static VOID ir_send_close_subcarrier( VOID )
{
	Timer0_Close();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ(SONY)
//

static VOID ir_send__event_send_sony_data_lo( VOID );

static VOID ir_send__event_send_sony_gap( VOID )
{
	if( gIr_Send_GapCnt >= gIr_Send_GapCntMax ) {
		gIr_Send_GapCnt = 0;
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_LEADER_0;
		ir_send_start_subcarrier();
	}
}

static VOID ir_send__event_send_sony_data_lo( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止

	// 終了判定
	if( (gIr_Send_ByteIdx == gIr_Frame.byte_idx)
	 && (gIr_Send_BitMask == gIr_Frame.bit_mask)
	 ) {
		gIr_Send_LoopCnt++;
		if( gIr_Send_LoopCnt >= gIr_Send_LoopMax ) {
			gIr_Send_Stat = E_IR_SEND_STAT_SONY_END;
			ir_send_close_subcarrier();
			return;
		}
		gIr_Send_ByteIdx = 0;
		gIr_Send_BitMask = 0x80;
		gIr_Send_Stat = E_IR_SEND_STAT_SONY_GAP;
		ir_send__event_send_sony_gap();
		return;
	}


	// 送信するビット0 or 1を判定
	gIr_Send_CurrBit = ((gIr_Frame.data[gIr_Send_ByteIdx] & gIr_Send_BitMask) != 0)?(1):(0);

	if( gIr_Send_BitMask == 0x01 ) {
		gIr_Send_ByteIdx++;
		gIr_Send_BitMask = 0x80;
	}
	else {
		gIr_Send_BitMask >>= 1;
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
// イベントハンドラ(NEC)
//

static VOID ir_send__event_send_nec_leader_hi_1( VOID )
{
	gIr_Send_LeaderCount = 15;	// Leader-Hi is 16T
	gIr_Send_Stat = E_IR_SEND_STAT_NEC_LEADER_HI_1;
}

static VOID ir_send__event_send_nec_leader_lo( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_stop_subcarrier();	// 発振停止
		gIr_Send_LeaderCount = 8;	// Leader-Hi is 8T
		gIr_Send_Stat = E_IR_SEND_STAT_NEC_LEADER_LO;
	}
}

static VOID ir_send__event_send_nec_data_hi_0( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_start_subcarrier();	// 発振開始
		gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_HI;
	}
}

static VOID ir_send__event_send_nec_data_lo( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止

	// 送信中のビット0 or 1を判定
	gIr_Send_CurrBit = ((gIr_Frame.data[gIr_Send_ByteIdx] & gIr_Send_BitMask) != 0)?(1):(0);

	if( gIr_Send_BitMask == 0x01 ) {
		gIr_Send_ByteIdx++;
		gIr_Send_BitMask = 0x80;
	}
	else {
		gIr_Send_BitMask >>= 1;
	}

	// 送信中のビット0 or 1を判定し状態遷移
	if( gIr_Send_CurrBit ) {
		gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_1_LO_0;
	}
	else {
		gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_0_LO_0;
	}
}

static VOID ir_send__event_send_nec_data_hi_1( VOID )
{
	ir_send_start_subcarrier();	// 発振開始

	// 終了判定
	if( (gIr_Send_ByteIdx == gIr_Frame.byte_idx)
	 && (gIr_Send_BitMask == gIr_Frame.bit_mask)
	 ) {
		// 1フレーム終了
		gIr_Send_LoopCnt++;
		if( gIr_Send_LoopCnt >= gIr_Send_LoopMax ) {
			// フレーム繰り返し終了
			gIr_Send_Stat = E_IR_SEND_STAT_NEC_LAST_LEADOUT;
			return;
		}

		gIr_Send_Stat = E_IR_SEND_STAT_NEC_LEADOUT;
		return;
	}
	gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_HI;
}

static VOID ir_send__event_send_nec_gap_0( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	gIr_Send_Stat = E_IR_SEND_STAT_NEC_GAP;
}

static VOID ir_send__event_send_nec_gap_1( VOID )
{
	if( gIr_Send_GapCnt >= gIr_Send_GapCntMax ) {
		gIr_Send_GapCnt = 0;

		// 次フレームのLedaer送信開始
		ir_send_start_subcarrier();

		gIr_Send_ByteIdx = 0;
		gIr_Send_BitMask = 0x80;

		gIr_Send_Stat = E_IR_SEND_STAT_NEC_LEADER_HI_0;
	}
}

static VOID ir_send__event_stop_nec_last_leadout( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	ir_send_close_subcarrier();
	gIr_Send_Stat = E_IR_SEND_STAT_NEC_END;
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ(AEHA)
//

static VOID ir_send__event_send_aeha_leader_hi_1( VOID )
{
	gIr_Send_LeaderCount = 7;	// Leader-Hi is 8T
	gIr_Send_Stat = E_IR_SEND_STAT_AEHA_LEADER_HI_1;
}

static VOID ir_send__event_send_aeha_leader_lo( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_stop_subcarrier();	// 発振停止
		gIr_Send_LeaderCount = 4;	// Leader-Hi is 4T
		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_LEADER_LO;
	}
}

static VOID ir_send__event_send_aeha_data_hi_0( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_start_subcarrier();	// 発振開始
		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_HI;
	}
}

static VOID ir_send__event_send_aeha_data_lo( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止

	// 送信中のビット0 or 1を判定
	gIr_Send_CurrBit = ((gIr_Frame.data[gIr_Send_ByteIdx] & gIr_Send_BitMask) != 0)?(1):(0);

	if( gIr_Send_BitMask == 0x01 ) {
		gIr_Send_ByteIdx++;
		gIr_Send_BitMask = 0x80;
	}
	else {
		gIr_Send_BitMask >>= 1;
	}

	// 送信中のビット0 or 1を判定し状態遷移
	if( gIr_Send_CurrBit ) {
		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_1_LO_0;
	}
	else {
		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_0_LO_0;
	}
}

static VOID ir_send__event_send_aeha_data_hi_1( VOID )
{
	ir_send_start_subcarrier();	// 発振開始

	// 終了判定
	if( (gIr_Send_ByteIdx == gIr_Frame.byte_idx)
	 && (gIr_Send_BitMask == gIr_Frame.bit_mask)
	 ) {
		// 1フレーム終了
		gIr_Send_LoopCnt++;
		if( gIr_Send_LoopCnt >= gIr_Send_LoopMax ) {
			// フレーム繰り返し終了
			gIr_Send_Stat = E_IR_SEND_STAT_AEHA_LAST_LEADOUT;
			return;
		}

		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_LEADOUT;
		return;
	}
	gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_HI;
}

static VOID ir_send__event_send_aeha_gap_0( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	gIr_Send_Stat = E_IR_SEND_STAT_AEHA_GAP;

	if( (gIr_Send_GapCnt +19) > gIr_Send_GapCntMax ) {
		gIr_Send_GapCntMax = gIr_Send_GapCnt +19;
	}
}

static VOID ir_send__event_send_aeha_gap_1( VOID )
{
	if( gIr_Send_GapCnt >= gIr_Send_GapCntMax ) {
		gIr_Send_GapCnt = 0;

		// 次フレームのLedaer送信開始
		ir_send_start_subcarrier();

		gIr_Send_ByteIdx = 0;
		gIr_Send_BitMask = 0x80;

		gIr_Send_Stat = E_IR_SEND_STAT_AEHA_LEADER_HI_0;
	}
}

static VOID ir_send__event_stop_aeha_last_leadout( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	ir_send_close_subcarrier();
	gIr_Send_Stat = E_IR_SEND_STAT_AEHA_END;
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ(Toshiba)
//

static VOID ir_send__event_send_toshiba_leader_hi_1( VOID )
{
	gIr_Send_LeaderCount = 7;	// Leader-Hi is 8T
	gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_LEADER_HI_1;
}

static VOID ir_send__event_send_toshiba_leader_lo( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_stop_subcarrier();	// 発振停止
		gIr_Send_LeaderCount = 8;	// Leader-Hi is 8T
		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_LEADER_LO;
	}
}

static VOID ir_send__event_send_toshiba_data_hi_0( VOID )
{
	gIr_Send_LeaderCount--;
	if( gIr_Send_LeaderCount == 0 ) {
		ir_send_start_subcarrier();	// 発振開始
		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_HI;
	}
}

static VOID ir_send__event_send_toshiba_data_lo( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止

	// 送信中のビット0 or 1を判定
	gIr_Send_CurrBit = ((gIr_Frame.data[gIr_Send_ByteIdx] & gIr_Send_BitMask) != 0)?(1):(0);

	if( gIr_Send_BitMask == 0x01 ) {
		gIr_Send_ByteIdx++;
		gIr_Send_BitMask = 0x80;
	}
	else {
		gIr_Send_BitMask >>= 1;
	}

	// 送信中のビット0 or 1を判定し状態遷移
	if( gIr_Send_CurrBit ) {
		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_0;
	}
	else {
		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_0_LO_0;
	}
}

static VOID ir_send__event_send_toshiba_data_hi_1( VOID )
{
	ir_send_start_subcarrier();	// 発振開始

	// 終了判定
	if( (gIr_Send_ByteIdx == gIr_Frame.byte_idx)
	 && (gIr_Send_BitMask == gIr_Frame.bit_mask)
	 ) {
		// 1フレーム終了
		gIr_Send_LoopCnt++;
		if( gIr_Send_LoopCnt >= gIr_Send_LoopMax ) {
			// フレーム繰り返し終了
			gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_LAST_LEADOUT;
			return;
		}

		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_LEADOUT;
		return;
	}
	gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_HI;
}

static VOID ir_send__event_send_toshiba_gap_0( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_GAP;

	if( (gIr_Send_GapCnt +9) > gIr_Send_GapCntMax ) {
		gIr_Send_GapCntMax = gIr_Send_GapCnt +9;
	}
}

static VOID ir_send__event_send_toshiba_gap_1( VOID )
{
	if( gIr_Send_GapCnt >= gIr_Send_GapCntMax ) {
		gIr_Send_GapCnt = 0;

		// 次フレームのLedaer送信開始
		ir_send_start_subcarrier();

		gIr_Send_ByteIdx = 0;
		gIr_Send_BitMask = 0x80;

		gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_LEADER_HI_0;
	}
}

static VOID ir_send__event_stop_toshiba_last_leadout( VOID )
{
	ir_send_stop_subcarrier();	// 発振停止
	ir_send_close_subcarrier();
	gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_END;
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// 送信制御タイマハンドラ

static VOID ir_send_ovf_inthdl( VOID )
{
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
			case E_IR_SEND_STAT_SONY_END:
				break;
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_ERR2:
			case E_IR_SEND_STAT_ERR_INIT:
			case E_IR_SEND_STAT_ERR_EMPTY:
			case E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE:
				break;
			default:
				gIr_Send_ErrData = gIr_Send_Stat;
				gIr_Send_Stat = E_IR_SEND_STAT_ERR;
				return;
		}
	}
	else if( gIr_Frame.type == E_IR_FRAME_TYPE_NEC ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_NEC_LEADER_HI_0:
				ir_send__event_send_nec_leader_hi_1();
				break;
			case E_IR_SEND_STAT_NEC_LEADER_HI_1:
				ir_send__event_send_nec_leader_lo();
				break;
			case E_IR_SEND_STAT_NEC_LEADER_LO:
				ir_send__event_send_nec_data_hi_0();
				break;
			case E_IR_SEND_STAT_NEC_DATA_HI:
				ir_send__event_send_nec_data_lo();
				break;
			case E_IR_SEND_STAT_NEC_DATA_0_LO_0:	// data bit'0'
				ir_send__event_send_nec_data_hi_1();
				break;
			case E_IR_SEND_STAT_NEC_DATA_1_LO_0:	// data bit'1' 1 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_1_LO_1;
				break;
			case E_IR_SEND_STAT_NEC_DATA_1_LO_1:	// data bit'1' 2 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_NEC_DATA_1_LO_2;
				break;
			case E_IR_SEND_STAT_NEC_DATA_1_LO_2:	// data bit'1' 3 of 3
				ir_send__event_send_nec_data_hi_1();
				break;
			case E_IR_SEND_STAT_NEC_LEADOUT:
				ir_send__event_send_nec_gap_0();
				break;
			case E_IR_SEND_STAT_NEC_GAP:
				ir_send__event_send_nec_gap_1();
				break;
			case E_IR_SEND_STAT_NEC_LAST_LEADOUT:
				ir_send__event_stop_nec_last_leadout();
				break;
			case E_IR_SEND_STAT_NEC_END:
				break;
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_ERR2:
			case E_IR_SEND_STAT_ERR_INIT:
			case E_IR_SEND_STAT_ERR_EMPTY:
			case E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE:
				break;
			default:
				gIr_Send_ErrData = gIr_Send_Stat;
				gIr_Send_Stat = E_IR_SEND_STAT_ERR;
				return;
		}
	}
	else if( gIr_Frame.type == E_IR_FRAME_TYPE_AEHA ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_AEHA_LEADER_HI_0:
				ir_send__event_send_aeha_leader_hi_1();
				break;
			case E_IR_SEND_STAT_AEHA_LEADER_HI_1:
				ir_send__event_send_aeha_leader_lo();
				break;
			case E_IR_SEND_STAT_AEHA_LEADER_LO:
				ir_send__event_send_aeha_data_hi_0();
				break;
			case E_IR_SEND_STAT_AEHA_DATA_HI:
				ir_send__event_send_aeha_data_lo();
				break;
			case E_IR_SEND_STAT_AEHA_DATA_0_LO_0:	// data bit'0'
				ir_send__event_send_aeha_data_hi_1();
				break;
			case E_IR_SEND_STAT_AEHA_DATA_1_LO_0:	// data bit'1' 1 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_1_LO_1;
				break;
			case E_IR_SEND_STAT_AEHA_DATA_1_LO_1:	// data bit'1' 2 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_AEHA_DATA_1_LO_2;
				break;
			case E_IR_SEND_STAT_AEHA_DATA_1_LO_2:	// data bit'1' 3 of 3
				ir_send__event_send_aeha_data_hi_1();
				break;
			case E_IR_SEND_STAT_AEHA_LEADOUT:
				ir_send__event_send_aeha_gap_0();
				break;
			case E_IR_SEND_STAT_AEHA_GAP:
				ir_send__event_send_aeha_gap_1();
				break;
			case E_IR_SEND_STAT_AEHA_LAST_LEADOUT:
				ir_send__event_stop_aeha_last_leadout();
				break;
			case E_IR_SEND_STAT_AEHA_END:
				break;
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_ERR2:
			case E_IR_SEND_STAT_ERR_INIT:
			case E_IR_SEND_STAT_ERR_EMPTY:
			case E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE:
				break;
			default:
				gIr_Send_ErrData = gIr_Send_Stat;
				gIr_Send_Stat = E_IR_SEND_STAT_ERR;
				return;
		}
	}
	else if( gIr_Frame.type == E_IR_FRAME_TYPE_TOSHIBA ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_TOSHIBA_LEADER_HI_0:
				ir_send__event_send_toshiba_leader_hi_1();
				break;
			case E_IR_SEND_STAT_TOSHIBA_LEADER_HI_1:
				ir_send__event_send_toshiba_leader_lo();
				break;
			case E_IR_SEND_STAT_TOSHIBA_LEADER_LO:
				ir_send__event_send_toshiba_data_hi_0();
				break;
			case E_IR_SEND_STAT_TOSHIBA_DATA_HI:
				ir_send__event_send_toshiba_data_lo();
				break;
			case E_IR_SEND_STAT_TOSHIBA_DATA_0_LO_0:	// data bit'0'
				ir_send__event_send_toshiba_data_hi_1();
				break;
			case E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_0:	// data bit'1' 1 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_1;
				break;
			case E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_1:	// data bit'1' 2 of 3
				gIr_Send_Stat = E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_2;
				break;
			case E_IR_SEND_STAT_TOSHIBA_DATA_1_LO_2:	// data bit'1' 3 of 3
				ir_send__event_send_toshiba_data_hi_1();
				break;
			case E_IR_SEND_STAT_TOSHIBA_LEADOUT:
				ir_send__event_send_toshiba_gap_0();
				break;
			case E_IR_SEND_STAT_TOSHIBA_GAP:
				ir_send__event_send_toshiba_gap_1();
				break;
			case E_IR_SEND_STAT_TOSHIBA_LAST_LEADOUT:
				ir_send__event_stop_toshiba_last_leadout();
				break;
			case E_IR_SEND_STAT_TOSHIBA_END:
				break;
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_ERR2:
			case E_IR_SEND_STAT_ERR_INIT:
			case E_IR_SEND_STAT_ERR_EMPTY:
			case E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE:
				break;
			default:
				gIr_Send_ErrData = gIr_Send_Stat;
				gIr_Send_Stat = E_IR_SEND_STAT_ERR;
				return;
		}
	}
	else {
		gIr_Send_ErrData = gIr_Frame.type;
		gIr_Send_Stat = E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE;
		return;
		// エラー
	}

#ifdef CO_ENABLE_IR_SEND_EVENT
	switch( gIr_Send_Stat ) {
		case E_IR_SEND_STAT_SONY_END:
		case E_IR_SEND_STAT_AEHA_END:
		case E_IR_SEND_STAT_NEC_END:
		case E_IR_SEND_STAT_ERR:
		case E_IR_SEND_STAT_IDLE:
			_gui_event_ir_send_end = 1;
			break;
		default:
			// do nothing
			break;
	}
#endif
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// タスクAPI サブルーチン
static VOID ir_send__start( const UINT fsc_hz, const UINT duty_sc, const UINT t_usec, const E_IR_SEND_STAT init_stat )
{
	// サブキャリア発振タイマ初期化
	if( ir_send_init_subcarrier( fsc_hz, duty_sc ) == FALSE ) {
		gIr_Send_Stat = E_IR_SEND_STAT_ERR_INIT;
		return;
	}

	// フレーム制御タイマ初期化
	Timer1_Init( 1 );
	Timer1_Set_Period( t_usec );	// 600usec
//	Timer1_Set_Period_Direct( 1, 9550 );	// 600usec
	Timer1_Enable_CompaIntr( ir_send_ovf_inthdl );

	gIr_Send_LoopCnt = 0;
	gIr_Send_ByteIdx = 0;
	gIr_Send_BitMask = 0x80;
	gIr_Send_GapCnt = 0;
	switch( gIr_Frame.type ) {
		case E_IR_FRAME_TYPE_SONY:
			gIr_Send_GapCntMax = 75;	// Frame length=45msec
			gIr_Send_LoopMax = 3;		// フレーム送信回数
			break;
		case E_IR_FRAME_TYPE_NEC:
			gIr_Send_GapCntMax = 192;	// Frame length=108msec
			gIr_Send_LoopMax = 3;		// フレーム送信回数
			break;
		case E_IR_FRAME_TYPE_AEHA:
			gIr_Send_GapCntMax = 305;	// Frame length=130msec
			gIr_Send_LoopMax = 3;		// フレーム送信回数
			break;
		case E_IR_FRAME_TYPE_TOSHIBA:
			gIr_Send_GapCntMax = 9;		// Frame length=4.4msec
			gIr_Send_LoopMax = 2;		// フレーム送信回数
			break;
		default:
			// fail safe
			gIr_Send_GapCntMax = 305;	// Frame length=130msec
			gIr_Send_LoopMax = 3;		// フレーム送信回数
	}


	// フレーム送信開始
	gIr_Send_Stat = init_stat;
	ir_send_start_subcarrier();
	Timer1_Start();


#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("ICR1=%u, OCR1B=%u"), ICR1, OCR1B );
	printf_P( PSTR(", TCCR1A=0x%x, TCCR1B=0x%x\n"), TCCR1A, TCCR1B );
	printf_P( PSTR("bit=0x%x\n"), gTimer1_ClockSelectBits );
#endif
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスクAPI
INT Ir_Send_Start( VOID )
{
	if( (gIr_Frame.byte_idx == 0) && (gIr_Frame.bit_mask == 0x80) ) {
		// empty
		gIr_Send_Stat = E_IR_SEND_STAT_ERR_EMPTY;
		return -1;
	}

	switch( gIr_Frame.type ) {
		case E_IR_FRAME_TYPE_SONY:
			ir_send__start(
					40000,	// サブキャリア周波数 40kHz
					3333,	// サブキャリアDuty 33% (1/3)
					600,	// フレーム単位時間 600usec
					E_IR_SEND_STAT_SONY_LEADER_0
					);
			break;
		case E_IR_FRAME_TYPE_NEC:
			ir_send__start(
					38000,	// サブキャリア周波数 38kHz
					3333,	// サブキャリアDuty 33% (1/3)
					562,	// フレーム単位時間 562usec
					E_IR_SEND_STAT_NEC_LEADER_HI_0
					);
			break;
		case E_IR_FRAME_TYPE_AEHA:
			ir_send__start(
					38000,	// サブキャリア周波数 33～40kHz(typ. 38kHz)
					3333,	// サブキャリアDuty 33% (1/3)
					425,	// フレーム単位時間 350～500usec (typ. 425usec)
					E_IR_SEND_STAT_AEHA_LEADER_HI_0
					);
			break;
		case E_IR_FRAME_TYPE_TOSHIBA:
			ir_send__start(
					38000,	// サブキャリア周波数 33～40kHz(typ. 38kHz)
					3333,	// サブキャリアDuty 33% (1/3)
					570,	// フレーム単位時間 570usec
					E_IR_SEND_STAT_TOSHIBA_LEADER_HI_0
					);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

VOID Ir_Send_Stop( VOID )
{
	ir_send_stop_subcarrier();
	ir_send_close_subcarrier();
	Timer1_Stop();
	Timer1_Close();
}

BOOL Ir_Send_WaitEnd( VOID )
{
	UINT wait_cnt = 0;

	while( TRUE ) {
		switch( gIr_Send_Stat ) {
			case E_IR_SEND_STAT_SONY_END:
			case E_IR_SEND_STAT_AEHA_END:
			case E_IR_SEND_STAT_NEC_END:
			case E_IR_SEND_STAT_TOSHIBA_END:
			case E_IR_SEND_STAT_IDLE:
				return TRUE;
			case E_IR_SEND_STAT_ERR:
			case E_IR_SEND_STAT_ERR2:
			case E_IR_SEND_STAT_ERR_INIT:
			case E_IR_SEND_STAT_ERR_EMPTY:
			case E_IR_SEND_STAT_ERR_UNKNOWN_FRAME_TYPE:
				printf_P( PSTR("error. stat=%u, %u\n"), gIr_Send_Stat, gIr_Send_ErrData );
				return FALSE;
			default:
				wait_cnt++;
				_delay_ms( 100 );
				if( wait_cnt >= 50 ) {	// 5sec
					printf_P( PSTR("timeout. stat=%u\n"), gIr_Send_Stat );
					return FALSE;
				}
				break;
		}
	}
}

