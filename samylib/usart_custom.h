
#ifndef __USART_CUSTOM_H__
#define __USART_CUSTOM_H__

// WinAVR
#include <avr/io.h>


//
// ��M�o�b�t�@�T�C�Y
//
#ifndef D_USART_RX_BUFFER_SIZE
#define D_USART_RX_BUFFER_SIZE 16
#endif

//
// ���M�o�b�t�@�T�C�Y
// 0�ƒ�`����Ƒ��M���荞�݂��g�p���Ȃ��Ȃ�
//
#ifndef D_USART_TX_BUFFER_SIZE
#define D_USART_TX_BUFFER_SIZE 16
#endif


//
// USART�̃��W�X�^�Q��`
//
#if defined(UBRRH) && defined(UBRRL)
#define D_UBRRH UBRRH
#define D_UBRRL UBRRL
#define D_UCSRA UCSRA
#define D_UCSRB UCSRB
#define D_UDR   UDR
#define D_RXEN  RXEN
#define D_TXEN  TXEN
#define D_RXCIE RXCIE
#define D_UDRIE UDRIE
#define D_UDRE  UDRE
#elif defined(UBRR0H) && defined(UBRR0L)
#define D_UBRRH UBRR0H
#define D_UBRRL UBRR0L
#define D_UCSRA UCSR0A
#define D_UCSRB UCSR0B
#define D_UDR   UDR0
#define D_RXEN  RXEN0
#define D_TXEN  TXEN0
#define D_RXCIE RXCIE0
#define D_UDRIE UDRIE0
#define D_UDRE  UDRE0
#elif defined(USBCON)
  #warning no serial port defined  (port 0)
#else
  #error no serial port defined  (port 0)
#endif


#endif /* __USART_CUSTOM_H__ */

