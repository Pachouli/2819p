/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：软按键音ID定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_KT_ID_H_
#define _COMMON_KT_ID_H_

#include "psp_includes.h"

typedef enum
{
    KT_ID_KEYTONE = 0,
    KT_ID_MAX, /*为了兼容硬按键音，最多不能超过16*/
} kt_id_e;

#ifdef USE_KEYTONE_FILE
//文件名必须是8+3格式
static const uint8 g_keytone_files[KT_ID_MAX][13] =
{
    "keytone.pcm",
};
#endif

#endif
