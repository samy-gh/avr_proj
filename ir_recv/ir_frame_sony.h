
#ifndef __IR_FRAME_SONY_H__
#define __IR_FRAME_SONY_H__

// samylib
#include <my_typedef.h>


// SONY形式フレームデータ: T_IR_SONY_FRAME
typedef struct {
	UCHAR data;		// 6bit
	UINT  addr;		// 4/7/11bit
} T_IR_SONY_FRAME;

#endif //__IR_FRAME_SONY_H__

