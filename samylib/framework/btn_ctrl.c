
// btn
#include "framework_btn.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


/****************************************************************************/
/* 定数宣言																	*/
/****************************************************************************/


/****************************************************************************/
/* マクロ宣言																*/
/****************************************************************************/


/****************************************************************************/
/* 構造体型宣言																*/
/****************************************************************************/


/****************************************************************************/
/* 割り込み動作制御変数														*/
/****************************************************************************/
#ifdef CO_ENABLE_PCINT0_BTN
T_BTN_CTRL gt_pcint0_btn;
#endif
#ifdef CO_ENABLE_PCINT1_BTN
T_BTN_CTRL gt_pcint1_btn;
#endif
#ifdef CO_ENABLE_PCINT10_BTN
T_BTN_CTRL gt_pcint10_btn;
#endif
#ifdef CO_ENABLE_PCINT11_BTN
T_BTN_CTRL gt_pcint11_btn;
#endif

UINT gui_long_push_wait_count = 0;
UINT gui_chatt_wait_count = 0;


/****************************************************************************/
/* イベント通知制御変数(ステートマシンが参照する)							*/
/****************************************************************************/
volatile UCHAR _gui_event_btn_push[E_BTN_TYPE_MAX];
volatile UCHAR _gui_event_btn_longpush[E_BTN_TYPE_MAX];
volatile UCHAR _gui_event_btn_release[E_BTN_TYPE_MAX];


/****************************************************************************/
/* プロトタイプ宣言															*/
/****************************************************************************/
static __inline__ T_BTN_ONOFF get_btn_stat( T_BTN_TYPE t_btn_type ) __attribute__ ((always_inline));
static __inline__ VOID set_rise( T_BTN_TYPE t_btn_type ) __attribute__ ((always_inline));
static __inline__ VOID set_fall( T_BTN_TYPE t_btn_type ) __attribute__ ((always_inline));
static __inline__ VOID btn_di( T_BTN_TYPE t_btn_type ) __attribute__ ((always_inline));
static __inline__ VOID btn_ei( T_BTN_TYPE t_btn_type ) __attribute__ ((always_inline));
static UCHAR btn_event_countup( T_BTN_CTRL* const pt_btn );


/*----------------------------------------------*/
/* ボタン変化割り込み(Rise/Fall)				*/
/*----------------------------------------------*/
VOID btn_chg_int( T_BTN_CTRL* const pt_btn )
{
	T_BTN_ONOFF t_btn_onoff;

	t_btn_onoff = get_btn_stat( pt_btn->t_btn_type );
	if( t_btn_onoff == pt_btn->uc_port_stat ) {
		// 割り込みは発生していない
		return;
	}

	pt_btn->uc_port_stat = t_btn_onoff;

	if( t_btn_onoff != E_BTN_ONOFF_OFF ) {
		// Fall割り込み

		if( pt_btn->t_stat == E_BTN_IDLE ) {
			/* IDLE状態でボタンイベント(Rise)受信 - 長押し判定開始 */
			/* int rise マスク */
			btn_di( pt_btn->t_btn_type );
			/* 割り込みモードをfallに設定 */
			set_fall( pt_btn->t_btn_type );
			/* 長押しタイマ開始 */
			pt_btn->t_long_push_wait_count = gui_long_push_wait_count;
			/* チャタリング監視タイマ開始 */
			pt_btn->t_chatt_wait_count = gui_chatt_wait_count;
			/* ステータス変更 */
			pt_btn->t_stat = E_BTN_RISE_CHATT_WAIT;
		}


	}
	else {
		// Rise割り込み

		if( pt_btn->t_stat == E_BTN_LONG_PUSH_WAIT ) {
			/* 長押し判定中にボタンイベント(Fall)受信 - 長押し確定 */
			/* SWイベント発生 */
			_gui_event_btn_push[pt_btn->t_btn_type] = btn_event_countup( pt_btn );
			/* int fallマスク */
			btn_di( pt_btn->t_btn_type );
			/* 長押しタイマ停止 */
			pt_btn->t_long_push_wait_count = 0;
			/* チャタリング監視タイマ開始 */
			pt_btn->t_chatt_wait_count = gui_chatt_wait_count;
			/* ステータス変更 */
			pt_btn->t_stat = E_BTN_FALL_CHATT_WAIT;
		} else


		if( pt_btn->t_stat == E_BTN_SW_OFF_WAIT ) {
			/* 長押し解除イベント発生 */
			_gui_event_btn_release[pt_btn->t_btn_type] = pt_btn->uc_event_count;
			/* int fallマスク */
			btn_di( pt_btn->t_btn_type );
			/* チャタリング監視タイマ開始 */
			pt_btn->t_chatt_wait_count = gui_chatt_wait_count;
			/* ステータス変更 */
			pt_btn->t_stat = E_BTN_FALL_CHATT_WAIT;
		}
	}
}


