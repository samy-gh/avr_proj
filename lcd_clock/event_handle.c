
// プロジェクト固有
#include "framework_hal.h"

// samylib
#include <mystdout.h>
#include <usart.h>
#include <lcd_hd44780.h>
#include <test_led.h>
#include <framework_event_loop.h>
#include <framework_btn.h>
#include <framework_clock_hdl.h>
#include <framework_softtimer.h>
#include <framework_i2c.h>
#include <i2c.h>
#include <ir.h>

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/eeprom.h>

// STD-C
#include <stdio.h>
#include <string.h>


/*----------------------------------------------*/
/* ステータス・イベント定義						*/
/*----------------------------------------------*/
/* イベントハンドラ */
static INT do_nop( VOID );
static INT do_serial_dump( VOID );
static INT do_i2c_write( VOID );
static INT do_i2c_write_cmd( VOID );
static INT do_i2c_test_stat( VOID );
static INT do_i2c_read( VOID );
static INT do_i2c_init( VOID );
static INT do_i2c_eeprom_test( VOID );
static INT do_i2c_addr1( VOID );
static INT do_i2c_addr2( VOID );
static INT do_timer( VOID );
static INT do_timer_stop( VOID );
static INT do_voice_test( VOID );
static INT do_ir_send_start( VOID );
static INT do_ir_send_end( VOID );
static INT do_ir_recv_start( VOID );
static INT do_ir_recv_end( VOID );
static INT do_ir_recv_err( VOID );

/* システムステータス種類 */
typedef enum {
	E_STAT_IDLE = E_STAT_SYS_IDLE,
	E_STAT_ADJ_HOUR,
	E_STAT_ADJ_MIN,
	E_STAT_ADJ_SEC,
	E_STAT_MAX
} TE_SYS_STAT;

/* ユーザイベント種類 */
typedef enum  {
	/* ユーザイベントなし */
	E_EVENT_SW_PRESS = E_EVENT_USER,		/* 128:スイッチ押下 */
	E_EVENT_SW_LONGPRESS,					/* 129:スイッチ長押し */
	E_EVENT_TO_TICK,						/* 130:1秒タイマ T.O. */
	E_EVENT_TO_SET_TIME,					/* 131:時刻調整タイマ T.O. */
	E_EVENT_TO_TIMER3,						/* 132:タイマ3 T.O. */
	E_EVENT_OUTPUT_CONSOLE,					/* 133:コンソール出力要求 */
	E_EVENT_SW_ADJ_PUSH,					/* 134:時刻合わせボタン押下 */
	E_EVENT_SERIAL_DUMP,					/* 135:シリアルからのダンプコマンド */
	E_EVENT_I2C_SEND,						/* 136: */
	E_EVENT_I2C_ADDR1,						/* 137: */
	E_EVENT_I2C_ADDR2,						/* 138: */
	E_EVENT_I2C_EEPROM_TEST,				/* 139: */
	E_EVENT_I2C_INIT,						/* 140: */
	E_EVENT_I2C_TEST_STAT,					/* 141: */
	E_EVENT_VOICE_TEST,						/* 142: */
	E_EVENT_MAX
} TE_SYS_EVENT;

