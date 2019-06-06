/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "bluetooth_engine.h"
#include "bt_engine_passthrough.h"

const int visualmode_to_index[] =
{ GAP_VM_DISABLE, GAP_VM_NONDISCOV, GAP_VM_DISCOV, GAP_VM_DEFAULT };

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e btengine_reply_msg(void* msg_ptr, bool ret_vals)
 * \回复同步消息
 * \param[in]    void  msg_ptr 消息结构体
 * \param[in]    bool  ret_vals 相关事件处理结果 TRUE 成功 FALSE 失败
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR 忽略
 * \ingroup      mengine_control.c
 * \note  回复消息时，根据ret_vals的结果，设置应答成功或失败
 */
/*******************************************************************************/
void btengine_reply_msg(void *msg_ptr, bool ret_vals)
{
    //消息指针
    private_msg_t *data_ptr = (private_msg_t *) msg_ptr;

    if (ret_vals == TRUE)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //回复同步消息(发送信号量)
    libc_sem_post(data_ptr->sem);

    return;
}

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
void enable_visible(void)
{
    if (btengine_global_variable.global_visible == 0)
    {
        uint8 temp = btengine_global_variable.global_visible;
        btengine_global_variable.global_visible = 1;
        BTEG_DBG_PRINTF("visible %d->%d\n",temp,btengine_global_variable.global_visible);
    }
}

void disable_visible(void)
{
    if (btengine_global_variable.global_visible == 1)
    {
        uint8 temp = btengine_global_variable.global_visible;
        btengine_global_variable.global_visible = 0;
        BTEG_DBG_PRINTF("visible %d->%d\n",temp,btengine_global_variable.global_visible);
    }
}
#endif

/*
 visual_mode:0-不可见，不可连接
 1-不可见，可连接
 2-可见，不可连接
 3-可见，可连接
 */
void bt_set_visual_mode(int visual_mode)
{
    uint8 i;
    BTEG_DBG_PRINTF("set visual_mode 0x%x\n", (uint32 ) visual_mode);
    if (visual_mode == GAP_VM_DEFAULT)
    {
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
        if (btengine_global_variable.global_visible == 0)
        {
            visual_mode = GAP_VM_NONDISCOV;
            BTEG_DBG_PRINTF("visual_mode change to 0x%x by config method %d\n", (uint32) visual_mode,bt_manager_cfg_info.visual_cfg_mode);
        }
#endif
    }
    BTEG_PRINTF("actual visual_mode 0x%x\n", (uint32 ) visual_mode);
    BTSRC_GAP_SetVisualModeA(0, NULL, visual_mode, NULL);
    for (i = 0; i < 4; i++)
    {
        if (visualmode_to_index[i] == visual_mode)
        {
            break;
        }
    }
    if (i >= 4)
    {
        i = 2;
    }
    btengine_global_variable.visual_mode = i;
    current_visual_mode = visual_mode;
}

