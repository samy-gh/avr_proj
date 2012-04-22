
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

/* Beep����������` */
typedef struct {
	UINT ui_on_msec;	/* �I�����msec */
	UINT ui_off_msec;	/* �I�t���msec */
	UINT ui_repeat;		/* �J��Ԃ��� */
} T_BEEP_DUTY;

/* Beep����`�e�[�u�� */
extern volatile T_BEEP_DUTY gt_beep_duty_tbl[];	/* TODO: �e�[�u���G�鎞�T�C�Y�`�F�b�N */
extern UINT gui_beep_duty_tbl_num;

// ���荞�݃n���h���p
VOID (*gBeep_PortCtrlFunc)( BOOL onoff );

/* Beep������ �c�N���b�N�� */
extern volatile UINT _gui_beep_count;
/* Beep������ �c�J��Ԃ��� */
extern volatile UINT _gui_beep_repeat;
/* ���ݖ�����Beep���^�C�v */
extern volatile T_BEEP_TYPE _gt_beep_type;

/* �C�x���g�ʒm����ϐ�														*/
extern volatile UINT _gui_event_beep_end;

// �O���֐�
extern VOID Beep_Int( VOID );
extern VOID Beep_Set_PortCtrl( VOID (*ctrl_func)( BOOL onoff ) );
extern INT Beep_Start( T_BEEP_TYPE t_beep_type, T_SYNC_MODE t_sync_mode );
extern VOID Beep_Stop( VOID );


#endif //__BEEP_H__

