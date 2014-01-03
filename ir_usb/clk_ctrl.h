
#ifndef __CLK_CTRL_H__
#define __CLK_CTRL_H__

// プロジェクト固有
#include "custom.h"

// samylib
#include <my_typedef.h>

extern volatile UCHAR gClk_Down_Disable_Cnt;


#ifdef CO_CLOCK_REDUCE_MODE
#define CLK_DOWN_DISABLE()	( gClk_Down_Disable_Cnt++ )
#define CLK_DOWN_ENABLE()	( gClk_Down_Disable_Cnt-- )
#define CLK_DOWN_ON()		{ \
		if( gClk_Down_Disable_Cnt == 0 ) { \
			CLK_DIVN(); \
		} \
	}
#define CLK_DOWN_OFF()		CLK_DIV1()
#else
#define CLK_DOWN_DISABLE()
#define CLK_DOWN_ENABLE()
#define CLK_DOWN_ON()
#define CLK_DOWN_OFF()
#endif


#endif //__CLK_CTRL_H__

