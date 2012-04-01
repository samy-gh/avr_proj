
// プロジェクト固有
#include "framework_hal.h"

// samylib
#include <mystdout.h>
#include <usart.h>
#include <lcd_hd44780.h>
#include <test_led.h>
#include <framework_event_loop.h>
#include <framework_btn.h>
#include <framework_clock_hdl.h>
#include <framework_softtimer.h>

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

// STD-C
#include <stdio.h>


/*----------------------------------------------*/
/* ステータス・イベント定義						*/
/*----------------------------------------------*/
/* イベントハンドラ */
static INT do_nop( VOID );
static INT do_serial_dump( VOID );
static INT do_timer( VOID );
static INT do_timer_stop( VOID );

/* システムステータス種類 */
typedef enum {
	E_STAT_IDLE = E_STAT_SYS_IDLE,
	E_STAT_ADJ_HOUR,
	E_STAT_ADJ_MIN,
	E_STAT_ADJ_SEC,
	E_STAT_MAX
} TE_SYS_STAT;

/* ユーザイベント種類 */
typedef enum  {
	/* ユーザイベントなし */
	E_EVENT_SW_PRESS = E_EVENT_USER,		/* 128:スイッチ押下 */
	E_EVENT_SW_LONGPRESS,					/* 129:スイッチ長押し */
	E_EVENT_TO_TICK,						/* 130:1秒タイマ T.O. */
	E_EVENT_TO_SET_TIME,					/* 131:時刻調整タイマ T.O. */
	E_EVENT_TO_TIMER3,						/* 132:タイマ3 T.O. */
	E_EVENT_OUTPUT_CONSOLE,					/* 133:コンソール出力要求 */
	E_EVENT_SW_ADJ_PUSH,					/* 134:時刻合わせボタン押下 */
	E_EVENT_SERIAL_DUMP,					/* 135:シリアルからのダンプコマンド */
	E_EVENT_MAX
} TE_SYS_EVENT;

/* イベント-ハンドラ テーブル */
T_SYS_EVENT_TBL gt_sys_event_tbl_idle[] = {
	{ E_EVENT_TO_TICK,				do_nop },
	{ E_EVENT_BTN0_PUSH,			do_nop },
	{ E_EVENT_BTN0_LONGPUSH,		do_nop },
	{ E_EVENT_BTN0_RELEASE,			do_nop },
	{ E_EVENT_BTN10_PUSH,			do_nop },
	{ E_EVENT_BTN10_LONGPUSH,		do_nop },
	{ E_EVENT_BTN10_RELEASE,		do_nop },
	{ E_EVENT_TO_SET_TIME,			do_nop },
	{ E_EVENT_SW_ADJ_PUSH,			do_timer_stop },
	{ E_EVENT_BEEP_END,				do_nop },
	{ E_EVENT_IDLE,					do_nop },
	{ E_EVENT_TO_TIMER3,			do_nop },
	{ E_EVENT_INT_NMI,				do_nop },
	{ E_EVENT_OUTPUT_CONSOLE,		do_timer },
	{ E_EVENT_SERIAL_DUMP,			do_serial_dump },
	{ 0xffff, NULL } /* terminator */
};

/* 状態-イベントテーブル テーブル */
T_SYS_STAT_TBL gt_sys_stat_tbl[] = {
	{ E_STAT_IDLE,					gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_HOUR,				gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_MIN,				gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_SEC,				gt_sys_event_tbl_idle },
	{ 0xffff, NULL } /* terminator */
};

/* シリアルコマンド-イベントテーブル */
T_SERIAL_CMD gt_serial_event_tbl[] = {
	{ 'c', E_EVENT_TO_TICK },
	{ 't', E_EVENT_OUTPUT_CONSOLE },
	{ 'r', E_EVENT_BTN0_PUSH },
	{ 'R', E_EVENT_SW_LONGPRESS },
	{ 'm', E_EVENT_SW_ADJ_PUSH },
	{ 'd', E_EVENT_SERIAL_DUMP },
	{ ' ', E_EVENT_SERIAL_DUMP },
	{ 'T', E_EVENT_SW_ADJ_PUSH },
	{ '\0', -1 } /* terminator */
};


/*----------------------------------------------*/
/* ソフトタイマ定義								*/
/*----------------------------------------------*/
/* タイマ種類 */
enum {
	E_TIMER_CLOCK_SEC,		/* 時計用秒タイマ */
	E_TIMER_SET_TIME,		/* 時刻調整タイマ */
	E_TIMER_3,				/* タイマ3 */
	E_TIMER_MAX
} E_TIMER;

/* ソフトタイマ パラメータ */
volatile T_TIMER_PARAM gt_timer_param[E_TIMER_MAX] = {
	/* t.o._msec                cycle_mode  t.o._event */
	{ (1000UL / CLOCK_COUNT_1SEC),	1,		E_EVENT_TO_TICK },	/* 約0.31秒 */
//	{ 1000UL,					1,			E_EVENT_TO_TICK },	/* 1秒 */
	{ 250UL,					1,			E_EVENT_TO_SET_TIME },	/* 0.25秒 */
	{ (3UL * 60 * 60 * 1000),	0,			E_EVENT_TO_TIMER3 },	/* 3時間 */
};
volatile UINT gt_event_timer[E_TIMER_SYSTEM_MAX] = { 0 };

UINT gui_timer_param_num = E_TIMER_MAX;


/*----------------------------------------------*/
/* イベントハンドラ共通							*/
/*----------------------------------------------*/
static INT do_nop( VOID )
{
	return 0;
}

static INT do_timer( VOID )
{
	softtimer_start( E_TIMER_SET_TIME );
	return 0;
}

static INT do_timer_stop( VOID )
{
	softtimer_stop( E_TIMER_SET_TIME );
	return 0;
}

static INT do_serial_dump( VOID )
{
	fprintf_P( stderr, PSTR("** serial_dump **\n") );
	fprintf_P( stderr, PSTR("clk=%u\n"), _gui_clock_count );
	fprintf_P( stderr, PSTR("pcicr=0x%x pcmsk0=0x%x pcmsk1=0x%x pcmsk2=0x%x clk=%u\n"), PCICR, PCMSK0, PCMSK1, PCMSK2, _gui_clock_count );

	btn_print( stderr );

	return 0;
}

