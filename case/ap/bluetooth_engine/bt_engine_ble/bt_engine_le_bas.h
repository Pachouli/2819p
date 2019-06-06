/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE BAS支持头文件。
 * 作者：dengcong
 ********************************************************************************/
#ifndef __BT_ENGINE_BAS_PROFILE_H__
#define __BT_ENGINE_BAS_PROFILE_H__

#include "bt_engine_le.h"

#if (BT_SUPPORT_BLE_BAS == 1)
void APP_GATT_BAS_Start(void);
void APP_GATT_BAS_Stop(void);
void APP_GATT_BAS_Callback(void *context, UINT8 ev, void *arg);
void APP_GATT_BAS_Notify(unsigned char bat);

#endif //end of #if (BT_SUPPORT_BLE_BAS == 1)

#endif //end of #ifndef __BT_ENGINE_BAS_PROFILE_H__