/*----------------------------------------------*/
/* システムクロック変化割り込み					*/
/*----------------------------------------------*/
VOID btn_clk_int( T_BTN_CTRL* const pt_btn )
{
	/* チャタリング監視タイマ処理 */
	if( pt_btn->t_chatt_wait_count != 0 ) {
		pt_btn->t_chatt_wait_count--;
		if( pt_btn->t_chatt_wait_count == 0 ) {
			T_BTN_ONOFF t_btn_onoff = get_btn_stat( pt_btn->t_btn_type );
			if( t_btn_onoff == E_BTN_ONOFF_ON ) {
				/*** チャタリング監視タイマT.O.(T.O.時のボタンON) ***/
				if( pt_btn->t_stat == E_BTN_RISE_CHATT_WAIT ) {
					/** ボタンONチャタリング監視中 */
					/* int fall許可 */
					btn_ei( pt_btn->t_btn_type );
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_LONG_PUSH_WAIT;
				} else
				if( pt_btn->t_stat == E_BTN_FALL_CHATT_WAIT ) {
					/** ボタンOFFチャタリング監視中 */
					/* 長押しタイマ開始 */
					pt_btn->t_long_push_wait_count = gui_long_push_wait_count;
					/* int fall許可 */
					btn_ei( pt_btn->t_btn_type );
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_LONG_PUSH_WAIT;
				}
			} else {
				/*** チャタリング監視タイマT.O.(T.O.時のボタンOFF) ***/
				if( pt_btn->t_stat == E_BTN_RISE_CHATT_WAIT ) {
					/** ボタンONチャタリング監視中 */
					/* SWイベント発生 */
					_gui_event_btn_push[pt_btn->t_btn_type] = btn_event_countup( pt_btn );
					/* 長押しタイマ停止 */
					pt_btn->t_long_push_wait_count = 0;
					/* チャタリング監視タイマ開始 */
					pt_btn->t_chatt_wait_count = gui_chatt_wait_count;
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_FALL_CHATT_WAIT;
				} else
				if( pt_btn->t_stat == E_BTN_FALL_CHATT_WAIT ) {
					/** ボタンOFFチャタリング監視中 */
					/* int rise許可 */
					set_rise( pt_btn->t_btn_type );
					btn_ei( pt_btn->t_btn_type );
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_IDLE;
				}
			}
		}
	}



	/* 長押しタイマ処理 */
	if( pt_btn->t_long_push_wait_count != 0 ) {
		pt_btn->t_long_push_wait_count--;
		if( pt_btn->t_long_push_wait_count == 0 ) {
			T_BTN_ONOFF t_btn_onoff = get_btn_stat( pt_btn->t_btn_type );
			if( t_btn_onoff == E_BTN_ONOFF_ON ) {
				/*** 長押しタイマT.O.(T.O.時のボタンON) ***/
				if( pt_btn->t_stat == E_BTN_RISE_CHATT_WAIT ) {
					/* 長押しイベント発生 */
					_gui_event_btn_longpush[pt_btn->t_btn_type] = btn_event_countup( pt_btn );
					/* int fall許可 */
					btn_ei( pt_btn->t_btn_type );
					/* チャタリング監視タイマ停止 */
					pt_btn->t_chatt_wait_count = 0;
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_SW_OFF_WAIT;
				} else
				if( pt_btn->t_stat == E_BTN_LONG_PUSH_WAIT ) {
					/* 長押しイベント発生 */
					_gui_event_btn_longpush[pt_btn->t_btn_type] = btn_event_countup( pt_btn );
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_SW_OFF_WAIT;
				}
			} else {
				/*** 長押しタイマT.O.(T.O.時のボタンOFF) ***/
				if( pt_btn->t_stat == E_BTN_LONG_PUSH_WAIT ) {
					/* SWイベント発生 */
					_gui_event_btn_push[pt_btn->t_btn_type] = btn_event_countup( pt_btn );
					/* int fallマスク */
					btn_di( pt_btn->t_btn_type );
					/* 長押しタイマ停止 */
					pt_btn->t_long_push_wait_count = 0;
					/* チャタリング監視タイマ開始 */
					pt_btn->t_chatt_wait_count = gui_chatt_wait_count;
					/* ステータス変更 */
					pt_btn->t_stat = E_BTN_FALL_CHATT_WAIT;
				}
			}
		}
	}
}


static UCHAR btn_event_countup( T_BTN_CTRL* const pt_btn )
{
	pt_btn->uc_event_count++;
	if( pt_btn->uc_event_count == 0 ) {
		pt_btn->uc_event_count = 1;
	}

	return pt_btn->uc_event_count;
}


