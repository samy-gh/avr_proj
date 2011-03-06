
#ifndef __COMMON_H__
#define __COMMON_H__

// WinAVR
#include <avr/io.h>


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#endif /* __COMMON_H__ */

