/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：btstack tws等。
 * 作者：kehaitao
 ********************************************************************************/
#include "./../bluetooth_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

void bt_Qos_setup(uint32 token_rate, uint32 latency)
{
    uint8 Qos_setup[24];
    uint16 acl_handle = 0;
    uint8 i;

    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if ((btengine_global_variable.linked_dev_list[i]->p_dev_status != NULL)
                && (btengine_global_variable.linked_dev_list[i]->dev_type == NO_TWS_DEV))
        {
            acl_handle = btengine_global_variable.linked_dev_list[i]->p_dev_status->acl_handle;
            break;
        }
    }

    if (acl_handle != 0)
    {
        Qos_setup[0] = 0x01;
        Qos_setup[1] = 0x07;
        Qos_setup[2] = 0x08;
        Qos_setup[3] = 0x14;
        Qos_setup[4] = (uint8) (acl_handle & 0xff);
        Qos_setup[5] = (uint8) (acl_handle >> 8);
        Qos_setup[6] = 0;
        Qos_setup[7] = 1;
        Qos_setup[8] = (uint8) (token_rate & 0xff);
        Qos_setup[9] = (token_rate >> 8) & 0xff;
        Qos_setup[10] = (token_rate >> 16) & 0xff;
        Qos_setup[11] = (token_rate >> 24);
        Qos_setup[12] = 0x00;
        Qos_setup[13] = 0x00;
        Qos_setup[14] = 0x00;
        Qos_setup[15] = 0x00;
        Qos_setup[16] = (uint8) (latency & 0xff);
        Qos_setup[17] = (latency >> 8) & 0xff;
        Qos_setup[18] = (latency >> 16) & 0xff;
        Qos_setup[19] = (latency >> 24);
        Qos_setup[20] = 0x00;
        Qos_setup[21] = 0x00;
        Qos_setup[22] = 0x00;
        Qos_setup[23] = 0x00;
        TWS_DBG_PRINTF("set_qos_cmd\n");
        Send(0, Qos_setup, 24);
    }
}

bt_dev_t *TWS_FindA2dpSrcDev(void)
{
    bt_dev_t *dev = NULL;
    if (devs_cfg.tws_paired_list.p_dev_status != NULL)
    {
        if (devs_cfg.tws_paired_list.p_dev_status->a2dp_source.is_source == 1)
        {
            dev = &devs_cfg.tws_paired_list;
        }
    }
    return dev;
}

