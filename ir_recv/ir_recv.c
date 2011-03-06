
// プロジェクト固有
#include "ir_recv.h"
#include "ir_frame.h"
#include "custom.h"

// samylib
#include <my_typedef.h>
#include <usart.h>
#include <test_led.h>
#include <timer1.h>

// STD-C
#include <stdio.h>
#include <string.h>


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// ステータスコード
//

E_IR_RECV_STAT volatile gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// エラーコード
//

E_IR_RECV_ERR volatile gIr_Recv_Err = E_IR_RECV_ERR_NONE;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントコード
//
typedef enum {
	E_IR_RECV_EVENT_NONE = 0,
	E_IR_RECV_EVENT_CHG_0,
	E_IR_RECV_EVENT_CHG_1,
	E_IR_RECV_EVENT_TO,
	E_IR_RECV_EVENT_MAX
} E_IR_RECV_EVENT;

E_IR_RECV_EVENT volatile g_ir_recv_event_last = E_IR_RECV_EVENT_NONE;
UINT volatile g_ir_recv_event_count = 0;


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// エラー発生時のタイマ値(usec変換後)
UINT volatile g_ir_timer_usec_last;

// イベント発生時のタイマ値
UINT volatile g_timer_count;


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントログ(デバッグ用)
//
#define D_IR_RECV_EVENT_HISTORY_NUM 16
static E_IR_RECV_EVENT volatile g_ir_recv_stat_history[D_IR_RECV_EVENT_HISTORY_NUM];
static E_IR_RECV_EVENT volatile g_ir_recv_event_history[D_IR_RECV_EVENT_HISTORY_NUM];
static UINT volatile g_ir_recv_event_history_head = 0;

VOID Ir_Recv_EventHistoryDump( VOID )
{
	UCHAR loopcnt;

	Usart_Set_Stdout();
	printf_P( PSTR("stat=%u err=%u usec=%u count=%u\n"),
			gIr_Recv_Stat,
			gIr_Recv_Err,
			g_ir_timer_usec_last,
			g_ir_recv_event_count
			);

	for( loopcnt = 0; loopcnt < D_IR_RECV_EVENT_HISTORY_NUM; loopcnt++ ) {
		printf_P( PSTR("event: idx=%u event=%u stat=%u\n"),
				loopcnt,
				g_ir_recv_event_history[(g_ir_recv_event_history_head + loopcnt) % D_IR_RECV_EVENT_HISTORY_NUM],
				g_ir_recv_stat_history[(g_ir_recv_event_history_head + loopcnt) % D_IR_RECV_EVENT_HISTORY_NUM]
			);
	}
}

static  VOID ir_event_history_store( const E_IR_RECV_EVENT event )
{
	switch( gIr_Recv_Stat ) {
		case E_IR_RECV_STAT_IDLE:	// fall throuth
		case E_IR_RECV_STAT_END:	// fall throuth
		case E_IR_RECV_STAT_ERR:
			return;
		default:
			break;
	}

	g_ir_recv_event_last = event;
	g_ir_recv_event_count++;

	g_ir_recv_stat_history[g_ir_recv_event_history_head] = gIr_Recv_Stat;
	g_ir_recv_event_history[g_ir_recv_event_history_head] = event;
	g_ir_recv_event_history_head = (g_ir_recv_event_history_head +1) % D_IR_RECV_EVENT_HISTORY_NUM;
}

