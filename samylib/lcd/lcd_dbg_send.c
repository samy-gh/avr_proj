/*
 * HD44780 LCD driver
 *		4bit���[�h�Abusy�`�F�b�N����
 *		sasami@shadow.ne.jp
 *
 *  Original is ....
 *   Program:    LCD driver
 *   Created:    28.8.99 21:32
 *   Author:     Brajer Vlado
 *   Comments:   HITACHI LCD driver
 *              .... thanks!
 *
 * SVNID: $Revision$ $Date::                            $
 */

#ifndef __3694_H__
#include <3694.h>
#endif

#ifndef __H8_COMMON_H__
#include "h8_common.h"     /* ���ʃv���b�g�t�H�[�� */
#endif

//#include <stdarg.h>
#ifndef __LCD_HD44780_H__
#include "lcd_hd44780.h"
#endif

#ifndef __LCD_HD44780_HAL_H__
#include "lcd_hd44780_hal.h"
#endif


/* �V���A�����HEX���͂��ꂽ1byte���̃f�[�^��LCD�ɑ���B */
/* RS=0, RW=0 */
void lcd_rs_1byte_data_send(void)
{
	unsigned char uc_data = rs_1byte_input();

	LCD_PORT = LCD_SET_ADDR_DATA((uc_data & 0xF0) >> 4);
	rs_printf( "\n d data = %x\n", (unsigned int)LCD_PORT );
	toggle_E();
	LCD_PORT = LCD_SET_ADDR_DATA(uc_data & 0x0F);
	rs_printf( "\n d data = %x\n", (unsigned int)LCD_PORT );
	toggle_E();
}


/* �V���A�����HEX���͂��ꂽ1byte����LCD�|�[�g�̐���f�[�^��LCD�ɑ���B */
/* RS, RW���܂� */
void lcd_rs_lcd_ctrl(void)
{
	unsigned char uc_data = rs_1byte_input();

	rs_printf( "\n D data = %x\n", (unsigned int)uc_data );
	LCD_PORT = uc_data & ~LCD_OUT_MASK;
	toggle_E();
}


/* LCD�|�[�g���w�肵���l�ɂ��� */
void lcd_port_ctrl(unsigned char uc_data)
{
	LCD_PORT = uc_data;
}
