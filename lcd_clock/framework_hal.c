
// プロジェクト固有
#include "framework_hal.h"
#include "test_led.h"

// samylib
#include <mystdout.h>
#include <timer2.h>
#include <i2c.h>
#include <usart.h>
#include <framework_clock_hdl.h>
#include <framework_btn.h>
#include <framework_msec2clock.h>
#include <framework_i2c.h>

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <avr/pgmspace.h>
#include <stdio.h>


// Pin Change Interrupt処理
SIGNAL(PCINT0_vect)
{
	TEST_LED2_TOGGLE();

#ifdef CO_ENABLE_PCINT0_BTN
	if( bit_is_set( D_BTN_REG_PCINT0_PCMSK, D_BTN_BIT_PCINT0_PCMSK ) ) {
		btn_chg_int( &gt_pcint0_btn );
	}
#endif
#ifdef CO_ENABLE_PCINT1_BTN
	if( bit_is_set( D_BTN_REG_PCINT1_PCMSK, D_BTN_BIT_PCINT1_PCMSK ) ) {
		btn_chg_int( &gt_pcint1_btn );
	}
#endif

#ifdef CO_SLEEP_ENABLE
	sleep_disable();
#endif
}

// Pin Change Interrupt処理
SIGNAL(PCINT1_vect)
{
	TEST_LED3_TOGGLE();

#if 1
#ifdef CO_ENABLE_PCINT10_BTN
	if( bit_is_set( D_BTN_REG_PCINT10_PCMSK, D_BTN_BIT_PCINT10_PCMSK ) ) {
		btn_chg_int( &gt_pcint10_btn );
	}
#endif
#ifdef CO_ENABLE_PCINT11_BTN
	if( bit_is_set( D_BTN_REG_PCINT11_PCMSK, D_BTN_BIT_PCINT11_PCMSK ) ) {
		btn_chg_int( &gt_pcint11_btn );
	}
#endif

#endif
#ifdef CO_SLEEP_ENABLE
	sleep_disable();
#endif
}


VOID framework_init( VOID )
{
	btn_init( CLOCK_COUNT_1SEC, CLOCK_COUNT_1SEC / 10 );
	msec2clock_init( CLOCK_COUNT_1SEC );
	i2c_slave_read_event_init();

	(VOID)i2c_Set_SlaveAddress_FromEeprom( 0 );

	i2c_Init_MasterWrite( 100000UL );
	i2c_Init_SlaveRead( 100000UL );

	Timer2_Init( 1 );
	Timer2_Set_Period_Direct( _BV(CS20) | _BV(CS21) | _BV(CS22), 125 );
//	Timer2_Set_Period_Direct( _BV(CS22), 250 );
	Timer2_Enable_CompaIntr( clock_hdl );
	Timer2_Start();

	Usart_Set_Stderr();
}

