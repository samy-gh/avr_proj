
#ifndef __TIMER1_H__
#define __TIMER1_H__

// samylib
#include "my_typedef.h"


#define D_TIMER1_RESOLUTION 65536    // Timer1 is 16 bit


extern VOID (*gTimer1_CompaCallbackFunc)( VOID );
extern VOID (*gTimer1_CompbCallbackFunc)( VOID );
extern VOID (*gTimer1_OvfCallbackFunc)( VOID );
extern UCHAR gTimer1_ClockSelectBits;
extern UINT gTimer1_PwmPeriod;

extern VOID Timer1_Init( ULONG usec );	// Initialize and Disable Power-save
extern VOID Timer1_Start( VOID );
extern VOID Timer1_Stop( VOID );
extern VOID Timer1_Close( VOID );		// Enable Power-save
extern VOID Timer1_Restart( VOID );
extern VOID Timer1_Set_PwmPin( UCHAR pin, UINT duty );
extern VOID Timer1_Unset_PwmPin( UCHAR pin );
extern VOID Timer1_Enable_OvfIntr( VOID (*isr)( VOID ) );
extern VOID Timer1_Enable_CompaIntr( VOID (*isr)( VOID ) );
extern VOID Timer1_Enable_CompbIntr( VOID (*isr)( VOID ) );
extern VOID Timer1_Disable_OvfIntr( VOID );
extern VOID Timer1_Disable_CompaIntr( VOID );
extern VOID Timer1_Disable_CompbIntr( VOID );
extern VOID Timer1_Set_Period( ULONG usec );
extern VOID Timer1_Set_Period_Direct( UCHAR clk_sel, ULONG ul_cycles );
extern VOID Timer1_Set_PwmDuty( UCHAR pin, UINT duty );

#endif //__TIMER1_H__

