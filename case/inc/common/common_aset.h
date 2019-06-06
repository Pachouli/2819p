/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASET工具供外部引用的头文件
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __COMMON_ASET_H
#define __COMMON_ASET_H

#include "applib.h"
#include "common_stub.h"
#include "post_process.h"

#if 0
extern void aset_test_loop_deal_inner(void);
#endif
#define aset_test_loop_deal() ((void(*)(void))COM_ASET_TEST_LOOP_DEAL)()

#if 0
extern int32 aset_switch_aux_mode_inner(uint8 mode);
#endif
#define aset_switch_aux_mode(a) ((void(*)(uint8))COM_ASET_SWITCH_AUX_MODE)((a))

#endif

