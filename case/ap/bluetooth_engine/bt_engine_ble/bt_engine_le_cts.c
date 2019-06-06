/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/09/11
 *****************************************************************************************************/

#include "bt_engine_le_cts.h"

#if (BT_SUPPORT_BLE_CTS == 1)

struct GATT_ServiceStru *cts_service;

/* Battery Service Declaration */
static struct le_gatt_attr cts_attrs[] =
{
    BTSRV_LE_GATT_PRIMARY_SERVICE(GATT_UUID_CURRENT_TIME_SERVICE, UUID_TYPE_16, APP_GATT_CTS_Callback),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_CURRENT_TIME, UUID_TYPE_16, NULL, 10,
            ATT_READ | ATT_NOTIFY),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(NULL),
    BTSRV_LE_GATT_ATTRS_END(),
};

void APP_GATT_CTS_Callback(void *context, UINT8 ev, void *arg)
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
                BLE_DBG_PRINTF("CTS HDL_TREE\n");
                struct GATT_ServiceStru *service = arg;
                BLE_DBG_PRINTF("cts service hdl : %d~%d\n",service->hdl,service->endhdl);
                BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
                cts_service = arg;
            }
             break;
        default:
            APP_GATT_Callback(context, ev, arg);
            break;
    }
}

void APP_GATT_CTS_Start(void)
{
    APP_LE_REGISTER_SERVICE(cts_attrs);
}

void APP_GATT_CTS_Stop(void)
{
    cts_service = NULL;
}
/*current time char value各个byte含义：
0-1：years
2:months
3:day
4:hours
5:minutes
6:seconds
7:Day of Week
8:Fractions 256 part of 'Exact Time 256'
9:Adjust reason
*/
void APP_GATT_CTS_Notify(UINT8 *buf)
{
    struct GATT_CharacteristicStru *c;
    struct GATT_DescriptorStru * d;

    if(buf && cts_service)
    {
        c = le_find_char_in_service(cts_service, GATT_UUID_CURRENT_TIME, UUID_TYPE_16);
        APP_GATT_Server_NotifyIndicate(c->value_hdl, buf, 10);
    }
}
#endif //end of (BT_SUPPORT_BLE_HOG == 1)
