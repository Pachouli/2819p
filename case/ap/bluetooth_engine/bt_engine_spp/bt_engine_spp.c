/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib_init.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "bluetooth_engine.h"
#include "bt_engine_spp_app_msg_deal.h"
#include "bt_engine_spp.h"
#include "bt_engine_passthrough.h"

struct ListStru s_server_hdl_list;
struct ListStru s_connection_hdl_list;

spp_trans_ctrl_st spp_trans_ctrl;

void SppStart(void)
{
    APP_Spp_Start();
    spp_trans_ctrl.send_len = 0;
    SPP_PRINTF("SppStart 0x%p\n", APP_SPP_START_ENTRY);
}

void SppStop(void)
{
    App_Spp_Stop();

    BTSRC_List_RemoveAll(&s_server_hdl_list);
    BTSRC_List_RemoveAll(&s_connection_hdl_list);
    SPP_PRINTF("SppStop\n");
}

/*可以选择第几个对象，默认为1表示第一个对象*/
void *ItemSelect(struct ListStru *list, UINT32 idx)
{
    UINT32 *item;

    if (list->head == NULL)
    {
        //printf("List empty!\n");
        return NULL;
    }

    item = list->head;
    while ((item != NULL) && (idx > 1))
    {
        item = LNEXT(item);
        idx--;
    }

    return item;
}

#define SPP_SVC_NAME_STRING "Serial Port Service - Channel"

void SppAppSvrStartCfm(UINT32 server_handle, struct SppStartServerCfmStru *param)
{
    UINT8 svc_name[64];
    UINT32 *item = BTSRC_List_NodeNew(sizeof(UINT32));
    uint8 svc_name_strlen = libc_strlen(SPP_SVC_NAME_STRING);

    *item = server_handle;
    BTSRC_List_AddTail(&s_server_hdl_list, item);
    libc_memcpy(svc_name, SPP_SVC_NAME_STRING, svc_name_strlen);
    if (param->server_channel >= 100)
    {
        svc_name[svc_name_strlen] = param->server_channel / 100 + 30;
        svc_name[svc_name_strlen + 1] = param->server_channel % 100 / 10 + 30;
        svc_name[svc_name_strlen + 2] = param->server_channel % 10 + 30;
        svc_name[svc_name_strlen + 3] = 0;
    }
    else if (param->server_channel >= 10)
    {
        svc_name[svc_name_strlen] = param->server_channel / 10 + 30;
        svc_name[svc_name_strlen + 1] = param->server_channel % 10 + 30;
        svc_name[svc_name_strlen + 2] = 0;
    }
    else
    {
        svc_name[svc_name_strlen] = param->server_channel + 30;
        svc_name[svc_name_strlen + 1] = 0;
    }

    SDAP_ModifyStringAttrib(param->service_record_handle, svc_name, 0x100);
    SPP_PRINTF("New SPP Server: handle = %d, channel = %d\n>", server_handle, param->server_channel);
    passth_register_server(RCP_WITH_SPP, SPP_MAX_MTU, SppAppWriteData);
    //SPP_PRINTF("New SPP Server,server handle 0x%p\n",server_handle);
    //SPP_PRINTF("channel = %d\n",param->server_channel);
}

void SppAppConnectComplete(UINT32 inst_handle, struct SppConnectionInforStru *param)
{
    UINT32 *item;

    if (param->result == APP_SPP_ER_SUCCESS)
    {
        item = BTSRC_List_NodeNew(sizeof(UINT32));
        *item = param->connection_handle;
        BTSRC_List_AddTail(&s_connection_hdl_list, item);
        //g_p_info->service_connect |= SPP_CONNECTED;//by clz 2013-12-6 9:45
        if (param->role == APP_SPP_ROLE_CLIENT)
        {
            //            SPP_PRINTF("succeed to create SPP connect with \n");
            //            DEBUG_PRINT_DATA(param->bd, 6, 1);
            SPP_PRINTF("Succeed to create SPP Connection with %02x:%02x:%02x:%02x:%02x:%02x!\n>", param->bd[5],
                    param->bd[4], param->bd[3], param->bd[2], param->bd[1], param->bd[0]);
        }
        else
        {
            //            DEBUG_PRINT_STRING("Local SPP Server\n");
            //            DEBUG_PRINT_DATA(param->bd, 6, 1);
            //            DEBUG_PRINT_STRING("inst_handle\n");
            //            DEBUG_PRINT_DATA(&inst_handle, 4, 1);
            SPP_PRINTF("Local SPP Server %d is connected by %02x:%02x:%02x:%02x:%02x:%02x!\n>", inst_handle,
                    param->bd[5], param->bd[4], param->bd[3], param->bd[2], param->bd[1], param->bd[0]);
        }

        //标志spp profile已经连接
        bt_dev_t *dev = RmtFindDevByBD(param->bd);
        dev->p_dev_status->service_connect |= SPP_CONNECTED;
        dev->profile |= SPP_CONNECTED;
    }
    else
    {
        SPP_PRINTF("Fail to create SPP connection!\n");
        SPP_PRINTF("result %d\n", param->result);
    }
}

