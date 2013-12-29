
// プロジェクト固有
#include "custom.h"

// samylib
#include <mystdout.h>
#include <common.h>
#include <usart.h>
#include <ir.h>

// WinAVR
#include <avr/sleep.h>

// STD-C
#include <stdio.h>



#if 0
VOID ir_ctrl_set_test_ir_code( VOID )
{
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
}
#endif


VOID Ir_Ctrl_Start_Recv_KeyEventHdl( VOID )
{
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

VOID Ir_Ctrl_Start_Send_KeyEventHdl( VOID )
{
	// UART初期化
	CLK_DIV1();
	Usart_Init( 38400 );
	Usart_Set_Stdout();
	printf_P( PSTR("\nsend begin\n") );

#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
	Usart_Wait_WriteComplete();
#endif

	// eeprom読み出し＆送信
	while( 1 ) {
		if( Ir_Read_Eeprom( D_IR_EEP_ADDR ) == FALSE ) {
			break;
		}
		if( Ir_Send_Start() != 0 ) {
			break;
		}

		Ir_Frame_Dump();

		if( Ir_Send_WaitEnd() == FALSE ) {
			break;
		}


		printf_P( PSTR("\nsend comoplete\n") );

		break;
	}

	// UART後片付け
#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
	Usart_Wait_WriteComplete();
#endif
//	Usart_Close();

	CLK_DIVN();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスク

// IR受信監視
VOID Ir_Ctrl_Recv_EventHdl( VOID )
{
	// IR受信イベント(完了/エラー)
	switch( gIr_Recv_Stat ) {
		default:
			break;

		// 受信完了
		case E_IR_RECV_STAT_END:
			// 受信終了＆eeprom保存
			Ir_Recv_Stop();
			Ir_Write_Eeprom( D_IR_EEP_ADDR );

			// 受信結果表示
			Usart_Set_Stdout();
			printf_P( PSTR("\nrecv success\n") );
			Ir_Frame_Dump();
			gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
			Ir_Recv_EventHistoryDump();
#ifdef CO_LED_CTRL
			TEST_LED3_OFF();
#endif


			// UART後片付け
			set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
			Usart_Wait_WriteComplete();
#endif
//			Usart_Close();

			CLK_DIVN();
			break;


		case E_IR_RECV_STAT_ERR:
			// エラー終了
			Ir_Recv_Stop();

			// 受信結果表示
			Usart_Set_Stdout();
			printf_P( PSTR("\nrecv error\n") );
			Ir_Recv_EventHistoryDump();
			Ir_Frame_Dump();
			gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
			gIr_Recv_Err = E_IR_RECV_ERR_NONE;
#ifdef CO_LED_CTRL
			TEST_LED3_ON();
#endif

			// UART後片付け
			set_sleep_mode( SLEEP_MODE_PWR_DOWN );
#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
			Usart_Wait_WriteComplete();
#endif
//			Usart_Close();

			CLK_DIVN();
			break;
	}
}

