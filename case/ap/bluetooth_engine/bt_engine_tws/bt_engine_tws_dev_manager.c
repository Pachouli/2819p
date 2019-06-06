/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：btstack tws等。
 * 作者：kehaitao
 ********************************************************************************/
#include "./../bluetooth_engine.h"

#define PAIR_PAIREDDEV_FIRST //如果已组队过，先回连，回连不到再搜索

extern uint8 TWS_Paired(void);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
bt_dev_t *TWS_FindByStreamHdl(void *stream_hdl)
{

    bt_dev_t *p_dev;

    p_dev = NULL;
    if (devs_cfg.tws_paired_list.p_dev_status != NULL)
    {
        if (devs_cfg.tws_paired_list.p_dev_status->a2dp_source.stream_handle == stream_hdl)
        {
            p_dev = &devs_cfg.tws_paired_list;
        }
    }
    return p_dev;
}
bt_dev_t *TWS_FindByAclHdl(uint16 acl_hdl)
{

    bt_dev_t *p_dev;

    p_dev = NULL;
    if (devs_cfg.tws_paired_list.p_dev_status != NULL)
    {
        if (devs_cfg.tws_paired_list.p_dev_status->acl_handle == acl_hdl)
        {
            p_dev = &devs_cfg.tws_paired_list;
        }
    }
    return p_dev;
}

bt_dev_t *TWS_FindPairedDevByAddr(uint8 *bdAddress)
{

    bt_dev_t *p_dev;

    p_dev = NULL;
    if (devs_cfg.tag == PAIRED_INFO_TAG)
    {
        if (!libc_memcmp(devs_cfg.tws_paired_list.dev_info.bd_addr.bytes, bdAddress, BD_ADDR_LEN))
        {
            p_dev = &devs_cfg.tws_paired_list;
        }
    }
    return p_dev;
}
void TWS_PerfectPairedInfo(uint8 *bd_addr, bt_dev_t *p_dev)
{
    libc_memcpy(p_dev->dev_info.bd_addr.bytes, bd_addr, BD_ADDR_LEN);
    /* maybe name is too long. */
    libc_memset(p_dev->dev_info.name, 0x55, MGR_MAX_REMOTE_NAME_LEN - 1);
    p_dev->dev_info.name[MGR_MAX_REMOTE_NAME_LEN - 1] = 0;
    p_dev->profile |= PROFILE_VALID; //保存了linkkey后再更新LINKKEY_VALID
    p_dev->dev_type = TWS_DEV;
#if 0
    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.linked_dev_list[idx] == NULL)
        {
            btengine_global_variable.linked_dev_list[idx] = p_dev;
            break;
        }
    }
#endif
}

