
// プロジェクト固有
#include "ir_frame.h"

// WinAVR
#include <avr/eeprom.h>


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// API
BOOL Ir_Write_Eeprom( UINT addr )
{
	UINT		loopcnt;

	if( (gIr_Frame.byte_idx == 0) && (gIr_Frame.bit_mask == 0x80) ) {
		// empty
		return FALSE;
	}

	// Magic string書き込み
	eeprom_write_byte( (uint8_t *)(addr++), 'I' );
	eeprom_write_byte( (uint8_t *)(addr++), 'R' );
	// データフォーマットバージョン書き込み
	eeprom_write_byte( (uint8_t *)(addr++), 0x01 );

	// データ書き込み
	eeprom_write_byte( (uint8_t *)(addr++), gIr_Frame.type );
	eeprom_write_byte( (uint8_t *)(addr++), gIr_Frame.byte_idx );
	eeprom_write_byte( (uint8_t *)(addr++), gIr_Frame.bit_mask );

	for( loopcnt = 0; loopcnt <= gIr_Frame.byte_idx; loopcnt++ ) {
		eeprom_write_byte( (uint8_t *)(addr++), gIr_Frame.data[loopcnt] );
	}

	return TRUE;
}

BOOL Ir_Read_Eeprom( UINT addr )
{
	UINT		loopcnt;
	BOOL		retcd = FALSE;

	while( TRUE ) {
		// Magic string書き込み
		if( eeprom_read_byte( (uint8_t *)(addr++) ) != 'I' ) {
			break;
		}
		if( eeprom_read_byte( (uint8_t *)(addr++) ) != 'R' ) {
			break;
		}
		// データフォーマットバージョン書き込み
		if( eeprom_read_byte( (uint8_t *)(addr++) ) !=  0x01 ) {
			break;
		}

		// データ書き込み
		gIr_Frame.type = eeprom_read_byte( (uint8_t *)(addr++) );
		gIr_Frame.byte_idx = eeprom_read_byte( (uint8_t *)(addr++) );
		gIr_Frame.bit_mask = eeprom_read_byte( (uint8_t *)(addr++) );

		if( gIr_Frame.byte_idx > sizeof(gIr_Frame.data) ) {
			break;
		}
		for( loopcnt = 0; loopcnt <= gIr_Frame.byte_idx; loopcnt++ ) {
			gIr_Frame.data[loopcnt] = eeprom_read_byte( (uint8_t *)(addr++) );
		}

		retcd = TRUE;
		break;
	}

	if( retcd == FALSE ) {
		gIr_Frame.byte_idx = 0;
	}

	return retcd;
}