/*----------------------------------------------*/
/* ボタン制御初期化								*/
/*----------------------------------------------*/
#if defined(CO_ENABLE_PCINT0_BTN) || defined(CO_ENABLE_PCINT1_BTN) || defined(CO_ENABLE_PCINT2_BTN) \
 || defined(CO_ENABLE_PCINT3_BTN) || defined(CO_ENABLE_PCINT4_BTN) || defined(CO_ENABLE_PCINT5_BTN) \
 || defined(CO_ENABLE_PCINT10_BTN) || defined(CO_ENABLE_PCINT11_BTN) \
 || defined(CO_ENABLE_IRQ0_BTN) || defined(CO_ENABLE_IRQ1_BTN) || defined(CO_ENABLE_IRQ2_BTN) \
 || defined(CO_ENABLE_IRQ3_BTN)
static VOID btn_init_sub( T_BTN_CTRL* const pt_btn, const T_BTN_TYPE t_btn_type )
{
	pt_btn->t_btn_type = t_btn_type;
	pt_btn->t_stat = E_BTN_IDLE;
	pt_btn->uc_event_count = 0;
	_gui_event_btn_push[t_btn_type] = 0;
	_gui_event_btn_longpush[t_btn_type] = 0;
	_gui_event_btn_release[t_btn_type] = 0;
	// 割り込み解除
	btn_ei( t_btn_type );
}
#endif

