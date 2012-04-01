
// btn
#include "framework_clock_hdl.h"
#include "framework_btn.h"
#include "framework_softtimer.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>

#include "test_led.h"

/****************************************************************************/
/* デバッグ用変数															*/
/****************************************************************************/
/* クロック割り込み発生回数 */
volatile UINT _gui_clock_count;


/****************************************************************************/
/* 割り込み動作制御変数														*/
/****************************************************************************/


/****************************************************************************/
/* イベント通知制御変数														*/
/****************************************************************************/
volatile UINT _gui_event_clock;					/* クロック割り込み発生 */

/* ユーザハンドラへのポインタ */
VOID (* volatile fn_int_timera_hdl)( VOID );


/*----------------------------------------------*/
/* タイマＡ										*/
/*----------------------------------------------*/
VOID clock_hdl( VOID )
{
	/* メインスレッドに通知 */
	_gui_event_clock = 1;

	_gui_clock_count++;

#ifdef CO_ENABLE_SOFTTIMER
	softtimer_int();
#endif

#if 0 // TODO:移植未
	/* Beep処理 */
	int_beep();
#endif

	/* ボタン制御ルーチンへのクロック供給 */
#ifdef CO_ENABLE_PCINT0_BTN
	btn_clk_int( &gt_pcint0_btn );
#endif
#ifdef CO_ENABLE_PCINT1_BTN
	btn_clk_int( &gt_pcint1_btn );
#endif
#ifdef CO_ENABLE_PCINT2_BTN
	btn_clk_int( &gt_pcint2_btn );
#endif
#ifdef CO_ENABLE_PCINT3_BTN
	btn_clk_int( &gt_pcint3_btn );
#endif
#ifdef CO_ENABLE_PCINT4_BTN
	btn_clk_int( &gt_pcint4_btn );
#endif
#ifdef CO_ENABLE_PCINT5_BTN
	btn_clk_int( &gt_pcint5_btn );
#endif
#ifdef CO_ENABLE_PCINT10_BTN
	btn_clk_int( &gt_pcint10_btn );
#endif
#ifdef CO_ENABLE_PCINT11_BTN
	btn_clk_int( &gt_pcint11_btn );
#endif
#ifdef CO_ENABLE_IRQ0_BTN
	btn_clk_int( &gt_irq0_btn );
#endif
#ifdef CO_ENABLE_IRQ1_BTN
	btn_clk_int( &gt_irq1_btn );
#endif
#ifdef CO_ENABLE_IRQ2_BTN
	btn_clk_int( &gt_irq2_btn );
#endif
#ifdef CO_ENABLE_IRQ3_BTN
	btn_clk_int( &gt_irq3_btn );
#endif
}

