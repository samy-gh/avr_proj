
#ifndef __TEST_SW_H__
#define __TEST_SW_H__

#include "my_typedef.h"
#include "common.h"
#include <avr/io.h>

// SW1: PC2 (PCINT10)
// SW2: PC3 (PCINT11)

#define TEST_SW1_ENABLE()	(	\
		cbi( DDRC, DDC2 ),		\
		sbi( PORTC, PC2 ),		\
		sbi( PCMSK1, PCINT10 ),	\
		sbi( PCICR, PCIE1 ),	\
		gTest_Sw1_Event = E_TEST_SW_EVENT_NONE,	\
		gTest_Sw1_Stat = TEST_SW1_STAT()	\
		)

#define TEST_SW1_DISABLE()	(	\
		cbi( DDRC, DDC2 ),		\
		cbi( PORTC, PC2 ),		\
		cbi( PCMSK1, PCINT10 )	\
		)

#define TEST_SW1_STAT()		( !(PINC & _BV(PINC2)) )


#define TEST_SW2_DISABLE()	(	\
		cbi( DDRC, DDC3 ),		\
		cbi( PORTC, PC3 ),		\
		cbi( PCMSK1, PCINT11 )	\
		)

#define TEST_SW2_ENABLE()	(	\
		cbi( DDRC, DDC3 ),		\
		sbi( PORTC, PC3 ),		\
		sbi( PCMSK1, PCINT11 ),	\
		sbi( PCICR, PCIE1 ),	\
		gTest_Sw2_Event = E_TEST_SW_EVENT_NONE,	\
		gTest_Sw2_Stat = TEST_SW2_STAT()	\
		)

#define TEST_SW2_STAT()		( !(PINC & _BV(PINC3)) )

typedef enum {
	E_TEST_SW_EVENT_NONE = 0,
	E_TEST_SW_EVENT_OFF,
	E_TEST_SW_EVENT_ON,
	E_TEST_SW_EVENT_MAX
} E_TEST_SW_EVENT;

extern UCHAR gTest_Sw1_Stat;
extern UCHAR gTest_Sw2_Stat;
extern E_TEST_SW_EVENT gTest_Sw1_Event;
extern E_TEST_SW_EVENT gTest_Sw2_Event;


E_TEST_SW_EVENT Test_Sw_Is_Sw1Chg( VOID );
E_TEST_SW_EVENT Test_Sw_Is_Sw2Chg( VOID );

// Ç±ÇÃÉnÉìÉhÉâÇÕPCINT1Ç…ê⁄ë±Ç∑ÇÈ
// ó·ÅF
// SIGNAL(PCINT1_vect)
// {
// 		Test_Sw_Sw1Pcint1Hdl();
// 		Test_Sw_Sw2Pcint1Hdl();
// }
extern VOID Test_Sw_Sw1Pcint1Hdl( VOID );
extern VOID Test_Sw_Sw2Pcint1Hdl( VOID );

#endif //__TEST_SW_H__

