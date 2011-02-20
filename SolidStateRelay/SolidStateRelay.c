#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include "wiring_private.h"

// ATTiny2313用

// Fuse
// Lo: E4
// Hi: 9D
// Ex: FF

// 制御コマンド
// ac on\n:   パワーオン
// ac off\n:  パワーオフ
// ac stat\n: 現在のパワー状態

// pin9: 外部リレー制御信号(Lo:off Hi:on)
// pin12: 強制OFFスイッチ(Hi:off Lo:on) 内部PullUP。SWをGndへ接続する。
// pin13: 強制ONスイッチ(Hi:off Lo:on) 内部PullUP。SWをGndへ接続する。

// 制御電圧 5.0V or 3.3V


// 内蔵RC発信機の校正方法
//   - Fuse-Lo=0xA4で書き込み
//   - 6番pinにオシロスコープを接続
//   - OSCCALの値を調整

// 8MHzにこだわらないのであれば周波数を測定した後、
// F_CPU define値をそれに合わせればよい。(AVR StudioはProjectのPropertyにFrequencyを設定する欄がある。)
//#define D_OSCCAL 0x59


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// 設定
// Version
#define D_VER_STR "Ver 1.02"

// pin12
#define D_PCINT0_PIN_ADDR		PINB
#define D_PCINT0_PIN_ADDR_BIT	PINB0
#define D_PCINT0_DIRECTION		DDRB
#define D_PCINT0_DIRECTION_BIT	DDB0
#define D_PCINT0_PORT			PORTB
#define D_PCINT0_PORT_BIT		PB0

// pin13
#define D_PCINT1_PIN_ADDR		PINB
#define D_PCINT1_PIN_ADDR_BIT	PINB1
#define D_PCINT1_DIRECTION		DDRB
#define D_PCINT1_DIRECTION_BIT	DDB1
#define D_PCINT1_PORT			PORTB
#define D_PCINT1_PORT_BIT		PB1

// pin9
#define D_AC_CTRL_DIRECTION		DDRD
#define D_AC_CTRL_DIRECTION_BIT	DD5
#define D_AC_CTRL_PORT			PORTD
#define D_AC_CTRL_PORT_BIT		PD5

//#define D_DEBUG_PRINT


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
const prog_char g_ver_cmd[] = "ver\n";
const prog_char g_on_cmd[] = "ac on\n";
const prog_char g_off_cmd[] = "ac off\n";
const prog_char g_stat_cmd[] = "ac stat\n";
unsigned char g_ver_idx = 0;
unsigned char g_on_idx = 0;
unsigned char g_off_idx = 0;
unsigned char g_stat_idx = 0;
unsigned char g_pcint_assert = 0;
unsigned char g_pcint_processed = 0;
unsigned char g_pcint0_last = 0;
unsigned char g_pcint1_last = 0;
unsigned char g_ac_on_cmd_last = 0;

#define  PGM_PUTS( str )	pgm_puts( PSTR str );

void pgm_puts( const PGM_P p_str )
{
	unsigned char c;

	while( 1 ) {
		c = pgm_read_byte( p_str++ );
		if( c == '\0' ) {
			return;
		}
		serialWrite( c );
	}
}

