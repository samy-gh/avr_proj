
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Restart( VOID )
{
	TCNT1 = 0;
}

