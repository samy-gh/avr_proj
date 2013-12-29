
#ifndef __CUSTOM_H__
#define __CUSTOM_H__

// LCD制御
#define CO_LCD_CTRL

// メインループの動作を見る
//#define CO_MAINLOOP_MONITOR

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

#define D_IR_EEP_ADDR		0

#endif //__CUSTOM_H__