void SppAppDisconnectComplete(struct SppConnectionInforStru *param)
{
    UINT32 *item;

    item = s_connection_hdl_list.head;
    while (item)
    {
        if (*item == param->connection_handle)
        {
            BTSRC_List_RemoveAt(&s_connection_hdl_list, item);
            LFREE(item);
            //g_p_info->service_connect &= SPP_DISCONNECTED;//by clz 2013-12-6 9:46
            break;
        }
        item = LNEXT(item);
    }
    SPP_PRINTF("Connection with %02x:%02x:%02x:%02x:%02x:%02x is disconnected!\n>", param->bd[5], param->bd[4],
            param->bd[3], param->bd[2], param->bd[1], param->bd[0]);
    passth_unregister_server(RCP_WITH_SPP);

    //标志spp profile已断开连接
    bt_dev_t *dev = RmtFindDevByBD(param->bd);
    dev->p_dev_status->service_connect &= SPP_DISCONNECTED;
    dev->profile &= SPP_DISCONNECTED;
}

void SppAppDataCfm(UINT32 inst_handle, struct SppDataCfmStru *param)
{
    SPP_PRINTF("TX Confirmation, require %d bytes; send %d bytes\n", param->data_size, param->sent_size);
    spp_trans_ctrl.send_len -= param->sent_size;
    FREE(param->data);
}

void SppAppCFGCfm(UINT32 inst_handle, struct Spp_RpnParStru *param)
{
    SPP_PRINTF("SPP CFG:\n");
    SPP_PRINTF("baudrate->%d\n", param->baudrate);
    SPP_PRINTF("numbits->%d\n", param->numbits);
    SPP_PRINTF("stopbits->%d\n", param->stopbits);
    SPP_PRINTF("parity->%d\n", param->parity);
    SPP_PRINTF("ptype->%d\n", param->ptype);
    SPP_PRINTF("fctrl->%d\n", param->fctrl);
    SPP_PRINTF("xon->%d\n", param->xon);
    SPP_PRINTF("xoff->%d\n", param->xoff);
    SPP_PRINTF("pm1->%d\n", param->pm1);
    SPP_PRINTF("pm2->%d\n", param->pm2);
    //DEBUG_PRINT_DATA(param, 10, 1);
    FREE(param);
}

void SppAppDataInd(UINT32 inst_handle, struct SppDataIndStru *param)
{
    SPP_PRINTF("SppAppDataInd:%d\n", param->data_size);
    //libc_printhex(param->data, param->data_size, 0);

    spp_rcv_rmt_data(param->data, param->data_size);

    //bt_engine_passth_receive(RCP_WITH_SPP,param->data,param->data_size);

    //passth_pkg_t tmp_rcp;
    // tmp_rcp.rcp_data = param->data;
    // tmp_rcp.rcp_len = param->data_size;

    // if (spp_receive_send_to_buf(&tmp_rcp) == FALSE)
    // {
    //     SPP_PRINTF_WARNNING("spp send to app fail\n");
    // }
    // spp_notify_app_receive();
}

void SppAppCbk(UINT32 inst_handle, tSpp_Event event, void *param)
{
    switch (event)
    {
        case APP_SPP_EV_SERVER_START_CFM:
        SppAppSvrStartCfm(inst_handle, param);
        SPP_PRINTF("APP_SPP_EV_SERVER_START_CFM\n")
        ;
        break;
        case APP_SPP_EV_CONNECT_COMPLETE:
        SppAppConnectComplete(inst_handle, param);
        SPP_PRINTF("APP_SPP_EV_CONNECT_COMPLETE\n")
        ;
        break;
        case APP_SPP_EV_DATA_CFM:
        SppAppDataCfm(inst_handle, param);
        SPP_PRINTF("APP_SPP_EV_DATA_CFM\n")
        ;
        break;
        case APP_SPP_EV_DATA_IND:
        SppAppDataInd(inst_handle, param);
        SPP_PRINTF("APP_SPP_EV_DATA_IND\n")
        ;
        break;
        case APP_SPP_EV_DISCONNECT_COMPLETE:
        SppAppDisconnectComplete(param);
        SPP_PRINTF("APP_SPP_EV_DISCONNECT_COMPLETE\n")
        ;
        break;
        case APP_SPP_EV_RPN_CFM:
        SppAppCFGCfm(inst_handle, param);
        break;
        default:
        break;
    }
}

void SppAppStopServer(void)
{
    UINT32 *server_handle = ItemSelect(&s_server_hdl_list, 1);

    if (server_handle != NULL)
    {
        App_Spp_StopServerReq(*server_handle);
        BTSRC_List_RemoveAt(&s_server_hdl_list, server_handle);
        LFREE(server_handle);
    }
}

