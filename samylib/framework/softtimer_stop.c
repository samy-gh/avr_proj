
// softtimer
#include "framework_softtimer.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/pgmspace.h>

// STD-C
#include <stdio.h>


/*----------------------------------------------*/
/* ソフトタイマ停止								*/
/*----------------------------------------------*/
INT softtimer_stop( T_TIMER_TYPE t_timer_type )
{
#ifdef CO_ENABLE_EVENT_TRACE_LOG
	fprintf_P( stderr, PSTR("softtimer_stop() = %u\n"), t_timer_type );
#endif

	/* タイマ停止 */
	_gt_timer_count[t_timer_type] = 0UL;	/* 残カウントを0に変更 */
	gt_event_timer[t_timer_type] = 0;	/* イベントクリア */

	return 0;
}

