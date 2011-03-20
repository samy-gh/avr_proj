
// プロジェクト固有
#include "ir_recv.h"
#include "custom.h"

// samylib
#include <my_typedef.h>
#include <common.h>
#include <timer0.h>
#include <timer1.h>
#include <usart.h>

// WinAVR
#include <avr/io.h>

// STD-C
#include <stdio.h>

static VOID ir_send_init_subcarrier( VOID )
{
	Timer0_Init( 1 );
//	Timer0_Set_Period( 26 );	// 26usec
	Timer0_Set_Period_Direct( _BV(CS01), 52 );	// 26usec
	Timer0_Set_PwmPin( 3333 );	// 33%

#if 0
	Usart_Set_Stdout();
	printf_P( PSTR("OCR0A=%u, OCR0B=%u"), OCR0A, OCR0B );
	printf_P( PSTR(", TCCR0A=0x%x, TCCR0B=0x%x\n"), TCCR0A, TCCR0B );
	printf_P( PSTR("bit=0x%x\n"), gTimer0_ClockSelectBits );
#endif
}

static VOID ir_send_start_subcarrier( VOID )
{
	Timer0_Start();
}

static VOID ir_send_stop_subcarrier( VOID )
{
	Timer0_Stop();
}

static VOID ir_send_close_subcarrier( VOID )
{
	Timer0_Close();
}

static VOID ir_send_compa_inthdl( VOID )
{
	ir_send_stop_subcarrier();
	PORTC ^= _BV(PC3);
}

static VOID ir_send_ovf_inthdl( VOID )
{
	ir_send_start_subcarrier();
	PORTC ^= _BV(PC3);
}

VOID Ir_Send( VOID )
{
	// デバッグ用
	sbi( DDRC, DDC3 );

	ir_send_init_subcarrier();
	ir_send_start_subcarrier();


#if 1
	Timer1_Init( 1 );
	Timer1_Set_Period( 600 *2 );	// 600usec
	Timer1_Set_PwmPin( 0, 5000 );	// 50%
	Timer1_Enable_CompaIntr( ir_send_compa_inthdl );
	Timer1_Enable_OvfIntr( ir_send_ovf_inthdl );
	
	Timer1_Start();

#if 1
	Usart_Set_Stdout();
	printf_P( PSTR("ICR1=%u, OCR1B=%u"), ICR1, OCR1B );
	printf_P( PSTR(", TCCR1A=0x%x, TCCR1B=0x%x\n"), TCCR1A, TCCR1B );
	printf_P( PSTR("bit=0x%x\n"), gTimer1_ClockSelectBits );
#endif

//	Timer1_Unset_PwmPin( 1 );
#endif

	while( 1 ) {
	}

	ir_send_stop_subcarrier();
	ir_send_close_subcarrier();
}