bt_dev_t *TWS_ConfigPairedInfo(uint8 *bdAddress)
{
    bt_dev_t *p_dev;
    uint32 tmp32;
    uint8 dev_class[3];
    uint8 *p_temp;
    uint8 need_auto_connect;
    uint8 is_source;

    //检查该设备是否为已配对TWS设备
    p_dev = TWS_FindPairedDevByAddr(bdAddress);
    if (p_dev == NULL)
    {
        TWS_PRINTF_WARNNING("cur dev not tws dev\n");
        return NULL;
    }

    //获取设备类信息
    tmp32 = btengine_param.class_device;
    dev_class[0] = (uint8) (tmp32); //minor device
    dev_class[1] = (uint8) (tmp32 >> 8); //major device
    dev_class[2] = (uint8) (tmp32 >> 16); //major service

    //比较高三字节地址
    if (libc_memcmp(&g_btengine_var.device_addr.bytes[3], (bdAddress+3), 3) == 0)
    {
        TWS_DBG_PRINTF("dev match\n");

        //初始化p_dev的状态
        if (p_dev->p_dev_status == NULL)
        {
            p_dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
            p_temp = NULL;
            need_auto_connect = 0x0;
            is_source = 0;
        }
        else
        {
            p_temp = p_dev->p_dev_status->a2dp_source.stream_handle;
            need_auto_connect = p_dev->p_dev_status->need_auto_connect;
            is_source = p_dev->p_dev_status->a2dp_source.is_source;
        }

        libc_memset(p_dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
        p_dev->p_dev_status->a2dp_source.stream_handle = p_temp;
        p_dev->p_dev_status->need_auto_connect = need_auto_connect;
        p_dev->p_dev_status->a2dp_source.is_source = is_source;

        //初始化p_dev,包括bt_addr/bt_name/bt_profile支持情况，设定p_dev设备类型
        TWS_PerfectPairedInfo(bdAddress, p_dev);

    }
    else
    {
        TWS_DBG_PRINTF("dev not match\n");
        p_dev = NULL;
    }
    return p_dev;
}

void TWS_AddDevToPairedList(uint8 *bd)
{

    if (devs_cfg.tag != PAIRED_INFO_TAG)
    {
        devs_cfg.tag = PAIRED_INFO_TAG;

    }
    libc_memcpy(devs_cfg.tws_paired_list.dev_info.bd_addr.bytes, bd, BD_ADDR_LEN);
    devs_cfg.tws_paired_list.is_source = 1;
    devs_cfg.tws_paired_list.dev_type = TWS_DEV;
}
bool TWS_CheckDev(bt_dev_t *dev)
{

    uint8 name_len;
    uint8 dev_full_name[BT_MAC_NAME_LEN_MAX];

    name_len = libc_strlen(g_btengine_var.device_name);

    if (name_len > BT_MAC_NAME_LEN_MAX)
    {
        name_len = BT_MAC_NAME_LEN_MAX;
    }

    bt_get_remote_phone_name(dev, dev_full_name);

    if (libc_memcmp(g_btengine_var.device_name, dev_full_name, name_len) != 0)
    {

        TWS_DBG_PRINTF("check name not match\n");
        return FALSE;
    }
    if (libc_memcmp(&g_btengine_var.device_addr.bytes[3],&dev->dev_info.bd_addr.bytes[3],3) != 0)
    {
        TWS_DBG_PRINTF("check addr not match\n");
        return FALSE;
    }

    return TRUE;
}

void TWS_MatchAvailableDevice(struct HCI_EIR_DataNodeStru *node, struct GAP_RemoteDeviceInfoStru *in)
{
    uint8 tws_pair_length_tmp;
    uint8 tws_romote_name_length;
    bt_dev_t *p_dev = NULL;

    if (TWS_GetPairedStatus() != TWS_SEARCH)
    {
        TWS_PRINTF_WARNNING("no search\n");
        return;
    }

    if ((bt_manager_cfg_info.tws_master_role_depending_on_key == 0) && (tws_status_info.have_paired_dev == TRUE))
    {
        p_dev = TWS_FindPairedDevByAddr(in->addr.bd);
        if (p_dev != NULL)
        {
            bt_linking_flag = TRUE;
            bt_linking_flag = TWS_ManuPaired();
            return;
        }
    }
    tws_pair_length_tmp = libc_strlen(g_btengine_var.device_name);

    if (tws_pair_length_tmp >= BT_MAC_NAME_LEN_MAX)
    {
        tws_pair_length_tmp = BT_MAC_NAME_LEN_MAX;
    }TWS_DBG_PRINTF("node name %s\n",node->u.name.data);TWS_DBG_PRINTF("local name %s\n",g_btengine_var.device_name);
    //远端设备名长度
    tws_romote_name_length = node->len - 1;
    if (tws_romote_name_length >= BT_MAC_NAME_LEN_MAX)
    {
        tws_romote_name_length = BT_MAC_NAME_LEN_MAX;
    }

    if (tws_romote_name_length == tws_pair_length_tmp)
    {
        //调试保证只每次搜索相同名字的设备时候只响应一次search ok !!!!!
        if (libc_memcmp(g_btengine_var.device_name,node->u.name.data,
                tws_pair_length_tmp) != 0)
        {
            //不符合对箱特征，退出连接，重置标志位
            bt_linking_flag = FALSE;
            TWS_DBG_PRINTF("name not match\n");
            return;
        }
        if (libc_memcmp(&g_btengine_var.device_addr.bytes[3],&in->addr.bd[3],3) != 0)
        {
            //不符合对箱特征，退出连接，重置标志位
            bt_linking_flag = FALSE;
            TWS_DBG_PRINTF("addr not match\n");
            return;
        }
        TWS_CancelDeviceDiscovery();
        TWS_PRINTF("search ok\n");
        if (tws_status_info.inquire_timer_id != -1)
        {
            kill_app_timer(THIS_APP_TIMER_GROUP, tws_status_info.inquire_timer_id);
            tws_status_info.inquire_timer_id = -1;
        }

        libc_memset(&devs_cfg.tws_paired_list, 0x00, sizeof(bt_dev_t));
        TWS_AddDevToPairedList(in->addr.bd);
        bt_linking_flag = TWS_Paired();
    }
    else
    {
        //不符合对箱特征，退出连接，重置标志位
        bt_linking_flag = FALSE;

    }

}

void TWS_RestartInquire(void)
{
    tws_status_info.start_inquire = 1;
    if (tws_status_info.inquire_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, tws_status_info.inquire_timer_id);
        tws_status_info.inquire_timer_id = -1;
    }
}

