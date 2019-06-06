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
 * \file     bt_engine_avrcp.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

//清除g_p_avrcp的数据
static void APP_AVRCP_data_clear(void)
{
    g_p_avrcp->EventMask_CT = 0x00; //清除
    g_p_avrcp->EventMask_TG = 0x00;

    g_p_avrcp->avrcp_play_status = AVRCP_PLAYSTATUS_STOPPED;
    g_p_avrcp->remote_absoluteVolume = AVRCP_ABSOLUTE_VOLUME_MAX + 1; //保证每次连接都会重新同步音量
}

/*---------------------------------------------------------------------------*/
/*                              6.4 Capabilities PDUs                        */
/*---------------------------------------------------------------------------*/
void APP_AVRCP_GetCapabilities_Req(UINT8 *bd, UINT8 capability_id)
{
    UINT8 *id = NEW(sizeof(UINT8));

    *id = capability_id;
    AVRCP_AdvancedControl_Req(bd, AVRCP_PDUID_GET_CAPABILITIES, id, sizeof(UINT8));
    FREE(id);
}

void APP_AVRCP_GetCapabilities_Cfm(UINT8 *bd, struct AVRCP_GetCapabilitiesRspStru *param)
{
    UINT8 count = 0;

    switch (param->id)
    {
        case AVRCP_CAPABILITYID_COMPANY_ID:
        break;
        case AVRCP_CAPABILITYID_EVENTS_SUPPORTED: //获取远端设备支持的功能
        AVRCP_DBG_PRINTF("get rmt capability\n");
        while (count < param->count)
        {
            //如果支持播放状态Notif,注册之
            if (param->capability[count] == AVRCP_EVENT_PLAYBACK_STATUS_CHANGED)
            {
                g_p_avrcp->EventMask_CT |= 1 << AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;
                APP_AVRCP_RegisterNotification_Req(bd, AVRCP_EVENT_PLAYBACK_STATUS_CHANGED);
            }
            count++;
        }
        break;
        default:/* error */
        break;
    }
}

void APP_AVRCP_GetCapabilitiesRsp(UINT8 *bd, UINT32 event, UINT8 cap_id)
{
    /* bd, event, cap_id come from the callback input, volume is the actual volume value set */
    struct AVRCP_GetCapabilitiesRspStru *avrcp_data;
    UINT16 size;

    switch (cap_id)
    {
        case AVRCP_CAPABILITYID_COMPANY_ID:
        //libc_printf("\n<avrcp> get cap:company\n");
        size = 1 + 1 + 3; /* Count = 1, Capability = Count * 3 */
        avrcp_data = NEW(size);
        avrcp_data->count = 1;
        avrcp_data->capability[0] = 0x00;
        avrcp_data->capability[1] = 0x19;
        avrcp_data->capability[2] = 0x58; //SIG Company
        break;

        case AVRCP_CAPABILITYID_EVENTS_SUPPORTED:
        //libc_printf("\n<avrcp> get cap:event_supported\n");
        if (bt_manager_cfg_info.sp_avrcp_vol_syn != 0)
        {
            size = 1 + 1 + 2; /* Count = 2 */
            avrcp_data = NEW(size);
            avrcp_data->count = 2;
            //data->capability[0] = AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;
            //data->capability[1] = AVRCP_EVENT_TRACK_CHANGED;
            avrcp_data->capability[0] = AVRCP_EVENT_BATT_STATUS_CHANGED;
            avrcp_data->capability[1] = AVRCP_EVENT_VOLUME_CHANGED;
        }
        else
        {
            size = 1 + 1 + 0; /* Count = 0 */
            avrcp_data = NEW(size);
            avrcp_data->count = 0;
        }
        break;

        default:
        avrcp_data = NULL;
        break;
    }

    if (avrcp_data != NULL)
    {
        avrcp_data->id = cap_id;
        AVRCP_AdvancedControl_Rsp(bd, event, avrcp_data, size);
        FREE(avrcp_data);
    }
    else
    {
        AVRCP_GeneralReject_Rsp(bd, AVRCP_ERROR_INVALID_PARAMETER, event);
    }
}

