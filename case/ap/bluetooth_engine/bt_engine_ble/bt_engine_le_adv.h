/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_LE_ADV_H__
#define __BT_ENGINE_LE_ADV_H__

#include "bt_engine_le.h"

#ifdef NEW_ADV_INTERFACT_TEST
extern void *app_new_adv_info(void);
extern void app_set_ble_adv_data(void *adv_info, void *adv_data, uint8 adv_len, tHCI_EIR_DataType adv_type);
extern bool app_set_ble_adv_data_complete(void *adv_info);
#endif

extern void ATT_Set_Scan_Response_Data(att_advertiser_info_t *att_advertiser_info);
extern void APP_LE_Set_Advertiser(UINT8 *arg);
extern void ATTS_Set_Advertiser(att_advertiser_info_t *att_advertiser_info);

#endif //end of #ifndef __BT_ENGINE_LE_ADV_H__
