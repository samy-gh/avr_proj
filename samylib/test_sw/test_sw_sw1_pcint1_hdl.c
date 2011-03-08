
// test_sw
#include "test_sw.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


UCHAR gTest_Sw1_Stat;
E_TEST_SW_EVENT gTest_Sw1_Event = E_TEST_SW_EVENT_NONE;

VOID Test_Sw_Sw1Pcint1Hdl( VOID )
{
	UCHAR sw_stat;

	if( bit_is_clear( PCMSK1, PCINT10 ) ) {
		return;
	}

	sw_stat = TEST_SW1_STAT();
	if( sw_stat != gTest_Sw1_Stat ) {
		gTest_Sw1_Stat = sw_stat;
		if( sw_stat ) {
			gTest_Sw1_Event = E_TEST_SW_EVENT_ON;
		}
		else {
			gTest_Sw1_Event = E_TEST_SW_EVENT_OFF;
		}
	}

}

