
#ifndef __HAL_TWI_MASTER_H__
#define __HAL_TWI_MASTER_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_twi_master_extern.h"

//private defines
extern void TWI_set_hw_info_sim(uint8 idx, twi_hw_info_t *p_twi_hw_info);
extern uint8 TWI_write_bytes_sim(uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode);
extern uint8 TWI_read_bytes_sim(uint8 idx, uint8 *buf, uint8 address, uint8 length);

extern void TWI_master_set_hw_info(uint8 idx, twi_master_hw_info_t *p_twi_master_hw_info);
extern uint8 TWI_master_write_bytes(uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode);
extern uint8 TWI_master_read_bytes(uint8 idx, uint8 *buf, uint8 address, uint8 length);

#endif
