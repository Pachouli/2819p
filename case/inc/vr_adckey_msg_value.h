/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：可变电阻器旋钮枚举类型。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _VR_ADCKEY_MSG_VALUE_H
#define _VR_ADCKEY_MSG_VALUE_H

#include "psp_includes.h"

typedef enum
{
    VR_ADCKEY_ID_ECHO_DECAY = 0,
    VR_ADCKEY_ID_MIC_VOLUME = 1,
    VR_ADCKEY_ID_MAX,
} vr_adckey_id_e;

typedef struct
{
    ic_pad_e pin; /* 0xff 表示接在选择器 selector 上 */
    uint8    para; /* 指定某个PIN时，0xff表示单独接一个LRADC，0-7表示复用同一路LRADC顺序，指定为选择器时，0-7表示复用顺序 */
} vr_adckey_pin_cfg_t;

#ifdef USE_VR_ADCKEY_PIN_CFG
static const vr_adckey_pin_cfg_t com_vr_adckey_pin_cfg[VR_ADCKEY_ID_MAX] =
{
    {PAD_GPIO13, 0},
    {PAD_GPIO19, 1},
};
#endif

#endif

