/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/09/05
 *****************************************************************************************************/

#include "bt_engine_le_client.h"

#if (BT_SUPPORT_BLE_CLIENT == 1)
//discover service 完成后的回调通知相关client
struct ListStru le_client_discover_list =
{   0};
//le设备断开后回调通知相关client
struct ListStru le_client_disconnect_list =
{   0};

struct ListStru notify_cbk_list =
{   0};

static void APP_GATT_Discover_Callback(void *context, UINT8 ev, void *arg)
{
    struct APP_PeerNodeStru *remote = context;
    struct GATT_ServiceStru *service = remote->cur_service;
    struct GATT_CharacteristicStru *c = remote->cur_character;
    struct GATT_IncludeStru *include;
    struct GATT_DescriptorStru *desc;
    void *k;
    UINT8 type = 0;

    BLE_PRINTF("client event %d\n",ev);
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
        GATT_Server_PendIND_Response(arg);
        break;

        case GATT_EV_CREATE_SERVICE:
        service = arg;
        //BLE_DBG_PRINTF("s-%02x-%02x",service->hdl,service->endhdl);
        k = APP_GATT_ItemGetByHDL(remote, &type, service->hdl);
        if (k != NULL)
        {/* Kill Duplicated */
            if (type == ATT_TYPE_SERVICE)
            {
                BTSRC_List_RemoveAt(&remote->service_list, k);
                GATT_Free_Service(k);
            }
            else
            {
                BLE_DBG_PRINTF("Found, but not a service,Hdl:0x%x,Type:%d\r\n", service->hdl, type);
            }
        }
        BTSRC_List_AddTail(&remote->service_list, service);
        break;
        case GATT_EV_CREATE_INCLUDE:
        include = arg;
        k = APP_GATT_ItemGetByHDL(remote, &type, include->hdl);
        if (k != NULL)
        {
            /* Kill Duplicated */
            if (type == ATT_TYPE_INCLUDE)
            {
                BTSRC_List_RemoveAt(&service->include, k);
                LFREE(k);
            }
            else
            {
                BLE_DBG_PRINTF("Found, but not a include,Hdl:0x%x,Type:%d\r\n", include->hdl, type);
            }
        }
        BTSRC_List_AddTail(&service->include, include);
        break;
        case GATT_EV_CREATE_CHARACTER:
        c = arg;
        k = APP_GATT_ItemGetByHDL(remote, &type, c->hdl);
        if (k != NULL)
        {/* Kill Duplicated */
            if (type == ATT_TYPE_CHARACTER)
            {
                /* UPF43, should make sure if k in this service or not??? maybe in others! */
                BTSRC_List_RemoveAt(&service->character, k);
                GATT_Free_Character(k);
            }
            else
            {
                BLE_DBG_PRINTF("Found, but not a character,Hdl:0x%x,Type:%d\r\n", c->hdl, type);
            }
        }
        BTSRC_List_AddTail(&service->character, c);
        break;
        case GATT_EV_CREATE_DESCRIPTOR:
        desc = arg;
        k = APP_GATT_ItemGetByHDL(remote, &type, desc->hdl);
        if (k != NULL)
        {/* Kill Duplicated */
            if (type == ATT_TYPE_DESCRIPTOR)
            {
                BTSRC_List_RemoveAt(&c->descriptor, k);
                GATT_Free_Descriptor(k);
            }
            else
            {
                BLE_DBG_PRINTF("Found, but not a descriptor,Hdl:0x%x,Type:%d\r\n", desc->hdl, type);
            }
        }
        BTSRC_List_AddTail(&c->descriptor, desc);
        break;
        case GATT_EV_TASK_COMPLETE:/* FIXME, callback arg, need result */
        {
            struct GATT_TaskCompleteStru *in = arg;
            APP_GATT_AutoCache(in->task, remote);
            FREE(arg);
        }
        break;
        default:
        //APP_GATT_Callback(context, ev, arg);
        FREE(arg);
        break;
    }
}

