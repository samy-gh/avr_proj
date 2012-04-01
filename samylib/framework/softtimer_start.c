
// softtimer
#include "framework_softtimer.h"
#include "framework_msec2clock.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/pgmspace.h>

// STD-C
#include <stdio.h>


/*----------------------------------------------*/
/* ソフトタイマ起動								*/
/*----------------------------------------------*/
/* 戻り値: -1: パラメータエラー */
/* 戻り値: -2: タイマ起動中 */
/* 戻り値: 0: タイマ起動完了 */
INT softtimer_start( T_TIMER_TYPE t_timer_type )
{
	if( t_timer_type >= gui_timer_param_num ) {
		return -1;
	}

	/* タイマ起動中の場合はエラー */
	if( _gt_timer_count[t_timer_type] != 0UL ) {
		return -2;
	}

	/* タイムアウト時間設定 */
	_gt_timer_count[t_timer_type] = msec2clock(gt_timer_param[t_timer_type].ul_msec);

#ifdef CO_ENABLE_EVENT_TRACE_LOG
	fprintf_P( stderr, PSTR("softtimer_start() = %u, count=%lu\n"), t_timer_type, _gt_timer_count[t_timer_type] );
#endif

	return 0;
}