bt_dev_t *get_last_phone_dev(void)
{
    uint8 idx, cur_idx;
    uint8 temp_mru;

    bt_dev_t *p_dev = NULL;

    if (devs_cfg.pl_len != 0)
    {
        idx = cur_idx = 0;
        temp_mru = devs_cfg.paired_list[0].mru_num;
        for (idx = 1; idx < (devs_cfg.pl_len); idx++)
        {
            if (temp_mru >= devs_cfg.paired_list[idx].mru_num)
            {
                temp_mru = devs_cfg.paired_list[idx].mru_num;
                cur_idx = idx;
            }
        }

        p_dev = &devs_cfg.paired_list[cur_idx];
    }
    return p_dev;
}

void printf_bd_addr_spp(uint8 *bd_addr)
{
    uint8 tmp_addr[15], t, i;

    for (i = 0; i < 6; i++)
    {
        t = bd_addr[5 - i] / 16;
        t = (t >= 10) ? ('A' + t - 10) : ('0' + t);
        tmp_addr[i * 2] = t;
        t = bd_addr[5 - i] % 16;
        t = (t >= 10) ? ('A' + t - 10) : ('0' + t);
        tmp_addr[i * 2 + 1] = t;
    }

    tmp_addr[i * 2] = '\r';
    tmp_addr[i * 2 + 1] = '\n';
    tmp_addr[i * 2 + 2] = 0x00;

    libc_printf("<recon dev bt addr>%s\n", tmp_addr);
}

/* Remote Device Instance Structure */
//struct AppRmtDevStru {
//  struct HCI_AddressStru addr;
//  UINT8 link_key[16];
//  UINT8 key_type;
//  UINT8 is_bond;
//  UINT8 rmt_name[255];
//  UINT32 dev_cls;
//};
//struct AppRmtDevStru *RmtDevSelect();
void SppAppCreateConnection(void)
{
    //struct AppRmtDevStru *rmt_dev = RmtDevSelect();

    if (btengine_global_variable.g_cur_rmt_dev == NULL)
    {
        bt_dev_t *p_dev;
        p_dev = get_last_phone_dev();
        if (p_dev != NULL)
        {
            //printf_bd_addr_spp(p_dev->dev_info.bd_addr.bytes);
            App_Spp_ConnectReq(SppAppCbk, p_dev->dev_info.bd_addr.bytes, 7, 4);
        }
        else
        {
            SPP_PRINTF("don't found last dev\n");
        }
    }
    else
    {
        App_Spp_ConnectReq(SppAppCbk, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes, 7, 4);
    }
}

void SppAppDisconnect(void)
{
    UINT32 *connection_handle = ItemSelect(&s_connection_hdl_list, 1);

    if (connection_handle != NULL)
    {
        App_Spp_DisconnectReq(*connection_handle);
    }
}

void SppApp_WaitCredits(UINT32 conn_handle)
{
    //uint32 time_a,time_b;
    //get cur time stemp
    //time_a = sys_get_ab_timer();
    uint32 i = 0;

    while (App_Spp_Get_Credits(conn_handle) == 0)
    {
        i++;

        BtDev_LibExc();

        //        //if(i>=)
        //        time_b = sys_get_ab_timer()-time_a;
        //        if((time_b%10)==0)
        //        {
        //            sys_os_time_dly(1);
        //        }
        //        else if(time_b>=2000)
        //        {
        //            break;
        //        }
        //        else
        //        {
        //            ;//do nothing
        //        }
        if ((i % 1000) == 0)
        {
            sys_os_time_dly(1);
        }
        else if (i >= 200000) //about 2 second

        {
            break;
        }
        else
        {
            ; //do nothing for qac
        }
    }
}

UINT8 SppAppWriteData(UINT8 *src, UINT16 length)
{
    uint8 *data = NULL;
    UINT32 *connection_handle = ItemSelect(&s_connection_hdl_list, 1);
    SPP_PRINTF("SppAppWriteData\n");
    if (connection_handle == NULL)
    {
        SPP_PRINTF("spp connection handle is null\n");
        return FALSE;
    }
    if (spp_trans_ctrl.send_len >= SPP_MAX_WAITTING_SEND)
    {
        SPP_PRINTF("spp send too fast\n");
        return FALSE;
    }

    //UINT8 *data = NEW(sizeof("AT+CSCS=ACTIONS\n"));
    //strcpy(data, "AT+CSCS=ACTIONS\n");
    //DEBUG_PRINT_DATA(src, length, 2);
    data = NEW(length);
    SppApp_WaitCredits(*connection_handle);
    libc_memcpy(data, src, length);
    spp_trans_ctrl.send_len += length;
    App_Spp_DataReq(SppAppCbk, *connection_handle, data, length);
    return TRUE;
}

void SppAppGetCFG(void)
{
    UINT32 *connection_handle = ItemSelect(&s_connection_hdl_list, 1);

    if (connection_handle != NULL)
    {
        UINT8 *data = NEW(sizeof(struct Spp_RpnParStru));
        libc_memset(data, 5, sizeof(struct Spp_RpnParStru));
        App_Spp_GetCFG(SppAppCbk, (uint32 )*connection_handle, (struct Spp_RpnParStru * )data);
    }
}
