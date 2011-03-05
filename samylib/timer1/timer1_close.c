
#include "timer1.h"
#include "common.h"
#include <avr/io.h>

VOID Timer1_Close( VOID )
{
	sbi( PRR, PRTIM1 );								// enable power-save mode
}


