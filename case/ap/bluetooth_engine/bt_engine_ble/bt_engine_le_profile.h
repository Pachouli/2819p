/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/

#ifndef _BT_ENGINE_LE_PROFILE_H_
#define _BT_ENGINE_LE_PROFILE_H_

#include "bt_engine_passthrough_profile.h"

//battery level
#define BLE_PROFILE_BASS 0

//actions define
#define BLE_PROFILE_ACTS 1
#if (BLE_PROFILE_ACTS == 1)
#define BLE_SPP_TEST_FUN 0 //a user define test func
#endif

//whether support tip
#define __SUPPORT_BLE_TIP_ 0

//wechat
#define __SUPPORT_BLE_WX_ 0

//ancs
#define __SUPPORT_BLE_ANCS_ 0

#endif
