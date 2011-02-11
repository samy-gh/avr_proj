#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include "wiring.h"

// ATTiny2313用

// Fuse
// Lo: E4
// Hi: 9D
// Ex: FF

// 内蔵RC発信機の公正方法
//   - Fuse-Lo=0xA4で書き込み
//   - 6番pinにオシロスコープを接続
//   - OSCCALの値を調整

// 8MHzにこだわらないのであれば周波数を測定した後、
// F_CPU define値をそれに合わせればよい。(AVR StudioはProjectのPropertyにFrequencyを設定する欄がある。)
//#define D_OSCCAL 0x59

const prog_char g_on_cmd[] = "hdd on";
const prog_char g_off_cmd[] = "hdd off";
const prog_char g_stat_cmd[] = "hdd stat";
unsigned char g_on_idx = 0;
unsigned char g_off_idx = 0;
unsigned char g_stat_idx = 0;

void setup( void )
{
	// 内蔵RC発信機を8MHzへ公正
#ifdef D_OSCCAL
	OSCCAL=D_OSCCAL;
#endif

	DDRD |= (1<<DD5);

	beginSerial( 38400 );

	WDTCSR = 0x00;

	sei();	// 割り込み許可

	set_sleep_mode(SLEEP_MODE_IDLE);	// USARTはIDLEモードでしか使えない
//	set_sleep_mode(SLEEP_MODE_STANDBY);
//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();

	g_on_idx = 0;
	g_off_idx = 0;
	g_stat_idx = 0;
}


void loop( void )
{
	char c;
	char c2;
	PGM_P p_str;

	while( 1 ) {
		if( !serialAvailable() ) {
			sleep_cpu();
			continue;
		}

		c = serialRead();
		serialWrite( c );

		// 改行文字が来たらコマンド判定をリセット
		if( (c == '\r')
		 || (c == '\n')
		 ) {
			g_on_idx = 0;
			g_off_idx = 0;
			g_stat_idx = 0;
			continue;
		}

		// ONコマンド判定
		p_str = &g_on_cmd[g_on_idx];
		c2 = pgm_read_byte(p_str);
		if( c == c2 ) {
			g_on_idx++;

			p_str = &g_on_cmd[g_on_idx];
			c2 = pgm_read_byte(p_str);
			if( c2 == '\0' ) {
				// ONコマンド受信
				g_on_idx = 0xff;
				PORTD |= (1<<PD5);		//PORTDの5番ピンが1になる
			}
		}
		else {
			g_on_idx = 0xff;
		}


		// OFFコマンド判定
		p_str = &g_off_cmd[g_off_idx];
		c2 = pgm_read_byte(p_str);
		if( c == c2 ) {
			g_off_idx++;

			p_str = &g_off_cmd[g_off_idx];
			c2 = pgm_read_byte(p_str);
			if( c2 == '\0' ) {
				// OFFコマンド受信
				g_off_idx = 0xff;
				PORTD &= ~(1<<PD5);		//PORTDの5番ピンが0になる
			}
		}
		else {
			g_off_idx = 0xff;
		}


		// STATコマンド判定
		p_str = &g_stat_cmd[g_stat_idx];
		c2 = pgm_read_byte(p_str);
		if( c == c2 ) {
			g_stat_idx++;

			p_str = &g_stat_cmd[g_stat_idx];
			c2 = pgm_read_byte(p_str);
			if( c2 == '\0' ) {
				// statコマンド受信
				g_stat_idx = 0xff;
				serialWrite( '\r' );
				serialWrite( '\n' );
				serialWrite( 'O' );
				if( PORTD & (1<<PD5) ) {
					serialWrite( 'N' );
				}
				else {
					serialWrite( 'F' );
					serialWrite( 'F' );
				}
				serialWrite( '\r' );
				serialWrite( '\n' );
			}
		}
		else {
			g_stat_idx = 0xff;
		}
	}
}


int main( void )
{
	setup();

	while( 1 ) {
		loop();
	}
}