/*-----------------------------------------------------------------------------
 Description:
 6.7.2 This primitive registers with the TG to receive notifications asynchronously based on
 specific events occurring. The initial response to this Notify command shall be an INTERIM
 response with current status, or a REJECTED/NOT IMPLEMENTED response. This has to take
 place within TMTP time from receiving the command. The following response shall be a
 CHANGED response with the updated status, or a REJECT response. This is as per 1394
 AV/C protocol specification. A registered notification gets changed on receiving CHANGED
 event notification. For a new notification additional NOTIFY command is expected to be sent.
 Only one EventID shall be used per notification registration.
 -----------------------------------------------------------------------------*/
void APP_AVRCP_RegisterNotification_Req(UINT8 *bd, UINT8 event_id)
{
    struct AVRCP_RegisterNotifReqStru *in;
    UINT16 len_in = sizeof(struct AVRCP_RegisterNotifReqStru);

    in = NEW(len_in);
    in->event_id = event_id;
    if (event_id == AVRCP_EVENT_PLAYBACK_POS_CHANGED)
    {
        in->playback_interval = 0x12345678;
    }
    else
    {
        in->playback_interval = 0;
    }

    AVRCP_AdvancedControl_Req(bd, AVRCP_PDUID_REGISTER_NOTIFICATION, in, len_in);
    FREE(in);
}

/*---------------------------------------------------------------------------*/
/*                              6.13 Volume Handling                                       */
/*---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 Description:
 6.13.2 This command is used to set an absolute volume to be used by the rendering device.
 This is in addition to the relative volume PASS THROUGH commands. It is expected that
 the audio sink will perform as the TG for this command.
 only bit 6-0 used, bit 7 should be reserved.
 -----------------------------------------------------------------------------*/
void APP_AVRCP_SetAbsoluteVolume_Req(UINT8 *volume)
{
    UINT8 *bd;
    struct AVRCP_RegisterNotifRspStru *avrcp_data;

    if (btengine_global_variable.g_cur_rmt_dev != NULL)
    {
        bd = btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes;
        avrcp_data = NEW(sizeof(struct AVRCP_RegisterNotifRspStru));
        avrcp_data->event_id = AVRCP_EVENT_VOLUME_CHANGED;
        avrcp_data->param[0] = *volume;
        AVRCP_RegisterNotif_Rsp(AVRCP_PDUID_REGISTER_NOTIFICATION, avrcp_data,
                sizeof(struct AVRCP_RegisterNotifRspStru));
        FREE(avrcp_data);
    }
}

/*---------------------------------------------------------------------------*/
/*                              6.7 Notification PDUs                                          */
/*---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 Description:
 6.7.1 This primitive is used by the CT to get the status of the currently playing media at
 the TG.
 -----------------------------------------------------------------------------*/
void APP_AVRCP_GetPlayStatus_Req(UINT8 *bd)
{
    AVRCP_AdvancedControl_Req(bd, AVRCP_PDUID_GET_PLAY_STATUS, NULL, 0);
}
//更新手机音量到音箱上，推歌时只有部分手机有音量同步的功能，例如iphone
inline void APP_AVRCP_Update_vol_to_ap(uint8 level)
{
    AVRCP_PRINTF("volume update %d\n", level, 2);
    sys_os_sched_lock();
    g_p_info->vol_syn_flag = 1;
    g_p_info->remote_volume = level;
    sys_os_sched_unlock();

}

