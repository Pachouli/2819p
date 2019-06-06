/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������IC���ð�������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_KEYTONE_H_
#define _COMMON_KEYTONE_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "mmm_ktp.h"

#define KT_FREQ_OPT      (3 << 0)
#define KT_FREQ_SHIFT    (0)

#define KT_PERIOD_OPT    (3 << 2)
#define KT_PERIOD_SHIFT  (2)

#define KT_BLOCK_NO    (0 << 4)
#define KT_BLOCK_YES   (1 << 4)
#define KT_BLOCK_OPT   (1 << 4)

#if 0
extern void keytone_set_on_off_inner(bool on_off);
#endif
#define keytone_set_on_off(a)    ((void(*)(bool))COM_KEYTONE_SET_ON_OFF)((a))

#if 0
extern bool keytone_play_inner(uint8 kt_mode);
#endif
#define keytone_play(a)          ((bool(*)(uint8))COM_KEYTONE_PLAY)((a))

#endif