//初始化
void APP_GATT_Init_TLIn(struct GATT_TL_ConnectStru *tlin, void *context, UINT8 sec_mode, void* cbk)
{
    tlin->cbk = cbk;
    tlin->context = context;
    tlin->require = (tlin->addr.cap & HCI_ADDRESS_CAP_LE) ? GATT_TLREQUIRE_LE : GATT_TLREQUIRE_BREDR;/* FIXME, auto */
    tlin->sec_mode = sec_mode;
    tlin->timeout = 0xFFFFFFFF;/* Disconnect mannuly(keep link), 0 for disconnect now, 10~60000 for idle timeout */
}

//void APP_GATT_Character_ReadValue(UINT8 *arg)
void APP_GATT_Character_ReadValue(void *cbk, struct GATT_CharacteristicStru *read_c)
{
    if(read_c && cbk)
    {
        //APP_GATT_Init_TLIn(c, APP_LE_READWRITE_SECURITY_MODE);
        APP_GATT_Init_TLIn(&g_bteg_le_env.app_attroot->tlin, read_c,
                APP_LE_DISCOVER_SECURITY_MODE, cbk);
        GATT_Read_Value(&g_bteg_le_env.app_attroot->tlin, read_c->value_hdl);// Characteristic
    }
}

static UINT8 APP_GATT_Discover_Character(struct APP_PeerNodeStru *remote)
{
    struct GATT_ServiceStru *service = remote->cur_service;
    if (service->hdl + 1 <= service->endhdl)
    {
        APP_GATT_Init_TLIn(&g_bteg_le_env.app_attroot->tlin, remote,
                APP_LE_DISCOVER_SECURITY_MODE, APP_GATT_Discover_Callback);
        GATT_Discover_Characteristics(&g_bteg_le_env.app_attroot->tlin, (UINT16)(service->hdl + 1),
                service->endhdl, 0);
        return 1;
    }
    else
    {
        //printf("\r\n[No_Character]\r\n");
        return 0;
    }
}
static UINT8 APP_GATT_Discover_Desc(struct APP_PeerNodeStru *remote)
{
    /* Discover Desc of a Character */
    struct GATT_ServiceStru *service = remote->cur_service;
    struct GATT_CharacteristicStru *c = remote->cur_character;
    UINT16 starthdl = c->value_hdl + 1;
    UINT16 endhdl;

    c = LNEXT(c);/* goto next character */
    if (c != NULL)
    {
        //if (c >= 0x10000000){
        endhdl = c->hdl - 1;
    }
    else
    {
        endhdl = service->endhdl;
    }

    if (starthdl <= endhdl)
    {
        APP_GATT_Init_TLIn(&g_bteg_le_env.app_attroot->tlin, remote,
                APP_LE_DISCOVER_SECURITY_MODE, APP_GATT_Discover_Callback);/* Display Only, no MITM */
        GATT_Discover_AllCharacteristic_Descriptors(&g_bteg_le_env.app_attroot->tlin,
                starthdl, endhdl);
        return 1;
    }
    else
    {
        //printf("\r\nDescriptor have no Handle Space\r\n");
        return 0;
    }

}

void le_remote_service_notify_reg(void *arg)
{
    BTSRC_List_AddTail(&notify_cbk_list, arg);
}

void le_remote_service_notify_unreg(void *arg)
{
    BTSRC_List_RemoveAt(&notify_cbk_list, arg);
}

void le_notify_ev_hdlvalue(struct ATT_HandleValueCbkStru *arg)
{
    struct gatt_notify_cbkstru *n = NULL;

    n = notify_cbk_list.head;
    while(n)
    {
        if(SMP_CompareAddr(&n->addr, &arg->addr) && n->hdl == arg->data.hdl)
        {
            n->cbk(arg);
        }
        n = LNEXT(n);
    }
}