//收到SetAbsoluteReq时需要回复
void APP_AVRCP_SetAbsoluteVolRsp(UINT8 *bd, UINT32 event, UINT8 volume)
{
    //同步音箱端设置
    if (bt_manager_cfg_info.sp_avrcp_vol_syn != 0)
    {
        if ((g_p_avrcp->avrcp_play_status != AVRCP_PLAYSTATUS_PLAYING)
                || (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_A2DP_PLAY))
        {
            AVRCP_PRINTF("ingnore avrcp %d\n", volume, 2);
            goto IGNORE_SET_VOL;
        }
        else
        {
            APP_AVRCP_Update_vol_to_ap(volume);
            /* bd, event come from the callback input, volume is the actual volume value set */
            g_p_avrcp->remote_absoluteVolume = volume;
            AVRCP_AdvancedControl_Rsp(bd, event, &volume, 1);
        }
    }
    else
    {
        IGNORE_SET_VOL:
        /* bd, event come from the callback input, volume is the actual volume value set */
        //although this update is ignored, we should change the remote volume
        g_p_avrcp->remote_absoluteVolume = volume;
        AVRCP_AdvancedControl_Rsp(bd, event, &volume, 1);
    }
}

//在收到Register,VolumeChanged时，需要先回复AVRCP_RSP_INTERIM
//之后在音量改变时再回复AVRCP_RSP_CHANGED,
void APP_AVRCP_VolumeChangedNTF(UINT8 *bd, UINT8 rsp_code, UINT8 cur_volume)
{
    /* bd comes from the callback input */
    UINT32 tlcr_pdu_id = AVRCP_PDUID_REGISTER_NOTIFICATION;
    struct AVRCP_RegisterNotifRspStru rsp;
    //BTEG_DBG_PRINTF("\n<avrcp> volume sync flag %d\n", g_p_avrcp->EventMask_TG);
    if (((((g_p_avrcp->EventMask_TG & (((uint32) 1) << AVRCP_EVENT_VOLUME_CHANGED)) != 0)
            && (g_p_avrcp->remote_absoluteVolume != cur_volume)) != 0) || (rsp_code == AVRCP_RSP_INTERIM))
    {
        AVRCP_PRINTF("vol changed %d\n", cur_volume);
        AVRCP_SET_RSPTYPE(tlcr_pdu_id, rsp_code);

        rsp.event_id = AVRCP_EVENT_VOLUME_CHANGED;
        rsp.param[0] = cur_volume;
        if (rsp_code != AVRCP_RSP_INTERIM)
        {
            g_p_avrcp->remote_absoluteVolume = cur_volume;
        }
        AVRCP_RegisterNotif_Rsp(tlcr_pdu_id, &rsp, 2);
    }
}

void APP_AVRCP_RegisterNotification_Cfm(UINT8 *bd, struct AVRCP_RegisterNotifRspStru *param, UINT8 rsp_type)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(bd);
    if (dev == NULL)
    {
        AVRCP_PRINTF_WARNNING("device is null\n");
        return;
    }

    switch (param->event_id)
    {
        /* FIXME, following is the param->param */
        case AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
        AVRCP_DBG_PRINTF("status change\n");
        g_p_avrcp->avrcp_play_status = param->param[0];
        if ((param->param[0] == AVRCP_PLAYSTATUS_STOPPED) || (param->param[0] == AVRCP_PLAYSTATUS_PAUSED))
        {
            if (param->param[0] == AVRCP_PLAYSTATUS_STOPPED)
            {
                AVRCP_PRINTF("stoped\n");
            }
            else
            {
                AVRCP_PRINTF("pause\n");
            }
            //只有之前是在播放状态下才更新成暂停
            if (get_cur_device_status(dev) == STATUS_A2DP_PLAY)
            {
                set_cur_device_status(dev, STATUS_A2DP_PAUSE);
            }
        }
        else if (param->param[0] == AVRCP_PLAYSTATUS_PLAYING)
        {
            AVRCP_PRINTF("play\n");
            //只有之前处于暂停，且sbc线程已经存在，才置位play
            //如果sbc线程不存在，在a2dp数据来时会有更新
            if (get_cur_device_status(dev) == STATUS_A2DP_PAUSE)
            {
                set_cur_device_status(dev, STATUS_A2DP_PLAY);
            }
        }
        else
        {
        }
        break;

        case AVRCP_EVENT_TRACK_CHANGED: /* 0x02 */
        break;
        case AVRCP_EVENT_TRACK_REACHED_END: /* 0x03 */
        break;
        case AVRCP_EVENT_TRACK_REACHED_START: /* 0x04 */
        break;
        case AVRCP_EVENT_PLAYBACK_POS_CHANGED: /* 0x05 */
        AVRCP_PRINTF("pos change\n")
        ;
        break;
        case AVRCP_EVENT_BATT_STATUS_CHANGED: /* 0x06 */
        AVRCP_PRINTF("batt status change\n")
        ;
        break;
        case AVRCP_EVENT_SYSTEM_STATUS_CHANGED: /* 0x07 */
        break;
        case AVRCP_EVENT_PLAYER_APPLICATION_SETTING_CHANGED: /* 0x08 */
        break;
        case AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED: /* 0x09 */
        AVRCP_PRINTF("content change\n")
        ;
        break;
        case AVRCP_EVENT_AVAILABLE_PLAYERS_CHANGED: /* 0x0A */
        break;
        case AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED: /* 0x0B */
        break;
        case AVRCP_EVENT_UIDS_CHANGED: /* 0x0C */
        /* The UID Counter of the currently browsed player as defined in 6.10.3 */
        btengine_global_variable.g_cur_rmt_dev->p_dev_status->avrcp_source.local_element_uidcounter =
                ((struct AVRCP_UIDsChangedNotifStru *) param)->uid_counter;
        break;
        case AVRCP_EVENT_VOLUME_CHANGED: /* 0x0D */
        AVRCP_PRINTF("vol change\n")
        ;
        break;
        default:/* unknown event */
        break;
    }
}

