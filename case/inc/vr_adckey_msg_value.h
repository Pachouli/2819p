/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������ɱ��������ťö�����͡�
 * ���ߣ�cailizhen
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
    ic_pad_e pin; /* 0xff ��ʾ����ѡ���� selector �� */
    uint8    para; /* ָ��ĳ��PINʱ��0xff��ʾ������һ��LRADC��0-7��ʾ����ͬһ·LRADC˳��ָ��Ϊѡ����ʱ��0-7��ʾ����˳�� */
} vr_adckey_pin_cfg_t;

#ifdef USE_VR_ADCKEY_PIN_CFG
static const vr_adckey_pin_cfg_t com_vr_adckey_pin_cfg[VR_ADCKEY_ID_MAX] =
{
    {PAD_GPIO13, 0},
    {PAD_GPIO19, 1},
};
#endif

#endif