#ifdef D_DEBUG_PRINT
void print_digit( unsigned int val )
{
	unsigned int print_lvl;
	unsigned int digit_1;

	// 最上位桁までスキップ
	for( print_lvl = 10000; print_lvl != 0; print_lvl /= 10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	if( print_lvl == 0 ) {
		return;
	}

	for( ; print_lvl != 0; print_lvl /= 10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

//		if( digit_1 < 10 ) {
			serialWrite( '0' + digit_1 );
//		}
//		else {
//			serialWrite( '?' );
//		}
	}
}
#else //!D_DEBUG_PRINT

//#define PGM_PUTS( str )
//#define pgm_puts( p_str )
#define print_digit( val )
#endif //!D_DEBUG_PRINT


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#define M_GET_PIN_STAT( pin_addr, pin_bit )		(((pin_addr) & _BV((pin_bit))) == 0)

// Pin change Interrupt処理
SIGNAL(PCINT_vect)
{
	g_pcint_assert++;
	sleep_disable();
}


// PCINT初期化
void pcint_setup( void )
{
	// PCINT0 Pull-UP
	sbi( D_PCINT0_DIRECTION, D_PCINT0_DIRECTION_BIT );
	sbi( D_PCINT0_PORT, D_PCINT0_PORT_BIT );
	// PCINT1 Pull-UP
	sbi( D_PCINT1_DIRECTION, D_PCINT1_DIRECTION_BIT );
	sbi( D_PCINT1_PORT, D_PCINT1_PORT_BIT );

	// Logic Low=1 High=0 (Pull-UP)
	g_pcint0_last = M_GET_PIN_STAT( D_PCINT0_PIN_ADDR, D_PCINT0_PIN_ADDR_BIT );
	g_pcint1_last = M_GET_PIN_STAT( D_PCINT1_PIN_ADDR, D_PCINT1_PIN_ADDR_BIT );

	// PCINT0 Enable
	sbi( PCMSK, PCINT0 );
	// PCINT1 Enable
	sbi( PCMSK, PCINT1 );
	// PCINTn Enable
	sbi( GIMSK, PCIE );
}


// PCINTピン変化チェック
unsigned char pcint_judge( unsigned char* const pin_last, const unsigned char pin_addr, const unsigned int pin_bit )
{
	const unsigned char pin_status = M_GET_PIN_STAT( pin_addr, pin_bit );

	if( (*pin_last) != pin_status ) {
		*pin_last = pin_status;
		return 1;
	}
	else {
		return 0;
	}
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// シリアルコマンド初期化
void serial_cmd_setup( void )
{
	g_ver_idx = 0;
	g_on_idx = 0;
	g_off_idx = 0;
	g_stat_idx = 0;
}


// シリアルコマンドの一致チェック
unsigned char serial_cmd_judge( const PGM_P const p_target_str, unsigned char* idx, unsigned char compare_char )
{
	const PGM_P p_str;
	unsigned char c;

	p_str = &p_target_str[*idx];
	c = pgm_read_byte(p_str);
	if( c == '\n' ) {
		switch( compare_char ) {
			case '\n':	// Fall through
			case '\r':
				break;
			default:
				*idx = 0xff;
				return 0;
		}
	}
	else {
		if( compare_char != c ) {
			*idx = 0xff;
			return 0;
		}
	}

	(*idx)++;

	p_str = &p_target_str[*idx];
	c = pgm_read_byte(p_str);
	if( c != '\0' ) {
		// コマンド文字列は継続している
		return 0;
	}

	// コマンドが一致した
	*idx = 0xff;
	return 1;
}


// シリアルコマンドを処理
void serial_recv_event( void )
{
	char c;

	c = serialRead();
	serialWrite( c );

	// Verコマンド判定
	if( serial_cmd_judge( g_ver_cmd, &g_ver_idx, c ) != 0 ) {
		PGM_PUTS(( D_VER_STR "  " __DATE__ " " __TIME__ "\n" ));
	}


	// ONコマンド判定
	if( serial_cmd_judge( g_on_cmd, &g_on_idx, c ) != 0 ) {
		g_ac_on_cmd_last = 1;
	}


	// OFFコマンド判定
	if( serial_cmd_judge( g_off_cmd, &g_off_idx, c ) != 0 ) {
		g_ac_on_cmd_last = 0;
	}


	// STATコマンド判定
	if( serial_cmd_judge( g_stat_cmd, &g_stat_idx, c ) != 0 ) {
		// statコマンド受信
		serialWrite( '\n' );
		if( g_pcint0_last != 0 ) {
			PGM_PUTS(( "FORCE OFF\n" ));
		}
		else
		if( g_pcint1_last != 0 ) {
			PGM_PUTS(( "FORCE ON\n" ));
		}
		if( g_ac_on_cmd_last != 0 ) {
			PGM_PUTS(( "CMD ON\n" ));
		}
		else {
			PGM_PUTS(( "CMD OFF\n" ));
		}

		if( D_AC_CTRL_PORT & _BV(D_AC_CTRL_PORT_BIT) ) {
			PGM_PUTS(( "AC ON\n" ));
		}
		else {
			PGM_PUTS(( "AC OFF\n" ));
		}
	}

	// 改行文字が来たらコマンド判定をリセット
	if( (c == '\r')
	 || (c == '\n')
	 ) {
		serialWrite( '\n' );
		serial_cmd_setup();
	}
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void ac_ctrl_port_setup( void )
{
	// AC Control port Direction is output
	sbi( D_AC_CTRL_DIRECTION, D_AC_CTRL_DIRECTION_BIT );
}


// ACコントロールポート制御
void ac_ctrl_port_drive( void )
{
	if( g_pcint0_last != 0 ) {
		// 強制OFFスイッチ
		cbi( D_AC_CTRL_PORT, D_AC_CTRL_PORT_BIT );
	}
	else
	if( g_pcint1_last != 0 ) {
		// 強制ONスイッチ
		sbi( D_AC_CTRL_PORT, D_AC_CTRL_PORT_BIT );
	}
	else {
		// シリアルコマンドに従う
		if( g_ac_on_cmd_last != 0 ) {
			// ON
			sbi( D_AC_CTRL_PORT, D_AC_CTRL_PORT_BIT );
		}
		else {
			// OFF
			cbi( D_AC_CTRL_PORT, D_AC_CTRL_PORT_BIT );
		}
	}
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void setup( void )
{
	// 内蔵RC発信機を8MHzへ校正
#ifdef D_OSCCAL
	OSCCAL = D_OSCCAL;
#endif

	WDTCSR = 0x00;

	ac_ctrl_port_setup();

	pcint_setup();

	serial_cmd_setup();

	beginSerial( 38400 );

	set_sleep_mode( SLEEP_MODE_IDLE );	// USARTはIDLEモードでしか使えない
//	set_sleep_mode( SLEEP_MODE_STANDBY );
//	set_sleep_mode( SLEEP_MODE_PWR_DOWN );

	sei();	// 割り込み許可
}


void loop( void )
{
	while( 1 ) {
		// すべての割り込みハンドラ内でsleep_disable()を実施している。
		// そうすることで、ループ先頭のsleep_enable()～ループ終端のsleep()までの間に割り込みが
		// 発生してもsleep()しなくなる。
		// sleep()するのは、sleep_enable()～sleep()までの間に割り込みが一度も発生しない場合のみとなる。
		cli();
		sleep_enable();
		sei();

		while( g_pcint_assert != g_pcint_processed ) {
			g_pcint_processed = g_pcint_assert;
#ifdef D_DEBUG_PRINT
			PGM_PUTS(( "PCINT Assert\n" ));
			print_digit( g_pcint_processed );
			serialWrite( '\n' );
#endif //D_DEBUG_PRINT

			(void)pcint_judge( &g_pcint0_last, D_PCINT0_PIN_ADDR, D_PCINT0_PIN_ADDR_BIT );
			(void)pcint_judge( &g_pcint1_last, D_PCINT1_PIN_ADDR, D_PCINT1_PIN_ADDR_BIT );

#ifdef D_DEBUG_PRINT
			PGM_PUTS(( "PCINT0 pin=" ));
			print_digit( g_pcint0_last );
			serialWrite( '\n' );
			PGM_PUTS(( "PCINT0 pin=" ));
			print_digit( g_pcint1_last );
			serialWrite( '\n' );
#endif //D_DEBUG_PRINT
		}

		while( serialAvailable() ) {
			serial_recv_event();
		}

		ac_ctrl_port_drive();

//		PGM_PUTS(( "sleep\n" ));
		sleep_cpu();
//		PGM_PUTS(( "wakeup\n" ));
	}
}


int main( void )
{
	setup();

	while( 1 ) {
		loop();
	}
}