void TWS_DevInquireComplete(void)
{
    uint8 stop_search = 0;
    uint8 cur_device_status;
    uint16 inquire_time_delay;
    BTSRC_GAP_CancelDeviceDiscovery((HANDLE)NULL, NULL);
    if ((TWS_GetPairedStatus() == TWS_IDLE) || (TWS_GetPairedStatus() == TWS_SEARCH))
    {

        cur_device_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
        if ((cur_device_status == STATUS_WAIT_PAIRED) || (cur_device_status == STATUS_NONE))
        {
#ifdef TWS_SLAVE_NO_DISC_PAIRED_BEFORE
            if((tws_status_info.have_paired_dev == TRUE)&&(devs_cfg.tws_paired_list.is_source == 0))
            {
                bt_set_visual_mode (GAP_VM_NONDISCOV);
            }
            else
            {
                bt_set_visual_mode (GAP_VM_DEFAULT);
            }
#else

            bt_set_visual_mode(GAP_VM_DEFAULT);
#endif
        }
        else
        {

            if (TWS_GetPairedStatus() == TWS_PAIRED)
            {
                bt_set_visual_mode(GAP_VM_PAIR);

            }
            else
            {
                bt_set_visual_mode(GAP_VM_NONDISCOV);
            }

        }
        if (tws_status_info.search_device_flag == 1)
        {
            TWS_DBG_PRINTF("inquiry cnt %d\n",tws_status_info.start_inquire_cnt);
            if (tws_status_info.start_inquire_cnt <= INQUIRY_CNT)
            {
                if (tws_status_info.inquire_timer_id != -1)
                {
                    kill_app_timer(THIS_APP_TIMER_GROUP, tws_status_info.inquire_timer_id);
                    tws_status_info.inquire_timer_id = -1;
                }
                if (devs_cfg.tws_paired_list.is_source == 1)
                {
                    inquire_time_delay = INQUIRY_TIME_DELAY_SOURCE;
                }
                else
                {
                    inquire_time_delay = INQUIRY_TIME_DELAY_SNK;

                }
                tws_status_info.inquire_timer_id = set_single_shot_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN,
                        APP_TIMER_ATTRB_CONTROL, inquire_time_delay, TWS_RestartInquire);
            }
            else
            {
                stop_search = 1;
                tws_status_info.start_inquire = 0;
                if (tws_status_info.inquire_timer_id != -1)
                {
                    kill_app_timer(THIS_APP_TIMER_GROUP, tws_status_info.inquire_timer_id);
                    tws_status_info.inquire_timer_id = -1;
                }
                tws_status_info.search_device_flag = 0;
                TWS_SetPairedStatus(TWS_IDLE);
                TWS_PRINTF("search finish,not find paired dev\n");
            }

            if (stop_search == 0)
            {
                tws_status_info.start_inquire_cnt++;
            }

        }
        TWS_SetPairedStatus(TWS_IDLE);

    }
}
void TWS_CancelDeviceDiscovery(void)
{
    //连接上设备，若还在搜索，停止搜索
    // if (tws_status_info.search_device_flag == 1)

    {
        BTSRC_GAP_CancelDeviceDiscovery((HANDLE)NULL, NULL);
        tws_status_info.search_device_flag = 0;
    }

    if (tws_status_info.inquire_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, tws_status_info.inquire_timer_id);
        tws_status_info.inquire_timer_id = -1;
    }

}
void TWS_A2dpConnectComplete(struct A2DP_OpenedStru *in, bt_dev_t *dev)
{
    //close fast scan
    bt_RestoreNormalScanMode();
    bt_auto_connect_cfg.tws_connect_type = AUTO_CONNECT_TYPE_TIMEOUT;

    //close repaire timer
    if (repaired_timer != -1)
    {
        kill_app_timer(APP_TIMER_GROUP_BTENGINE, repaired_timer);
        repaired_timer = -1;
    }

    //set a2dp role
    if (dev->p_dev_status->a2dp_source.is_source == 0)
    {

        tws_status_info.is_src_flag = FALSE;
        g_p_info->dev_role = TWS_SLAVE;
    }
    else
    {
        tws_status_info.is_src_flag = TRUE;
        g_p_info->dev_role = TWS_MASTER;
    }

    //set bt write link supervision timeout
    if (g_p_info->dev_role == TWS_MASTER)
    {
        TWS_PRINTF("supervision_timeout 0x%x\n", TWS_LINK_LOSS_TIMEOUT);
        bt_write_link_supervision_timeout(dev->p_dev_status->acl_handle, TWS_LINK_LOSS_TIMEOUT);
    }

    //save tws info
    tws_status_info.src_stream_cid = L2CAP_GetRemoteCID(in->cid);
    TWS_PRINTF("s cid %d\n", tws_status_info.src_stream_cid);
    TWS_SaveDevInfo(dev);
    //TWS_DBG_PRINTF("READY FOR PHONE2\n");

    //tws conect complete
    tws_connect_flag = FALSE;

    //set visual mode
    if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
    {
        if (current_visual_mode != GAP_VM_PAIR)
        {
            bt_set_visual_mode(GAP_VM_PAIR);

        }
    }
    else
    {
        if (devs_cfg.tws_paired_list.is_source == 0)
        {

            if (current_visual_mode != GAP_VM_PAIR)
            {
                bt_set_visual_mode(GAP_VM_PAIR);

            }

        }
        else
        {
            if (current_visual_mode != GAP_VM_DEFAULT)
            {
                bt_set_visual_mode(GAP_VM_DEFAULT);
            }
        }
    }
    TWS_SetPairedStatus(TWS_PAIRED);
    return;
}