void APP_AVRCP_RegisterNotification_Ind(UINT8 *bd, struct AVRCP_RegisterNotifReqStru *param)
{
    uint16 tmp_volume;

    g_p_avrcp->EventMask_TG |= 1 << AVRCP_EVENT_VOLUME_CHANGED;

    switch (param->event_id)
    {
        case AVRCP_EVENT_VOLUME_CHANGED: /* 0x0D */
        if (bt_manager_cfg_info.sp_avrcp_vol_syn != 0)
        {
            tmp_volume = (uint16) g_p_info->remote_volume;
            APP_AVRCP_VolumeChangedNTF(bd, AVRCP_RSP_INTERIM, (uint8) tmp_volume);
        }
        else
        {
            AVRCP_GeneralReject_Rsp(bd, AVRCP_ERROR_INVALID_PARAMETER, AVRCP_PDUID_REGISTER_NOTIFICATION);
        }
        break;

        default:/* unknown event */
        AVRCP_PRINTF("unknown event\n")
        ;
        AVRCP_GeneralReject_Rsp(bd, AVRCP_ERROR_INVALID_PARAMETER, AVRCP_PDUID_REGISTER_NOTIFICATION);
        break;
    }
}

void APP_AVRCP_GetFolderItems_Cfm(UINT8 *bd, struct AVRCP_GetFolderItemsRspStru *param)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(bd);
    if ((dev == NULL) || (dev->p_dev_status == NULL))
    {
        AVRCP_PRINTF_WARNNING("\n<avrcp> device is null\n");
        return;
    }

    dev->p_dev_status->avrcp_source.remote_element_uidcounter = param->uid_counter;
    switch (((struct AVRCP_MediaPlayerItemStru *) param->item)->item_type)
    {
        case AVRCP_ITEMTYPE_MEDIAPLAYER_ITEM:
        break;
        case AVRCP_ITEMTYPE_FOLDER_ITEM:
        {
            struct AVRCP_FolderItemStru *item_this = (struct AVRCP_FolderItemStru *) param->item;
            dev->p_dev_status->avrcp_source.remote_folder_uidcounter = param->uid_counter;
            libc_memcpy(dev->p_dev_status->avrcp_source.remote_folder_uid, item_this->folder_uid, 8);
        }
        break;
        case AVRCP_ITEMTYPE_MEDIAELEMENT_ITEM:
        {
            struct AVRCP_MediaElementItemStru *item_this = (struct AVRCP_MediaElementItemStru *) param->item;
            libc_memcpy(dev->p_dev_status->avrcp_source.remote_element_uid, item_this->element_uid, 8);
        }
        break;
        default:
        break;
    }
}

