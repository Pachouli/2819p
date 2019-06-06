/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE HOGP支持头文件。
 * 作者：dengcong
 ********************************************************************************/
#ifndef __BT_ENGINE_HOG_PROFILE_H__
#define __BT_ENGINE_HOG_PROFILE_H__

#include "bt_engine_le.h"

#if (BT_SUPPORT_BLE_HOG == 1)
void APP_GATT_Hog_Start(void);
void APP_GATT_Hog_Stop(void);
void APP_GATT_Hog_Callback(void *context, UINT8 ev, void *arg);
#endif //end of #if (BT_SUPPORT_BLE_HOG == 1)

#endif //end of #ifndef __BT_ENGINE_HOG_PROFILE_H__

