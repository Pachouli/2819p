/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/08/23
 *****************************************************************************************************/

#include "bt_engine_le_bas.h"

#if (BT_SUPPORT_BLE_BAS == 1)

static unsigned char battery = 100;
struct GATT_ServiceStru *bas_service;

/* Battery Service Declaration */
static struct le_gatt_attr bas_attrs[] =
{
    BTSRV_LE_GATT_PRIMARY_SERVICE(GATT_UUID_BATTERY_SERVICE, UUID_TYPE_16, APP_GATT_BAS_Callback),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_BATTERY_LEVEL, UUID_TYPE_16, &battery, sizeof(battery),
            ATT_READ | ATT_NOTIFY),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED),
    BTSRV_LE_GATT_ATTRS_END(),
};

void APP_GATT_BAS_Callback(void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
            GATT_Server_PendIND_Response(arg);
            break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
            {
                //注册本地的service时会返回，AP记录下来
                BLE_DBG_PRINTF("BAS HDL_TREE\n");
                struct GATT_ServiceStru *service = arg;
                BLE_DBG_PRINTF("bas service hdl : %d~%d\n",service->hdl,service->endhdl);
                BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
                bas_service = arg;
            }
            break;

        default:
            APP_GATT_Callback(context, ev, arg);
            break;

    }
}

void APP_GATT_BAS_Start(void)
{
    APP_LE_REGISTER_SERVICE(bas_attrs);
}
void APP_GATT_BAS_Stop(void)
{

    bas_service = NULL;
}
void APP_GATT_BAS_Notify(unsigned char bat)
{
    struct GATT_CharacteristicStru *c;
    if(bat > 100)
    {
        BLE_PRINTF_ERROR("battery value is %d\n:",bat);
        return;
    }
    battery = bat;
    if(bas_service)
    {
        c = le_find_char_in_service(bas_service, GATT_UUID_BATTERY_LEVEL, UUID_TYPE_16);
        APP_GATT_Server_NotifyIndicate(c->value_hdl,&battery,sizeof(battery));
    }
}
#endif //end of (BT_SUPPORT_BLE_BAS == 1)