//作为AG端是，其他设备通过 PASSTHROUGH来控制小机的音量
void APP_AVRCP_Rmt_cur_volume(UINT8 *bd, UINT32 event, UINT8 *param)
{
    UINT32 tlcr_pdu_id;
    struct AVRCP_PassThroughStru *p_cmd = (struct AVRCP_PassThroughStru *) param;

    AVRCP_PRINTF("pass through cmd 0x%x\n", p_cmd->op_id);

    tlcr_pdu_id = event;
    AVRCP_SET_RSPTYPE(tlcr_pdu_id, AVRCP_RSP_ACCEPTED);
    if (p_cmd->state_flag == AVRCP_STATE_FLAG_BUTTON_RELEASED)
    {
        //修改共享内存时进行锁调度，防止其他地方同时在做修改
        sys_os_sched_lock();
        switch (p_cmd->op_id)
        {
            case AVRCP_OPID_VOLUME_UP:
            g_p_info->vol_syn_flag = 2;
            break;

            case AVRCP_OPID_VOLUME_DOWN:
            g_p_info->vol_syn_flag = 3;
            break;
            default:
            break;
        }
        sys_os_sched_unlock();
    }
    AVRCP_PassThrough_Rsp(bd, p_cmd, tlcr_pdu_id);
}
void deal_avrcp_connection_event(bt_dev_t *dev, UINT8 *bd, UINT32 event, UINT8 *param)
{
    switch (event)
    {
        case AVRCP_EV_CONNECT_COMPLETE:
        {
            struct AVRCP_ConnectCompleteStru *in = (struct AVRCP_ConnectCompleteStru *) param;
            AVRCP_PRINTF("connection complete,channel %d,result %d\n", in->channel, in->result);
            if (dev->p_dev_status == NULL)
            {
                dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
                libc_memset(dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
            }
            if (in->channel == AVCTP_CONTROL_CHANNEL)
            {
                if (in->result == BT_SUCCESS)
                {
                    libc_memcpy(bd, bd, BD_ADDR_LEN);
                    //清除需要回连标志
                    dev->p_dev_status->need_auto_connect &= (uint8) AVRCP_DISCONNECTED;
                    dev->p_dev_status->avrcp_source.connected_flag = TRUE;
                    dev->p_dev_status->service_connect |= AVRCP_CONNECTED_MY;
                    dev->profile |= AVRCP_CONNECTED_MY;
                    if (get_cur_device_status(dev) == STATUS_WAIT_PAIRED)
                    {
                        bt_linking_flag = FALSE;
                        set_cur_device_status(dev, STATUS_LINKED);
                    }
                    save_rmtdev_info();
                    APP_AVRCP_data_clear();
                    APP_AVRCP_GetCapabilities_Req(bd, AVRCP_CAPABILITYID_EVENTS_SUPPORTED);
                }
            }
            else
            {
                if (in->result == BT_SUCCESS)
                {
                    dev->p_dev_status->avrcp_source.outgoing_browsing_mtu = in->mtu;
                }
                else
                {
                    dev->p_dev_status->avrcp_source.outgoing_browsing_mtu = 0;
                }
            }
        }
        break;
        case AVRCP_EV_DISCONNECT_COMPLETE:
        {
            struct AVCTP_ConnectCompleteStru *in = (struct AVCTP_ConnectCompleteStru *) param;
            AVRCP_PRINTF("disconnection complete,channel %d\n", in->channel);
            if (in->channel == AVCTP_CONTROL_CHANNEL)
            {
                if (dev->p_dev_status != NULL)
                {
                    dev->p_dev_status->avrcp_source.connected_flag = FALSE;
                    dev->p_dev_status->service_connect &= (uint8) AVRCP_DISCONNECTED;
                }
                //g_p_info->service_connect &= (uint8) AVRCP_DISCONNECTED;
                btengine_global_variable.recv_quit_flag &= (uint8) AVRCP_DISCONNECTED;
                APP_AVRCP_data_clear();
            }
        }
        break;

        default:
        break;
    }
}
void deal_avrcp_response_event(UINT8 *bd, UINT32 event, UINT8 *param)
{
    UINT8 rsp_type = AVRCP_GET_4BIT_CR(event);

    if ((rsp_type == AVRCP_RSP_REJECTED) || (rsp_type == AVRCP_RSP_NOT_IMPLEMENTED))
    {
        struct AVRCP_GeneralRejectCfmStru *in = (struct AVRCP_GeneralRejectCfmStru *) param;
        AVRCP_PRINTF_WARNNING("REJECTED\n");
        if ((in->ev_pduid & 0xFF) == AVRCP_PDUID_REGISTER_NOTIFICATION)
        {
            //APP_AVRCP_RegisterNotification_Req(bd, (UINT8)(in->ev_pduid >> 8));
        }
    }
    else
    {
        switch (AVRCP_GET_PDUID(event))
        {
            /* only pduid? FIXME, 1bit cr and 8bits pduid = event & 0x1FF */
            case AVRCP_PDUID_GET_CAPABILITIES: /* 0x10 AV/C STATUS */
            APP_AVRCP_GetCapabilities_Cfm(bd, (struct AVRCP_GetCapabilitiesRspStru *) param);
            break;
            case AVRCP_PDUID_REGISTER_NOTIFICATION: /* 0x31 AV/C NOTIFY */
            APP_AVRCP_RegisterNotification_Cfm(bd, (struct AVRCP_RegisterNotifRspStru *) param, rsp_type);
            break;
            case AVRCP_PDUID_GET_FOLDER_ITEMS: /* 0x71 Browsing */
            APP_AVRCP_GetFolderItems_Cfm(bd, (struct AVRCP_GetFolderItemsRspStru *) param);
            break;
            default:
            break;
        }
    }
}
void deal_avrcp_command_event(UINT8 *bd, UINT32 event, UINT8 *param)
{
    switch (AVRCP_GET_PDUID(event))
    {
        /* only pduid? FIXME, 1bit cr and 8bits pduid = event & 0x1FF */
        case AVRCP_PDUID_GET_CAPABILITIES: /* 0x10 AV/C STATUS */
        APP_AVRCP_GetCapabilitiesRsp(bd, event, *(uint8 *) param);
        break;

        case AVRCP_PDUID_REGISTER_NOTIFICATION: /* 0x31 AV/C NOTIFY */
        AVRCP_PRINTF("register\n")
        ;
        APP_AVRCP_RegisterNotification_Ind(bd, (struct AVRCP_RegisterNotifReqStru *) param);
        break;

        case AVRCP_PDUID_SET_ABSOLUTE_VOLUME: /* 0x50 AV/C CONTROL */
        AVRCP_PRINTF("set vol\n")
        ;
        APP_AVRCP_SetAbsoluteVolRsp(bd, event, *(uint8 *) param);
        break;
        //由于本身注册了TG，因此是支持接收别人发过来的passthrough
        case AVRCP_PDUID_PASSTHROUGH:
        APP_AVRCP_Rmt_cur_volume(bd, event, param);
        break;
        default:
        AVRCP_PRINTF("other cmd\n")
        ;
        break;
    }
}
/*-----------------------------------------------------------------------------
 Description:
 This sample function is used by app to register callback to AVRCP.
 event:     <8bit reserved>,
 <4bit reserved>,<4bit tl>,
 <4bit ctype/rsp>,<1bit channel>,<1bit grouping reserved>,<2bit cr/grouping>,
 <8bit pdu_id>
 -----------------------------------------------------------------------------*/
void APP_AVRCP_Cbk(UINT8 *bd, UINT32 event, UINT8 *param, UINT16 len)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(bd);

    if (dev == NULL)
    {
        AVRCP_PRINTF_WARNNING("device is null\n");
        return;
    }

    //BTEG_DBG_PRINTF("\n<avrcp> event 0x%x\n", AVRCP_GET_CBK_GROUPING(event));
    switch (AVRCP_GET_CBK_GROUPING(event))
    {
        /* grouping */
        case AVRCP_EV_GROUP_CONNECTION:
        deal_avrcp_connection_event(dev, bd, event, param);
        break;
        case AVRCP_EV_GROUP_RESPONSE:/* Confirm */
        deal_avrcp_response_event(bd, event, param);
        break;
        case AVRCP_EV_GROUP_COMMAND:
        deal_avrcp_command_event(bd, event, param);
        break;
        default:
        AVRCP_PRINTF_WARNNING("other event\n")
        ;
        break;
    }
    BTSRC_AVRCP_FreeEventParam(event, param);
}

