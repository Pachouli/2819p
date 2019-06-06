
//_manfan 2018.05.18
#include "hal_inner.h"

void hal_iic_init(SMBUS_ID bus, SMBUS_SPEED speed, hdmi_hw_info_t *p_hdmi_hw_info)
{
    iic_init(bus, speed, p_hdmi_hw_info);
}

SMBUS_STATUS hal_iic_sync(SMBUS_ID bus, uint8 addr, uint8 *buf, uint16 length, SMBUS_MODE mode)
{
    SMBUS_STATUS ret = SMBUS_STATUS_Success;

    ret = iic_sync(bus, addr, buf, length, mode);

    return ret;
}

