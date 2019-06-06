/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动API接口表，必须与 key_interface.h 中的API ID一一对应。
 * 作者：cailizhen
 ********************************************************************************/

#include "key_inner.h"

/*2.定义接口操作集结构体，并进行初始化*/
const uint32 key_op[] =
{
    (uint32) key_inner_get_battery,
    (uint32) key_inner_aux_hot_detect,
    (uint32) key_inner_usbhost_hot_detect,
    (uint32) key_inner_usbdev_hot_detect,
    (uint32) key_inner_adaptor_hot_detect,
    (uint32) key_inner_card_hot_detect,
    (uint32) key_inner_usb_detect_switch,

    (uint32) key_inner_adckey_switch, /*only for debug*/

#if (SUPPORT_FAST_PREVIEW == 1)
    (uint32) key_inner_set_fast_preview_cbk,
#else
    (uint32) /*key_inner_set_fast_preview_cbk*/ 0,
#endif

#if (SUPPORT_VR_ADCKEY == 1)
    (uint32) key_inner_get_vr_adc_value,
#else
    (uint32) /*key_inner_get_vr_adc_value*/ 0,
#endif

#if (SUPPORT_IR_KEY == 1)
    (uint32) key_ir_init,
    (uint32) key_ir_deinit,
#endif
};