void TWS_A2dpStreamStart(uint8 dev_type)
{
    bt_dev_t *dev;
    if ((tws_status_info.is_src_flag == 1) && (dev_type == NO_TWS_DEV))
    {
        dev = TWS_FindA2dpSrcDev();
        if ((dev != NULL) && (dev->p_dev_status != NULL))
        {
            TWS_PRINTF("stream start\n");
            BTSRC_A2DP_StreamStart(dev->p_dev_status->a2dp_source.stream_handle);
        }
    }
}
void TWS_A2dpStreamSuspend(uint8 dev_type)
{
    bt_dev_t *dev;
    if ((tws_status_info.is_src_flag == 1) && (dev_type == NO_TWS_DEV))
    {
        dev = TWS_FindA2dpSrcDev();
        if ((dev != NULL) && (dev->p_dev_status != NULL))
        {
            TWS_PRINTF("stream suspend\n");
            BTSRC_A2DP_StreamSuspend(dev->p_dev_status->a2dp_source.stream_handle);
        }
    }
}
void TWS_SetA2DPSource(bt_dev_t *dev, uint8 mask)
{
    bt_dev_t *dev_linked;
    if ((mask & A2DP_STREAM_MASK_SRC) == A2DP_STREAM_MASK_SRC)
    {
        if (devs_cfg.tws_paired_list.is_source == 1)
        {
            dev->p_dev_status->a2dp_source.is_source = 1;
            TWS_PRINTF("is source\n");
        }
        else
        {
            TWS_PRINTF("SNK change to SOURCE\n");
            devs_cfg.tws_paired_list.is_source = 1;
            dev->p_dev_status->a2dp_source.is_source = 1;
            bt_set_cur_role_with_retry(dev, ROLE_MASTER, ROLE_SWITCH_RETRY_INVL, ROLE_SWITCH_RETRY_TIMES);
            //TWS_DisconnectAclLink();
        }

    }
    else
    {
        if (devs_cfg.tws_paired_list.is_source == 0)
        {
            dev->p_dev_status->a2dp_source.is_source = 0;
            TWS_PRINTF("is snk\n");
        }
        else
        {
            TWS_PRINTF("SOURCE change to SNK,need judge\n");
            dev_linked = RmtFindDevInLinkedList();
            if (dev_linked == NULL)
            {
                devs_cfg.tws_paired_list.is_source = 0;
                dev->p_dev_status->a2dp_source.is_source = 0;
                bt_set_cur_role_with_retry(dev, ROLE_SLAVER, ROLE_SWITCH_RETRY_INVL, ROLE_SWITCH_RETRY_TIMES);
                TWS_PRINTF("change to SNK\n");

            }
            else
            {

                TWS_PRINTF("is linked! cant change to SNK\n");
                TWS_DisconnectAclLink();
            }

        }
    }
}
void TWS_MsgListInit(void)
{
    libc_memset(&send_list, 0, sizeof(struct_send_list));
    send_list.avdtp_cache.raw_data = g_tws_avdtp_cache;
    send_list.avdtp_cache.capacity = A2DP_M2S_AVDTP_CACHE_LEN;
}
uint8 TWS_DisconnectDeal(bt_dev_t *dev, struct GAP_ConnectionEventStru *in)
{
    if (dev == NULL)
    {
        return 0;
    }
    senddata_cnt = 0;

    tws_tick_count = 0;

    tws_status_info.acl_handle = 0;

    //TWS_PRINTF("tws disconnect\n");
    tws_connect_flag = FALSE;

    g_p_info->dev_role = NORMAL_DEV;
    change_tws_role(NORMAL_DEV);
    TWS_SetPairedStatus(TWS_IDLE);
    switch (in->reason)
    {
        case HCI_STATUS_PAGE_TIMEOUT:
        case HCI_STATUS_CONNECTION_TIMEOUT:
        case HCI_STATUS_COMMAND_DISALLOWED:
        //iphone通话过程中reset或掉电重启时会出现该错误，所以将该错误加入回连条件
        case HCI_STATUS_ACL_CONNECTION_ALREADY_EXISTS:

        tws_status_info.need_repaired = TRUE;
        break;
        case HCI_STATUS_CONNECTION_REJECTED_LIMITED_RESOURCES:
        if (repaired_timer != -1)
        {
            kill_app_timer(APP_TIMER_GROUP_BTENGINE, repaired_timer);
            repaired_timer = -1;
        }
        repaired_timer = set_single_shot_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL,
                8000, (timer_proc )TWS_RepairedTimer);

        break;
        case HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST:
        if (snk_to_source_judge_flag == TRUE)
        {
            tws_status_info.need_repaired = TRUE;

        }
        break;

        default:
        tws_status_info.need_repaired = FALSE;
        if (repaired_timer != -1)
        {
            kill_app_timer(APP_TIMER_GROUP_BTENGINE, repaired_timer);
            repaired_timer = -1;
        }
        break;
    }
    if (dev->is_source == 0)
    {
        bt_SetFasterScanMode();
        return 0;
    }

    if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_WAIT_PAIRED)
            || (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_NONE))
    {
        bt_SetFasterScanMode();
        bt_set_visual_mode(GAP_VM_DEFAULT);
    }
    else
    {
        //与手机已经处理连接，则可用于组对，但不可见
        bt_set_visual_mode(GAP_VM_NONDISCOV);
    }

    tws_status_info.is_src_flag = FALSE;
    if (dev->p_dev_status != NULL)
    {
        dev->p_dev_status->acl_handle = 0;
        dev->p_dev_status->sniff_flag = 0;

        FREE(dev->p_dev_status);
        dev->p_dev_status = NULL;
    }

    return 1;
}
uint8 tws_deal_tws_quit(void)
{
    if ((btengine_global_variable.recv_quit_flag & FORCE_QUIT_APP_FLAG) == FORCE_QUIT_APP_FLAG)
    {
        if ((TWS_GetPairedStatus() == TWS_IDLE) || (tws_quit_time > TWS_QUIT_TIMEOUT))
        {
            tws_quit_time = 0;
            return TRUE;

        }
        else
        {
            tws_quit_time++;
        }
    }
    return FALSE;
}
tGAP_IndCbk_Result tws_connectionreply_deal(uint8 *bdAddress)
{
    tGAP_IndCbk_Result ret = GAP_IND_RESULT_NOT_CARE;
    bt_dev_t *dev_linked;
    bt_dev_t * dev;

    dev_linked = RmtFindDevInLinkedList();
    dev = RmtFindDevByBD(bdAddress);

    //已连接手机
    if ((dev_linked != NULL) && (g_btengine_cfg_data.max_link_num <= 2))
    {
        //连接手机后，只支持被曾组队的tws设备回连
        if ((libc_memcmp(&g_btengine_var.device_addr.bytes[3],bdAddress+3,3) != 0)
                || ((dev != NULL) && (dev->dev_type != TWS_DEV)))
        {
            TWS_PRINTF_WARNNING("phone connect, not tws,reject\n");
            return GAP_IND_RESULT_REJECT;
        }
    }

    //不支持tws
    if (bt_manager_cfg_info.sp_tws_link == 0)
    {
        //拒绝tws设备的连接请求
        if (libc_memcmp(&g_btengine_var.device_addr.bytes[3],bdAddress+3,3) == 0)
        {
            TWS_PRINTF_WARNNING("single box mode,reject twsdev\n");
            return GAP_IND_RESULT_REJECT;
        }

    }

    //正在回连tws中或者正在回连手机中，不接受连接请求
    if ((tws_connect_flag == TRUE) || (phone_connect_flag == TRUE))
    {
        TWS_PRINTF_WARNNING("linking,reject reply\n");
        if ((dev != NULL) && (dev->dev_type == TWS_DEV))
        {
            tws_status_info.need_repaired = TRUE;
            tws_paired_count = 0;
        }
        return GAP_IND_RESULT_REJECT;

    }

    //tws配对中不支持连接请求
    if (TWS_GetPairedStatus() == TWS_PAIREING)
    {
        TWS_PRINTF_WARNNING("tws is pairing,reject con reply\n");
        ret = GAP_IND_RESULT_REJECT;

    }

    //tws副箱已组队，拒绝任何连接请求
    if ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 0))
    {
        TWS_PRINTF_WARNNING("tws slave is paired,reject any con\n");
        ret = GAP_IND_RESULT_REJECT;

    }

    //曾组队TWS设备
    if ((dev != NULL) && (dev->dev_type == TWS_DEV))
    {
        //不支持，拒绝
        if (bt_manager_cfg_info.sp_tws_link == 0)
        {
            return GAP_IND_RESULT_REJECT;
        }

        //通话、SIRI中不接受副箱回连请求
        switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
        {
            case STATUS_CALL_INCOMING:
            case STATUS_CALL_OUTGOING:
            case STATUS_CALL_HFP:
            case STATUS_CALL_PHONE:
            case STATUS_SIRI:
            TWS_PRINTF_WARNNING("is calling\n")
            ;
            tws_status_info.need_repaired = TRUE;
            tws_paired_count = 0;
            return GAP_IND_RESULT_REJECT;
            break;
        }

        //a2dp未断开，拒绝回连
        if (tws_status_info.a2dp_stream_status.status != A2DP_STREAM_DONE)
        {
            TWS_PRINTF_WARNNING("a2dpstream is creating\n");
            tws_status_info.need_repaired = TRUE;
            return GAP_IND_RESULT_REJECT;

        }

        //tws已组队，拒绝回连
        if (TWS_GetPairedStatus() == TWS_PAIRED)
        {
            TWS_PRINTF_WARNNING("tws is paired\n");
            ret = GAP_IND_RESULT_REJECT;
        }
        else
        {
            //已连接手机
            if (dev_linked != NULL)
            {
                //当前箱体为主箱并且不需要进行角色转换时，才连接。
                if ((devs_cfg.tws_paired_list.is_source == 1) && (snk_to_source_judge_flag == FALSE))
                {
                    TWS_SetPairedStatus(TWS_PAIREING);
                    //连接后需要切换主从角色
                    bt_set_cur_role(dev, ROLE_MASTER);
                }
                else
                {
                    tws_status_info.need_repaired = TRUE;
                    tws_paired_count = 0;
                    TWS_PRINTF_WARNNING("is linked! reject connect\n");
                    ret = GAP_IND_RESULT_REJECT;
                }
            }
            else
            {
                TWS_SetPairedStatus(TWS_PAIREING);
                if (devs_cfg.tws_paired_list.is_source == 1)
                {
                    //如果当前时主箱，那么需要role_switch
                    bt_set_cur_role(dev, ROLE_MASTER);
                }
            }
        }
    }
    return ret;
}
void tws_connect_deal(bt_dev_t *dev)
{
    //这是一台手机
    if (dev->dev_type != TWS_DEV)
    {
        TWS_DBG_PRINTF("READY FOR TWS2\n");
        //手机还未连接
        phone_connect_flag = FALSE;
        //正在开始连接手机
        already_start_connect_phone_flag = TRUE;
        //这是一台已组队的副箱
        if ((tws_status_info.have_paired_dev == TRUE) && (TWS_GetPairedStatus() == TWS_IDLE))
        {
            if (devs_cfg.tws_paired_list.is_source == 0)
            {
                //副箱连上手机后，需要转换角色为主箱
                snk_to_source_judge_flag = TRUE;
                devs_cfg.tws_paired_list.is_source = 1;
                tws_paired_count = 0;
            }
        }
        //手机不需要回连了
        BT_SetReconnectFlag(dev->dev_type, FALSE);

    }
    else
    {
        senddata_cnt = 0;
        tws_tick_count = 0;
        tws_status_info.need_repaired = FALSE;
        TWS_SetPairedStatus(TWS_PAIREING);
        if (devs_cfg.tws_paired_list.is_source == 0)
        {
            //副箱被主箱连接了，就不能回连手机了
            g_p_info->isneed_reconnect_flag = FALSE;

        }
        else
        {
            //主箱被回连，那么不需要转换角色
            if (snk_to_source_judge_flag == TRUE)
            {
                snk_to_source_judge_flag = FALSE;
            }
        }

    }
    TWS_PRINTF("dev role %d\n", dev->is_source, 2);
}

#endif
