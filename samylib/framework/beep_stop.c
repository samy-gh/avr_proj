
// btn
#include "framework_beep.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/interrupt.h>


/*----------------------------------------------*/
/* Beep強制停止									*/
/*----------------------------------------------*/
VOID Beep_Stop( VOID )
{
		cli();
	{
		_gui_beep_repeat = 0;
		_gui_beep_count = 0;
	}
		sei();

	if( gBeep_PortCtrlFunc == NULL ) {
		/* ポート制御関数が無い */
		return;
	}

	gBeep_PortCtrlFunc( FALSE );
}