void le_client_disconnect_cbk_reg(cbk_ptr arg)
{
    struct cbk_stru *c = BTSRC_List_NodeNew(sizeof(struct cbk_stru));
    c->cbk = arg;
    BTSRC_List_AddTail(&le_client_disconnect_list, c);
}

void le_client_disconnect_cbk_unreg(void)
{
    BTSRC_List_RemoveAll(&le_client_disconnect_list);
}

void le_client_disconnect_cbk(struct APP_PeerNodeStru *remote)
{
    struct cbk_stru *c = le_client_disconnect_list.head;
    while(c)
    {

        c->cbk(remote);
        c = LNEXT(c);
    }
}

void le_client_discover_cbk_reg(cbk_ptr arg)
{
    struct cbk_stru *c = BTSRC_List_NodeNew(sizeof(struct cbk_stru));
    c->cbk = arg;
    BTSRC_List_AddTail(&le_client_discover_list, c);
}
void le_client_discover_cbk_unreg(void)
{
    BTSRC_List_RemoveAll(&le_client_discover_list);
}

void le_client_discover_complete_cbk(struct APP_PeerNodeStru *remote)
{
    struct cbk_stru *c = le_client_discover_list.head;
    while(c)
    {

        c->cbk(remote);
        c = LNEXT(c);
    }
}

//是完成发现设备所有服务等等属性的状态机
//关键所在:就是一层一层发现
void APP_GATT_AutoCache(UINT8 task, void *context)
{
    struct APP_PeerNodeStru *remote = context;

    switch (task)
    { /* Check Context type */
        case GATT_TASK_DISCOVER_ALLSERVICES:
        case GATT_TASK_DISCOVER_SERVICES_BYUUID:
        remote->cur_service = remote->service_list.head;
        if(remote->cur_service)
        {
            //APP_GATT_AutoCache_FindInclude();/* Find Include */
            if (APP_GATT_Discover_Character(remote) == 0)
            {/* No Character */
                goto service;
            }
        }
        break;

        case GATT_TASK_DISCOVER_ALLCHARACTERISTIC_OFASERVICE:
        case GATT_TASK_DISCOVER_CHARACTERISTIC_BYUUID:
        remote->cur_character = remote->cur_service->character.head;/* 1st time */
        if(remote->cur_character)
        {
            if (APP_GATT_Discover_Desc(remote) == 0)
            {/* No desc */
                goto desc;
            }
        }
        else
        {
            goto service;
        }
        break;

        case GATT_TASK_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS:
        desc:
        remote->cur_character = LNEXT(remote->cur_character);
        if(remote->cur_character)
        {
            if (APP_GATT_Discover_Desc(remote) == 0)
            {/* No desc */
                goto desc;
            }
        }
        else
        {
            service:
            remote->cur_service = LNEXT(remote->cur_service);
            if(remote->cur_service)
            {
                //APP_GATT_AutoCache_FindInclude();/* Find Include */
                if (APP_GATT_Discover_Character(remote) == 0)
                {/* No Character */
                    goto service;
                }
            }
            else
            {
                le_client_discover_complete_cbk(remote);
            }
        }
        break;

        case GATT_TASK_FIND_INCLUDED_SERVICES:
        break;
    }
}

//发起发现设备的全部信息，关键是APP_ATTMASK_AUTOCACHE
void APP_GATT_Discover(struct APP_PeerNodeStru *remote)
{
    //struct APP_PeerNodeStru *remote = NULL;
    struct GATT_ServiceStru *service = NULL;

    BLE_DBG_PRINTF("GATT DISCOVER\n");
    if (remote == NULL)
    {
        BLE_DBG_PRINTF("remote == NULL\n");
        return;
    }

    APP_PeerNode_SelectCUR(remote);

    /* Clean all exist services, FIXME */
    while ((service = BTSRC_List_RemoveHead(&remote->service_list)) != NULL)
    {
        GATT_Free_Service(service);/* Clean all exist services, FIXME */
    }

    remote->mask |= APP_PEERMASK_DISCOVRING; //正在DISCOVER

    APP_GATT_Init_TLIn(&g_bteg_le_env.app_attroot->tlin, remote,
            APP_LE_READWRITE_SECURITY_MODE, APP_GATT_Discover_Callback);
    GATT_Discover_Services(&g_bteg_le_env.app_attroot->tlin, NULL);//uuid
}