uint8 TWS_GetDevRole(bt_dev_t *dev)
{
    if ((dev->dev_type == TWS_DEV) && (dev->is_source == TRUE))
    {
        return 1;
    }
    return 0;
}
uint8 TWS_GetDevType(bt_dev_t *dev)
{
    if (TWS_CheckDev(dev) == TRUE)
    {
        TWS_PRINTF("is tws dev\n");
        dev->dev_type = TWS_DEV;
        g_p_info->dev_type = TWS_DEV;

        return TRUE;
    }
    else
    {
        TWS_PRINTF("not tws dev\n");
        dev->dev_type = NO_TWS_DEV;
        g_p_info->dev_type = NO_TWS_DEV;
        return FALSE;
    }
    return FALSE;

}
void TWS_PairedDev_Copyto_TWSDev(bt_dev_t *dev)
{
    bt_dev_t *dev_temp;
    TWS_DBG_PRINTF("copyto twspairlist\n");
    dev_temp = BT_RmtFindDevByBD(dev->dev_info.bd_addr.bytes);
    if (dev_temp != NULL)

    {
        TWS_DBG_PRINTF("find the dev\n");
        if (devs_cfg.tws_paired_list.p_dev_status != NULL)
        {
            FREE(devs_cfg.tws_paired_list.p_dev_status);
            devs_cfg.tws_paired_list.p_dev_status = NULL;
        }
        libc_memcpy(&devs_cfg.tws_paired_list, dev_temp, sizeof(bt_dev_t));
        devs_cfg.tws_paired_list.mru_num = 1;
        devs_cfg.tws_paired_list.p_dev_status = NULL;
        devs_cfg.tws_paired_list.p_dev_status = NEW(sizeof(bt_dev_status_t));

        TWS_DBG_PRINTF("devtype %d\n",devs_cfg.tws_paired_list.dev_type);
        if (dev_temp->p_dev_status != NULL)
        {
            libc_memcpy(devs_cfg.tws_paired_list.p_dev_status, dev_temp->p_dev_status, sizeof(bt_dev_status_t));
        }
        RmtRemoveDevFromLinkedList(dev_temp);
        RmtRemovePairedDev(dev_temp);

    }

}

void TWS_SearchMatchingDev(void)
{
    if (TWS_GetPairedStatus() == TWS_PAIRED)
    {
        return;
    }
    if ((tws_status_info.start_inquire == 0) && (tws_status_info.inquire_timer_id == -1))
    {
        tws_status_info.start_inquire_cnt = 0;
        tws_status_info.paried_mode = TWS_PAIR_PAIRED;
        TWS_DBG_PRINTF("tws paired mode %d\n",tws_status_info.paried_mode);
        tws_status_info.start_inquire = 1;

    }
}

bool cur_dev_have_phone_connected(void)
{
    if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_WAIT_PAIRED)
            && (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_NONE))
    {
        return TRUE;
    }

    return FALSE;
}

