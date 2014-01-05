
#ifndef __CUSTOM_H__
#define __CUSTOM_H__

// スリープ(省電力)モードON
//#define CO_SLEEP_ENABLE

// クロック間引き(省電力)モードON
//#define CO_CLOCK_REDUCE_MODE

#ifdef CO_CLOCK_REDUCE_MODE
#define CLK_DIVN()		clock_prescale_set( 6 )
#define CLK_DIV1()		clock_prescale_set( 0 )
#else
#define CLK_DIVN()
#define CLK_DIV1()
#endif

#if defined(CO_CLOCK_REDUCE_MODE) || defined(CO_SLEEP_ENABLE)
#define UART_FLUSH()	Usart_Wait_WriteComplete()
#else
#define UART_FLUSH()
#endif

#define D_IR_EEP_ADDR		0

#endif //__CUSTOM_H__

