
// プロジェクト固有
#include "custom.h"

// samylib
#include <mystdout.h>
#include <common.h>
#include <usart.h>
#include <lcd_hd44780.h>
#include <test_led.h>
#include <test_sw.h>
#include <ir.h>

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>

// STD-C
#include <stdio.h>



#define D_IR_EEP_ADDR		0


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// 割り込みハンドラ


// Pin Change Interrupt処理
SIGNAL(PCINT1_vect)
{
	IR_RECV_PCINT8_HDL();

	Test_Sw_Sw1Pcint1Hdl();
	Test_Sw_Sw2Pcint1Hdl();

#ifdef CO_SLEEP_ENABLE
	sleep_disable();
#endif
}



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスク

INT usart_poll( VOID )
{
	INT c;

	c = Usart_Read();
	if( c > 0 ) {
		Usart_Write( c );
	}

	return c;
}

VOID print_banner( VOID )
{
	//
	// バナー表示
	//
	prog_char* line_feed_str = PSTR("\n\n");
	prog_char* hello_str = PSTR("IR Remo-con");
	Usart_Set_Stdout();
	printf_P( line_feed_str );
	printf_P( hello_str );
	printf_P( line_feed_str );
#ifdef CO_LCD_CTRL
	Lcd_Set_Stdout();
	printf_P( hello_str );
	Lcd_Close( TRUE );
#endif

#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
	Usart_Wait_WriteComplete();
#endif
}

VOID setup( VOID )
{
	// WDT停止
	WDTCSR = 0x00;


	// パワーセーブ
#ifdef CO_SLEEP_ENABLE
//	set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
//	set_sleep_mode( SLEEP_MODE_STANDBY );
	set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#endif
#ifdef power_all_disable
	power_all_disable();
#endif



#ifdef CO_PCINT_MONITOR_PC3
	// PC3=out
	sbi( DDRC, DDC3 );
#endif


	TEST_SW1_ENABLE();
	TEST_SW2_ENABLE();


#ifdef CO_LED_CTRL
	TEST_LED1_ON();
#endif


	// PCINTn Enable
	sbi( PCICR, PCIE1 );


	// USART初期化
	Usart_Init( 38400 );


	// LCD初期化
#ifdef CO_LCD_CTRL
	Lcd_Init();
#endif


	// 割り込み許可
	sei();
}

VOID main( VOID ) __attribute__((noreturn));
VOID main( VOID )
{
#ifdef CO_MAINLOOP_MONITOR
	ULONG loopcnt = 0;
#endif
	INT usart_recv_c;

	setup();
	print_banner();

#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("timer1 clock sel=%X\ntimer1 period=%u\n"), gTimer1_ClockSelectBits, gTimer1_PwmPeriod );
#endif

	CLK_DIVN();

	while( 1 ) {	/* main event loop */
		cbi( PORTD, PD5 );
		sbi( DDRD, DDD5 );

#ifdef CO_SLEEP_ENABLE
		// すべての割り込みハンドラ内でsleep_disable()を実施している。
		// そうすることで、ループ先頭のsleep_enable()～ループ終端のsleep()までの間に割り込みが
		// 発生してもsleep()しなくなる。
		// sleep()するのは、sleep_enable()～sleep()までの間に割り込みが一度も発生しない場合のみとなる。
		cli();
		sleep_enable();
		sei();
#endif


		// シリアル受信イベント
		usart_recv_c = usart_poll();
#if 0
		if( usart_recv_c > 0 ) {
			switch( gIr_Recv_Stat ) {
				default:
					Ir_Recv_EventHistoryDump();
					break;
				case E_IR_RECV_STAT_IDLE:
					Usart_Set_Stdout();
					printf_P( PSTR("\nrecv begin\n") );
					Ir_Recv_Start();
					break;
			}
		}
#endif

		if( Test_Sw_Is_Sw1Chg() == E_TEST_SW_EVENT_ON ) {
			switch( gIr_Recv_Stat ) {
				default:
					Ir_Recv_EventHistoryDump();
					break;
				case E_IR_RECV_STAT_IDLE:
					CLK_DIV1();
					Usart_Init( 38400 );
					Usart_Set_Stdout();
					printf_P( PSTR("\nrecv begin\n") );
					Ir_Recv_Start();
					set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
					break;
			}
		}

		if( Test_Sw_Is_Sw2Chg() == E_TEST_SW_EVENT_ON ) {
			CLK_DIV1();
			Usart_Init( 38400 );
			Usart_Set_Stdout();
			printf_P( PSTR("\nsend begin\n") );
#if 0
			{	// SONY CDチェンジャー
				UCHAR frame[] = { 0xA9, 0x14 };
				Ir_Frame_Set( E_IR_FRAME_TYPE_SONY, frame, sizeof(frame), 2 );
			}
#endif
#if 0
			{	// 東芝TV
				UCHAR frame[] = { 0x02, 0xFD, 0x48, 0xB7 };
				Ir_Frame_Set( E_IR_FRAME_TYPE_NEC, frame, sizeof(frame), 1 );
			}
#endif
#if 0
			{	// 東芝エアコン
				UCHAR frame[] = { 0xF2, 0x0D, 0x04, 0xFB, 0x09, 0x00, 0x83, 0x00, 0x04, 0x8E };
				Ir_Frame_Set( E_IR_FRAME_TYPE_TOSHIBA, frame, sizeof(frame), 1 );
			}
#endif
			Ir_Read_Eeprom( D_IR_EEP_ADDR );
			Ir_Send_Start();
			Ir_Frame_Dump();
			Ir_Send_WaitEnd();
			Test_Sw_Is_Sw2Chg();
			printf_P( PSTR("\nsend comoplete\n") );

#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
			Usart_Wait_WriteComplete();
#endif
			Usart_Close();

			CLK_DIVN();
		}

		// IR受信イベント(完了/エラー)
		switch( gIr_Recv_Stat ) {
			default:
				break;
			case E_IR_RECV_STAT_END:
				Ir_Recv_Stop();
				Ir_Write_Eeprom( D_IR_EEP_ADDR );
				Usart_Set_Stdout();
				printf_P( PSTR("\nrecv success\n") );
				Ir_Frame_Dump();
				gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
				Ir_Recv_EventHistoryDump();
#ifdef CO_LED_CTRL
				TEST_LED3_OFF();
#endif

				set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
				Usart_Wait_WriteComplete();
#endif
				Usart_Close();

				CLK_DIVN();
				break;
			case E_IR_RECV_STAT_ERR:
				Ir_Recv_Stop();
				Usart_Set_Stdout();
				printf_P( PSTR("\nrecv error\n") );
				Ir_Recv_EventHistoryDump();
				Ir_Frame_Dump();
				gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
				gIr_Recv_Err = E_IR_RECV_ERR_NONE;
#ifdef CO_LED_CTRL
				TEST_LED3_ON();
#endif

				set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
				Usart_Wait_WriteComplete();
#endif
				Usart_Close();

				CLK_DIVN();
				break;
		}


		// メインループ受信モニタ用
#ifdef CO_MAINLOOP_MONITOR
#ifdef CO_LCD_CTRL
		Lcd_Init();
		Lcd_Set_Stdout();
		Lcd_Goto( 0, 1 );
		printf_P( PSTR("%5u"), loopcnt );
		Lcd_Close();
#endif
		loopcnt++;
#endif

#ifdef CO_SLEEP_ENABLE
		sleep_cpu();
#endif
	}
}

