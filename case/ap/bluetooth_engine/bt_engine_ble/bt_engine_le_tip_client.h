/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE TIP CLIENT支持头文件。
 * 作者：dengcong
 ********************************************************************************/
#ifndef __BT_ENGINE_TIP_CLINET_PROFILE_H__
#define __BT_ENGINE_TIP_CLINET_PROFILE_H__

#include "bt_engine_le.h"

#if (BT_SUPPORT_BLE_TIP_CLINET == 1)

void le_tip_client_start(void);
void le_tip_read_current_time(void);

#endif //end of #if (BT_SUPPORT_BLE_TIP_CLINET == 1)

#endif //end of #ifndef __BT_ENGINE_TIP_CLINET_PROFILE_H__
