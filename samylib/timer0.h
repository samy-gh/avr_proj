
#ifndef __TIMER0_H__
#define __TIMER0_H__

// samylib
#include "my_typedef.h"


#define D_TIMER0_RESOLUTION 256    // Timer0 is 8 bit


extern VOID (*gTimer0_CompbCallbackFunc)( VOID );
extern VOID (*gTimer0_OvfCallbackFunc)( VOID );
extern volatile UCHAR gTimer0_ClockSelectBits;
extern volatile UCHAR gTimer0_PwmPeriod;
extern volatile UCHAR gTimer0_PwmPinEnable;

extern VOID Timer0_Init( ULONG usec );	// Initialize and Disable Power-save
extern VOID Timer0_Start( VOID );
extern VOID Timer0_Stop( VOID );
extern VOID Timer0_Close( VOID );		// Enable Power-save
extern VOID Timer0_Restart( VOID );
extern BOOL Timer0_Set_PwmPin( UINT duty );
extern VOID Timer0_Unset_PwmPin( VOID );
extern VOID Timer0_Enable_OvfIntr( VOID (*isr)( VOID ) );
extern VOID Timer0_Enable_CompbIntr( VOID (*isr)( VOID ) );
extern VOID Timer0_Disable_OvfIntr( VOID );
extern VOID Timer0_Disable_CompbIntr( VOID );
extern VOID Timer0_Set_Period( ULONG usec );
extern VOID Timer0_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles );
extern VOID Timer0_Set_PwmDuty( UINT duty );

#endif //__TIMER0_H__

