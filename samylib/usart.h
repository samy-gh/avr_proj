
#ifndef __USART_H__
#define __USART_H__

// usart
#include "usart_custom.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/pgmspace.h>


#define  USART_PGM_PUTS( str )	Usart_PgmPuts( PSTR str );

extern VOID Usart_Init( const ULONG baud );
extern VOID Usart_Close( VOID );
extern VOID Usart_Set_Stdout( VOID );
extern VOID Usart_Set_Stderr( VOID );
extern VOID Usart_Write( const UCHAR c );
extern UCHAR Usart_Available( VOID );
extern BOOL Usart_Wait_WriteComplete( VOID );
extern INT Usart_Read( VOID );
extern VOID Usart_Flush( VOID );
extern VOID Usart_PgmPuts( PGM_P p_str );
extern VOID Usart_Puts( const CHAR* p_str );
extern VOID Usart_PrintDigit( UINT val );
extern VOID Usart_PrintDigitN( UINT val, UINT print_lvl );
extern VOID Usart_PrintHex( UINT val );
extern VOID Usart_PrintHexN( UINT val, UINT print_lvl );




/* for Usart driver use only */

#if defined(D_USART_TX_BUFFER_SIZE) && (D_USART_TX_BUFFER_SIZE > 0)
#define CO_USART_TX_INTERRUPT
#endif

// T_USART_RING_BUF_RX
typedef struct {
  UCHAR buffer[D_USART_RX_BUFFER_SIZE];
  INT head;
  INT tail;
} T_USART_RING_BUF_RX;
extern volatile T_USART_RING_BUF_RX gUsart_RxBuffer;

#ifdef CO_USART_TX_INTERRUPT
// T_USART_RING_BUF_TX
typedef struct {
  UCHAR buffer[D_USART_TX_BUFFER_SIZE];
  INT head;
  INT tail;
} T_USART_RING_BUF_TX;
extern volatile T_USART_RING_BUF_TX gUsart_TxBuffer;
#endif //CO_USART_TX_INTERRUPT


#endif /* __USART_H__ */

