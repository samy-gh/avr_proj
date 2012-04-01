
#ifndef __BTN_H__
#define __BTN_H__

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


/* �{�^�������� */
typedef enum {
	E_BTN_IDLE,					/* �ʏ��� */
	E_BTN_RISE_CHATT_WAIT,		/* �{�^���������̃`���^�����O������ */
	E_BTN_LONG_PUSH_WAIT,		/* �������Ď��� */
	E_BTN_SW_OFF_WAIT,			/* �������m���̊J���҂� */
	E_BTN_FALL_CHATT_WAIT,		/* �{�^��������̃`���^�����O������ */
	E_BTN_STAT_MAX
} T_BTN_STAT;

/* �{�^����� */
typedef enum {
	E_BTN_PCINT0,
	E_BTN_PCINT1,
	E_BTN_PCINT2,
	E_BTN_PCINT3,
	E_BTN_PCINT4,
	E_BTN_PCINT5,
	E_BTN_PCINT10,
	E_BTN_PCINT11,
	E_BTN_IRQ0,
	E_BTN_IRQ1,
	E_BTN_IRQ2,
	E_BTN_IRQ3,
	E_BTN_TYPE_MAX
} T_BTN_TYPE;

typedef enum {
	E_BTN_ONOFF_OFF = 0,
	E_BTN_ONOFF_ON
} T_BTN_ONOFF;

/* �{�^���Ǘ��\���� */
typedef struct {
	UCHAR uc_port_stat;				/* �Ō�̊��荞�݂ɂ�����|�[�g��� */
	T_BTN_TYPE t_btn_type;
	T_BTN_STAT t_stat;
	UINT t_chatt_wait_count;
	UINT t_long_push_wait_count;
	UCHAR uc_event_count;			/* �C�x���g������ */
} T_BTN_CTRL;

// ���������莞��(�^�C�}�J�E���g) boot���ɐݒ肷�邱��
extern UINT gui_long_push_wait_count;
// �`���^�����O���莞��(�^�C�}�J�E���g) boot���ɐݒ肷�邱��
extern UINT gui_chatt_wait_count;

// event_loop�p
extern volatile unsigned int _gui_event_btn_push[E_BTN_TYPE_MAX];
extern volatile unsigned int _gui_event_btn_longpush[E_BTN_TYPE_MAX];
extern volatile unsigned int _gui_event_btn_release[E_BTN_TYPE_MAX];

// ���荞�݃n���h���p
#ifdef CO_ENABLE_PCINT0_BTN
extern T_BTN_CTRL gt_pcint0_btn;
#endif
#ifdef CO_ENABLE_PCINT1_BTN
extern T_BTN_CTRL gt_pcint1_btn;
#endif
#ifdef CO_ENABLE_PCINT10_BTN
extern T_BTN_CTRL gt_pcint10_btn;
#endif
#ifdef CO_ENABLE_PCINT11_BTN
extern T_BTN_CTRL gt_pcint11_btn;
#endif

// �O���֐�
extern VOID btn_init( UINT ui_long_push_wait_count, UINT ui_chatt_wait_count );
extern VOID btn_chg_int( T_BTN_CTRL* const pt_btn );
extern VOID btn_clk_int( T_BTN_CTRL* const pt_btn );
extern VOID btn_print( FILE* const fh );


#endif //__BTN_H__

