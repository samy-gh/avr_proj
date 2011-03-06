
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "mystdout.h"
#include "usart.h"
#include "lcd_hd44780.h"
#include "timer1.h"
#include "test_led.h"
#include "test_sw.h"


// メインループの動作を見る
//#define CO_MAINLOOP_MONITOR

// PC3ピンでIR受信状況をモニタする
//#define CO_PCINT_MONITOR_PC3

// IR波形計測用タイマの最大時間
#define D_TIMER1_PERIOD_USEC 10000UL

// IR受信ピンの状態取得用
#define D_IR_RECV_PIN	(PINC & _BV(PINC0))


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// SONY形式フレームデータハンドリング

// SONY形式フレームデータ: T_IR_SONY_FRAME
typedef struct {
	UCHAR data;		// 6bit
	UINT  addr;		// 4/7/11bit
} T_IR_SONY_FRAME;

T_IR_SONY_FRAME volatile g_sony_frame;


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// フレームデータハンドリング

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

T_IR_FRAME volatile g_ir_frame;

VOID ir_frame_init( VOID )
{
	g_ir_frame.byte_idx = 0;
	g_ir_frame.bit_mask = 1;
	memset( (void*)g_ir_frame.data, '\0', sizeof(g_ir_frame.data) );
}

VOID ir_frame_set_type( E_IR_FRAME_TYPE type )
{
	g_ir_frame.type = type;
}

BOOL ir_frame_store_bit( UCHAR bit )
{
	if( bit ) {
		g_ir_frame.data[g_ir_frame.byte_idx] |= g_ir_frame.bit_mask;
	}

	if( g_ir_frame.bit_mask == 0x80 ) {
		g_ir_frame.byte_idx++;
		g_ir_frame.bit_mask = 1;
	}
	else {
		g_ir_frame.bit_mask <<= 1;
	}

	if( g_ir_frame.byte_idx >= sizeof(g_ir_frame.data) ) {
		return FALSE;
	}

	return TRUE;
}

