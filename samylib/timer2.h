
#ifndef __TIMER2_H__
#define __TIMER2_H__

// samylib
#include "my_typedef.h"


#define D_TIMER2_RESOLUTION 256    // Timer2 is 8 bit


extern VOID (*gTimer2_CompaCallbackFunc)( VOID );
extern VOID (*gTimer2_CompbCallbackFunc)( VOID );
extern VOID (*gTimer2_OvfCallbackFunc)( VOID );
extern volatile UCHAR gTimer2_ClockSelectBits;
extern volatile UINT gTimer2_PwmPeriod;
extern volatile UCHAR gTimer2_PwmPinAEnable;
extern volatile UCHAR gTimer2_PwmPinBEnable;

extern VOID Timer2_Init( ULONG usec );	// Initialize and Disable Power-save
extern VOID Timer2_Start( VOID );
extern VOID Timer2_Stop( VOID );
extern VOID Timer2_Close( VOID );		// Enable Power-save
extern VOID Timer2_Restart( VOID );
extern VOID Timer2_Set_PwmPin( UCHAR pin, UINT duty );
extern VOID Timer2_Unset_PwmPin( UCHAR pin );
extern VOID Timer2_Enable_OvfIntr( VOID (*isr)( VOID ) );
extern VOID Timer2_Enable_CompaIntr( VOID (*isr)( VOID ) );
extern VOID Timer2_Enable_CompbIntr( VOID (*isr)( VOID ) );
extern VOID Timer2_Disable_OvfIntr( VOID );
extern VOID Timer2_Disable_CompaIntr( VOID );
extern VOID Timer2_Disable_CompbIntr( VOID );
extern VOID Timer2_Set_Period( ULONG usec );
extern VOID Timer2_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles );
extern VOID Timer2_Set_PwmDuty( UCHAR pin, UINT duty );

#endif //__TIMER2_H__