//根据autoconnect_list中的设备信息进行自动回连
void bt_auto_reconnect(void)
{
    btengine_global_variable.g_cur_rmt_dev = RmtFindDevInAutoConnectList();
    if (btengine_global_variable.g_cur_rmt_dev == NULL)
    {
        BTEG_PRINTF_WARNNING("<boot auto-reconn> no reconnect device\n");
        return;
    }

    //默认不回连
    g_p_info->need_auto_connect = 0;

    if (bt_manager_cfg_info.sp_auto_connect == 0)
    {
        BTEG_PRINTF_WARNNING("<boot auto-reconn> no support reconnect device\n");
        return;
    }

    if (devs_cfg.tag != PAIRED_INFO_TAG)
    {
        BTEG_PRINTF_WARNNING("<boot auto-reconn> no devs_cfg tag err!\n");
        return;
    }

    if (g_forbid_startup_connect == 1)
    {
        BTEG_PRINTF_WARNNING("<boot auto-reconn> forbid startup connect!\n");
        g_forbid_startup_connect = 0;
        return;
    }

    g_p_info->need_auto_connect = btengine_global_variable.g_cur_rmt_dev->p_dev_status->need_auto_connect;
    BTEG_PRINTF("need reconnect profile 0x%x\n", g_p_info->need_auto_connect);
    BTEG_PRINTF("reconnect device name %s\n", (uint8 * ) &btengine_global_variable.g_cur_rmt_dev->dev_info.name);

    btengine_auto_connect();

}
//根据蓝牙地址从配对列表中找出对应的设备进行回连
void reconnect_by_addr(uint8 *bd_addr)
{

    bt_dev_t *dev;
    dev = RmtFindDevByBD(bd_addr);
    if (dev != NULL)
    {
        btengine_global_variable.autoconnect_dev_list[0] = dev;
        if (dev->p_dev_status == NULL)
        {
            dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
        }
        libc_memset(dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
        dev->p_dev_status->need_auto_connect = dev->profile & (uint8) (~(LINKKEY_VALID | PROFILE_VALID));
    }
    bt_auto_reconnect();
}
//回连最近一次使用的设备
void reconnect_last_device(bt_dev_t *dev)
{
    uint8 idx, cur_idx, temp_mru;
    bt_dev_t *p_dev;

    //只有未连接等待状态下，可以发起自动回连
    if (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_WAIT_PAIRED)
    {
        BTEG_PRINTF_WARNNING("bt linked...\n");
        return;
    }

    //仅同时连接一条链路
    if (bt_linking_flag == TRUE)
    {
        BTEG_PRINTF_WARNNING("bt linking...\n");
        return;
    }

    //参数错误，当前还有连接设备信息存在，警告后重置当前连接设备信息
    if (dev != NULL)
    {
        BTEG_PRINTF_WARNNING("reconnect device exist\n");
        RmtRemoveDevice(dev);
    }

    if (devs_cfg.pl_len != 0)
    {
        //查找最近的连接过的一台设备，存入autoconnect_dev_lsit
        idx = cur_idx = 0;
        temp_mru = devs_cfg.paired_list[0].mru_num;
        for (idx = 1; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
        {
            if (devs_cfg.paired_list[idx].data_validity == 0)
            {
                continue;
            }

            if (temp_mru >= devs_cfg.paired_list[idx].mru_num)
            {
                temp_mru = devs_cfg.paired_list[idx].mru_num;
                cur_idx = idx;
            }
        }
        p_dev = &devs_cfg.paired_list[cur_idx];
        btengine_global_variable.autoconnect_dev_list[temp_mru - 1] = p_dev;

        //初始化设备信息
        if (p_dev->p_dev_status == NULL)
        {
            p_dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
        }
        libc_memset(p_dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
        p_dev->p_dev_status->need_auto_connect = p_dev->profile & (uint8) (~(LINKKEY_VALID | PROFILE_VALID));

        //根据autoconnect_dev_lsit信息进行重连
        bt_auto_reconnect();
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
        disable_visible();
#endif

    }
    else
    {
        BTEG_PRINTF_WARNNING("no reconnect device A\n");
    }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    already_start_connect_phone_flag = TRUE;
#endif

}

void app_msg_open_bt_visible(void)
{
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    uint8 cur_dev_status;

    // open global visiable when it's press CONNECT KEY
    if (btengine_global_variable.global_visible == 0)
    {
        cur_dev_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
        //手机未连接时，打开其可见性
        if (cur_dev_status == STATUS_WAIT_PAIRED)
        {
            btengine_global_variable.global_visible = 1;
            bt_set_visual_mode(GAP_VM_DEFAULT);
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if(bt_manager_cfg_info.sp_tws_link == 1)
        {
            //副箱在组队后，即使未连接手机，也不可见不可连接可配对
            if (TWS_GetPairedStatus() == TWS_PAIRED)
            {
                if (tws_status_info.is_src_flag == TRUE)
                {
                    return;
                }

                btengine_global_variable.global_visible = 0;
                bt_set_visual_mode(GAP_VM_PAIR);
            }
            //主副箱在配对时不可见不可连接可配对
            else if (TWS_GetPairedStatus() == TWS_PAIREING)
            {
                btengine_global_variable.global_visible = 0;
                bt_set_visual_mode(GAP_VM_PAIR);
            }
            //主箱在搜索时不可见可连接可配对
            else if (TWS_GetPairedStatus() == TWS_SEARCH)
            {
                btengine_global_variable.global_visible = 0;
                bt_set_visual_mode(GAP_VM_NONDISCOV);
            }
        }
#endif
        if (btengine_global_variable.global_visible == 1)
        {
            BTEG_DBG_PRINTF("global_visible change %d\n",btengine_global_variable.global_visible);
        }
    }
#endif
}

//回连最后一个设备
void app_reconnect_last_device(void)
{
    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL)
            && (btengine_global_variable.g_cur_rmt_dev->p_dev_status->acl_handle != 0))
    {
        BTEG_PRINTF_WARNNING("device is not disconnect..\n");
    }
    else
    {
        BTEG_PRINTF("manual reconnect\n");
        reconnect_last_device(btengine_global_variable.g_cur_rmt_dev);
    }
}

//断开连接/回连最后一个设备
void app_msg_deal_force_waitpaired(uint8 force_type)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        //回连
        BTEG_DBG_PRINTF("force disconnect in wait paired!\n")
        ;
        bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);
#ifdef bt_auto_test
        //BTEG_PRINTF("manual reconnect\n");
        reconnect_last_device(btengine_global_variable.g_cur_rmt_dev);
        return;
#endif
        break;
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        case STATUS_CALL_INCOMING:
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        BTEG_PRINTF("manual disconnect\n")
        ;
        //断开连接
        BTEG_DBG_PRINTF("force disconnect in linked!\n")
        ;
        bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);
        break;
        case STATUS_NONE:
        g_forbid_startup_connect = 1; //倘若模组还没初始化完成调用该命令，初始化成功后也不回连
        break;
        default:
        break;
    }

    RmtRemoveDevFromAutoConnectList(btengine_global_variable.g_cur_rmt_dev);
    if (CURRENT_DEVICE_ACL == 0)
    {
        RmtRemoveDevice(btengine_global_variable.g_cur_rmt_dev);
        btengine_global_variable.g_cur_rmt_dev = NULL;
    }

    if (force_type != 0)
    {
        if (force_type == 0xFF) /* clear all paired list info. */
        {
            libc_memset(devs_cfg.paired_list, 0, (sizeof(bt_dev_t)) * bt_manager_cfg_info.max_pair_list_num);
            devs_cfg.tag = 0xF0E0;
            devs_cfg.pl_len = 0;
            save_plist_to_vram();
        }
        else if (force_type == 0x1) /* clear current paired list info. */
        {
            libc_memset(&btengine_global_variable.g_cur_rmt_dev, 0, sizeof(bt_dev_t));
            updata_paired_list_length(&devs_cfg);
            save_plist_to_vram();
        }
        else
        {
        }
    }
}

