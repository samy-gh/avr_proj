
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>

// STD-C
#include <stdio.h>


static int usart_putchar( char c, struct __file* dummy )
{
	Usart_Write( c );
	return 0;
}

static FILE g_usart_stderr = FDEV_SETUP_STREAM( usart_putchar, NULL, _FDEV_SETUP_WRITE );

VOID Usart_Set_Stderr( VOID )
{
	stderr = &g_usart_stderr;
}