//half-second timer中检查tws是否需要回连
void TWS_CheckPairedState(void)
{
    if (tws_status_info.start_inquire == 1)
    {
#if (SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED == 0)
        //手机已连接，不开放搜索组队
        if (cur_dev_have_phone_connected() == TRUE)
        {
            TWS_PRINTF("have phone connected,no searching\n");
            tws_status_info.start_inquire = 0;
            return;
        }
#endif

#ifdef  PAIR_PAIREDDEV_FIRST
        if (TWS_GetPairedStatus() == TWS_PAIRED)
        {
            tws_status_info.start_inquire = 0;
            return;
        }

        //tws_pair_paireddev_first_flag
        if (tws_status_info.paried_mode == TWS_PAIR_PAIRED)
        {
            if (TWS_GetPairedStatus() != TWS_IDLE)
            {
                //TWS_DBG_PRINTF("tws not idle\n");
                //tws_status_info.start_inquire = 0;
                return;
            }

            if (bt_linking_flag == TRUE)
            {
                //TWS_PRINTF_WARNNING("f linking,no searching\n");
                return;
            }

            //已有配对且角色不随发起组队按键转换
            if ((tws_status_info.have_paired_dev == TRUE)
                    && (bt_manager_cfg_info.tws_master_role_depending_on_key == 0))
            {
                //如果当前tws设备没有完全断开，那么先等待当前设备断开
                if (devs_cfg.tws_paired_list.p_dev_status != NULL)
                {
                    if ((devs_cfg.tws_paired_list.p_dev_status->service_connect & A2DP_CONNECTED) != 0)
                    {
                        TWS_PRINTF_WARNNING("wait a2dp disconnect\n");
                        return;
                    }
                }

                //有曾组队音箱，发起组队回连
                bt_auto_connect_cfg.tws_connect_type = AUTO_CONNECT_TYPE_SEARCH;
                tws_paired_count = 1;
                uint8 tmp_paried_mode = tws_status_info.paried_mode;
                tws_status_info.paried_mode = TWS_SEARCH_NEW_DEV;
                TWS_DBG_PRINTF("tws paired mode change %d->%d\n",tmp_paried_mode,tws_status_info.paried_mode);
                TWS_ManuPaired();

            }
            else
            {
                //无曾组队音箱，搜索组队
                TWS_RmtSearchDev();
                tws_status_info.start_inquire = 0;
            }
        }
        else if (tws_status_info.paried_mode == TWS_SEARCH_NEW_DEV)
        {
            if ((bt_linking_flag == TRUE))
            {
                TWS_PRINTF_WARNNING("linking,no searching\n");
                return;
            }

            if (tws_status_info.need_repaired == TRUE)
            {
                TWS_DBG_PRINTF("need repaired,absort search\n");
                return;
            }

            if ((TWS_GetPairedStatus() == TWS_PAIREING) || (TWS_GetPairedStatus() == TWS_SEARCH))
            {
                TWS_DBG_PRINTF("Tws status err,absort search\n");
                return;
            }

            TWS_RmtSearchDev();
            tws_status_info.start_inquire = 0;

        }

#else
        //if tws don't repairing first
        if((TWS_GetPairedStatus()==TWS_IDLE)&&(bt_linking_flag==FALSE))
        {
            TWS_RmtSearchDev();

        }

        tws_status_info.start_inquire = 0;

#endif

    }
}
void TWS_CheckForceUlink(void)
{
    uint8 i;
    if (tws_status_info.force_ulink_flag == TRUE)
    {
        tws_status_info.force_ulink_flag = FALSE;
        for (i = 0; i < g_btengine_cfg_data.max_link_num; i++)
        {
            if (btengine_global_variable.linked_dev_list[i] != NULL)
            {
                TWS_DBG_PRINTF("tws force unlink!\n");
                bt_disconnect_acl(btengine_global_variable.linked_dev_list[i]);
            }
        }
    }
}
void TWS_GetClassOfDevice(void)
{
    uint8 dev_class[3];
    uint32 tmp32;
    tmp32 = btengine_param.class_device;
    dev_class[0] = (uint8) (tmp32); //minor device
    dev_class[1] = (uint8) (tmp32 >> 8); //major device
    dev_class[2] = (uint8) (tmp32 >> 16); //major service
    libc_memcpy(tws_status_info.classOfDevice, dev_class, 3);
}

