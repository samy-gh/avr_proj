
#ifndef __TEST_LED_H__
#define __TEST_LED_H__

#include "common.h"
#include <avr/io.h>

// LED1: PB5 (SCK)
// LED2: PB4 (MISO)
// LED3: PB3 (MOSI)

#define TEST_LED1_TOGGLE()	( DDRB ^= _BV(DDB5), PORTB ^= _BV(PB5) )
#define TEST_LED1_ON()		( sbi( DDRB, DDB5 ), sbi( PORTB, PB5 ) )
#define TEST_LED1_OFF()		( cbi( DDRB, DDB5 ), cbi( PORTB, PB5 ) )
#define TEST_LED2_TOGGLE()	( DDRB ^= _BV(DDB4), PORTB ^= _BV(PB4) )
#define TEST_LED2_ON()		( sbi( DDRB, DDB4 ), sbi( PORTB, PB4 ) )
#define TEST_LED2_OFF()		( cbi( DDRB, DDB4 ), cbi( PORTB, PB4 ) )
#define TEST_LED3_TOGGLE()	( DDRB ^= _BV(DDB3), PORTB ^= _BV(PB3) )
#define TEST_LED3_ON()		( sbi( DDRB, DDB3 ), sbi( PORTB, PB3 ) )
#define TEST_LED3_OFF()		( cbi( DDRB, DDB3 ), cbi( PORTB, PB3 ) )

#endif //__TEST_LED_H__

