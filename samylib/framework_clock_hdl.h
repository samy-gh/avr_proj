
#ifndef __CLOCK_HDL_H__
#define __CLOCK_HDL_H__

// samylib
#include "my_typedef.h"
#include "common.h"


/* クロック割り込み発生 */
extern volatile UINT _gui_event_clock;

/* ユーザハンドラへのポインタ */
extern VOID (* volatile fn_int_timera_hdl)( VOID );

/* クロック割り込み発生回数 */
extern volatile UINT _gui_clock_count;

extern VOID clock_hdl( VOID );

#endif //__CLOCK_HDL_H__