//half-second timer中检查tws是否需要回连
void TWS_CheckPairedStatus(void)
{
    uint8 max_connect_count;

    if (bt_linking_flag == TRUE)
    {
        return;
    }

    //tws在回连手机和tws设备间交替，标志在确认需要对某种设备发起回连时改变
    if (tws_status_info.cur_reconnect_dev == NO_TWS_DEV)
    {
        //正在回连手机
        if (phone_connect_flag == TRUE)
        {
            //TWS_DBG_PRINTF("phone is pairing\n");
            return;
        }
    }

    //TWS音箱需要回连并且tws还未连接
    if ((tws_status_info.need_repaired == TRUE) && (TWS_GetPairedStatus() == TWS_IDLE))
    {
        //主箱回连策略：tws设备与手机轮流回连
        if (devs_cfg.tws_paired_list.is_source == 1)
        {
            //上次是对tws设备发起回连
            if (tws_status_info.cur_reconnect_dev == TWS_DEV)
            {
                //需要回连手机
                if ((already_start_connect_phone_flag == FALSE) || (g_p_info->isneed_reconnect_flag == TRUE))
                {
                    tws_tick_count = 0;
                    return;
                }
            }
        }

        //暂时使用伪随机数
        //random_value = bt_random();
        //解决tws回连冲突碰撞
        if (devs_cfg.tws_paired_list.is_source != 1)
        {
            random_value = 30;
        }
        else
        {
            random_value = 40;
        }

        //标志位目前用于hfp接收到回连请求后，会拒绝连接。并且在hfp关闭后立即发起回连
        if (have_wait_for_manupair_flag == FALSE)
        {
            //第一次回连组队时
            if ((tws_paired_count == 0) && (tws_tick_count == 0))
            {
                //副箱需要在5S后发起组队回连
                if (devs_cfg.tws_paired_list.is_source == 0)
                {
                    tws_tick_count = random_value - 10;
                }
            }

            tws_tick_count++;
            if (bt_auto_connect_cfg.tws_connect_type == AUTO_CONNECT_TYPE_STARTUP)
            {
                max_connect_count = bt_auto_connect_cfg.tws_connect_count_startup;
            }
            else if (bt_auto_connect_cfg.tws_connect_type == AUTO_CONNECT_TYPE_TIMEOUT)
            {
                max_connect_count = bt_auto_connect_cfg.tws_connect_count_timeout;

            }
            else
            {
                max_connect_count = bt_auto_connect_cfg.tws_connect_count_before_search;

            }

            //超过次数断开
            if ((max_connect_count != 0) && (tws_paired_count >= max_connect_count))
            {
                tws_status_info.need_repaired = FALSE;
                // tws_paired_count=0;
                tws_tick_count = 0;
                //libc_memset(&devs_cfg.tws_paired_list,0x0,sizeof(bt_dev_t));
                return;
            }

            //随机等待一些周期，目前伪随机，再进行回连
            if (tws_tick_count >= random_value)
            {

                tws_tick_count = 0;
                tws_paired_count++;
                TWS_ManuPaired();
                //TWS_AutoPaired();
            }
        }
        else
        {
            have_wait_for_manupair_flag = FALSE;
            TWS_ManuPaired();
        }
    }
    else
    {
        //连接成功
        if (TWS_GetPairedStatus() == TWS_PAIRED)
        {
            tws_paired_count = 0;
        }
        tws_status_info.need_repaired = FALSE;
        tws_tick_count = 0;
    }

}
uint8 TWS_AutoPaired(void)
{
    TWS_PRINTF("auto pair\n");
    if ((devs_cfg.tws_paired_list.dev_type != TWS_DEV) || (devs_cfg.tws_paired_list.is_source == 1))
    {
        TWS_PRINTF_WARNNING("paired info empty\n");
        return FALSE;
    }
    if (TWS_GetPairedStatus() != TWS_IDLE)
    {
        TWS_PRINTF_WARNNING("in search or pairing\n");
    }
    if (bt_linking_flag == TRUE)
    {
        TWS_PRINTF_WARNNING("is linking\n");
        return FALSE;
    }

    //TWS回连，临时提高频率
    bt_adjust_clk(BT_LINK_WORK_FREQ);

    TWS_CancelDeviceDiscovery();

    devs_cfg.tws_paired_list.is_source = 0;
    TWS_SetPairedStatus(TWS_PAIREING);

    A2dpConnect(0, devs_cfg.tws_paired_list.dev_info.bd_addr.bytes);

    return TRUE;
}
uint8 TWS_ManuPaired(void)
{
    uint8 ret;

    if (TWS_GetPairedStatus() != TWS_IDLE)
    {
        TWS_PRINTF_WARNNING("search or pairing\n");
        return FALSE;
    }

    TWS_PRINTF("manu pair\n");

    ret = TWS_Paired();
    return ret;

}
uint8 TWS_Paired(void)
{

    tws_status_info.need_repaired = FALSE;

    //没有回连配对信息
    if (devs_cfg.tws_paired_list.dev_type != TWS_DEV)
    {
        TWS_PRINTF_WARNNING("pair info empty\n");
        return FALSE;
    }

#if (SUPPORT_TWS_CONNECT_WHEN_PHONE_CONNECTED == 0)
    //手机已连接，不开放搜索组队
    if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_WAIT_PAIRED)
            && (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_NONE))
    {
        TWS_PRINTF_WARNNING("have phone connected,no pairing\n");
        return FALSE;
    }
#endif

    if (bt_linking_flag == TRUE)
    {
        TWS_PRINTF_WARNNING("is linking\n");
        return FALSE;
    }

    //tws a2dp还未断开，等待断开
    if ((devs_cfg.tws_paired_list.p_dev_status != NULL)
            && ((devs_cfg.tws_paired_list.p_dev_status->service_connect & A2DP_CONNECTED) != 0))
    {
        TWS_PRINTF_WARNNING("pair_wait a2dp disconnect\n");
        tws_status_info.need_repaired = TRUE;
        return FALSE;
    }

    //通话中不接受回连请求（但是也没拒绝啊）
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_CALL_INCOMING:
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        case STATUS_SIRI:
        tws_status_info.need_repaired = TRUE;
        have_wait_for_manupair_flag = TRUE;
        TWS_PRINTF_WARNNING("is calling\n")
        ;
        return FALSE;
        break;
    }

    TWS_PRINTF("pairing\n");

    TWS_SetPairedStatus(TWS_PAIREING);

    //TWS正在连接，临时提高频率
    bt_adjust_clk(BT_LINK_WORK_FREQ);

    TWS_CancelDeviceDiscovery();
    tws_connect_flag = TRUE;
    tws_status_info.cur_reconnect_dev = TWS_DEV;

    bt_set_visual_mode(GAP_VM_PAIR);

    A2dpConnect(devs_cfg.tws_paired_list.is_source, devs_cfg.tws_paired_list.dev_info.bd_addr.bytes);

    return TRUE;
}