VOID ir_frame_dump( VOID )
{
	UCHAR loopcnt;
	UCHAR byte_idx;

	byte_idx = g_ir_frame.byte_idx;
	if( g_ir_frame.bit_mask != 0x01 ) {
		byte_idx++;
	}

	Usart_Set_Stdout();
	printf_P( PSTR("type=%u byte=%u bit=%x:"), g_ir_frame.type, g_ir_frame.byte_idx, g_ir_frame.bit_mask );
	for( loopcnt = 0; loopcnt < byte_idx; loopcnt++ ) {
		printf_P( PSTR(" %02X"), g_ir_frame.data[loopcnt] );
	}
	putchar( '\n' );
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// ステータスコード
//
typedef enum {
	E_IR_RECV_STAT_IDLE = 0,
	E_IR_RECV_STAT_LEADER1_WAIT,
	E_IR_RECV_STAT_LEADER1_MEASURE,
	E_IR_RECV_STAT_LEADER0_NEC_MEASURE,
	E_IR_RECV_STAT_LEADER0_AEHA_MEASURE,
	E_IR_RECV_STAT_SONY_FRAME_WAIT,
	E_IR_RECV_STAT_SONY_FRAME_MEASURE,
	E_IR_RECV_STAT_NEC_FRAME_WAIT,
	E_IR_RECV_STAT_NEC_FRAME_MEASURE,
	E_IR_RECV_STAT_AEHA_FRAME_WAIT,
	E_IR_RECV_STAT_AEHA_FRAME_MEASURE,
	E_IR_RECV_STAT_END,
	E_IR_RECV_STAT_ERR,
	E_IR_RECV_STAT_MAX
} E_IR_RECV_STAT;

E_IR_RECV_STAT volatile g_ir_recv_stat = E_IR_RECV_STAT_IDLE;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// エラーコード
//
typedef enum {
	E_IR_RECV_ERR_NONE = 0,
	E_IR_RECV_ERR_LEADER1_WAIT,
	E_IR_RECV_ERR_LEADER1_MEASURE,
	E_IR_RECV_ERR_LEADER1_MEASURE_TO,
	E_IR_RECV_ERR_LEADER0_NEC_MEASURE,
	E_IR_RECV_ERR_LEADER0_NEC_MEASURE_TO,
	E_IR_RECV_ERR_LEADER0_AEHA_MEASURE,
	E_IR_RECV_ERR_LEADER0_AEHA_MEASURE_TO,
	E_IR_RECV_ERR_UNKNOWN_LEADER,
	E_IR_RECV_ERR_FRAME_DATA_1_NULL,
	E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH,
	E_IR_RECV_ERR_FRAME_DATA_0_NULL,
	E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH,
	E_IR_RECV_ERR_BUF_OVERFLOW,
	E_IR_RECV_ERR_ILLEGAL_STAT,
	E_IR_RECV_ERR_UNKNOWN_TO,
	E_IR_RECV_ERR_SONY_FRAME,
	E_IR_RECV_ERR_SONY_FRAME_MEASURE_TO,
	E_IR_RECV_ERR_NEC_FRAME_WAIT_TO,
	E_IR_RECV_ERR_AEHA_FRAME_WAIT_TO,
	E_IR_RECV_ERR_MAX
} E_IR_RECV_ERR;

E_IR_RECV_ERR volatile g_ir_recv_err = E_IR_RECV_ERR_NONE;

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
E_IR_RECV_EVENT volatile g_ir_recv_stat_history[D_IR_RECV_EVENT_HISTORY_NUM];
E_IR_RECV_EVENT volatile g_ir_recv_event_history[D_IR_RECV_EVENT_HISTORY_NUM];
UINT volatile g_ir_recv_event_history_head = 0;

VOID ir_event_history_dump( VOID )
{
	UCHAR loopcnt;

	Usart_Set_Stdout();
	printf_P( PSTR("stat=%u err=%u usec=%u count=%u\n"),
			g_ir_recv_stat,
			g_ir_recv_err,
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

VOID ir_event_history_store( const E_IR_RECV_EVENT event )
{
	switch( g_ir_recv_stat ) {
		case E_IR_RECV_STAT_IDLE:	// fall throuth
		case E_IR_RECV_STAT_END:	// fall throuth
		case E_IR_RECV_STAT_ERR:
			return;
		default:
			break;
	}

	g_ir_recv_event_last = event;
	g_ir_recv_event_count++;

	g_ir_recv_stat_history[g_ir_recv_event_history_head] = g_ir_recv_stat;
	g_ir_recv_event_history[g_ir_recv_event_history_head] = event;
	g_ir_recv_event_history_head = (g_ir_recv_event_history_head +1) % D_IR_RECV_EVENT_HISTORY_NUM;
}

VOID ir_event_history_init( VOID )
{
	g_ir_recv_event_history_head = 0;
	memset( (void*)g_ir_recv_event_history, '\0', sizeof(g_ir_recv_event_history) );
	memset( (void*)g_ir_recv_stat_history, '\0', sizeof(g_ir_recv_stat_history) );
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// イベントハンドラ
//
UINT ir_recv_calc_usec( VOID )
{
	return D_TIMER1_PERIOD_USEC * g_timer_count / gTimer1_PwmPeriod;
}

VOID ir_recv__event_leader_1( VOID )
{
	g_ir_recv_stat = E_IR_RECV_STAT_LEADER1_MEASURE;
}

VOID ir_recv__event_leader_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_LEADER1_MEASURE;
		return;
	}

	if( (8800 < usec) && (usec < 9200) ) {	// 8992 usec
		// NECフォーマットのリーダー1
		g_ir_recv_stat = E_IR_RECV_STAT_LEADER0_NEC_MEASURE;
	}
	else if( (2700 < usec) && (usec < 4000) ) {
		// AEHAフォーマットのリーダー1
		g_ir_recv_stat = E_IR_RECV_STAT_LEADER0_AEHA_MEASURE;
	}
	else if( (2300 < usec) && (usec < 2500) ) {	// 2400 usec
		// Sonyフォーマットのリーダー1
		g_ir_recv_stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
		ir_frame_set_type( E_IR_FRAME_TYPE_SONY );
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_leader_0_nec( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_LEADER0_NEC_MEASURE;
		return;
	}

	if( (4300 < usec) && (usec < 4700) ) {	// 4496 usec
		// NECフォーマットのリーダー0
		g_ir_recv_stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
		ir_frame_set_type( E_IR_FRAME_TYPE_NEC );
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_leader_0_aeha( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_LEADER0_AEHA_MEASURE;
		return;
	}

	if( (1400 < usec) && (usec < 2200) ) {	// 1400-2000 usec
		// AEHAフォーマットのリーダー0
		g_ir_recv_stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
		ir_frame_set_type( E_IR_FRAME_TYPE_AEHA );
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_UNKNOWN_LEADER;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_sony_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (450 < usec) && (usec < 750) ) {
		// Sonyフォーマットのフレーム開始
		g_ir_recv_stat = E_IR_RECV_STAT_SONY_FRAME_MEASURE;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_sony_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (450 < usec) && (usec < 750) ) {
		// BIT=0
		if( !ir_frame_store_bit( 0 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
	}
	else if( (1050 < usec) && (usec < 1350) ) {
		// BIT=1
		if( !ir_frame_store_bit( 1 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_SONY_FRAME_WAIT;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}

VOID ir_recv__event_nec_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (462 < usec) && (usec < 662) ) {	// 562 usec
		// NECフォーマットのフレーム開始
		g_ir_recv_stat = E_IR_RECV_STAT_NEC_FRAME_MEASURE;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_nec_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (362 < usec) && (usec < 762) ) {	// 562 usec
		// BIT=0
		if( !ir_frame_store_bit( 0 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
	}
	else if( (1486 < usec) && (usec < 1886) ) {	// 1686 usec
		// BIT=1
		if( !ir_frame_store_bit( 1 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_NEC_FRAME_WAIT;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}

VOID ir_recv__event_aeha_frame_data_0( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_NULL;
		g_ir_timer_usec_last = usec;
		return;
	}

	if( (150 < usec) && (usec < 700) ) {	// 350-500 usec
		// AEHAフォーマットのフレーム開始
		g_ir_recv_stat = E_IR_RECV_STAT_AEHA_FRAME_MEASURE;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_1_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
	}
}

VOID ir_recv__event_aeha_frame_data_1( VOID )
{
	UINT usec = ir_recv_calc_usec();

	if( usec == 0 ) {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_NULL;
		return;
	}

	if( (150 < usec) && (usec < 700) ) {	// 350-500 usec
		// BIT=0
		if( !ir_frame_store_bit( 0 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
	}
	else if( (850 < usec) && (usec < 1700) ) {	// 1050-1500 usec
		// BIT=1
		if( !ir_frame_store_bit( 1 ) ) {
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_BUF_OVERFLOW;
			return;
		}
		g_ir_recv_stat = E_IR_RECV_STAT_AEHA_FRAME_WAIT;
	}
	else {
		g_ir_recv_stat = E_IR_RECV_STAT_ERR;
		g_ir_recv_err = E_IR_RECV_ERR_FRAME_DATA_0_UNKNOWN_LENGTH;
		g_ir_timer_usec_last = usec;
		return;
	}
}


// Timer1満了割り込み
VOID timer1_compa_hdl( VOID )
{
	ir_event_history_store( E_IR_RECV_EVENT_TO );

	switch( g_ir_recv_stat ) {
		default:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_UNKNOWN_TO;
			break;
		case E_IR_RECV_STAT_ERR:
			break;
		case E_IR_RECV_STAT_END:
			break;
		case E_IR_RECV_STAT_LEADER1_WAIT:
			break;
		case E_IR_RECV_STAT_LEADER1_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_LEADER1_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_LEADER0_NEC_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_LEADER0_NEC_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_LEADER0_AEHA_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_LEADER0_AEHA_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_SONY_FRAME_WAIT:
			g_ir_recv_stat = E_IR_RECV_STAT_END;
			break;
		case E_IR_RECV_STAT_SONY_FRAME_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_SONY_FRAME_MEASURE_TO;
			break;
		case E_IR_RECV_STAT_NEC_FRAME_WAIT:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_NEC_FRAME_WAIT_TO;
			break;
		case E_IR_RECV_STAT_NEC_FRAME_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_END;
			break;
		case E_IR_RECV_STAT_AEHA_FRAME_WAIT:
			g_ir_recv_stat = E_IR_RECV_STAT_ERR;
			g_ir_recv_err = E_IR_RECV_ERR_AEHA_FRAME_WAIT_TO;
			break;
		case E_IR_RECV_STAT_AEHA_FRAME_MEASURE:
			g_ir_recv_stat = E_IR_RECV_STAT_END;
			break;
	}

	sleep_disable();
}

UCHAR volatile g_ir_recv_pin_last = 0;

// PCINT8割り込み
VOID pcint8_hdl( VOID )
{
	if( (PCMSK1 & _BV(PCINT8)) == 0 ) {
		return;
	}

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
		switch( g_ir_recv_stat ) {
			default:
				g_ir_recv_stat = E_IR_RECV_STAT_ERR;
				g_ir_recv_err = E_IR_RECV_ERR_ILLEGAL_STAT;
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
		switch( g_ir_recv_stat ) {
			default:
				g_ir_recv_stat = E_IR_RECV_STAT_ERR;
				g_ir_recv_err = E_IR_RECV_ERR_ILLEGAL_STAT;
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
VOID ir_recv_stop( VOID )
{
	TEST_LED2_OFF();

	// PortC0(PCINT8) disable
	cbi( PCMSK1, PCINT8 );
	Timer1_Stop();
}

VOID ir_recv_start( VOID )
{
	TEST_LED2_ON();

	Timer1_Stop();

	Timer1_Close();	


	ir_frame_init();
	g_ir_recv_event_count = 0;
	g_ir_timer_usec_last = 0;

	g_ir_recv_pin_last = D_IR_RECV_PIN;

	g_ir_recv_stat = E_IR_RECV_STAT_IDLE;

	// PortC0(PCINT8) enable
	sbi( PCMSK1, PCINT8 );

	// すでに立っているPCINT8を空処理(割り込みハンドラ動作を期待)


	// Timer1初期化＆開始
	Timer1_Init( D_TIMER1_PERIOD_USEC );
	Timer1_Enable_CompaIntr( timer1_compa_hdl );

	Timer1_Start();

	g_ir_recv_stat = E_IR_RECV_STAT_LEADER1_WAIT;
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// 割り込みハンドラ


// Pin Change Interrupt処理
SIGNAL(PCINT1_vect)
{
	pcint8_hdl();

	Test_Sw_Sw1Pcint1Hdl();
	Test_Sw_Sw2Pcint1Hdl();

	sleep_disable();
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスク

INT usart_poll( VOID )
{
	INT c;

	c = Usart_Read();
	if( c > 0 ) {
		Usart_Write( c );
	}

	return c;
}

VOID print_banner( VOID )
{
	//
	// バナー表示
	//
	prog_char* line_feed_str = PSTR("\n\n");
	prog_char* hello_str = PSTR("IR Remo-con");
	Usart_Set_Stdout();
	printf_P( line_feed_str );
	printf_P( hello_str );
	printf_P( line_feed_str );
	Lcd_Set_Stdout();
	printf_P( hello_str );
}

VOID setup( VOID )
{
	// WDT停止
	WDTCSR = 0x00;


	// パワーセーブ
	set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
//	set_sleep_mode( SLEEP_MODE_STANDBY );
//	set_sleep_mode( SLEEP_MODE_PWR_DOWN );
	PRR = 0xFF;


#ifdef CO_PCINT_MONITOR_PC3
	// PC3=out
	sbi( DDRC, DDC3 );
#endif


	TEST_SW1_ENABLE();
	TEST_SW2_ENABLE();


	TEST_LED1_ON();


	// PCINTn Enable
	sbi( PCICR, PCIE1 );


	// USART初期化
	Usart_Init( 38400 );


	// LCD初期化
	Lcd_Init();


	// 割り込み許可
	sei();
}

VOID main( VOID ) __attribute__((noreturn));
VOID main( VOID )
{
#ifdef CO_MAINLOOP_MONITOR
	ULONG loopcnt = 0;
#endif
	INT usart_recv_c;

	setup();
	print_banner();

#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("timer1 clock sel=%X\ntimer1 period=%u\n"), gTimer1_ClockSelectBits, gTimer1_PwmPeriod );
#endif


	while( 1 ) {	/* main event loop */
		// すべての割り込みハンドラ内でsleep_disable()を実施している。
		// そうすることで、ループ先頭のsleep_enable()～ループ終端のsleep()までの間に割り込みが
		// 発生してもsleep()しなくなる。
		// sleep()するのは、sleep_enable()～sleep()までの間に割り込みが一度も発生しない場合のみとなる。
		cli();
		sleep_enable();
		sei();


		// シリアル受信イベント
		usart_recv_c = usart_poll();
		if( usart_recv_c > 0 ) {
			switch( g_ir_recv_stat ) {
				default:
					ir_event_history_dump();
					break;
				case E_IR_RECV_STAT_IDLE:
					Usart_Set_Stdout();
					printf_P( PSTR("recv begin\n") );
					putchar( '\n' );
					ir_recv_start();
					break;
			}
		}

		if( Test_Sw_Is_Sw1Chg() == E_TEST_SW_EVENT_ON ) {
			switch( g_ir_recv_stat ) {
				default:
					ir_event_history_dump();
					break;
				case E_IR_RECV_STAT_IDLE:
					Usart_Set_Stdout();
					printf_P( PSTR("recv begin\n") );
					putchar( '\n' );
					ir_recv_start();
					break;
			}
		}


		// IR受信イベント(完了/エラー)
		switch( g_ir_recv_stat ) {
			default:
				break;
			case E_IR_RECV_STAT_END:
				ir_recv_stop();
				Usart_Set_Stdout();
				printf_P( PSTR("recv success\n") );
				ir_frame_dump();
				g_ir_recv_stat = E_IR_RECV_STAT_IDLE;
				ir_event_history_dump();

				break;
			case E_IR_RECV_STAT_ERR:
				ir_recv_stop();
				Usart_Set_Stdout();
				printf_P( PSTR("recv error code=%u usec=%u\n"), g_ir_recv_err, g_ir_timer_usec_last );
				ir_event_history_dump();
				ir_frame_dump();
				g_ir_recv_stat = E_IR_RECV_STAT_IDLE;
				g_ir_recv_err = E_IR_RECV_ERR_NONE;
				break;
		}


		// メインループ受信モニタ用
#ifdef CO_MAINLOOP_MONITOR
		Lcd_Set_Stdout();
		Lcd_Goto( 0, 1 );
		printf_P( PSTR("%5u"), loopcnt );
		loopcnt++;
#endif

		sleep_cpu();
	}
}