static  VOID ir_event_history_init( VOID )
{
	g_ir_recv_event_history_head = 0;
	memset( (void*)g_ir_recv_event_history, '\0', sizeof(g_ir_recv_event_history) );
	memset( (void*)g_ir_recv_stat_history, '\0', sizeof(g_ir_recv_stat_history) );
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ
//
static UINT ir_recv_calc_usec( VOID )
{
	return D_TIMER1_PERIOD_USEC * g_timer_count / gTimer1_PwmPeriod;
}

static VOID ir_recv__event_leader_1( VOID )
{
	gIr_Recv_Stat = E_IR_RECV_STAT_LEADER1_MEASURE;
}

static VOID ir_recv__event_leader_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_LEADER1_MEASURE;
		return;
	}

	if( (8800 < usec) && (usec < 9200) ) {	// 8992 usec
		// NECフォーマットのリーダー1
		gIr_Recv_Stat = E_IR_RECV_STAT_LEADER0_NEC_MEASURE;
	}
	else if( (2700 < usec) && (usec < 4000) ) {
		// AEHAフォーマットのリーダー1
		gIr_Recv_Stat = E_IR_RECV_STAT_LEADER0_AEHA_MEASURE;
	}
	else if( (2300 < usec) && (usec < 2500) ) {	// 2400 usec
		// Sonyフォーマットのリーダー1
		gIr_Recv_Stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
		Ir_Frame_Set_Type( E_IR_FRAME_TYPE_SONY );
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_leader_0_nec( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_LEADER0_NEC_MEASURE;
		return;
	}

	if( (4300 < usec) && (usec < 4700) ) {	// 4496 usec
		// NECフォーマットのリーダー0
		gIr_Recv_Stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
		Ir_Frame_Set_Type( E_IR_FRAME_TYPE_NEC );
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_leader_0_aeha( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_LEADER0_AEHA_MEASURE;
		return;
	}

	if( (1400 < usec) && (usec < 2200) ) {	// 1400-2000 usec
		// AEHAフォーマットのリーダー0
		gIr_Recv_Stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
		Ir_Frame_Set_Type( E_IR_FRAME_TYPE_AEHA );
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_sony_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (450 < usec) && (usec < 750) ) {
		// Sonyフォーマットのフレーム開始
		gIr_Recv_Stat = E_IR_RECV_STAT_SONY_FRAME_MEASURE;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_sony_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (450 < usec) && (usec < 750) ) {
		// BIT=0
		if( !Ir_Frame_Store_Bit( 0 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
	}
	else if( (1050 < usec) && (usec < 1350) ) {
		// BIT=1
		if( !Ir_Frame_Store_Bit( 1 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}

static VOID ir_recv__event_nec_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (362 < usec) && (usec < 762) ) {	// 562 usec
		// NECフォーマットのフレーム開始
		gIr_Recv_Stat = E_IR_RECV_STAT_NEC_FRAME_MEASURE;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_nec_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (362 < usec) && (usec < 762) ) {	// 562 usec
		// BIT=0
		if( !Ir_Frame_Store_Bit( 0 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
	}
	else if( (1486 < usec) && (usec < 1886) ) {	// 1686 usec
		// BIT=1
		if( !Ir_Frame_Store_Bit( 1 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}

static VOID ir_recv__event_aeha_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (150 < usec) && (usec < 700) ) {	// 350-500 usec
		// AEHAフォーマットのフレーム開始
		gIr_Recv_Stat = E_IR_RECV_STAT_AEHA_FRAME_MEASURE;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

static VOID ir_recv__event_aeha_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (150 < usec) && (usec < 700) ) {	// 350-500 usec
		// BIT=0
		if( !Ir_Frame_Store_Bit( 0 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
	}
	else if( (850 < usec) && (usec < 1700) ) {	// 1050-1500 usec
		// BIT=1
		if( !Ir_Frame_Store_Bit( 1 ) ) {
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		gIr_Recv_Stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
	}
	else {
		gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
		gIr_Recv_Err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}


// Timer1満了割り込み
static VOID ir_recv_timer1_compa_hdl( VOID )
{
	ir_event_history_store( E_IR_RECV_EVENT_TO );

	switch( gIr_Recv_Stat ) {
		default:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_UNKNOWN_TO;
			break;
		case E_IR_RECV_STAT_ERR:
			break;
		case E_IR_RECV_STAT_END:
			break;
		case E_IR_RECV_STAT_LEADER1_WAIT:
			break;
		case E_IR_RECV_STAT_LEADER1_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_LEADER1_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_LEADER0_NEC_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_LEADER0_NEC_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_LEADER0_AEHA_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_LEADER0_AEHA_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_SONY_FRAME_WAIT:
			gIr_Recv_Stat = E_IR_RECV_STAT_END;
			break;
		case E_IR_RECV_STAT_SONY_FRAME_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_SONY_FRAME_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_NEC_FRAME_WAIT:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_NEC_FRAME_WAIT_TO;
			break;
		case E_IR_RECV_STAT_NEC_FRAME_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_END;
			break;
		case E_IR_RECV_STAT_AEHA_FRAME_WAIT:
			gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
			gIr_Recv_Err = E_IR_RECV_ERR_AEHA_FRAME_WAIT_TO;
			break;
		case E_IR_RECV_STAT_AEHA_FRAME_MEASURE:
			gIr_Recv_Stat = E_IR_RECV_STAT_END;
			break;
	}
}

static UCHAR volatile g_ir_recv_pin_last = 0;

// PCINT8割り込み
VOID Ir_Recv_Pcint8Hdl( VOID )
{
	const UCHAR ir_recv_pin = D_IR_RECV_PIN;

	if( ir_recv_pin == g_ir_recv_pin_last ) {
		return;
	}
	g_ir_recv_pin_last = ir_recv_pin;
	// PCINT8発生

	g_timer_count = TCNT1;
	Timer1_Restart();

		// PCINT8 event
#ifdef CO_PCINT_MONITOR_PC3
	PORTC ^= _BV(PC3);
#endif
	// IRは負論理
	if( ir_recv_pin == 0 ) {
		// 0->1 (Hi->Lo)
		ir_event_history_store( E_IR_RECV_EVENT_CHG_1 );
		switch( gIr_Recv_Stat ) {
			default:
				gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
				gIr_Recv_Err = E_IR_RECV_ERR_ILLEGAL_STAT;
				break;
			case E_IR_RECV_STAT_LEADER1_WAIT:
				ir_recv__event_leader_1();
				break;
			case E_IR_RECV_STAT_LEADER0_NEC_MEASURE:
				ir_recv__event_leader_0_nec();
				break;
			case E_IR_RECV_STAT_LEADER0_AEHA_MEASURE:
				ir_recv__event_leader_0_aeha();
				break;
			case E_IR_RECV_STAT_SONY_FRAME_WAIT:
				ir_recv__event_sony_frame_data_1();
				break;
			case E_IR_RECV_STAT_NEC_FRAME_MEASURE:
				ir_recv__event_nec_frame_data_1();
				break;
			case E_IR_RECV_STAT_AEHA_FRAME_MEASURE:
				ir_recv__event_aeha_frame_data_1();
				break;
			case E_IR_RECV_STAT_IDLE:	// fall through
			case E_IR_RECV_STAT_END:	// fall through
			case E_IR_RECV_STAT_ERR:
				break;
		}
	}
	else {
		// 1->0 (Lo->Hi)
		ir_event_history_store( E_IR_RECV_EVENT_CHG_0 );
		switch( gIr_Recv_Stat ) {
			default:
				gIr_Recv_Stat = E_IR_RECV_STAT_ERR;
				gIr_Recv_Err = E_IR_RECV_ERR_ILLEGAL_STAT;
				break;
			case E_IR_RECV_STAT_LEADER1_MEASURE:
				ir_recv__event_leader_0();
				break;
			case E_IR_RECV_STAT_SONY_FRAME_MEASURE:
				ir_recv__event_sony_frame_data_0();
				break;
			case E_IR_RECV_STAT_NEC_FRAME_WAIT:
				ir_recv__event_nec_frame_data_0();
				break;
			case E_IR_RECV_STAT_AEHA_FRAME_WAIT:
				ir_recv__event_aeha_frame_data_0();
				break;
			case E_IR_RECV_STAT_IDLE:	// fall through
			case E_IR_RECV_STAT_END:	// fall through
			case E_IR_RECV_STAT_ERR:
				break;
		}
	}
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスクAPI
VOID Ir_Recv_Stop( VOID )
{
#ifdef CO_LED_CTRL
	TEST_LED2_OFF();
#endif

	// PortC0(PCINT8) disable
	cbi( PCMSK1, PCINT8 );
	Timer1_Stop();
}

VOID Ir_Recv_Start( VOID )
{
#ifdef CO_LED_CTRL
	TEST_LED2_ON();
#endif

	Timer1_Stop();

	Timer1_Close();	


	ir_event_history_init();
	Ir_Frame_Init();
	g_ir_recv_event_count = 0;
	g_ir_timer_usec_last = 0;

	g_ir_recv_pin_last = D_IR_RECV_PIN;

	gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;

	// PortC0(PCINT8) enable
	sbi( PCMSK1, PCINT8 );

	// すでに立っているPCINT8を空処理(割り込みハンドラ動作を期待)


	// Timer1初期化＆開始
	Timer1_Init( D_TIMER1_PERIOD_USEC );
	Timer1_Enable_CompaIntr( ir_recv_timer1_compa_hdl );

	Timer1_Start();

	gIr_Recv_Stat = E_IR_RECV_STAT_LEADER1_WAIT;
}

