
#ifndef __CUSTOM_H__
#define __CUSTOM_H__

// LCD����
#define CO_LCD_CTRL

// ���C�����[�v�̓��������
//#define CO_MAINLOOP_MONITOR

// �X���[�v(�ȓd��)���[�hON
//#define CO_SLEEP_ENABLE

// �N���b�N�Ԉ���(�ȓd��)���[�hON
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

