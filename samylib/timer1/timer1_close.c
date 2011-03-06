
// timer1
#include "timer1.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/power.h>


VOID Timer1_Close( VOID )
{
#ifdef power_timer1_disable
	power_timer1_disable();								// enable power-save mode
#endif
}

