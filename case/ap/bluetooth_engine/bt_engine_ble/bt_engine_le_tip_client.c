/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/09/07
 *****************************************************************************************************/

#include "bt_engine_le_tip_client.h"

#if (BT_SUPPORT_BLE_TIP_CLINET == 1)
static struct GATT_ServiceStru *remote_tip_sever = NULL;
static struct GATT_CharacteristicStru *remote_tip_char = NULL;
static struct gatt_notify_cbkstru *notify_cbk = NULL;

void display_current_time(uint8 *arg)
{
    uint16 years;
    libc_memcpy(&years, arg, 2);
    BLE_PRINTF("Current time: %04u.%02u.%02u, %02u:%02u:%02u, Day of week:%u, Fractions: %u/256 s, Adjust reason: %u",
            years, *(arg + 2), *(arg + 3), *(arg + 4), *(arg + 5),
            *(arg + 6), *(arg + 7), *(arg + 8), *(arg + 9));
}

void APP_GATT_TIP_Callback(void *context, UINT8 ev, void *arg)
{
    //BLE_DBG_PRINTF("GATT TIP Callback ev %d\n",ev);
    switch (ev)
    {
        case GATT_EV_HDLVALUE:
        {
            struct ATT_HandleValueCbkStru *in = arg;
            struct GATT_CharacteristicStru * c = context;
            if(get_16bit_uuid_by_16or128bit_uuid(c->uuid.u, 2) == GATT_UUID_CURRENT_TIME
                    || get_16bit_uuid_by_16or128bit_uuid(c->uuid.u, 16) == GATT_UUID_CURRENT_TIME)
            {
                display_current_time(in->data.val.value);
            }
            FREE(arg);
        }
        break;
        case GATT_EV_TASK_COMPLETE:
        FREE(arg);
        break;

        default:
        FREE(arg);
        break;
    }
}
void le_tip_read_current_time(void)
{
    if(remote_tip_char)
    {
        APP_GATT_Character_ReadValue((void *)APP_GATT_TIP_Callback, remote_tip_char);
    }
}
void le_tip_time_notify_enable(struct APP_PeerNodeStru *remote, bool enable)
{
    struct write_ccc_stru ccc_arg;
    ccc_arg.srv_uuid = GATT_UUID_CURRENT_TIME_SERVICE;
    ccc_arg.c_uuid = GATT_UUID_CURRENT_TIME;
    ccc_arg.uuid_type = UUID_TYPE_16;
    ccc_arg.cbk = APP_GATT_TIP_Callback;
    ccc_arg.enable = enable;
    APP_GATT_Desc_WriteCCC_Enable(remote, &ccc_arg);
}

void le_tip_notify_cbk(void *arg)
{
    struct ATT_HandleValueCbkStru *in = (struct ATT_HandleValueCbkStru *)arg;
    display_current_time(in->data.val.value);
}

void le_tip_service_cbk(void *arg)
{
    struct APP_PeerNodeStru * remote = (struct APP_PeerNodeStru *)arg;

    BLE_PRINTF("le tip cbk\n");
    remote_tip_sever = le_find_service_in_remote(remote, GATT_UUID_CURRENT_TIME_SERVICE, UUID_TYPE_16);
    if(remote_tip_sever)
    {
        remote_tip_char = le_find_char_in_service(remote_tip_sever, GATT_UUID_CURRENT_TIME, UUID_TYPE_16);
    }
    else
    {
        return;
    }
    if(remote_tip_char)
    {
        notify_cbk = BTSRC_List_NodeNew(sizeof(struct gatt_notify_cbkstru));
        libc_memcpy(&notify_cbk->addr, &remote->addr, sizeof(struct HCI_AddressStru));
        notify_cbk->hdl = remote_tip_char->hdl;
        notify_cbk->cbk = le_tip_notify_cbk;
        le_remote_service_notify_reg(notify_cbk);
        le_tip_time_notify_enable(remote, TRUE);
    }
}

void le_tip_client_dis(void *arg)
{
    struct APP_PeerNodeStru * remote = (struct APP_PeerNodeStru *)arg;
    remote_tip_sever = NULL;
    remote_tip_char = NULL;

    if(notify_cbk && SMP_CompareAddr(&notify_cbk->addr, &remote->addr))
    {
        le_remote_service_notify_unreg(notify_cbk);
        LFREE(notify_cbk);
        notify_cbk = 0;
    }
}

void le_tip_client_start(void)
{
    le_client_discover_cbk_reg(le_tip_service_cbk);
    le_client_disconnect_cbk_reg(le_tip_client_dis);
}
/*void le_tip_clint_stop(struct APP_PeerNodeStru * remote)
 {
 le_tip_remote_service_stop(remote)
 }*/
#endif //end of (BT_SUPPORT_BLE_TIP_CLINET== 1)