/* イベント-ハンドラ テーブル */
T_SYS_EVENT_TBL gt_sys_event_tbl_idle[] = {
	{ E_EVENT_TO_TICK,				do_i2c_write },
	{ E_EVENT_BTN0_PUSH,			do_nop },
	{ E_EVENT_BTN0_LONGPUSH,		do_nop },
	{ E_EVENT_BTN0_RELEASE,			do_nop },
	{ E_EVENT_BTN10_PUSH,			do_ir_send_start },
//	{ E_EVENT_BTN10_LONGPUSH,		do_i2c_write_cmd },
	{ E_EVENT_BTN10_RELEASE,		do_nop },
//	{ E_EVENT_BTN11_PUSH,			do_i2c_eeprom_test },
	{ E_EVENT_BTN11_PUSH,			do_ir_recv_start },
	{ E_EVENT_BTN11_LONGPUSH,		do_nop },
	{ E_EVENT_BTN11_RELEASE,		do_nop },
	{ E_EVENT_IR_RECV_END,			do_ir_recv_end },
	{ E_EVENT_IR_RECV_ERR,			do_ir_recv_err },
	{ E_EVENT_IR_SEND_END,			do_ir_send_end },
	{ E_EVENT_TO_SET_TIME,			do_nop },
	{ E_EVENT_SW_ADJ_PUSH,			do_timer_stop },
	{ E_EVENT_BEEP_END,				do_nop },
	{ E_EVENT_IDLE,					do_nop },
	{ E_EVENT_TO_TIMER3,			do_nop },
	{ E_EVENT_INT_NMI,				do_nop },
	{ E_EVENT_OUTPUT_CONSOLE,		do_timer },
	{ E_EVENT_SERIAL_DUMP,			do_serial_dump },
	{ E_EVENT_I2C_EEPROM_TEST,		do_i2c_eeprom_test },
	{ E_EVENT_I2C_INIT,				do_i2c_init },
	{ E_EVENT_I2C_SEND,				do_i2c_write_cmd },
	{ E_EVENT_I2C_ADDR1,			do_i2c_addr1 },
	{ E_EVENT_I2C_ADDR2,			do_i2c_addr2 },
	{ E_EVENT_I2C_SLAVE_READ,		do_i2c_read },
	{ E_EVENT_I2C_TEST_STAT,		do_i2c_test_stat },
	{ E_EVENT_VOICE_TEST,			do_voice_test },
	{ 0xffff, NULL } /* terminator */
};

/* 状態-イベントテーブル テーブル */
T_SYS_STAT_TBL gt_sys_stat_tbl[] = {
	{ E_STAT_IDLE,					gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_HOUR,				gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_MIN,				gt_sys_event_tbl_idle },
	{ E_STAT_ADJ_SEC,				gt_sys_event_tbl_idle },
	{ 0xffff, NULL } /* terminator */
};

/* シリアルコマンド-イベントテーブル */
T_SERIAL_CMD gt_serial_event_tbl[] = {
	{ 'c', E_EVENT_TO_TICK },
	{ 't', E_EVENT_OUTPUT_CONSOLE },
	{ 'r', E_EVENT_BTN0_PUSH },
	{ 'R', E_EVENT_SW_LONGPRESS },
	{ 'm', E_EVENT_SW_ADJ_PUSH },
	{ 'd', E_EVENT_SERIAL_DUMP },
	{ ' ', E_EVENT_SERIAL_DUMP },
	{ 'T', E_EVENT_SW_ADJ_PUSH },
	{ 'w', E_EVENT_I2C_SEND },
	{ '1', E_EVENT_I2C_ADDR1 },
	{ '2', E_EVENT_I2C_ADDR2 },
	{ 'i', E_EVENT_I2C_INIT },
	{ 'e', E_EVENT_I2C_EEPROM_TEST },
	{ 's', E_EVENT_I2C_TEST_STAT },
	{ 'v', E_EVENT_VOICE_TEST },
	{ '\0', -1 } /* terminator */
};


/*----------------------------------------------*/
/* ソフトタイマ定義								*/
/*----------------------------------------------*/
/* タイマ種類 */
enum {
	E_TIMER_CLOCK_SEC,		/* 時計用秒タイマ */
	E_TIMER_SET_TIME,		/* 時刻調整タイマ */
	E_TIMER_3,				/* タイマ3 */
	E_TIMER_MAX
} E_TIMER;

/* ソフトタイマ パラメータ */
volatile T_TIMER_PARAM gt_timer_param[E_TIMER_MAX] = {
	/* t.o._msec                cycle_mode  t.o._event */
	{ (1000UL / CLOCK_COUNT_1SEC),	1,		E_EVENT_TO_TICK },	/* 約0.31秒 */
//	{ 1000UL,					1,			E_EVENT_TO_TICK },	/* 1秒 */
	{ 250UL,					1,			E_EVENT_TO_SET_TIME },	/* 0.25秒 */
	{ (3UL * 60 * 60 * 1000),	0,			E_EVENT_TO_TIMER3 },	/* 3時間 */
};
volatile UINT gt_event_timer[E_TIMER_SYSTEM_MAX] = { 0 };

