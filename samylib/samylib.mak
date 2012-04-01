
CUR_PATH=../samylib
CFLAGS+=-I$(CUR_PATH)

CSRCS+=$(CUR_PATH)/mystdout/mystdout_print_digit.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_print_digit_n.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_print_hex.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_print_hex_n.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_puts.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_pgm_puts.c
CSRCS+=$(CUR_PATH)/mystdout/mystdout_putc.c

CSRCS+=$(CUR_PATH)/usart/usart_rx_vect.c
CSRCS+=$(CUR_PATH)/usart/usart_init.c
CSRCS+=$(CUR_PATH)/usart/usart_close.c
CSRCS+=$(CUR_PATH)/usart/usart_write.c
CSRCS+=$(CUR_PATH)/usart/usart_read.c
CSRCS+=$(CUR_PATH)/usart/usart_available.c
CSRCS+=$(CUR_PATH)/usart/usart_wait_write_complete.c
CSRCS+=$(CUR_PATH)/usart/usart_flush.c
CSRCS+=$(CUR_PATH)/usart/usart_udre_vect.c
CSRCS+=$(CUR_PATH)/usart/usart_set_stdout.c
CSRCS+=$(CUR_PATH)/usart/usart_pgm_puts.c
CSRCS+=$(CUR_PATH)/usart/usart_puts.c
CSRCS+=$(CUR_PATH)/usart/usart_print_digit.c
CSRCS+=$(CUR_PATH)/usart/usart_print_digit_n.c
CSRCS+=$(CUR_PATH)/usart/usart_print_hex.c
CSRCS+=$(CUR_PATH)/usart/usart_print_hex_n.c

CSRCS+=$(CUR_PATH)/lcd/lcd_init.c
CSRCS+=$(CUR_PATH)/lcd/lcd_close.c
CSRCS+=$(CUR_PATH)/lcd/lcd_cls.c
CSRCS+=$(CUR_PATH)/lcd/lcd_control.c
CSRCS+=$(CUR_PATH)/lcd/lcd_home.c
CSRCS+=$(CUR_PATH)/lcd/lcd_goto.c
CSRCS+=$(CUR_PATH)/lcd/lcd_open.c
CSRCS+=$(CUR_PATH)/lcd/lcd_setfont.c
CSRCS+=$(CUR_PATH)/lcd/lcd_set_stdout.c
CSRCS+=$(CUR_PATH)/lcd/lcd_shift.c
CSRCS+=$(CUR_PATH)/lcd/lcd_puts.c
CSRCS+=$(CUR_PATH)/lcd/lcd_pgm_puts.c
CSRCS+=$(CUR_PATH)/lcd/lcd_print_digit.c
CSRCS+=$(CUR_PATH)/lcd/lcd_print_digit_n.c
CSRCS+=$(CUR_PATH)/lcd/lcd_print_hex.c
CSRCS+=$(CUR_PATH)/lcd/lcd_print_hex_n.c
CSRCS+=$(CUR_PATH)/lcd/lcd_write.c
CSRCS+=$(CUR_PATH)/lcd/lcd_test.c
CSRCS+=$(CUR_PATH)/lcd/lcd_toggle_e.c
CSRCS+=$(CUR_PATH)/lcd/lcd_write_data.c
CSRCS+=$(CUR_PATH)/lcd/lcd_is_open_val.c
CSRCS+=$(CUR_PATH)/lcd/lcd_set_block.c

CSRCS+=$(CUR_PATH)/timer0/timer0_init.c
CSRCS+=$(CUR_PATH)/timer0/timer0_set_period.c
CSRCS+=$(CUR_PATH)/timer0/timer0_set_period_direct.c
CSRCS+=$(CUR_PATH)/timer0/timer0_ovf_vect.c
CSRCS+=$(CUR_PATH)/timer0/timer0_enable_ovf_intr.c
CSRCS+=$(CUR_PATH)/timer0/timer0_start.c
CSRCS+=$(CUR_PATH)/timer0/timer0_set_pwm_duty.c
CSRCS+=$(CUR_PATH)/timer0/timer0_set_pwmpin.c
CSRCS+=$(CUR_PATH)/timer0/timer0_unset_pwmpin.c
CSRCS+=$(CUR_PATH)/timer0/timer0_compb_vect.c
CSRCS+=$(CUR_PATH)/timer0/timer0_enable_compb_intr.c
CSRCS+=$(CUR_PATH)/timer0/timer0_stop.c
CSRCS+=$(CUR_PATH)/timer0/timer0_close.c
CSRCS+=$(CUR_PATH)/timer0/timer0_restart.c

