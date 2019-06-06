/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _KEY_COMMON_INNER_H
#define _KEY_COMMON_INNER_H

#include "psp_includes.h"
#include "key_interface.h"

typedef enum
{
	KEY_PHY_TYPE_KEY = 0,
	KEY_PHY_TYPE_IR,
} key_phy_type_e;

typedef enum
{
    KEY_ACTIVE_PHY_TYPE_NULL = 0,
	KEY_ACTIVE_PHY_TYEP_KEY,
	KEY_ACTIVE_PHY_TYEP_IR,
} key_active_phy_type_e;

extern keymap_para_t g_keymap_para;

#define SPECIAL_KEY_SHOUT_UP 2
//short_up 0-DOWN,1-SHORT_UP,2-SPECIAL_KEY_SHOUT_UP
extern void PutSysMsg(uint8 short_up, uint8 key, key_phy_type_e phy_type);

#endif
