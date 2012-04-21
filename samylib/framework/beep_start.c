
// framework
#include "framework_beep.h"
#include "framework_msec2clock.h"

// samylib
#include "my_typedef.h"


/****************************************************************************/
/* 動作パラメータ宣言														*/
/****************************************************************************/
#define BEEP_GAP_MSEC		100		/* 100msec ... 前のBeep終了後に次の発音を開始するまでの待ち時間 */


/*----------------------------------------------*/
/* Beep開始										*/
/*----------------------------------------------*/
/* 戻り値: -1: Beep動作中
 * 戻り値: 1: Beep開始(ASYNC時)
 * 戻り値: 0: Beep完了
 */
INT Beep_Start( T_BEEP_TYPE t_beep_type, T_SYNC_MODE t_sync_mode )
{
	if( gBeep_PortCtrlFunc == NULL ) {
		/* ポート制御関数が無い */
		return -1;
	}

	if( _gui_beep_repeat > 0 ) {
		/* beep動作中 */
		if( t_sync_mode == E_ASYNC ) {
			/* ASYNCなのでエラー終了 */
			return -1;
		}

		/* 同期モードの場合は前のBeepが終わるのを待つ */
		while( _gui_event_beep_end == 0 ) {
			;
		}
		time_wait( BEEP_GAP_MSEC );
	}

	_gt_beep_type = t_beep_type;

	_gui_beep_repeat = gt_beep_duty_tbl[t_beep_type].ui_repeat;
	_gui_beep_count = msec2clock(gt_beep_duty_tbl[_gt_beep_type].ui_on_msec);
	gBeep_PortCtrlFunc( TRUE );	/* Beep発音開始 */


#ifdef WITH_GDB_SIM
	_gui_event_beep_end = 1;
#endif

	/* 非同期モードの場合はすぐ抜ける */
	if( t_sync_mode == E_ASYNC ) {
		return 1;
	}

	/* 同期モードの場合は終了を待つ */
	while( _gui_event_beep_end == 0 ) {
		;
	}

	_gui_event_beep_end = 0;

	return 0;

}