int8 APP_AVRCP_PassThrough_Req(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id)
{
    struct AVRCP_PassThroughStru *in = NULL;
    UINT8 *bd = NULL;

    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->avrcp_source.connected_flag))
    {
        if (dev->p_dev_status->sniff_flag != 0)
        {
            bt_exit_sniff(dev);
        }

        bd = dev->dev_info.bd_addr.bytes;

        in = NEW(sizeof(struct AVRCP_PassThroughStru));
        in->vendor_unique_id = vendor_id;
        in->state_flag = AVRCP_STATE_FLAG_BUTTON_PUSHED;
        in->op_id = op_id;
        BTSRC_AVRCP_PassThrough_Req(bd, in);

        sys_mdelay(5);
        in->vendor_unique_id = vendor_id;
        in->state_flag = AVRCP_STATE_FLAG_BUTTON_RELEASED;
        in->op_id = op_id;
        BTSRC_AVRCP_PassThrough_Req(bd, in);
        FREE(in);

        return 0;
    }
    else
    {
        AVRCP_PRINTF_WARNNING("passthrough req null\n");
        return -1;
    }
}

int8 APP_AVRCP_PassThrough_Press(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id)
{
    struct AVRCP_PassThroughStru *in = NULL;
    UINT8 *bd = NULL;

    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->avrcp_source.connected_flag))
    {
        bd = dev->dev_info.bd_addr.bytes;

        in = NEW(sizeof(struct AVRCP_PassThroughStru));
        in->vendor_unique_id = vendor_id;
        in->state_flag = AVRCP_STATE_FLAG_BUTTON_PUSHED;
        in->op_id = op_id;
        BTSRC_AVRCP_PassThrough_Req(bd, in);
        FREE(in);
        return 0;
    }
    else
    {
        AVRCP_PRINTF_WARNNING("passthrough press null\n");
        return -1;
    }
}

