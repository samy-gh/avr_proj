
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID Timer1_Restart( VOID )
{
	TCNT1 = 0;
}