UINT gui_timer_param_num = E_TIMER_MAX;


/*----------------------------------------------*/
/* イベントハンドラ共通							*/
/*----------------------------------------------*/
static INT do_nop( VOID )
{
	return 0;
}

static INT do_timer( VOID )
{
	softtimer_start( E_TIMER_CLOCK_SEC );
	return 0;
}

static INT do_timer_stop( VOID )
{
	softtimer_stop( E_TIMER_CLOCK_SEC );
	return 0;
}

static INT do_serial_dump( VOID )
{
	fprintf_P( stderr, PSTR("** serial_dump **\n") );
	fprintf_P( stderr, PSTR("clk=%u\n"), _gui_clock_count );
	fprintf_P( stderr, PSTR("pcicr=0x%x pcmsk0=0x%x pcmsk1=0x%x pcmsk2=0x%x clk=%u\n"), PCICR, PCMSK0, PCMSK1, PCMSK2, _gui_clock_count );

	btn_print( stderr );

	return 0;
}

ULONG send_count = 0;
ULONG recv_count = 0;

static INT do_i2c_test_stat( VOID )
{
	fprintf_P( stderr, PSTR("send=%lu recv=%lu\n"), send_count, recv_count );
	return 0;
}

static INT do_i2c_write( VOID )
{
	UCHAR result;
	UCHAR slave_addr;
	static UCHAR count = 0;
	UCHAR snd_buf[4];

	snd_buf[0] = count +0;
	snd_buf[1] = count +1;
	snd_buf[2] = count +2;
	snd_buf[3] = count +3;

	slave_addr = eeprom_read_byte( (uint8_t*)1 );
	result = i2c_Write_Master( slave_addr, snd_buf, sizeof(snd_buf), 1 );
	if( result != 0 ) {
		fprintf_P( stderr, PSTR("result = %u\n"), result );
	}
	else {
		count += 4;
	}
	send_count++;
	return 0;
}

static INT do_i2c_read( VOID )
{
	UCHAR* buf_addr;
	UINT buf_len;
	UINT loopcnt;
	static UCHAR count = 0;

//	fprintf_P( stderr, PSTR("** I2C read test **\n") );
	i2c_slave_read_event_get_buf( &buf_addr, &buf_len );
	recv_count++;

	if( buf_len == 0 ) {
		fprintf_P( stderr, PSTR("len=%u\n"), buf_len );
		return -1;
	}

	for( loopcnt = 0; loopcnt < buf_len; loopcnt++ ) {
		if( buf_addr[loopcnt] != count ) {
			fprintf_P( stderr, PSTR("recv err byte%u=%02x and=%02x\n"), loopcnt, buf_addr[loopcnt], count );
		}
		count++;
	}

	return 0;
}

static INT do_i2c_write_cmd( VOID )
{
	UCHAR result;
	UCHAR slave_addr;

	slave_addr = eeprom_read_byte( (uint8_t*)1 );

	fprintf_P( stderr, PSTR("** I2C write test to %u **\n"), slave_addr );

	i2c_Init_MasterWrite( 100000UL );
	result = i2c_Write_Master( slave_addr, (UCHAR*)"\xde\xad\xbe\xef", 4, 1 );
	fprintf_P( stderr, PSTR("result = %u\n"), result );
	return 0;
}

#if 0
static INT do_i2c_read_cmd( VOID )
{
	UCHAR* buf_addr;
	UINT buf_len;
	UINT loopcnt;

	fprintf_P( stderr, PSTR("** I2C read test **\n") );
	i2c_slave_read_event_get_buf( &buf_addr, &buf_len );

	fprintf_P( stderr, PSTR("len=%u\n"), buf_len );
	for( loopcnt = 0; loopcnt < buf_len; loopcnt++ ) {
		fprintf_P( stderr, PSTR("%02x\n"), buf_addr[loopcnt] );
	}

	return 0;
}
#endif

static INT do_i2c_init( VOID )
{
	UCHAR addr;
	addr = i2c_Set_SlaveAddress_FromEeprom( 0 );
	i2c_Init_SlaveRead( 100000UL );

	fprintf_P( stderr, PSTR("** I2C init %u **\n"), addr );

	return 0;
}

