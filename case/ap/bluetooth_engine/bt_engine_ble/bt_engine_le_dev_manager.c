/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

/*********** PeerNode Operation ***********/
void APP_PeerNode_SelectCUR(struct APP_PeerNodeStru *node)
{
    if (node != NULL)
    {
        libc_memcpy(&g_bteg_le_env.app_attroot->tlin.addr, &node->addr, sizeof(struct HCI_AddressStru)); /* Select connection candidate */
        g_bteg_le_env.app_attroot->cur_service = NULL; /* Rediscover */
        g_bteg_le_env.app_attroot->cur_character = NULL; /* Rediscover */
        g_bteg_le_env.app_attroot->cur_desc = NULL; /* Rediscover */
    }
}

struct APP_PeerNodeStru *APP_PeerNode_FindByADDR(struct HCI_AddressStru *addr)
{
    struct APP_PeerNodeStru *remote;

    if (g_bteg_le_env.s_att_inited < 0)
    {
        return NULL;
    }

    remote = g_bteg_le_env.app_attroot->remote_list.head;
    while (remote != NULL)
    {
        if (SMP_CompareAddr(&remote->addr, addr) != 0)
        { /* FIXME, private convert, UPF43 */
            break; /* Found */
        }
        remote = LNEXT(remote);
    }

    return remote;
}

struct APP_PeerNodeStru *APP_PeerNode_FindByUser(uint8 *addr)
{
    struct APP_PeerNodeStru *remote;

    if (g_bteg_le_env.s_att_inited < 0)
    {
        return NULL;
    }
    remote = g_bteg_le_env.app_attroot->remote_list.head;
    while (remote != NULL)
    {
        if (memcmp(&remote->addr.bd, addr, BD_ADDR_LEN) == 0)
        { /* FIXME, private convert, UPF43 */
            break; /* Found */
        }
        remote = LNEXT(remote);
    }

    return remote;
}

#if 0
/***********************************
 * mdoe ==0 select exist 
 * mode ==1 select empty
 ************************************/
INT8 APP_PeerNode_GETID_ByADDR(struct HCI_AddressStru *addr, UINT8 mode, remote_list_e which_list)
{
    UINT8 i = 0, ret_val = 0, con_ret = 0;

    for (i = 0; i < MAX_SERVER_NUM; i++)
    {
        if (which_list == REMOTE_SERVER)
        {
            ret_val = SMP_CompareAddr(&g_bteg_le_env.server.remote_info[i].addr, addr);

            con_ret = (g_bteg_le_env.server.remote_info[i].serv_conn & BLE_CONNECTED);
        }
        else if (which_list == REMOTE_CLIENT)
        {
            ret_val = SMP_CompareAddr(&g_bteg_le_env.client.remote_info[i].addr, addr);

            con_ret = (g_bteg_le_env.client.remote_info[i].serv_conn & BLE_CONNECTED);
        }
        else
        {
            if ((mode == EXIST_CHECK_MODE) || (mode == EMPTY_CHECK_MODE))
            {
                ret_val = SMP_CompareAddr(&g_bteg_le_env.server.remote_info[i].addr, addr) || SMP_CompareAddr(&g_bteg_le_env.client.remote_info[i].addr, addr);
            }
            else
            {
                BLE_PRINTF("Need TODO,not support now/n");
                while (1)
                ;
            }
        }

        if (mode == EXIST_CHECK_MODE)
        {
            if (ret_val != 0) //exist
            {
                break;
            }
        }
        else
        {
            //empty break
            if ((ret_val == 0) && (con_ret == 0))
            {
                break;
            }
        }
    }
    return i;
}
#endif

struct APP_PeerNodeStru *APP_PeerNode_FindCUR(void)
{
    struct APP_PeerNodeStru *remote;

    remote = APP_PeerNode_FindByADDR(&g_bteg_le_env.app_attroot->tlin.addr); /* Found Current selected remote */
    if (remote == NULL)
    {
        remote = g_bteg_le_env.app_attroot->remote_list.head; /* Patch */
    }
    return remote;
}

struct APP_PeerNodeStru *APP_PeerNode_Create(struct HCI_AddressStru *addr)
{
    struct APP_PeerNodeStru *remote = NULL;

    if (g_bteg_le_env.s_att_inited < 0)
    {
        return NULL;
    }

    remote = BTSRC_List_NodeNew(sizeof(struct APP_PeerNodeStru));
    //remote必须初始化
    libc_memset(remote, 0x00, sizeof(struct APP_PeerNodeStru));
    BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->remote_list, remote);
    libc_memcpy(&remote->addr, addr, sizeof(struct HCI_AddressStru));

    return remote;
}

void APP_PeerNode_Remove(struct HCI_AddressStru *addr)
{
    struct APP_PeerNodeStru *remote = NULL;

    if (g_bteg_le_env.s_att_inited < 0)
    {
        return;
    }

    remote = g_bteg_le_env.app_attroot->remote_list.head;
    while (remote != NULL)
    {
        if (SMP_CompareAddr(&remote->addr, addr) != 0)
        {
            List_RemoveAt(&g_bteg_le_env.app_attroot->remote_list, remote);
            LFREE(remote);
            break;
        }
        remote = LNEXT(remote);
    }
}

void APP_PeerNode_RemoveDevices(void)
{
    struct APP_PeerNodeStru *remote;
    struct GATT_ServiceStru *service;

    while (1)
    {
        remote = BTSRC_List_RemoveHead(&g_bteg_le_env.app_attroot->remote_list);
        if (remote == NULL)
        {
            break;
        }
        while (1)
        {
            service = BTSRC_List_RemoveHead(&remote->service_list);
            if (service == NULL)
            {
                break;
            }
            GATT_Free_Service(service);
        }
        LFREE(remote);
    }
}

#endif //end of #ifdef BT_SUPPORT_BLE
