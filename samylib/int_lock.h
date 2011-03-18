
#ifndef __INT_LOCK_H__
#define __INT_LOCK_H__

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


#define Int_Unlock( key )	do { SREG = (key); } while( 0 )

extern UCHAR Int_Lock( VOID );


#endif //__INT_LOCK_H__

