
#ifndef __IR_CUSTOM_H__
#define __IR_CUSTOM_H__


// IR波形計測用タイマの最大時間
#define D_TIMER1_PERIOD_USEC 10000UL

// IR受信ピンの状態取得用
#define D_IR_RECV_PIN	bit_is_set( PINC, PINC0 )


#endif //__IR_CUSTOM_H__
