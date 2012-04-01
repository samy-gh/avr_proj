
// softtimer
#include "framework_softtimer.h"
#include "framework_msec2clock.h"

// samylib
#include "my_typedef.h"


/****************************************************************************/
/* 割り込み動作制御変数														*/
/****************************************************************************/
/* ソフトタイマ タイムアウト残時間 */
volatile ULONG _gt_timer_count[E_TIMER_SYSTEM_MAX];


/****************************************************************************/
/* イベント通知制御変数														*/
/****************************************************************************/


/*----------------------------------------------*/
/* ソフトタイマ制御(クロック割り込み内)			*/
/*----------------------------------------------*/
VOID softtimer_int( VOID )
{
	T_TIMER_TYPE t_timer_type;

	for( t_timer_type = (T_TIMER_TYPE)0; t_timer_type < E_TIMER_SYSTEM_MAX; t_timer_type++ ) {
		if( _gt_timer_count[t_timer_type] == 0UL ) {
			continue;
		}

		_gt_timer_count[t_timer_type]--;
		if( _gt_timer_count[t_timer_type] == 0UL ) {
			gt_event_timer[t_timer_type] = 1;
			if( gt_timer_param[t_timer_type].ui_cycle_mode != 0 ) {
				_gt_timer_count[t_timer_type] = msec2clock( gt_timer_param[t_timer_type].ul_msec );
			}
		}
	}
}

