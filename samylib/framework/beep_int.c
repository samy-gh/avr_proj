
// btn
#include "framework_beep.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


volatile UINT _gui_event_beep_end = NULL;

/****************************************************************************/
/* 割り込み動作制御変数														*/
/****************************************************************************/

/* Beep音制御 残クロック数 */
volatile UINT _gui_beep_count;
/* Beep音制御 残繰り返し数 */
volatile UINT _gui_beep_repeat;
/* 現在鳴動中のBeep音タイプ */
volatile T_BEEP_TYPE _gt_beep_type = -1;


/****************************************************************************/
/* イベント通知制御変数														*/
/****************************************************************************/
volatile UINT _gui_event_beep_end;				/* Beep完了 */


/****************************************************************************/
/* 外部パラメータ変数														*/
/****************************************************************************/
UINT gui_beep_duty_tbl_num;


/*----------------------------------------------*/
/* Beep制御(タイマＡ割り込み内)					*/
/*----------------------------------------------*/
VOID Beep_Int( VOID )
{
	if( _gui_beep_count == 0 ) {
		return;
	}

	_gui_beep_count--;

	/* 発音・Mute区間が終わったかどうか */
	if( _gui_beep_count > 0 ) {
		/* 終わってない */
		return;
	}

	if( BEEP_STAT() != 0 ) {
		/* 現在発音中  */
		_gui_beep_count = msec2count(gt_beep_duty_tbl[_gt_beep_type].ui_off_msec);
		if( _gui_beep_count != 0 ) {
			/* mute時間がある場合はmute開始 */
			BEEP_RESET();		/* Beep停止 */
			return;
		}
		/* mute時間がないため１サイクル完了 */
	}

	/* 現在mute中 or Mute時間指定が0ルート */

	/* 1サイクル完了 */
	_gui_beep_repeat--;
	if( _gui_beep_repeat == 0 ) {
		/* Beep終了 */
		BEEP_RESET();			/* Beep停止 */
		_gui_event_beep_end = 1;	/* Beep完了通知 */
		return;
	}

	/* 次のサイクルに入る */
	/* 発音開始 */
	_gui_beep_count = msec2count(gt_beep_duty_tbl[_gt_beep_type].ui_on_msec);
	BEEP_SET();	/* Beep発音開始 */
}