void APP_GATT_Client_Remove_services(struct APP_PeerNodeStru* remote)
{
    struct GATT_ServiceStru *service;
    //remove all services read form remote
    while ((service = BTSRC_List_RemoveHead(&remote->service_list)) != NULL)
    {
        //printf("  %s 0x%x\n", "free",*(uint32*)service);
        GATT_Free_Service(service);
    }
}

static struct ATT_HandleValueStru *APP_GATT_ValueInput(UINT8 *arg, UINT8 len, UINT16 hdl)
{
    struct ATT_HandleValueStru *in;
    UINT8 i;

    in = NEW(sizeof(struct ATT_HandleValueStru) + len);
    in->hdl = hdl;
    in->offset = 0;
    in->val.len = len;

    i = 0;
    while (len--)
    {
        in->val.value[i] = arg[i];
        i++;
    }

    return in;
}

void APP_GATT_Desc_WriteCCC_Enable(struct APP_PeerNodeStru *remote, struct write_ccc_stru *ccc_arg) //UINT16 srv_uuid, UINT16 c_uuid, bool enable)
{
    struct GATT_CharacteristicStru *c = NULL;
    struct GATT_DescriptorStru *d = NULL;
    struct ATT_HandleValueStru *in;
    UINT16 data;

    if((c = le_find_char_in_remote(remote, ccc_arg->srv_uuid, ccc_arg->c_uuid, ccc_arg->uuid_type))
            && (d = le_find_CCC_in_char(c)))
    {
        if(!ccc_arg->enable)
        {
            data = 0x0000;
        }
        else if(c->properties & ATT_NOTIFY)
        {
            data = 0x0001;
        }
        else if(c->properties & ATT_INDICATE)
        {
            data = 0x0002;
        }
        else
        {
            BLE_DBG_PRINTF("c_uuid %x not support notify and indicate",ccc_arg->c_uuid);
            return;
        }

        in = APP_GATT_ValueInput((UINT8 *)&data, 2, d->hdl);

        APP_GATT_Init_TLIn(&g_bteg_le_env.app_attroot->tlin, remote,
                APP_LE_READWRITE_SECURITY_MODE, ccc_arg->cbk);
        //APP_GATT_Init_TLIn(d, LE_GAP_SECURITY_MODE1_LEVEL1);
        GATT_Write_Value(&g_bteg_le_env.app_attroot->tlin, in, GATT_TASK_WRITE_VALUE);// Characteristic
    }
    else
    {
        BLE_DBG_PRINTF("not found ccc\n");
    }
}

void le_client_start(void)
{
#if (BT_SUPPORT_BLE_TIP_CLINET == 1)
    le_tip_client_start();
#endif
}
void le_client_stop(void)
{
    le_client_discover_cbk_unreg();
    le_client_disconnect_cbk_unreg();
}

void le_client_connect(struct APP_PeerNodeStru * remote)
{
    APP_GATT_Discover(remote);
}

void le_client_disconnect(struct APP_PeerNodeStru * remote)
{
    le_client_disconnect_cbk(remote);
    APP_GATT_Client_Remove_services(remote);
}
/*
void le_client_test_loop(void)
{
    static uint32 last_time = 0;
    uint32 cur_time = sys_get_ab_timer();
    if (cur_time - last_time > 5000)
    {
        le_tip_read_current_time();
        last_time = cur_time;
    }
}
*/
#endif //end of (BT_SUPPORT_BLE_CLINET == 1)
