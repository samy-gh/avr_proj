
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

#include "common.h"
#include "mystdout.h"
#include "usart.h"
#include "lcd_hd44780.h"
#include "timer1.h"



INT usart_poll( VOID )
{
	INT c;

	c = Usart_Read();
	if( c > 0 ) {
		Usart_Write( c );
	}

	return c;
}

UCHAR timer1_flg = 0;
#define D_TIMER1_PERIOD_USEC 10000UL


VOID timer1_ovf_hdl( VOID )
{
	timer1_flg = 1;

	sleep_disable();
}


VOID setup( VOID )
{
	// WDT停止
	WDTCSR = 0x00;


	// パワーセーブ
	set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
//	set_sleep_mode( SLEEP_MODE_STANDBY );
//	set_sleep_mode( SLEEP_MODE_PWR_DOWN );
	PRR = 0xFF;


	// PC3=out
	sbi( DDRC, DDC3 );


	// PCINTn Enable
	sbi( PCICR, PCIE1 );


	// USART初期化
	Usart_Init( 38400 );


	// LCD初期化
	Lcd_Init();


	// Timer1初期化
	Timer1_Init( D_TIMER1_PERIOD_USEC );
	Timer1_Enable_CompaIntr( timer1_ovf_hdl );


	// 割り込み許可
	sei();
}

UCHAR volatile g_ir_recv_start = 0;
UCHAR volatile g_pcint_assert = 0;
UCHAR volatile g_ir_recv_pin_last = 0;
UINT volatile g_ir_timer_count;

#define D_IR_RECV_PIN	(PINC & _BV(PINC0))

// Pin change Interrupt処理
SIGNAL(PCINT1_vect)
{
	g_pcint_assert++;

	if( g_ir_recv_start ) {
		UCHAR ir_recv_pin = D_IR_RECV_PIN;

		if( ir_recv_pin != g_ir_recv_pin_last ) {
			g_ir_recv_pin_last = ir_recv_pin;
			// IRは負論理
			if( ir_recv_pin == 0 ) {
				// 0->1 (Hi->Lo)
				Timer1_Restart();
				timer1_flg = 0;
	PORTC ^= _BV(PC3);

			}
			else {
				// 0->1 (Hi->Lo)
				g_ir_timer_count = TCNT1;
				Timer1_Stop();
	PORTC ^= _BV(PC3);
	g_ir_recv_start = 0;

			}
		}
	}

	sleep_disable();
}


VOID ir_recv_start( VOID )
{
	g_ir_recv_pin_last = D_IR_RECV_PIN;

	// PortC0(PCINT8) enable
	sbi( PCMSK1, PCINT8 );

	// すでに立っているPCINT8を空処理(割り込みハンドラ動作を期待)


	Timer1_Start();

	g_ir_recv_start = 1;
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

	Usart_Set_Stdout();
	printf_P( PSTR("timer1 clock sel=%X\ntimer1 period=%u\n"), gTimer1_ClockSelectBits, gTimer1_PwmPeriod );

	ir_recv_start();

	ULONG loopcnt = 0;
	while( 1 ) {	/* main event loop */
		// すべての割り込みハンドラ内でsleep_disable()を実施している。
		// そうすることで、ループ先頭のsleep_enable()～ループ終端のsleep()までの間に割り込みが
		// 発生してもsleep()しなくなる。
		// sleep()するのは、sleep_enable()～sleep()までの間に割り込みが一度も発生しない場合のみとなる。
		cli();
		sleep_enable();
		sei();

		if( usart_poll() == 'a' ) {
			ir_recv_start();
		}

		if( g_ir_timer_count ) {
			Usart_Set_Stdout();
			if( timer1_flg ) {
				puts_P( PSTR("overflow\n") );
			}
			else {
				printf_P( PSTR("timer1=%5u %lu usec\n"),
						g_ir_timer_count,
						D_TIMER1_PERIOD_USEC * g_ir_timer_count / gTimer1_PwmPeriod
						);
			}
			g_ir_timer_count  = 0;
		}
#if 0
		Lcd_Set_Stdout();
		Lcd_Goto( 0, 1 );
//		Mystdout_PrintDigit( loopcnt );
		UINT count = TCNT1;
		printf_P( PSTR("%5u"), count );
#endif

#if 0
		if( timer1_flg == 1) {
			timer1_flg = 0;
			Usart_Set_Stdout();
			puts_P( PSTR("timer1\n") );

		}
#endif
		loopcnt++;
		sleep_cpu();
	}
}