static INT do_i2c_eeprom_test( VOID )
{
	UCHAR result;
	fprintf_P( stderr, PSTR("** I2C serial eeprom test **\n") );

	fprintf_P( stderr, PSTR("** I2C 2 **\n") );
	result = i2c_Write_Master( 0x51, (UCHAR*)"\x00\x00", 2, 1 );
	fprintf_P( stderr, PSTR("result = %u\n"), result );

	return 0;
}

static INT do_i2c_addr1( VOID )
{
	fprintf_P( stderr, PSTR("** I2C addr write 1 **\n") );

	eeprom_write_byte( (uint8_t*)0, 0x01 );
	eeprom_write_byte( (uint8_t*)1, 0x02 );

	return 0;
}

static INT do_i2c_addr2( VOID )
{
	fprintf_P( stderr, PSTR("** I2C addr write 2 **\n") );

	eeprom_write_byte( (uint8_t*)0, 0x02 );
	eeprom_write_byte( (uint8_t*)1, 0x01 );

	return 0;
}

static INT do_voice_test( VOID )
{
	UCHAR result;
	UCHAR* str = (UCHAR*)"komattana-\r";

	fprintf_P( stderr, PSTR("** voice test **\n") );

	i2c_Init_MasterWrite( 100000UL );
	result = i2c_Write_Master( 0x2E, str, strlen((char*)str), 1 );

	fprintf_P( stderr, PSTR("** result = %u **\n"), result );

	return 0;
}

static INT do_ir_send_start( VOID )
{
	fprintf_P( stderr, PSTR("** IR send test **\n") );

#if 1
//	Lcd_Close( 0 );

	// デバッグ用 送信フレーム
#if 0
	Ir_Frame_Set(
			E_IR_FRAME_TYPE_SONY,
			(UCHAR*)"\x95\x07",
			2,
			0x10
			);
#endif
#if 1
	Ir_Frame_Set(
			E_IR_FRAME_TYPE_SONY,
			(UCHAR*)"\xA9\xE0",
			2,
			0x10
			);
#endif
#if 0
	Ir_Frame_Set(
			E_IR_FRAME_TYPE_AEHA,
			(UCHAR*)"\x23\xCB\x26\x01\x00\x24\x03\x0A\x00\x00\x00\x00\x8E\xD4",
			14,
			0x01
			);
#endif
#if 0
	Ir_Frame_Set(
			E_IR_FRAME_TYPE_AEHA,
			(UCHAR*)"\x55\xAA",
			2,
			0x01
			);
#endif


	Ir_Send_Start();
//	Ir_Send_WaitEnd();
//	Ir_Send_Stop();
//	Lcd_Open();
#endif

	fprintf_P( stderr, PSTR("end\n") );

	return 0;
}

static INT do_ir_send_end( VOID )
{
	fprintf_P( stderr, PSTR("** IR send end event **\n") );

	Ir_Send_Stop();

	fprintf_P( stderr, PSTR("end\n") );

	return 0;
}

static INT do_ir_recv_start( VOID )
{
	fprintf_P( stderr, PSTR("** IR recv test **\n") );

	fprintf_P( stderr, PSTR("recv event %u %u\n"), _gui_event_ir_recv_end, _gui_event_ir_recv_err );
	Ir_Recv_Start();
#if 0
	if( Ir_Recv_WaitEnd() == FALSE ) {
		Ir_Recv_EventHistoryDump();
	}
	else {
		Ir_Frame_Dump();
	}
	Ir_Recv_Stop();
#endif

	fprintf_P( stderr, PSTR("end\n") );

	return 0;
}

static INT do_ir_recv_end( VOID )
{
	fprintf_P( stderr, PSTR("** IR recv end **\n") );

	Ir_Frame_Dump();
	Ir_Recv_Stop();

	fprintf_P( stderr, PSTR("end\n") );

	return 0;
}

static INT do_ir_recv_err( VOID )
{
	fprintf_P( stderr, PSTR("** IR recv err **\n") );

	Ir_Recv_EventHistoryDump();
	Ir_Recv_Stop();

	fprintf_P( stderr, PSTR("end\n") );

	return 0;
}

