
#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

// samylib
#include "my_typedef.h"
#include "common.h"


/* システムステータス値 */
#define E_STAT_SYS_IDLE			0

/* システムイベント値 */
#define E_EVENT_IDLE			0
#define E_EVENT_AD0_END			1
#define E_EVENT_AD1_END			2
#define E_EVENT_BEEP_END		3
#define E_EVENT_INT_PCINT0		4
#define E_EVENT_INT_PCINT1		5
#define E_EVENT_INT_PCINT2		6
#define E_EVENT_INT_PCINT3		7
#define E_EVENT_INT_PCINT4		8
#define E_EVENT_INT_PCINT5		9
#define E_EVENT_BTN0_LONGPUSH	10
#define E_EVENT_BTN0_PUSH		11
#define E_EVENT_BTN0_RELEASE	12
#define E_EVENT_BTN1_LONGPUSH	13
#define E_EVENT_BTN1_PUSH		14
#define E_EVENT_BTN1_RELEASE	15
#define E_EVENT_BTN2_LONGPUSH	16
#define E_EVENT_BTN2_PUSH		17
#define E_EVENT_BTN2_RELEASE	18
#define E_EVENT_BTN3_LONGPUSH	19
#define E_EVENT_BTN3_PUSH		20
#define E_EVENT_BTN3_RELEASE	21
#define E_EVENT_BTN4_LONGPUSH	22
#define E_EVENT_BTN4_PUSH		23
#define E_EVENT_BTN4_RELEASE	24
#define E_EVENT_BTN5_LONGPUSH	25
#define E_EVENT_BTN5_PUSH		26
#define E_EVENT_BTN5_RELEASE	27
#define E_EVENT_BTN6_LONGPUSH	28
#define E_EVENT_BTN6_PUSH		29
#define E_EVENT_BTN6_RELEASE	30
#define E_EVENT_BTN7_LONGPUSH	31
#define E_EVENT_BTN7_PUSH		32
#define E_EVENT_BTN7_RELEASE	33
#define E_EVENT_BTN8_LONGPUSH	34
#define E_EVENT_BTN8_PUSH		35
#define E_EVENT_BTN8_RELEASE	36
#define E_EVENT_BTN9_LONGPUSH	37
#define E_EVENT_BTN9_PUSH		38
#define E_EVENT_BTN9_RELEASE	39
#define E_EVENT_INT_IRQ0		40
#define E_EVENT_INT_IRQ1		41
#define E_EVENT_INT_IRQ2		42
#define E_EVENT_INT_IRQ3		43
#define E_EVENT_INT_NMI			44
#define E_EVENT_INT_CLOCK		45
#define E_EVENT_INT_PCINT10		46
#define E_EVENT_BTN10_LONGPUSH	47
#define E_EVENT_BTN10_PUSH		48
#define E_EVENT_BTN10_RELEASE	49
#define E_EVENT_INT_PCINT11		50
#define E_EVENT_BTN11_LONGPUSH	51
#define E_EVENT_BTN11_PUSH		52
#define E_EVENT_BTN11_RELEASE	53
#define E_EVENT_I2C_SLAVE_READ	54
#define E_EVENT_I2C_SLAVE_WRITE	55
#define E_EVENT_IR_RECV_END		56
#define E_EVENT_IR_RECV_ERR		57
#define E_EVENT_IR_SEND_END		58

#define E_EVENT_USER			128


typedef UINT T_SYS_EVENT;
typedef UINT T_SYS_STAT;

/* システムイベントテーブル型宣言 */
typedef struct {
	T_SYS_EVENT t_sys_event;
	INT (*fn_hdl)( VOID );
} T_SYS_EVENT_TBL;

/* システムステータステーブル型宣言 */
typedef struct {
	T_SYS_STAT t_sys_stat;
	T_SYS_EVENT_TBL *pt_sys_event_tbl;
} T_SYS_STAT_TBL;

/* シリアルコマンドパラメータ定義 */
typedef struct {
	CHAR		c;				/* コマンド文字 */
	T_SYS_EVENT t_sys_event;	/* 発生イベント(T_SYS_EVENT型) */
} T_SERIAL_CMD;


/* システムステータステーブル */
extern T_SYS_STAT_TBL gt_sys_stat_tbl[];	/* TODO: サイズチェックを行う */

/* シリアルコマンドパラメータ定義テーブル */
extern T_SERIAL_CMD gt_serial_event_tbl[];

/* タイマイベント通知テーブル */
extern volatile UINT gt_event_timer[];

extern VOID event_loop( VOID );

#endif //__EVENT_LOOP_H__

