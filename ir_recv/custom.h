
#ifndef __CUSTOM_H__
#define __CUSTOM_H__


// ���C�����[�v�̓��������
//#define CO_MAINLOOP_MONITOR

// PC3�s����IR��M�󋵂����j�^����
//#define CO_PCINT_MONITOR_PC3

// �X���[�v(�ȓd��)���[�hON
#define CO_SLEEP_ENABLE

// �N���b�N�Ԉ���(�ȓd��)���[�hON
//#define CO_CLOCK_REDUCE_MODE
//
#ifdef CO_CLOCK_REDUCE_MODE
#define CLK_DIV256()	clock_prescale_set( 8 )
#define CLK_DIV1()		clock_prescale_set( 0 )
#else
#define CLK_DIV256()
#define CLK_DIV1()
#endif

// IR�g�`�v���p�^�C�}�̍ő厞��
#define D_TIMER1_PERIOD_USEC 10000UL

// IR��M�s���̏�Ԏ擾�p
#define D_IR_RECV_PIN	(PINC & _BV(PINC0))

#endif //__CUSTOM_H__

