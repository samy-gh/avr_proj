
#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

// samylib
#include "my_typedef.h"
#include "framework_event_loop.h"


/* ソフトタイマ数 */
#ifndef E_TIMER_SYSTEM_MAX
#define E_TIMER_SYSTEM_MAX 2
#endif

typedef UINT T_TIMER_TYPE;

/* タイマパラメータ定義 */
typedef struct {
	ULONG ul_msec;		/* タイムアウト時間 */
	UINT ui_cycle_mode;	/* 周期モード(0:シングルショット 1:サイクル) */
	T_SYS_EVENT t_timeout_event;/* タイムアウト時のイベント(T_SYS_EVENT型) */
} T_TIMER_PARAM;

/* ソフトタイマ タイムアウト残時間 */
extern volatile ULONG _gt_timer_count[E_TIMER_SYSTEM_MAX];

/* タイマイベント通知テーブル */
extern volatile UINT gt_event_timer[E_TIMER_SYSTEM_MAX];

/* タイマ定義テーブル */
extern volatile T_TIMER_PARAM gt_timer_param[];	/* TODO: テーブル触る時サイズチェック */
extern UINT gui_timer_param_num;

// 外部関数
extern VOID softtimer_int( VOID );
extern INT softtimer_start( T_TIMER_TYPE t_timer_type );
extern INT softtimer_stop( T_TIMER_TYPE t_timer_type );


#endif //__SOFTTIMER_H__

