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
#include "bt_engine_le_server.h"
#include "bt_engine_le_test.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

//GATT Event 默认处理函数
void APP_GATT_Event_Default_Deal(void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {

        //远端设备notify到来事件
        case GATT_EV_HDLVALUE:
        {
#if (BT_SUPPORT_BLE_CLIENT == 1)
            struct ATT_HandleValueCbkStru* in = (struct ATT_HandleValueCbkStru*) arg;
            if (!context)
            {
                le_notify_ev_hdlvalue(in);
            }
            else
            {

            }
#endif
            FREE(arg);
        //deal_gatt_ev_hdlvalue((struct ATT_HandleValueCbkStru*)arg);
        }
        break;

        //GATT任务完成事件
        case GATT_EV_TASK_COMPLETE:
        //deal_gatt_ev_task_complete((struct GATT_CharacteristicStru *)context, (struct GATT_TaskCompleteStru *)arg);
        break;

        //GATT任务创建事件
        case GATT_EV_TASK_CREATE: //在任务创建时上报至此
        {
            struct GATT_TaskCompleteStru *in = arg;
            g_bteg_le_env.app_attroot->task_index = in->index; /* remember this */
            BLE_PRINTF("task id 0x%x\n", in->task);
            FREE(arg);
        }
        break;

        //GATT任务创建服务
        case GATT_EV_CREATE_SERVICE:
        //deal_gatt_ev_creat_service((struct GATT_ServiceStru*)arg);
        break;

        //GATT任务创建INCLUDE
        case GATT_EV_CREATE_INCLUDE:
        //deal_gatt_ev_creat_include((struct GATT_IncludeStru*)arg);
        break;

        //GATT任务创建特性声明
        case GATT_EV_CREATE_CHARACTER:
        //deal_gatt_ev_creat_character((struct GATT_CharacteristicStru*)arg);
        break;

        //GATT任务创建特性描述符
        case GATT_EV_CREATE_DESCRIPTOR:
        //deal_gatt_ev_creat_descriptor((struct GATT_DescriptorStru*)arg);
        break;

        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
        deal_gatt_ev_pend_ind((struct ATT_PendIndStru *) arg); //函数内部释放arg
        break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
        //注册本地的service时会返回，AP记录下来
        BLE_DBG_PRINTF("HDL_TREE\n")
        ;
        struct GATT_ServiceStru *service = arg;
        BLE_DBG_PRINTF("defaul service(%p) hdl : %d~%d\n", service, service->hdl, service->endhdl)
        ;
        BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg); /* struct GATT_ServiceStru */
        break;

        //GATT本地notify完成确认事件
        case GATT_EV_HDLVALUE_NOTIFY_CONFIRM:
        {
            FREE(arg);
        }

        break;

        default:
        FREE(arg);
        break;
    }
}

//GATT Event 分发函数
void APP_GATT_Callback(void *context, UINT8 ev, void *arg)
{
    if (g_bteg_le_env.app_attroot != NULL)
    {
        APP_GATT_Event_Default_Deal(context, ev, arg);
    }
}

/*********** callback func ***********/
//ATT's channel callback
void APP_GATT_Channel_Callback(void *context, UINT8 Event, void *arg)
{
    if (g_bteg_le_env.app_attroot != NULL)
    {
        switch (Event)
        {
            case ATT_CHANNEL_MTU_REQ:
            {
                struct APP_PeerNodeStru *remote;
                struct HCI_AddressStru *bt_addr = (struct HCI_AddressStru *) context;

                uint16 mtu = *(uint8 *) arg | ((uint16) (*(uint8 *) (arg + 1)) << 8);
                uint16 data_mtu = mtu - 3;

                remote = g_bteg_le_env.rmt_client.p_remote_node;
                if ((remote != NULL) && (SMP_CompareAddr(bt_addr, &remote->addr) != 0))
                {
                    g_bteg_le_env.rmt_client.mtu = data_mtu;
                    BLE_PRINTF("rmt client mtu %d\n", data_mtu);
                }
            }
            break;
            default:
            break;
        }
    }
}

void deal_gatt_ev_pend_ind(struct ATT_PendIndStru *in)
{
    GATT_Server_PendIND_Response(in);
}

#endif //end of #ifdef BT_SUPPORT_BLE