#define M_PORT_SET_PCINT( pcint_no )	{ \
	cbi( D_BTN_REG_ ## pcint_no ## _DDR, D_BTN_BIT_ ## pcint_no ## _DDR ); \
	sbi( D_BTN_REG_ ## pcint_no ## _PORT, D_BTN_BIT_ ## pcint_no ## _PORT ); \
}

VOID btn_init( UINT ui_long_push_wait_count, UINT ui_chatt_wait_count )
{
	gui_long_push_wait_count = ui_long_push_wait_count;
	gui_chatt_wait_count = ui_chatt_wait_count;

#ifdef CO_ENABLE_PCINT0_BTN
	btn_init_sub( &gt_pcint0_btn, E_BTN_PCINT0 );
	M_PORT_SET_PCINT( PCINT0 );
#endif
#ifdef CO_ENABLE_PCINT1_BTN
	btn_init_sub( &gt_pcint1_btn, E_BTN_PCINT1 );
	M_PORT_SET_PCINT( PCINT1 );
#endif
#ifdef CO_ENABLE_PCINT2_BTN
	btn_init_sub( &gt_pcint2_btn, E_BTN_PCINT2 );
	M_PORT_SET_PCINT( PCINT2 );
#endif
#ifdef CO_ENABLE_PCINT3_BTN
	btn_init_sub( &gt_pcint3_btn, E_BTN_PCINT3 );
	M_PORT_SET_PCINT( PCINT3 );
#endif
#ifdef CO_ENABLE_PCINT10_BTN
	btn_init_sub( &gt_pcint10_btn, E_BTN_PCINT10 );
	M_PORT_SET_PCINT( PCINT10 );
#endif
#ifdef CO_ENABLE_PCINT11_BTN
	btn_init_sub( &gt_pcint11_btn, E_BTN_PCINT11 );
	M_PORT_SET_PCINT( PCINT11 );
#endif

#ifdef PCIF0
	sbi( PCICR, PCIE0 );
#endif
#ifdef PCIF1
	sbi( PCICR, PCIE1 );
#endif
#ifdef PCIF2
	sbi( PCICR, PCIE2 );
#endif
}


/*----------------------------------------------*/
/* Rising Edge割り込みモードに設定				*/
/*----------------------------------------------*/
/* rise/fall割り込みが発生しないとわかっているタイミングでコールすること。
 * (モード変更時に割り込みが発生する場合があるため、割り込みフラグをクリアしている
 */
static __inline__ VOID set_rise( T_BTN_TYPE t_btn_type )
{
#ifdef CO_ENABLE_PCINT0_BTN
	if( t_btn_type == E_BTN_PCINT0 ) {
		IEGR2.BIT.WPEG0 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_PCINT1_BTN
	if( t_btn_type == E_BTN_PCINT1 ) {
		IEGR2.BIT.WPEG1 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_PCINT2_BTN
	if( t_btn_type == E_BTN_PCINT2 ) {
		IEGR2.BIT.WPEG2 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_PCINT3_BTN
	if( t_btn_type == E_BTN_PCINT3 ) {
		IEGR2.BIT.WPEG3 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_PCINT4_BTN
	if( t_btn_type == E_BTN_PCINT4 ) {
		IEGR2.BIT.WPEG4 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_PCINT5_BTN
	if( t_btn_type == E_BTN_PCINT5 ) {
		IEGR2.BIT.WPEG5 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ0_BTN
	if( t_btn_type == E_BTN_IRQ0 ) {
		IEGR1.BIT.IEG0 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ1_BTN
	if( t_btn_type == E_BTN_IRQ1 ) {
		IEGR1.BIT.IEG1 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ2_BTN
	if( t_btn_type == E_BTN_IRQ2 ) {
		IEGR1.BIT.IEG2 = 1;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ3_BTN
	if( t_btn_type == E_BTN_IRQ3 ) {
		IEGR1.BIT.IEG3 = 1;
		return;
	}
#endif
}


/*----------------------------------------------*/
/* Falling Edge割り込みモードに設定				*/
/*----------------------------------------------*/
/* rise/fall割り込みが発生しないとわかっているタイミングでコールすること。
 * (モード変更時に割り込みが発生する場合があるため、割り込みフラグをクリアしている
 */
static __inline__ VOID set_fall( T_BTN_TYPE t_btn_type )
{
#ifdef CO_ENABLE_IRQ0_BTN
	if( t_btn_type == E_BTN_IRQ0 ) {
		IEGR1.BIT.IEG0 = 0;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ1_BTN
	if( t_btn_type == E_BTN_IRQ1 ) {
		IEGR1.BIT.IEG1 = 0;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ2_BTN
	if( t_btn_type == E_BTN_IRQ2 ) {
		IEGR1.BIT.IEG2 = 0;
		return;
	}
#endif
#ifdef CO_ENABLE_IRQ3_BTN
	if( t_btn_type == E_BTN_IRQ3 ) {
		IEGR1.BIT.IEG3 = 0;
		return;
	}
#endif
}

#define M_PORT_STAT_PCINT( pcint_no )		( (!(D_BTN_REG_ ## pcint_no ## _PIN & _BV(D_BTN_BIT_ ## pcint_no ## _PIN)))?(E_BTN_ONOFF_ON):(E_BTN_ONOFF_OFF) )

/*----------------------------------------------*/
/* ボタンのOn/Off状態を取得する					*/
/*----------------------------------------------*/
static __inline__ T_BTN_ONOFF get_btn_stat( T_BTN_TYPE t_btn_type )
{
	switch( t_btn_type ) {
#ifdef CO_ENABLE_PCINT0_BTN
		case E_BTN_PCINT0:
			return M_PORT_STAT_PCINT( PCINT0 );
#endif
#ifdef CO_ENABLE_PCINT10_BTN
		case E_BTN_PCINT10:
			return M_PORT_STAT_PCINT( PCINT10 );
#endif
#ifdef CO_ENABLE_PCINT11_BTN
		case E_BTN_PCINT11:
			return M_PORT_STAT_PCINT( PCINT11 );
#endif
		default:
			return E_BTN_ONOFF_OFF;
	}
}


/*----------------------------------------------*/
/* ボタン割り込み許可							*/
/*----------------------------------------------*/
static __inline__ VOID btn_ei( T_BTN_TYPE t_btn_type )
{
	switch( t_btn_type ) {
#ifdef CO_ENABLE_PCINT0_BTN
		case E_BTN_PCINT0:
			sbi( D_BTN_REG_PCINT0_PCMSK, D_BTN_BIT_PCINT0_PCMSK );
			return;
#endif
#ifdef CO_ENABLE_PCINT10_BTN
		case E_BTN_PCINT10:
			sbi( D_BTN_REG_PCINT10_PCMSK, D_BTN_BIT_PCINT10_PCMSK );
			return;
#endif
#ifdef CO_ENABLE_PCINT11_BTN
		case E_BTN_PCINT11:
			sbi( D_BTN_REG_PCINT11_PCMSK, D_BTN_BIT_PCINT11_PCMSK );
			return;
#endif
		default:
			return;
	}
}


/*----------------------------------------------*/
/* ボタン割り込み禁止							*/
/*----------------------------------------------*/
static __inline__ VOID btn_di( T_BTN_TYPE t_btn_type )
{
	switch( t_btn_type ) {
#ifdef CO_ENABLE_PCINT0_BTN
		case E_BTN_PCINT0:
			cbi( D_BTN_REG_PCINT0_PCMSK, D_BTN_BIT_PCINT0_PCMSK );
			return;
#endif
#ifdef CO_ENABLE_PCINT10_BTN
		case E_BTN_PCINT10:
			cbi( D_BTN_REG_PCINT10_PCMSK, D_BTN_BIT_PCINT10_PCMSK );
			return;
#endif
#ifdef CO_ENABLE_PCINT11_BTN
		case E_BTN_PCINT11:
			cbi( D_BTN_REG_PCINT11_PCMSK, D_BTN_BIT_PCINT11_PCMSK );
			return;
#endif
		default:
			return;
	}
}

