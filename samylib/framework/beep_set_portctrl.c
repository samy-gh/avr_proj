
// timer0
#include "framework_beep.h"

// samylib
#include "my_typedef.h"
#include "common.h"


VOID Beep_Set_PortCtrl( VOID (*ctrl_func)( BOOL onoff ) )
{
	gBeep_PortCtrlFunc = ctrl_func;		// register the user's callback with the real ISR
}


