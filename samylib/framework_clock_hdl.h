
#ifndef __CLOCK_HDL_H__
#define __CLOCK_HDL_H__

// samylib
#include "my_typedef.h"
#include "common.h"


/* �N���b�N���荞�ݔ��� */
extern volatile UINT _gui_event_clock;

/* ���[�U�n���h���ւ̃|�C���^ */
extern VOID (* volatile fn_int_timera_hdl)( VOID );

/* �N���b�N���荞�ݔ����� */
extern volatile UINT _gui_clock_count;

extern VOID clock_hdl( VOID );

#endif //__CLOCK_HDL_H__

