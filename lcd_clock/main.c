
// プロジェクト固有
#include "framework_hal.h"

// samylib
#include <mystdout.h>
#include <usart.h>
#include <lcd_hd44780.h>
#include <test_led.h>
#include <framework_event_loop.h>

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// STD-C
#include <stdio.h>


// メインループの動作を見る
#define CO_MAINLOOP_MONITOR


VOID setup( VOID )
{
	// WDT停止
	WDTCSR = 0x00;

	// パワーセーブ
#ifdef CO_SLEEP_ENABLE
	set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
//	set_sleep_mode( SLEEP_MODE_STANDBY );
//	set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#endif
#ifdef power_all_disable
	power_all_disable();
#endif

	// USART初期化
	Usart_Init( 38400 );

	// LCD初期化
	Lcd_Init();

	framework_init();

	TEST_LED1_ON();
	TEST_LED2_OFF();
	TEST_LED3_OFF();


	// 割り込み許可
	sei();
}


VOID print_banner( VOID )
{
	//
	// バナー表示
	//
	// ローカル標準出力関数のテスト
	prog_char* hello_str = PSTR("HIDlib FW sample");
	Usart_Set_Stdout();
	Mystdout_PgmPuts( hello_str );
	Mystdout_Putc( '\n' );
	Lcd_Set_Stdout();
	Mystdout_PgmPuts( hello_str );
}


VOID main( VOID ) __attribute__((noreturn));
VOID main( VOID )
{
	setup();
	print_banner();

	while( 1 ) {
		event_loop();
		// no reached
	}
}

