
//_manfan 2018.05.18
#ifndef __hal_hdmi_h__
#define __hal_hdmi_h__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_hdmi_extern.h"

extern void iic_init(SMBUS_ID BusSel, SMBUS_SPEED Speed, hdmi_hw_info_t *p_hdmi_hw_info);
extern SMBUS_STATUS iic_sync(SMBUS_ID BusSel, uint8_t Addr, uint8_t *pDatas, uint16_t Length, SMBUS_MODE Mode);

#endif

