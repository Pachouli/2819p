/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������ID���塣
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_KT_ID_H_
#define _COMMON_KT_ID_H_

#include "psp_includes.h"

typedef enum
{
    KT_ID_KEYTONE = 0,
    KT_ID_MAX, /*Ϊ�˼���Ӳ����������಻�ܳ���16*/
} kt_id_e;

#ifdef USE_KEYTONE_FILE
//�ļ���������8+3��ʽ
static const uint8 g_keytone_files[KT_ID_MAX][13] =
{
    "keytone.pcm",
};
#endif

#endif