int8 APP_AVRCP_PassThrough_Release(bt_dev_t *dev, UINT8 op_id, UINT8 vendor_id)
{
    struct AVRCP_PassThroughStru *in = NULL;
    UINT8 *bd = NULL;

    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->avrcp_source.connected_flag))
    {
        bd = dev->dev_info.bd_addr.bytes;

        in = NEW(sizeof(struct AVRCP_PassThroughStru));
        in->vendor_unique_id = vendor_id;
        in->state_flag = AVRCP_STATE_FLAG_BUTTON_RELEASED;
        in->op_id = op_id;
        BTSRC_AVRCP_PassThrough_Req(bd, in);
        FREE(in);
        return 0;
    }
    else
    {
        AVRCP_PRINTF_WARNNING("passthrough release null\n");
        return -1;
    }
}

void Avrcp_Connect_Req(bt_dev_t *dev)
{
    struct AVRCP_ConnectReqStru *para;

    AVRCP_PRINTF("connect start\n");

    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        if ((dev->p_dev_status->service_connect & AVRCP_CONNECTED_MY) == 0)
        {
            para = NEW(sizeof(struct AVRCP_ConnectReqStru));
            libc_memcpy(para->bd_addr, dev->dev_info.bd_addr.bytes, BD_ADDR_LEN);
            para->browsing_support = TRUE; /* remote device supports browsing channel or not */
            BTSRC_AVRCP_DisconnectReq(dev->dev_info.bd_addr.bytes);
            BTSRC_AVRCP_ConnectReq(para);
        }
    }
}

