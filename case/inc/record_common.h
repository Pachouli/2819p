/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������ز�����غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�Ϊ���ز���ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __RECORD_COMMON_H__
#define __RECORD_COMMON_H__

#include "psp_includes.h"
#include "record_module_extern.h"
#include "enhanced.h"

//RECORDǰ̨����ģʽ
#define PARAM_RECORD_CARD_DIR     0 //����Ŀ¼¼��
#define PARAM_RECORD_UHOST_DIR    1 //U��Ŀ¼¼��
#define PARAM_RECORD_RECOVER      2 //¼���ļ��ָ�

typedef enum
{
    RECORD_STA_STOP = 0,
    RECORD_STA_RECORDING,
    RECORD_STA_PAUSE,
    RECORD_STA_ERROR,
} record_status_e;

#endif //__RECORD_COMMON_H__
