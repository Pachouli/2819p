/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：IC内置按键音使用模式宏定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_KT_MODE_H_
#define _COMMON_KT_MODE_H_

#include "psp_includes.h"
#include "common_keytone.h"

#define KT_MODE_KEYTONE  ((KT_FREQ_1000HZ<<KT_FREQ_SHIFT) | (KT_PERIOD_48<<KT_PERIOD_SHIFT) | KT_BLOCK_NO)

#endif
