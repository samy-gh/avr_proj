
#include "test_sw.h"
#include "common.h"
#include <avr/io.h>


E_TEST_SW_EVENT Test_Sw_Is_Sw2Chg( VOID )
{
	const E_TEST_SW_EVENT sw_event = gTest_Sw2_Event;

	if( sw_event == E_TEST_SW_EVENT_NONE ) {
		return E_TEST_SW_EVENT_NONE;
	}

	const UCHAR sw_stat = TEST_SW2_STAT();

	gTest_Sw2_Event = E_TEST_SW_EVENT_NONE;

	if( sw_stat && (sw_event == E_TEST_SW_EVENT_ON ) ) {
		return E_TEST_SW_EVENT_ON;
	}
	else if( !sw_stat && (sw_event == E_TEST_SW_EVENT_OFF ) ) {
		return E_TEST_SW_EVENT_OFF;
	}

	return E_TEST_SW_EVENT_NONE;
}

