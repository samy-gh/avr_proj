
#ifndef __CUSTOM_H__
#define __CUSTOM_H__


// メインループの動作を見る
//#define CO_MAINLOOP_MONITOR

// PC3ピンでIR受信状況をモニタする
//#define CO_PCINT_MONITOR_PC3

// スリープ(省電力)モードON
#define CO_SLEEP_ENABLE

// クロック間引き(省電力)モードON
//#define CO_CLOCK_REDUCE_MODE
//
#ifdef CO_CLOCK_REDUCE_MODE
#define CLK_DIV256()	clock_prescale_set( 8 )
#define CLK_DIV1()		clock_prescale_set( 0 )
#else
#define CLK_DIV256()
#define CLK_DIV1()
#endif

// IR波形計測用タイマの最大時間
#define D_TIMER1_PERIOD_USEC 10000UL

// IR受信ピンの状態取得用
#define D_IR_RECV_PIN	(PINC & _BV(PINC0))

#endif //__CUSTOM_H__

