
#ifndef __SOFTTIMER_H__
#define __SOFTTIMER_H__

// samylib
#include "my_typedef.h"
#include "framework_event_loop.h"


/* �\�t�g�^�C�}�� */
#ifndef E_TIMER_SYSTEM_MAX
#define E_TIMER_SYSTEM_MAX 2
#endif

typedef UINT T_TIMER_TYPE;

/* �^�C�}�p�����[�^��` */
typedef struct {
	ULONG ul_msec;		/* �^�C���A�E�g���� */
	UINT ui_cycle_mode;	/* �������[�h(0:�V���O���V���b�g 1:�T�C�N��) */
	T_SYS_EVENT t_timeout_event;/* �^�C���A�E�g���̃C�x���g(T_SYS_EVENT�^) */
} T_TIMER_PARAM;

/* �\�t�g�^�C�} �^�C���A�E�g�c���� */
extern volatile ULONG _gt_timer_count[E_TIMER_SYSTEM_MAX];

/* �^�C�}�C�x���g�ʒm�e�[�u�� */
extern volatile UINT gt_event_timer[E_TIMER_SYSTEM_MAX];

/* �^�C�}��`�e�[�u�� */
extern volatile T_TIMER_PARAM gt_timer_param[];	/* TODO: �e�[�u���G�鎞�T�C�Y�`�F�b�N */
extern UINT gui_timer_param_num;

// �O���֐�
extern VOID softtimer_int( VOID );
extern INT softtimer_start( T_TIMER_TYPE t_timer_type );
extern INT softtimer_stop( T_TIMER_TYPE t_timer_type );


#endif //__SOFTTIMER_H__