void TWS_SaveDevInfo(bt_dev_t *dev)
{
    tws_vram_info_t tws_vram_info;

    if (dev->dev_type == TWS_DEV)
    {
        tws_status_info.have_paired_dev = TRUE;

        tws_vram_info.magic = VRAM_MAGIC(VM_BT_TWS_INFO);
        tws_vram_info.paired_flag = 1;

        devs_cfg.tws_paired_list.is_source = dev->p_dev_status->a2dp_source.is_source;
        if (dev->p_dev_status->a2dp_source.is_source == 0)
        {
            libc_memcpy(&devs_cfg.tws_paired_list, dev, sizeof(bt_dev_t));
        }
        libc_memcpy(&tws_vram_info.tws_paired_list, &devs_cfg.tws_paired_list, sizeof(bt_dev_t));
        tws_vram_info.tws_paired_list.p_dev_status = NULL;
        tws_vram_info.paired_count = vram_devs_cfg.paired_count + 1;
        sys_vm_write(&tws_vram_info, VM_BT_TWS_INFO, sizeof(tws_vram_info_t));
        if (devs_cfg.tws_paired_list.is_source == 1)
        {
            TWS_DBG_PRINTF("save src info\n");
        }
        else
        {
            TWS_DBG_PRINTF("save snk info\n");
        }
    }
}

void clear_tws_info(void)
{
    tws_vram_info_t tws_vram_info;
    libc_memset(&tws_vram_info, 0x0, sizeof(tws_vram_info_t));
    tws_vram_info.magic = VRAM_MAGIC(VM_BT_TWS_INFO);
    sys_vm_write(&tws_vram_info, VM_BT_TWS_INFO, sizeof(tws_vram_info_t));
}

void TWS_global_var_init(void)
{
    senddata_cnt = 0;
    random_value = 0;

    TWS_GetClassOfDevice();
    tws_status_info.search_device_flag = 0;
    tws_status_info.paired_count = 0;
    tws_status_info.inquire_timer_id = -1;
    tws_status_info.tws_paired_src_reconn_phone_enable = 1;
    tws_status_info.a2dp_stream_status.status = A2DP_STREAM_DONE;
    tws_status_info.a2dp_stream_status.stream_hdl = NULL;
}

void TWS_GetPairedInfo_Form_Vram(void)
{
    tws_vram_info_t tws_vram_info;

    if (sys_vm_read(&tws_vram_info, VM_BT_TWS_INFO, sizeof(tws_vram_info_t)) < 0)
    {
        tws_vram_info.magic = 0xffff;
    }
    if (tws_vram_info.magic != VRAM_MAGIC(VM_BT_TWS_INFO))
    {
        libc_memset(&tws_vram_info, 0x0, sizeof(tws_vram_info_t));
        tws_vram_info.magic = VRAM_MAGIC(VM_BT_TWS_INFO);
        sys_vm_write(&tws_vram_info, VM_BT_TWS_INFO, sizeof(tws_vram_info_t));
    }
    else
    {
        libc_memcpy(&devs_cfg.tws_paired_list, &tws_vram_info.tws_paired_list, sizeof(bt_dev_t));
        devs_cfg.tws_paired_list.p_dev_status = NULL;
        devs_cfg.tag = PAIRED_INFO_TAG;
        tws_status_info.have_paired_dev = tws_vram_info.paired_flag;
        tws_status_info.paired_count = tws_vram_info.paired_count;
    }
}
void TWS_SetPairedStatus(uint8 cur_status)
{
    if (g_p_info == NULL)
    {
        return;
    }
    TWS_PRINTF("status %d->%d\n", g_p_info->tws_pair_status, cur_status);
    g_p_info->tws_pair_status = cur_status;
}
uint8 TWS_GetPairedStatus(void)
{
    if (g_p_info == NULL)
    {
        return TWS_IDLE;
    }
    return g_p_info->tws_pair_status;
}

void TWS_RepairedTimer(void)
{
    uint8 cur_device_status;
    cur_device_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    if ((TWS_GetPairedStatus() == TWS_IDLE)
            && ((cur_device_status == STATUS_WAIT_PAIRED) || (cur_device_status == STATUS_NONE)))
    {
        tws_status_info.need_repaired = TRUE;
        tws_tick_count = 0;
    }TWS_DBG_PRINTF("RepairedTimer handle\n");
    if (repaired_timer != -1)
    {
        kill_app_timer(APP_TIMER_GROUP_BTENGINE, repaired_timer);
        repaired_timer = -1;
    }

}
void TWS_DisconnectAclLink(void)
{
    bt_dev_t *dev;
    dev = &devs_cfg.tws_paired_list;
    TWS_DBG_PRINTF("tws dsicon acl!\n");
    bt_disconnect_acl(dev);
}

uint8 TWS_reconnect_last_phone_when_disconnect(void)
{
    bt_dev_t *p_autocon_dev;

    //get last connect phone info
    tws_reconnect_lastphone_init(NULL);
    p_autocon_dev = RmtFindDevInAutoConnectList();
    if (p_autocon_dev == NULL)
    {
        return 0;
    }
    else if (p_autocon_dev == RmtFindDevInLinkedList())        //make sure dev is not connected
    {
        return 0;
    }
    else
    {
        reconnect_last_device(p_autocon_dev);
    }

    return 1;
}