CSRCS+=$(CUR_PATH)/timer1/timer1_ovf_vect.c
CSRCS+=$(CUR_PATH)/timer1/timer1_compa_vect.c
CSRCS+=$(CUR_PATH)/timer1/timer1_compb_vect.c
CSRCS+=$(CUR_PATH)/timer1/timer1_init.c
CSRCS+=$(CUR_PATH)/timer1/timer1_enable_ovf_intr.c
CSRCS+=$(CUR_PATH)/timer1/timer1_enable_compa_intr.c
CSRCS+=$(CUR_PATH)/timer1/timer1_enable_compb_intr.c
CSRCS+=$(CUR_PATH)/timer1/timer1_start.c
CSRCS+=$(CUR_PATH)/timer1/timer1_restart.c
CSRCS+=$(CUR_PATH)/timer1/timer1_stop.c
CSRCS+=$(CUR_PATH)/timer1/timer1_close.c
CSRCS+=$(CUR_PATH)/timer1/timer1_set_period.c
CSRCS+=$(CUR_PATH)/timer1/timer1_set_period_direct.c
CSRCS+=$(CUR_PATH)/timer1/timer1_set_pwm_duty.c
CSRCS+=$(CUR_PATH)/timer1/timer1_set_pwmpin.c
CSRCS+=$(CUR_PATH)/timer1/timer1_unset_pwmpin.c

CSRCS+=$(CUR_PATH)/timer2/timer2_ovf_vect.c
CSRCS+=$(CUR_PATH)/timer2/timer2_compa_vect.c
CSRCS+=$(CUR_PATH)/timer2/timer2_compb_vect.c
CSRCS+=$(CUR_PATH)/timer2/timer2_init.c
CSRCS+=$(CUR_PATH)/timer2/timer2_enable_ovf_intr.c
CSRCS+=$(CUR_PATH)/timer2/timer2_enable_compa_intr.c
CSRCS+=$(CUR_PATH)/timer2/timer2_enable_compb_intr.c
CSRCS+=$(CUR_PATH)/timer2/timer2_start.c
CSRCS+=$(CUR_PATH)/timer2/timer2_restart.c
CSRCS+=$(CUR_PATH)/timer2/timer2_stop.c
CSRCS+=$(CUR_PATH)/timer2/timer2_close.c
CSRCS+=$(CUR_PATH)/timer2/timer2_set_period.c
CSRCS+=$(CUR_PATH)/timer2/timer2_set_period_direct.c
CSRCS+=$(CUR_PATH)/timer2/timer2_set_pwm_duty.c
CSRCS+=$(CUR_PATH)/timer2/timer2_set_pwmpin.c
CSRCS+=$(CUR_PATH)/timer2/timer2_unset_pwmpin.c

CSRCS+=$(CUR_PATH)/test_sw/test_sw_is_sw1_chg.c
CSRCS+=$(CUR_PATH)/test_sw/test_sw_is_sw2_chg.c
CSRCS+=$(CUR_PATH)/test_sw/test_sw_sw1_pcint1_hdl.c
CSRCS+=$(CUR_PATH)/test_sw/test_sw_sw2_pcint1_hdl.c

CSRCS+=$(CUR_PATH)/framework/btn_ctrl.c
CSRCS+=$(CUR_PATH)/framework/btn_print.c
CSRCS+=$(CUR_PATH)/framework/event_loop.c
CSRCS+=$(CUR_PATH)/framework/clock_hdl.c
CSRCS+=$(CUR_PATH)/framework/softtimer_int.c
CSRCS+=$(CUR_PATH)/framework/softtimer_start.c
CSRCS+=$(CUR_PATH)/framework/softtimer_stop.c
CSRCS+=$(CUR_PATH)/framework/msec2clock.c

CSRCS+=$(CUR_PATH)/int_lock/int_lock.c

