
#ifndef __BEEP_H__
#define __BEEP_H__

// samylib
#include "my_typedef.h"
#include "common.h"
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#include "framework_btn_atmega328p.h"
#endif

// WinAVR
#include <avr/io.h>

// STD-C
#include <stdio.h>


typedef UINT T_BEEP_TYPE;

typedef enum {
	E_SYNC,
	E_ASYNC
} T_SYNC_MODE;

/* Beep音鳴動周期定義 */
typedef struct {
	UINT ui_on_msec;	/* オン区間msec */
	UINT ui_off_msec;	/* オフ区間msec */
	UINT ui_repeat;		/* 繰り返し回数 */
} T_BEEP_DUTY;

/* Beep音定義テーブル */
extern volatile T_BEEP_DUTY gt_beep_duty_tbl[];	/* TODO: テーブル触る時サイズチェック */
extern UINT gui_beep_duty_tbl_num;

// 割り込みハンドラ用
VOID (*gBeep_PortCtrlFunc)( BOOL onoff );

/* Beep音制御 残クロック数 */
extern volatile UINT _gui_beep_count;
/* Beep音制御 残繰り返し数 */
extern volatile UINT _gui_beep_repeat;
/* 現在鳴動中のBeep音タイプ */
extern volatile T_BEEP_TYPE _gt_beep_type;

/* イベント通知制御変数														*/
extern volatile UINT _gui_event_beep_end;

// 外部関数
extern VOID Beep_Int( VOID );
extern VOID Beep_Set_PortCtrl( VOID (*ctrl_func)( BOOL onoff ) );
extern INT Beep_Start( T_BEEP_TYPE t_beep_type, T_SYNC_MODE t_sync_mode );
extern VOID Beep_Stop( VOID );


#endif //__BEEP_H__

