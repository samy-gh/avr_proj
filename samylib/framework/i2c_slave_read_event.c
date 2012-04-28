
// framework
#include "framework_event_loop.h"
#include "framework_i2c.h"

// samylib
#include "i2c.h"
#include "my_typedef.h"


#ifdef CO_ENABLE_I2C_SLAVE_READ
/****************************************************************************/
/* イベント通知制御変数(ステートマシンが参照する)							*/
/****************************************************************************/
volatile UCHAR _gui_event_i2c_slave_read = 0;
volatile static UCHAR _gui_event_i2c_slave_read_finish = 0;
volatile static UCHAR* g_i2c_slave_read_buf_addr;
volatile static INT g_i2c_slave_read_buf_len;


/*----------------------------------------------*/
/* イベントハンドラ登録							*/
/*----------------------------------------------*/

static VOID i2c_slave_read_event_hdl( UCHAR* buf_addr, INT buf_len )
{
	_gui_event_i2c_slave_read = 1;
	_gui_event_i2c_slave_read_finish = 1;
	g_i2c_slave_read_buf_addr = buf_addr;
	g_i2c_slave_read_buf_len = buf_len;
}

VOID i2c_slave_read_event_init( VOID )
{
	i2c_Attach_SlaveRxEvent( i2c_slave_read_event_hdl );
}

VOID i2c_slave_read_event_finish( VOID )
{
	if( _gui_event_i2c_slave_read_finish != 0 ) {
		_gui_event_i2c_slave_read_finish = 0;
		i2c_Release_Bus();
	}
}

VOID i2c_slave_read_event_get_buf( UCHAR** const buf_addr, UINT* const buf_len )
{
	if( _gui_event_i2c_slave_read_finish == 0 ) {
		*buf_len = 0;
		return;
	}

	*buf_addr = (UCHAR*)g_i2c_slave_read_buf_addr;
	*buf_len = g_i2c_slave_read_buf_len;
}

#endif

