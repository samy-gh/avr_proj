
// framework
#include "framework_event_loop.h"
#include "framework_clock_hdl.h"
#include "framework_btn.h"
#include "framework_softtimer.h"
#include "framework_i2c.h"

// samylib
#include "my_typedef.h"
#include "usart.h"
#include "ir.h"

// WinAVR
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// STD-C
#include <stdio.h>


/****************************************************************************/
/* 定数宣言																	*/
/****************************************************************************/


/****************************************************************************/
/* マクロ宣言																*/
/****************************************************************************/


/****************************************************************************/
/* 割り込み動作制御変数														*/
/****************************************************************************/


/****************************************************************************/
/* テーブル宣言																*/
/****************************************************************************/
/* 状態エンジン ステータス */
volatile T_SYS_STAT _gt_sys_stat = E_STAT_SYS_IDLE;

typedef struct {
	T_SYS_EVENT t_longpush;
	T_SYS_EVENT t_release;
	T_SYS_EVENT t_push;
} T_BTN_EVENT_ID_TBL;



/****************************************************************************/
/* 追加したもの																*/
/****************************************************************************/
int (*fn_serial_cmd_hdl)( char c );

static __inline__ T_SYS_EVENT get_btn_event( T_BTN_TYPE btn_type, const T_BTN_EVENT_ID_TBL* pt_event_id_tbl );


/*----------------------------------------------*/
/* シリアルコマンド判定							*/
/*----------------------------------------------*/
#ifdef CO_ENABLE_SERIAL_CMD
__inline__ static T_SYS_EVENT get_serial_command(VOID)
{
	INT c;
	T_SERIAL_CMD* pt_cmd_tbl;
	T_SYS_EVENT t_sys_event = E_EVENT_IDLE;

	c = Usart_Read();   /* 1文字受信 */
	if( c == -1 ) {
		return t_sys_event;
	}

	Usart_Write( c );   /* ホストへエコーバック */
	Usart_PgmPuts( PSTR("\r\n") );

	for( pt_cmd_tbl = gt_serial_event_tbl; pt_cmd_tbl->c != '\0'; pt_cmd_tbl++ ) {
		if( pt_cmd_tbl->c == c ) {
			t_sys_event = pt_cmd_tbl->t_sys_event;
			return t_sys_event;
		}
	}

	if( (fn_serial_cmd_hdl == NULL)
	 || (fn_serial_cmd_hdl( c ) != 0)
	 ) {
		Usart_PgmPuts( PSTR("command not found\n") );
	}

	return t_sys_event;
}
#endif //CO_ENABLE_SERIAL_CMD



/*----------------------------------------------*/
/* ボタンイベント判定							*/
/*----------------------------------------------*/
static __inline__ T_SYS_EVENT get_btn_event( T_BTN_TYPE btn_type, const T_BTN_EVENT_ID_TBL* pt_event_id_tbl )
{
	if( _gui_event_btn_release[btn_type] != 0 ) {
		/* リリースイベントが長押しイベントに対応するものでなければイベント発生 */
		if( _gui_event_btn_release[btn_type] != _gui_event_btn_longpush[btn_type] ) {
			_gui_event_btn_release[btn_type] = 0;
			return pt_event_id_tbl->t_release;
		}
		/* リリースイベントが長押しイベントに対応するものならば長押しイベントを先に発生 */
	}
	if( _gui_event_btn_longpush[btn_type] != 0 ) {
		_gui_event_btn_longpush[btn_type] = 0;
		return pt_event_id_tbl->t_longpush;
	}
	if( _gui_event_btn_push[btn_type] != 0 ) {
		_gui_event_btn_push[btn_type] = 0;
		return pt_event_id_tbl->t_push;
	}
	return E_EVENT_IDLE;
}



