/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_profile.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

/*---------------------------------------------------------------------------
 Description:
 GATT menu...
 ---------------------------------------------------------------------------*/
void APP_LE_Set_Scan_Disable(UINT8 *arg)
{
    if (g_bteg_le_env.s_att_inited < 0)
    {
        return;
    }
    //DBG_BLE("APP_LE_Set_Scan_Disable");
    LE_Set_Scan_Enable((UINT8)HCI_LE_SCAN_ENABLE_SCANNING_DISABLED);
}

void APP_LE_Set_Scan_Enable(UINT8 *arg)
{
    if (g_bteg_le_env.s_att_inited == 0)
    {
        return;
    }
    //DBG_BLE("APP_LE_Set_Scan_Enable");
    LE_Set_Scan_Enable((UINT8)HCI_LE_SCAN_ENABLE_SCANNING_ENABLED);
    //g_bteg_le_env.app_attroot->adv_count = 0;
}

static void APP_LE_Set_Scan_Parameters(UINT8 *arg)
{
    //"[7]LE_Set_Scan_Parameters"
    //passive就是被动的，就只有收外设的ADVERTISING
    //active就是主动的，在收到ADVERTISING后还会发scan req
    //其实最终都是有advertising_reporting ev上来。
    //active会有更多数据
    LE_Set_Scan_Parmeters((UINT8)HCI_LE_SCAN_TYPE_ACTIVE_SCANNING, (UINT8)LE_DISCOVERYMODE_GENERAL, (UINT8)LE_FAST);
}

void APP_LE_Set_Scaner(UINT8 *arg)
{
    APP_LE_Set_Scan_Parameters(arg); /* internal */
    APP_LE_Set_Scan_Enable(arg); /* internal */
}

#endif //#ifdef SUPPORT_BLE_DEF