/*
 设定tws中主箱回连手机端的规则
 在line in tws场景下，如果主箱发起对手机的回连，会导致播歌卡音，
 所以需要在某些场景下禁止主箱回连手机，并且如果已发起连接，需要主动退出连接。
 在离开这些场景后，规则恢复，此时如果在场景中有发生过回连，则认为此时主箱需要
 回连手机，则会在离开场景后主动发起对手机的回连。
 cancel_cur_create_connection 表示是否需要强制退出当前回连 1：需要 0：不需要
 close_autoconnect            表示是否关闭对手机自动回连 1：需要 0：不需要
 delay_reconn                 表示是否需要在离开场景后主动发起回连 1：需要 0：不需要
 */
uint8 app_set_tws_dev_reconnect_phone_policy(tws_dev_reconn_phone_policy_t *par)
{
    tws_status_info.tws_paired_src_reconn_phone_enable = par->tws_paired_src_reconn_phone_enable;

    if (par->tws_paired_src_reconn_phone_enable == 1)
    {
        TWS_DBG_PRINTF("disable tws src box auto reconnect phone\n");
    }
    else
    {
        TWS_DBG_PRINTF("enable tws src box auto reconnect phone\n");
    }

    //设定策略后，如果需要强制退出回连并且已组队的主箱正在回连手机时，需要强制退出此次回连
    if (par->cancel_cur_create_connection == 1)
    {
        if ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 1))
        {
            if (bt_linking_flag == TRUE)
            {
                bt_dev_t *p_autocon_dev;
                p_autocon_dev = RmtFindDevInAutoConnectList();
                if (p_autocon_dev != NULL)
                {
                    //由于没有判断是否是该手机主动发起的连接，所以有可能把该手机发起的回连也给退出了
                    cancel_create_connection(p_autocon_dev);
                    TWS_DBG_PRINTF("force cancel tws src box create connection with phone\n");
                }
            }
        }
    }

    return TWS_DEV_RECONNECT_PHONE_OK;
}

uint8 app_message_deal_tws(private_msg_t *pri_msg)
{
    uint8 rst;

    rst = 0;

    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_TWS_LINK_SYNC:
        TWS_SearchMatchingDev();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_TWS_FORCE_UNLINK_SYNC:
        TWS_DisconnectAclLink();
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_TWS_SET_SCAN_MODE_SYNC:
        {
            uint8 scan_mode = pri_msg->msg.content.data[0];
            if (scan_mode == 0)
            {
                bt_RestoreNormalScanMode();
            }
            else
            {
                bt_SetFasterScanMode();
            }
        }
        break;

        case MSG_BTENGINE_TWS_GET_BT_CBK_SYNC:
        {
            bt_clock_cbk_t *p_bt_clock_cbk = (bt_clock_cbk_t *) (pri_msg->msg.content.addr);

            p_bt_clock_cbk->tws_get_bt_clk = tws_get_bt_clk_inner;
            p_bt_clock_cbk->tws_set_bt_clk = tws_set_bt_clk_inner;
            p_bt_clock_cbk->tws_reg_bt_clk_sync_cbk = tws_register_bt_clk_sync_cbk_inner;
            p_bt_clock_cbk->tws_has_avdtp_pkg_lost = tws_has_avdtp_pkg_lost_inner;

            btengine_reply_msg(pri_msg, TRUE);
        }
        break;

        case MSG_BTENGINE_TWS_GET_SEND_FRAME_CBK_SYNC:
        {
            f_send_frame *pp_f_send_frame = (f_send_frame *) (pri_msg->msg.content.addr);

            *pp_f_send_frame = linein_tws_send_one_frame;

            btengine_reply_msg(pri_msg, TRUE);
        }
        break;

        case MSG_BTENGINE_SET_TWS_DEV_RECONNECT_PHONE_POLICY_SYNC:
        {
            tws_dev_reconn_phone_policy_t *par = pri_msg->msg.content.addr;
            app_set_tws_dev_reconnect_phone_policy(par);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}
void tws_reconnect_lastphone_init(bt_dev_t *dev)
{
    uint8 idx, cur_idx, temp_mru;
    bt_dev_t *p_dev;
    if (dev != NULL)
    {
        TWS_PRINTF_WARNNING("reconnect device exist\n");
        RmtRemoveDevice(dev);
    }
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
        btengine_global_variable.autoconnect_dev_list[temp_mru - 1] = p_dev;
        if (p_dev->p_dev_status == NULL)
        {
            p_dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
        }
        libc_memset(p_dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
        p_dev->p_dev_status->need_auto_connect = p_dev->profile & (uint8) (~(LINKKEY_VALID | PROFILE_VALID));
    }
    else
    {
        already_start_connect_phone_flag = TRUE;
    }
}
#endif
