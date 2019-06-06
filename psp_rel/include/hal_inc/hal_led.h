
#ifndef __HAL_LED_H__
#define __HAL_LED_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_led_extern.h"

//private defines
extern void hal_led_phy_init(void *p_led_phy_para);
extern void hal_led_phy_exit(void);
extern void hal_led_refresh(void);
extern void hal_led_display(uint32 *p_seg_disp_data);
extern void hal_led_adjust_light(uint8 light_level);

#endif