app_result_e app_msg_deal_app_quit(void)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    btengine_global_variable.recv_quit_flag = g_p_info->service_connect | (uint8) FORCE_QUIT_APP_FLAG;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        if (TWS_GetPairedStatus() == TWS_SEARCH)
        {
            TWS_CancelDeviceDiscovery();
        }
        else if (TWS_GetPairedStatus() != TWS_IDLE)
        {
            TWS_DisconnectAclLink();
        }
    }
#endif

    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        BTEG_DBG_PRINTF("app quit,acl disconnect in wait paried!\n")
        ;
        bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);
        case STATUS_NONE:
        case STATUS_ERROR:
        if (bt_linking_flag == FALSE)
        {
            app_ret = RESULT_APP_QUIT;
            break;
        }

        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        case STATUS_CALL_INCOMING:
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        BTEG_DBG_PRINTF("app quit,acl disconnect in linking!\n")
        ;
        bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);

        //        force_quit_linked();
        //        app_ret = RESULT_APP_QUIT;
        break;

        default:
        break;
    }
    //关闭可见性
    bt_set_visual_mode(GAP_VM_NONDISCOV);

    return app_ret;
}

uint8 app_message_deal_btcfg(private_msg_t *pri_msg)
{
    uint8 rst;
    int tmp_visual_mode;

    rst = 0;
    switch (pri_msg->msg.type)
    {
        case MSG_AUTOTEST_RELEASE_CONNECT_SYNC:
        case MSG_BTENGINE_FORCE_WAITPAIRED_SYNC: //强制蓝牙进入等待连接状态
        app_msg_deal_force_waitpaired((uint8) pri_msg->msg.content.data[0]);
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_SET_CONNECT_LAST_SYNC:
        app_reconnect_last_device();
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_STOP_AUTO_CONNECT_SYNC: //有限次自动回连停止
        set_auto_connect_flag(0);
        //g_p_info->isneed_reconnect_flag = FALSE;
        BT_SetReconnectFlag(DEV_TYPE_NONE, FALSE);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_MGR_SET_VISUALMODE_SYNC:
        if ((uint8) pri_msg->msg.content.data[0] > 3)
        {
            btengine_reply_msg(pri_msg, FALSE);
        }
        else
        {
            tmp_visual_mode = visualmode_to_index[(uint8) pri_msg->msg.content.data[0]];
            bt_set_visual_mode(tmp_visual_mode);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        case MSG_AUTOTEST_GET_BTINFO_SYNC:
        btengine_get_btinfo(pri_msg);
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_AUTOTEST_SET_BTINFO_SYNC:
        btengine_set_btinfo(pri_msg);
        break;

        case MSG_AUTOTEST_SET_RECONNECT_SYNC:
        btengine_set_connect_info(pri_msg);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_RF_TEST_SYNC:
        bt_dut_test();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_BQB_TEST_SYNC:
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_OPEN_BT_VISIBLE_SYNC:
        app_msg_open_bt_visible();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SET_CONNECT_ADDR_SYNC:
        {
            uint8 bd_addr[BD_ADDR_LEN];
            libc_memcpy(bd_addr, ((t_bdaddr * ) (pri_msg->msg.content.addr))->bytes, BD_ADDR_LEN);
            reconnect_by_addr(bd_addr);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;

        case MSG_BTENGINE_ENTER_SNIFF_SYNC:
        if (bt_stack_cfg_info.sp_sniff_enable == TRUE)
        {
            //某些应用不可进入sniff模式
            if (btengine_global_variable.disable_bt_sniff_by_app == FALSE)
            {
                bt_enter_sniff(btengine_global_variable.g_cur_rmt_dev);
            }
            else
            {
                BTEG_DBG_PRINTF("disable sniff is %d\n", btengine_global_variable.disable_bt_sniff_by_app);
            }
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_EXIT_SNIFF_SYNC:
        bt_exit_sniff(btengine_global_variable.g_cur_rmt_dev);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_DISABLE_ENTER_BT_SNIFF_SYNC:
        //for disable bt sniff by app
        btengine_global_variable.disable_bt_sniff_by_app = TRUE;
        BTEG_DBG_PRINTF("disable sniff is %d\n", btengine_global_variable.disable_bt_sniff_by_app)
        ;

        //exit sniff mode
        bt_exit_sniff(btengine_global_variable.g_cur_rmt_dev);

        //modify link policy setting to disable bt sniff
        bt_set_link_policy(btengine_global_variable.g_cur_rmt_dev,
                (uint8) (HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH));
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_ENABLE_ENTER_BT_SNIFF_SYNC:
        //release for disable bt sniff by app
        btengine_global_variable.disable_bt_sniff_by_app = FALSE;
        BTEG_DBG_PRINTF("disable sniff is %d\n", btengine_global_variable.disable_bt_sniff_by_app)
        ;

        //modify link policy setting to enable bt sniff
        bt_set_link_policy(btengine_global_variable.g_cur_rmt_dev,
                (uint8) (HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH | HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE));
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SET_VOLUME_SYNC: //设置音量
        {
            btengine_volume_t *set_vol = pri_msg->msg.content.addr;

            //同步音量
            if (set_vol->need_sync == 1) //如果不为1，说明是手机端发起的音量设置，无需再同步回去
            {
                app_msg_deal_volume_sync(set_vol->volume);
            }
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC: //清除音量同步标志
        if (g_p_info->vol_syn_flag != 1)
        {
            g_p_info->vol_syn_flag = 0;
        }
        else if ((uint8) pri_msg->msg.content.data[0] == g_p_info->remote_volume)
        {
            g_p_info->vol_syn_flag = 0;
        }
        else
        {

        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}
void bt_get_paired_list_info(private_msg_t *pri_msg)
{
    uint8 idx;
    bt_dev_t *p_dev;
    bt_paired_info_t *p_dev_info;
    uint8 pd_num;

    p_dev_info = (bt_paired_info_t *) (pri_msg->msg.content.addr);
    p_dev = devs_cfg.paired_list;
    pd_num = 0;
    for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
    {
        if ((p_dev[idx].profile & PROFILE_VALID) != 0)
        {
            libc_memcpy(p_dev_info->dev[pd_num].bd_addr.bytes, p_dev[idx].dev_info.bd_addr.bytes, BD_ADDR_LEN);
            libc_memcpy(p_dev_info->dev[pd_num].name, p_dev[idx].dev_info.name, MGR_MAX_REMOTE_NAME_LEN);
            pd_num++;
        }
    }
    p_dev_info->dev_num = pd_num;
}
void bt_clr_paired_list_info(void)
{
    libc_memset(&devs_cfg.tag, 0x0, sizeof(bt_devs_cfg_t) - 2);
    save_plist_to_vram();
}

void bt_get_cur_active_phone_name(bt_get_phone_name_info_st* p_bt_get_pn_info)
{
    uint8 r_len = MGR_MAX_REMOTE_NAME_LEN + 3;

    //param err
    if (p_bt_get_pn_info == NULL)
    {
        p_bt_get_pn_info->read_status = STATUS_READ_ERROR_PARAM_ERR;
        return;
    }

    if ((p_bt_get_pn_info->phone_name_max_len <= 3) || (p_bt_get_pn_info->phone_name == NULL))
    {
        p_bt_get_pn_info->read_status = STATUS_READ_ERROR_PARAM_ERR;
        return;
    }

    //not exist connected phone device
    if (btengine_global_variable.g_cur_rmt_dev == NULL)
    {
        p_bt_get_pn_info->read_status = STATUS_READ_ERROR_DEV_NOT_EXITS;
        return;
    }

    if (p_bt_get_pn_info->phone_name_max_len < r_len)
    {
        r_len = p_bt_get_pn_info->phone_name_max_len;
    }

    //first three byte means string encode is utf-8
    p_bt_get_pn_info->phone_name[0] = 0xEF;
    p_bt_get_pn_info->phone_name[1] = 0xBB;
    p_bt_get_pn_info->phone_name[2] = 0xBF;

    //libc_memcpy(&p_bt_get_pn_info->phone_name[3],btengine_global_variable.g_cur_rmt_dev->dev_info.name,r_len);
    if (bt_get_remote_phone_name(btengine_global_variable.g_cur_rmt_dev, &p_bt_get_pn_info->phone_name[3]) == FALSE)
    {
        BTEG_PRINTF_ERROR("get cur dev remote name err!\n");
        p_bt_get_pn_info->read_status = STATUS_READ_ERROR_BTEG_GET_REMOTE_NAME_FAILED;
        return;
    }

    p_bt_get_pn_info->phone_name[r_len - 1] = 0;
    //watting app read this info
    p_bt_get_pn_info->read_status = STATUS_READ_SUCCESS;
}

void add_dev_to_paired_list(private_msg_t *pri_msg)
{
    bt_dev_info_t *dev_info;
    bt_dev_t *dev;
    dev_info = (bt_dev_info_t *) (pri_msg->msg.content.addr);
    dev = RmtAddDevice(dev_info->bd_addr.bytes);
    if (dev != NULL)
    {
        dev->profile |= A2DP_CONNECTED;
        dev->profile |= HFP_CONNECTED;
        updata_paired_list_length(&devs_cfg);
        save_rmtdev_info();
        save_plist_to_vram();
    }
    btengine_reply_msg(pri_msg, TRUE);
}
uint8 app_message_deal_info(private_msg_t *pri_msg)
{
    uint8 rst;
    rst = 0;
    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_GET_MEDIA_TYPE_SYNC:
        pri_msg->reply->content = (void *) &btengine_global_variable.media_codec_type;
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_GET_PROFILE_SYNC: //获取本地或已连接设备所支持的服务
        if (pri_msg->msg.content.data[0] == REMOTE_DEVICE)
        {
            pri_msg->reply->content = (void *) &btengine_global_variable.g_cur_rmt_dev->profile;
        }
        else
        {
            pri_msg->reply->content = (void *) &btengine_global_variable.local_device_support_profile;
        }
        //pri_msg->reply->content = (void *) &btengine_global_variable.g_cur_rmt_dev->profile;
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_GET_VISUALMODE_SYNC: //获取当前的可见性，可连接性

        pri_msg->reply->content = (void *) &btengine_global_variable.visual_mode;
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_GET_PAIRED_LIST_SYNC:
        bt_get_paired_list_info(pri_msg);
        btengine_reply_msg(pri_msg, TRUE);
        break;
        case MSG_BTENGINE_CLR_PAIRED_LIST_SYNC:
        {
            clear_plist_info_async_start((uint8) pri_msg->msg.content.data[0]);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        case MSG_BTENGINE_ADD_DEV_TO_PAIRED_LIST_SYNC:
        add_dev_to_paired_list(pri_msg);
        break;
        case MSG_BTENGINE_GET_PB_CONTENT_SYNC:
        {
            pb_data_t * p_pb_content;
            p_pb_content = (pb_data_t *) (pri_msg->msg.content.addr);

            p_pb_content->phone_status = btengine_global_variable.g_btengine_info.phone_status;
            if (p_pb_content->phone_status != 0)
            {
                libc_memcpy(p_pb_content->phonebuf, btengine_global_variable.g_btengine_info.phone_buf, MAX_PHONENUM);
            }

            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        case MSG_BTENGINE_GET_CURRENT_ACTIVE_CONNECTED_PHONE_NAME_SYNC:
        {
            bt_get_cur_active_phone_name((bt_get_phone_name_info_st *) (pri_msg->msg.content.addr));
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        default:
        rst = 1;
        break;
    }

    return rst;
}

uint8 app_message_deal_misc(private_msg_t *pri_msg)
{
    uint8 rst;

    rst = 0;

    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_CLEAR_ERR_SYNC:
        set_cur_device_status(btengine_global_variable.g_cur_rmt_dev, g_status_bak);
        g_p_info->err_status = BT_ERR_NONE;
        //g_btengine_info.status = g_status;
        //g_btengine_info.err_status = BT_ERR_NONE;
        btengine_reply_msg(pri_msg, TRUE);
        break;

        /* for restart incoming-call-ring after tts-pb-once.   asyn msg, not need to reply!!!! */
        case MSG_BTENGINE_RESTART_INCOMING_CALL_RING:
        btengine_reply_msg(pri_msg, TRUE);
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}

app_result_e app_message_deal_enter_s2(private_msg_t *pri_msg)
{
    os_event_t *standby_sem = (os_event_t *) (pri_msg->msg.content.addr);

    //备份现场，准备进入睡眠模式

    //回复前台，然后接着跑就会睡眠
    btengine_reply_msg(pri_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //从睡眠模式回来，恢复现场
    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

app_result_e app_message_deal(private_msg_t *pri_msg)
{
    app_result_e app_ret;
    uint8 rst = 1;

    app_ret = RESULT_NULL;

#if (SUPPORT_ONLY_BT_TEST == 1)
    if ((g_p_info->status == STATUS_CALL_INCOMING)
            || (g_p_info->status == STATUS_CALL_OUTGOING)
            || (g_p_info->status == STATUS_CALL_HFP)
            || (g_p_info->status == STATUS_CALL_PHONE)
            || (g_p_info->status == STATUS_SIRI))
    {
        if (g_p_info->enter_hfp_flag == FALSE)
        {
            g_p_info->enter_hfp_flag = TRUE;
            hfp_thread_init();
        }
    }
    else
    {
        if (g_p_info->enter_hfp_flag == TRUE)
        {
            hfp_thread_exit();
            g_p_info->enter_hfp_flag = FALSE;
        }
    }
#endif

    //查询蓝牙引擎状态
    if (pri_msg->msg.type == MSG_BTENGINE_GET_STATUS_SYNC)
    {
        //防止BTPLAY/BTCALL引擎在状态更新一半时来读取 BT Manager 的 bt_info ，所以必须加锁
        sys_os_sched_lock();
        libc_memcpy((btengine_info_t * ) (pri_msg->msg.content.addr), g_p_info, sizeof(btengine_info_t));
        sys_os_sched_unlock();
        btengine_reply_msg(pri_msg, TRUE);
        return RESULT_NULL;
    }

    rst = app_message_deal_a2dp(pri_msg);
    rst &= app_message_deal_avrcp(pri_msg);
    rst &= app_message_deal_hfp(pri_msg);
    if (bt_manager_cfg_info.sp_hid == 1)
    {
        rst &= app_message_deal_hid(pri_msg);
    }
    rst &= app_message_deal_btcfg(pri_msg);
    rst &= app_message_deal_info(pri_msg);
    rst &= app_message_deal_misc(pri_msg);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        rst &= app_message_deal_tws(pri_msg);
    }
#endif
    rst &= app_message_deal_passthrough(pri_msg);
    rst &= app_message_deal_bqb(pri_msg);
    if (rst == 1)
    {
        switch (pri_msg->msg.type)
        {
            case MSG_SYSTEM_ENTER_S2_SYNC:
            app_ret = app_message_deal_enter_s2(pri_msg);
            break;

            case MSG_APP_QUIT: //退出引擎
            //BTEG_PRINTF("quit btengine\n");
            app_ret = app_msg_deal_app_quit();
            break;

            default:
            BTEG_PRINTF("invalid msg 0x%x\n", pri_msg->msg.type)
            ;
            btengine_reply_msg(pri_msg, FALSE);
            break;
        }
    }

    return app_ret;
}