/*----------------------------------------------*/
/* イベント判定									*/
/*----------------------------------------------*/
__inline__ static T_SYS_EVENT get_event( VOID )
{
#ifdef CO_ENABLE_SERIAL_CMD
	{
		T_SYS_EVENT event;
		event = get_serial_command();
		if( event != E_EVENT_IDLE ) {
			return event;
		}
	}
#endif //CO_ENABLE_SERIAL_CMD

#ifdef CO_ENABLE_I2C_SLAVE_READ
	if( _gui_event_i2c_slave_read != 0 ) {
		_gui_event_i2c_slave_read = 0;
		return E_EVENT_I2C_SLAVE_READ;
	}
#endif
#ifdef CO_ENABLE_I2C_SLAVE_WRITE
	if( _gui_event_i2c_slave_write != 0 ) {
		_gui_event_i2c_slave_write = 0;
		return E_EVENT_I2C_SLAVE_WRITE;
	}
#endif

#ifdef CO_ENABLE_SOFTTIMER
	{
		T_TIMER_TYPE t_timer_type;

		/* タイムアウトイベント */
		for( t_timer_type = (T_TIMER_TYPE)0; t_timer_type < gui_timer_param_num; t_timer_type++ ) {
			if( gt_event_timer[t_timer_type] != 0 ) {
				gt_event_timer[t_timer_type] = 0;
				return gt_timer_param[t_timer_type].t_timeout_event;
			}
		}
	}
#endif //CO_ENABLE_SOFTTIMER

#if 0 // TODO:移植未
	/* AD0取得ASYNC完了 */
	if( _gui_event_adc_end[0] != 0 ) {
		_gui_event_adc_end[0] = 0;
		return E_EVENT_AD0_END;
	}

	/* AD1取得ASYNC完了 */
	if( _gui_event_adc_end[1] != 0 ) {
		_gui_event_adc_end[1] = 0;
		return E_EVENT_AD1_END;
	}
#endif

	/* クロック割り込み発生 */
	if( _gui_event_clock != 0 ) {
		_gui_event_clock = 0;
		/* 無視 - クロック割り込みをイベント通知していると他のイベントをこぼす */
//		return E_EVENT_INT_CLOCK;
	}


#ifdef CO_ENABLE_PCINT0_BTN
	{
		T_SYS_EVENT t_btn_event;
		static T_BTN_EVENT_ID_TBL const t_event_id_tbl = {
			E_EVENT_BTN0_LONGPUSH, E_EVENT_BTN0_RELEASE, E_EVENT_BTN0_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT0, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT1_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN1_LONGPUSH, E_EVENT_BTN1_RELEASE, E_EVENT_BTN1_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT1, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT2_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN2_LONGPUSH, E_EVENT_BTN2_RELEASE, E_EVENT_BTN2_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT2, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT3_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN3_LONGPUSH, E_EVENT_BTN3_RELEASE, E_EVENT_BTN3_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT3, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT4_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN4_LONGPUSH, E_EVENT_BTN4_RELEASE, E_EVENT_BTN4_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT4, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT5_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN5_LONGPUSH, E_EVENT_BTN5_RELEASE, E_EVENT_BTN5_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT5, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT10_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN10_LONGPUSH, E_EVENT_BTN10_RELEASE, E_EVENT_BTN10_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT10, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_PCINT11_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN11_LONGPUSH, E_EVENT_BTN11_RELEASE, E_EVENT_BTN11_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_PCINT11, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_IRQ0_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN6_LONGPUSH, E_EVENT_BTN6_RELEASE, E_EVENT_BTN6_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_IRQ0, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_IRQ1_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN7_LONGPUSH, E_EVENT_BTN7_RELEASE, E_EVENT_BTN7_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_IRQ1, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_IRQ2_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN8_LONGPUSH, E_EVENT_BTN8_RELEASE, E_EVENT_BTN8_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_IRQ2, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_IRQ3_BTN
	{
		T_SYS_EVENT t_btn_event;
		const static T_BTN_EVENT_ID_TBL t_event_id_tbl = {
			E_EVENT_BTN9_LONGPUSH, E_EVENT_BTN9_RELEASE, E_EVENT_BTN9_PUSH
		};

		t_btn_event = get_btn_event( E_BTN_IRQ3, &t_event_id_tbl );
		if( t_btn_event != E_EVENT_IDLE ) {
			return t_btn_event;
		}
	}
#endif
#ifdef CO_ENABLE_IR_RECV_EVENT
	if( _gui_event_ir_recv_end != 0 ) {
		_gui_event_ir_recv_end = 0;
		return E_EVENT_IR_RECV_END;
	}
	if( _gui_event_ir_recv_err != 0 ) {
		_gui_event_ir_recv_err = 0;
		return E_EVENT_IR_RECV_ERR;
	}
#endif
#ifdef CO_ENABLE_IR_SEND_EVENT
	if( _gui_event_ir_send_end != 0 ) {
		_gui_event_ir_send_end = 0;
		return E_EVENT_IR_SEND_END;
	}
#endif

	return E_EVENT_IDLE;
}


/*----------------------------------------------*/
/* メインスレッドループ							*/
/*----------------------------------------------*/
VOID event_loop( VOID )
{
	T_SYS_EVENT t_sys_event = E_EVENT_IDLE;
	T_SYS_EVENT_TBL *pt_sys_event_tbl;
	UINT ui_loopnum;

#ifdef CO_ENABLE_EVENT_TRACE_LOG
	Usart_Set_Stderr();
#endif

	while( 1 ) {
//#ifdef CO_ENABLE_EVENT_TRACE_LOG
//	fprintf_P( stderr, PSTR( "wait next event\n") );
//#endif

#ifdef CO_ENABLE_I2C_SLAVE_READ
	if( t_sys_event == E_EVENT_I2C_SLAVE_READ ) {
		i2c_slave_read_event_finish();
	}
#endif

#ifdef CO_SLEEP_ENABLE
		sleep_cpu();
#endif

		t_sys_event = get_event();

//#ifdef CO_ENABLE_EVENT_TRACE_LOG
//		fprintf_P( stderr, PSTR( "recv event=%u, stat=%u\n"), t_sys_event, _gt_sys_stat );
//#endif

#ifdef CO_SLEEP_ENABLE
		// すべての割り込みハンドラ内でsleep_disable()を実施している。
		// そうすることで、ループ先頭のsleep_enable()～ループ終端のsleep()までの間に割り込みが
		// 発生してもsleep()しなくなる。
		// sleep()するのは、sleep_enable()～sleep()までの間に割り込みが一度も発生しない場合のみとなる。
		cli();
		sleep_enable();
		sei();
#endif

#ifdef CO_ENABLE_EVENT_TRACE_LOG
		if( t_sys_event != E_EVENT_IDLE ) {
			fprintf_P( stderr, PSTR( "begin event=%u, stat=%u\n"), t_sys_event, _gt_sys_stat );
		}
#endif

		/* TODO:ステータスチェンジはgo_sys_stat()でやれば良い */
		pt_sys_event_tbl = NULL;
		for( ui_loopnum = 0; gt_sys_stat_tbl[ui_loopnum].pt_sys_event_tbl != NULL; ui_loopnum++ ) {
			if( gt_sys_stat_tbl[ui_loopnum].t_sys_stat == _gt_sys_stat ) {
				pt_sys_event_tbl = gt_sys_stat_tbl[ui_loopnum].pt_sys_event_tbl;
				break;
			}
		}
		if( pt_sys_event_tbl == NULL ) {
#ifdef CO_ENABLE_EVENT_TRACE_LOG
			fprintf_P( stderr, PSTR("unknown sys stat event=%u, stat=%u\n"), t_sys_event, _gt_sys_stat );
#endif
			continue;
		}


		for( ui_loopnum = 0; pt_sys_event_tbl[ui_loopnum].fn_hdl != NULL; ui_loopnum++ ) {
			if( pt_sys_event_tbl[ui_loopnum].t_sys_event == t_sys_event ) {
#ifdef CO_ENABLE_EVENT_TRACE_LOG
				int i_ret = pt_sys_event_tbl[ui_loopnum].fn_hdl();
				if( i_ret != 0 ) {
					fprintf_P( stderr, PSTR("event handler error returned, ret=%d\n"), i_ret );
				}
#else
				(VOID)pt_sys_event_tbl[ui_loopnum].fn_hdl();
#endif
				break;
			}
		}
		if( pt_sys_event_tbl[ui_loopnum].fn_hdl == NULL ) {
#ifdef CO_ENABLE_EVENT_TRACE_LOG
			fprintf_P( stderr, PSTR("unknown sys event event=%u, stat=%u\n"), t_sys_event, _gt_sys_stat );
#endif
			continue;
		}


#ifdef CO_ENABLE_EVENT_TRACE_LOG
		if( t_sys_event != E_EVENT_IDLE ) {
			fprintf_P( stderr, PSTR("return event loop stat=%u\n\n"), _gt_sys_stat );
		}
#endif
	}
}

