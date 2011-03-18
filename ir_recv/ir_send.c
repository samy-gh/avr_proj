
// プロジェクト固有
#include "ir_recv.h"
#include "custom.h"

// samylib
#include <my_typedef.h>
#include <common.h>
#include <timer1.h>
#include <usart.h>

// WinAVR
#include <avr/io.h>

// STD-C
#include <stdio.h>

static VOID ir_send_compa_inthdl( VOID )
{
	PORTC ^= _BV(PC3);
}

static VOID ir_send_compb_inthdl( VOID )
{
	PORTC ^= _BV(PC3);
}

static VOID ir_send_ovf_inthdl( VOID )
{
	PORTC ^= _BV(PC3);
}

VOID Ir_Send( VOID )
{
	sbi( DDRC, DDC3 );
	sbi( DDRB, DDB1 );
	cbi( PORTB ,PB1 );


	Timer1_Init( 1000000UL / 38000UL );			// 38kHz = 26usec
	Timer1_Set_PwmPin( 1, 3333 );	// 50%
//	Timer1_Set_PwmPin( 1, 5000 );	// 50%
//	Timer1_Set_PwmDuty( 1, 5000 );
//	Timer1_Enable_CompaIntr( ir_send_compa_inthdl );
//	Timer1_Enable_CompbIntr( ir_send_compb_inthdl );
	Timer1_Enable_OvfIntr( ir_send_ovf_inthdl );
	
	Timer1_Start();

	Usart_Set_Stdout();
	printf_P( PSTR("ICR1=%u, OCR1A=%u"), ICR1, OCR1A );
	printf_P( PSTR(", TCCR1A=0x%x, TCCR1B=0x%x\n"), TCCR1A, TCCR1B );

//	Timer1_Unset_PwmPin( 0 );
	while( 1 ) {
	}
}

