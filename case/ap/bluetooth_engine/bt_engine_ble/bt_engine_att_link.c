/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_test.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

/*
 BLE连接成功后更新连接参数，一般连接成功后5秒再设置会比较好
 */
void le_update_default_connection_param(void)
{
    struct L2CAP_ConnectionParameterUpdateStru *par;

    if ((g_bteg_le_env.rmt_client.serv_conn & BLE_CONNECTED) != BLE_CONNECTED)
    {
        return;
    }

    if (g_bteg_le_env.app_attroot == NULL)
    {
        return;
    }

    par = NEW(sizeof(struct L2CAP_ConnectionParameterUpdateStru));
    libc_memcpy(&par->addr,&g_bteg_le_env.app_attroot->tlin.addr, sizeof(struct HCI_AddressStru));
    //old is 60 ~ 80
    par->interval_min = 24;        //ce interval, unit:30ms
    par->interval_max = 36;        //ce interval,unit:45ms
    par->slave_latency = 0;        //从机latency，单位为connect event
    par->timeout_multiplier = 200; //超时时间,单位2s

    BLE_DBG_PRINTF("le conn param updata,interval %d~%d,latency%d,timeout %d\n", par->interval_min, par->interval_max,
            par->slave_latency, par->timeout_multiplier);

    L2CAP_Connection_Parameter_Update_Req(par);
}

void le_update_sco_connection_param(void)
{
    struct L2CAP_ConnectionParameterUpdateStru *par;

    if ((g_bteg_le_env.rmt_client.serv_conn & BLE_CONNECTED) != BLE_CONNECTED)
    {
        return;
    }

    if (g_bteg_le_env.app_attroot == NULL)
    {
        return;
    }

    par = NEW(sizeof(struct L2CAP_ConnectionParameterUpdateStru));
    libc_memcpy(&par->addr,&g_bteg_le_env.app_attroot->tlin.addr, sizeof(struct HCI_AddressStru));
    //old is 60 ~ 80
    par->interval_min = 48;        //ce interval, unit:30ms
    par->interval_max = 72;        //ce interval,unit:45ms
    par->slave_latency = 0;        //从机latency，单位为connect event
    par->timeout_multiplier = 200; //超时时间,单位2s

    BLE_DBG_PRINTF("le conn param updata,interval %d~%d,latency%d,timeout %d\n", par->interval_min, par->interval_max,
            par->slave_latency, par->timeout_multiplier);

    L2CAP_Connection_Parameter_Update_Req(par);
}

/**
 * 更新le连接参数
 * 输入参数：uint16 min 最小连接间隔参数
 *          uint16 max 最大连接间隔参数
 *          uint16 latency 连接潜伏数
 *          uint16 timeout 连接超时参数
 * 返回值:无
 * */
void le_update_rmt_client_conn_par(uint16 min, uint16 max, uint16 latency, uint16 timeout)
{
    struct L2CAP_ConnectionParameterUpdateStru *par;

    par = NEW(sizeof(struct L2CAP_ConnectionParameterUpdateStru));
    libc_memcpy(&par->addr, &g_bteg_le_env.rmt_client.p_remote_node->addr, sizeof(struct HCI_AddressStru));
    //old is 60 ~ 80
    par->interval_min = min;        //ce interval, unit:1.25ms
    par->interval_max = max;        //ce interval,unit:1.25ms
    par->slave_latency = latency;        //从机latency，单位为connect event
    par->timeout_multiplier = timeout; //超时时间,单位10ms

    BLE_DBG_PRINTF("le conn param updata,interval %d~%d,latency%d,timeout %d\n", par->interval_min, par->interval_max,
            par->slave_latency, par->timeout_multiplier);

    L2CAP_Connection_Parameter_Update_Req(par);
}

void rmt_client_init(struct APP_PeerNodeStru * remote)
{
    //init rmt client env
    libc_memset(&g_bteg_le_env.rmt_client, 0, sizeof(le_remote_client_st));
    g_bteg_le_env.rmt_client.p_remote_node = remote;
    g_bteg_le_env.rmt_client.mtu = DEFAULT_BLE_MTU;
    g_bteg_le_env.rmt_client.serv_conn |= BLE_CONNECTED;
}

void rmt_client_deinit(void)
{
    //init rmt client env
    g_bteg_le_env.rmt_client.p_remote_node = NULL;
    g_bteg_le_env.rmt_client.mtu = DEFAULT_BLE_MTU;
    g_bteg_le_env.rmt_client.serv_conn &= BLE_DISCONNECTED;
}

//link连接回调函数
void APP_GATTC_Link_Connected_Hook(struct GAP_ConnectionEventStru *in)
{
    struct APP_PeerNodeStru *remote;

    if (in->connected == FALSE)
    {
        return;
    }

    BLE_PRINTF("LE CONN\n");

    //get remote peernode
    remote = APP_PeerNode_FindByADDR(&in->addr);
    if (remote == NULL)
    {
        BLE_PRINTF("node create\n");
        remote = APP_PeerNode_Create(&in->addr);
        if (remote == NULL)
        {
            BLE_PRINTF("le create peernode failed\n");
            return;
        }
    }

    //set cur tlin
    APP_PeerNode_SelectCUR(remote);
    rmt_client_init(remote);
#if (BT_SUPPORT_BLE_CLIENT == 1)
    le_client_connect(remote);
#endif

    BLE_PRINTF("LE CONN OK\n");

#if (BT_ENGINE_BLE_TEST_ENABLE == 1)
    le_start_test();
#endif
}

//link断开回调函数
void APP_GATTC_Link_Loss_Hook(struct GAP_ConnectionEventStru *in)
{
    struct APP_PeerNodeStru *remote;

    if (in->connected == TRUE)
    {
        return;
    }

    BLE_PRINTF("LE DISCO\n");

    GATT_TL_Disconnect(&in->addr, NULL, NULL);

    remote = g_bteg_le_env.rmt_client.p_remote_node;
    if ((remote != NULL) && (SMP_CompareAddr(&in->addr, &remote->addr) != 0))
    {
        remote->mask &= ~APP_PEERMASK_LINK;       //断开
        remote->mask &= ~APP_PEERMASK_DISCOVRING; //断开
#if (BT_SUPPORT_BLE_CLIENT == 1)
                le_client_disconnect(remote);
#endif
        BTSRC_List_RemoveAt(&g_bteg_le_env.app_attroot->remote_list, remote);
        LFREE(remote);

        rmt_client_deinit();
        LE_Set_Advertise_Enable((UINT8)HCI_ADVERTISING_ENABLE_ENABLED);
    }
    else
    {
        BLE_PRINTF("LE DISCO,rmt %p,addr cmp ret %d\n", remote, SMP_CompareAddr(&in->addr, &remote->addr));
    }

#if (BT_ENGINE_BLE_TEST_ENABLE == 1)
    le_stop_test();
#endif
}

#endif
