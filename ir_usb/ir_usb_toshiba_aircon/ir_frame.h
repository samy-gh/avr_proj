
#ifndef __IR_FRAME_H__
#define __IR_FRAME_H__

#include "usb_proto.h"


typedef enum {
	E_DRIVE_MODE_AUTO = 0,
	E_DRIVE_MODE_COOLER,
	E_DRIVE_MODE_DEHUMI,
	E_DRIVE_MODE_HEATER,
	E_DRIVE_MODE_STOP,
	E_DRIVE_MODE_MAX,
} E_DRIVE_MODE;

typedef enum {
	E_STRENGTH_NORMAL = 0,
	E_STRENGTH_POWERFUL,
	E_STRENGTH_SAVING,
	E_STRENGTH_MAX,
} E_STRENGTH;

typedef enum {
	E_FANSPEED_AUTO = 0,
	E_FANSPEED_SILENT,
	E_FANSPEED_SLOW,
	E_FANSPEED_MID,
	E_FANSPEED_FAST,
	E_FANSPEED_MAX,
} E_FANSPEED;

typedef enum {
	E_HUMI_40 = 0,
	E_HUMI_45,
	E_HUMI_50,
	E_HUMI_55,
	E_HUMI_60,
	E_HUMI_65,
	E_HUMI_MAX,
} E_HUMI;

typedef struct {
	E_DRIVE_MODE	d_mode;
	float			templature;		// d_mode = E_DRIVE_MODE_AUTO, E_DRIVE_MODE_COOLER or E_DRIVE_MODE_HEATER
	E_STRENGTH		str;
	E_FANSPEED		fanspeed;
	E_HUMI			humi;			// d_mode = E_DRIVE_MODE_DEHUMI only.
} T_AIRCON_PARAM;


extern int IrFrame_AirConStart( T_USB_PROTO_IR_CODE* const ir_code, const T_AIRCON_PARAM* const aircon_param );
extern int IrFrame_AirConStop( T_USB_PROTO_IR_CODE* const ir_code );


#endif /* __IR_FRAME_H__ */

