
// プロジェクト固有
#include "custom.h"
#include "clk_ctrl.h"
#include "ir_ctrl.h"
#include "usb_proto.h"

// samylib
#include <mystdout.h>
#include <common.h>
#include <usart.h>
#include <ir.h>
#include <test_led.h>


// WinAVR
#include <avr/sleep.h>
#include <util/delay.h>

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


#define IR_RECV_POW_ON()		( sbi( DDRC, DDC1 ), sbi( PORTC, PC1 ), _delay_ms(10) )
#define IR_RECV_POW_OFF()		( cbi( DDRC, DDC1 ), cbi( PORTC, PC1 ) )



// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// ハンドラ

// IR受信監視 ハンドラ(mainloop)から呼ぶ
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
			IR_RECV_POW_OFF();
			Ir_Write_Eeprom( D_IR_EEP_ADDR );

			// 受信結果表示
			printf_P( PSTR("\nrecv success\n") );
			Ir_Frame_Dump();
			gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
			Ir_Recv_EventHistoryDump();

			TEST_LED1_OFF();

			CLK_DOWN_ENABLE();
			break;


		case E_IR_RECV_STAT_ERR:
			// エラー終了
			Ir_Recv_Stop();
			IR_RECV_POW_OFF();

			// 受信結果表示
			printf_P( PSTR("\nrecv error\n") );
			Ir_Recv_EventHistoryDump();
			Ir_Frame_Dump();
			gIr_Recv_Stat = E_IR_RECV_STAT_IDLE;
			gIr_Recv_Err = E_IR_RECV_ERR_NONE;

			TEST_LED1_OFF();

			CLK_DOWN_ENABLE();
			break;
	}
}



static VOID ir_ctrl_set_frame( T_USB_PROTO_IR_CODE* const buf )
{
	E_IR_FRAME_TYPE type;

	switch( buf->frame_type ) {
		case D_USB_IR_FRAME_TYPE_NEC:
			type = E_IR_FRAME_TYPE_NEC;
			break;
		case D_USB_IR_FRAME_TYPE_AEHA:
			type = E_IR_FRAME_TYPE_AEHA;
			break;
		case D_USB_IR_FRAME_TYPE_SONY:
			type = E_IR_FRAME_TYPE_SONY;
			break;
		case D_USB_IR_FRAME_TYPE_TOSHIBA:
			type = E_IR_FRAME_TYPE_TOSHIBA;
			break;
		default:
			return;
	}

	Ir_Frame_Set( type, buf->data, buf->bytes, buf->bit_pos );

	Ir_Frame_Dump();
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// タスク

// IR受信しEEPROMへ保存
VOID Ir_Ctrl_Start_Recv_KeyEventHdl( VOID )
{
	switch( gIr_Recv_Stat ) {
		default:
			Ir_Ctrl_Abort_Recv_KeyEventHdl();
			break;
		case E_IR_RECV_STAT_IDLE:
			CLK_DOWN_DISABLE();

			TEST_LED1_ON();

			printf_P( PSTR("\nrecv begin\n") );
			IR_RECV_POW_ON();
			Ir_Recv_Start();
			break;
	}
}


// 受信処理を中断
VOID Ir_Ctrl_Abort_Recv_KeyEventHdl( VOID )
{
	printf_P( PSTR("\nrecv abort\n") );
	Ir_Recv_Stop();

	TEST_LED1_OFF();

	CLK_DOWN_ENABLE();
}


// EEPROM内容をIR送信
BOOL Ir_Ctrl_Start_Send_KeyEventHdl( VOID )
{
	BOOL retcd = FALSE;

	// UART初期化
	printf_P( PSTR("\nsend begin\n") );

	CLK_DOWN_DISABLE();

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

		retcd = TRUE;
		break;
	}

	CLK_DOWN_ENABLE();

	return retcd;
}


// EEPROM上のIRコードをHostへ送る
INT Ir_Ctrl_Get_IrCodeEeprom( T_USB_PROTO_IR_CODE* const buf )
{
	if( Ir_Read_Eeprom( D_IR_EEP_ADDR ) == FALSE ) {
		return -1;
	}
	return Ir_Ctrl_Get_IrCode( buf );
}


// メモリ上のIRコードをHostへ送る
INT Ir_Ctrl_Get_IrCode( T_USB_PROTO_IR_CODE* const buf )
{
	UCHAR loopcnt;
	UCHAR byte_idx;
	UCHAR bit_mask;

	if( (gIr_Frame.byte_idx == 0) && (gIr_Frame.bit_mask == 0x80) ) {
		// empty
		return 0;
	}

	byte_idx = gIr_Frame.byte_idx;
	if( gIr_Frame.bit_mask == 0x80 ) {
		bit_mask = 0x01;
	}
	else {
		bit_mask = gIr_Frame.bit_mask << 1;
		byte_idx++;
	}

	switch( gIr_Frame.type ) {
		case E_IR_FRAME_TYPE_NEC:
			buf->frame_type = D_USB_IR_FRAME_TYPE_NEC;
			break;
		case E_IR_FRAME_TYPE_AEHA:
			buf->frame_type = D_USB_IR_FRAME_TYPE_AEHA;
			break;
		case E_IR_FRAME_TYPE_SONY:
			buf->frame_type = D_USB_IR_FRAME_TYPE_SONY;
			break;
		case E_IR_FRAME_TYPE_TOSHIBA:
			buf->frame_type = D_USB_IR_FRAME_TYPE_TOSHIBA;
			break;
		default:
			return -1;
	}

	buf->bytes = byte_idx;
	buf->bit_pos = bit_mask;
	for( loopcnt = 0; loopcnt < byte_idx; loopcnt++ ) {
		buf->data[loopcnt] = gIr_Frame.data[loopcnt];
	}

	return 3 + buf->bytes;
}


// Hostから受け取ったIRコードを送信する
BOOL Ir_Ctrl_Send_IrCode( T_USB_PROTO_IR_CODE* const buf )
{
	BOOL retcd = FALSE;

	ir_ctrl_set_frame( buf );

	// UART初期化
	printf_P( PSTR("\nsend begin\n") );

	CLK_DOWN_DISABLE();

	TEST_LED1_ON();

	// eeprom読み出し＆送信
	while( 1 ) {

		if( Ir_Send_Start() != 0 ) {
			break;
		}

		Ir_Frame_Dump();

		if( Ir_Send_WaitEnd() == FALSE ) {
			break;
		}


		printf_P( PSTR("\nsend comoplete\n") );

		retcd = TRUE;
		break;
	}

	TEST_LED1_OFF();

	CLK_DOWN_ENABLE();

	return retcd;
}


// Hostから受け取ったIRコードをEEPROMへ書き込む
BOOL Ir_Ctrl_Set_Eeprom( T_USB_PROTO_IR_CODE* const buf )
{
	BOOL retcd = FALSE;

	TEST_LED1_ON();

	ir_ctrl_set_frame( buf );
	Ir_Write_Eeprom( D_IR_EEP_ADDR );

	TEST_LED1_OFF();

	retcd = TRUE;

	return retcd;
}

