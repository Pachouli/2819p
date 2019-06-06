/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙管理器LOOP，进行蓝牙事件处理，还会主动检测电池电量变化并上报，音量同步等。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"

extern uint8 com_get_sound_volume_inner(void);
extern uint8 get_bat_value_inner(void);

#define Volume_Delay_Update_Time 4000

#define BATTERY_REPORT_MAX (9)
#define BATTERY_REPORT_MIN (0)

/* com读取手机当前时间接口 */
void com_read_phone_clock_req(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_READ_PHONE_CLOCK_REQ_SYNC;
    send_sync_msg_inner(APP_ID_BTENGINE, &msg, NULL, 0);
}

/* com读取手机当前时间接口*/
void com_read_phone_clock(bt_read_phone_time_t *reply)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_READ_PHONE_CLOCK_SYNC;
    msg.content.addr = reply;
    send_sync_msg_inner(APP_ID_BTENGINE, &msg, NULL, 0);
}

void com_bt_battery_report(uint8 mode, uint8 bat_val)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_REPORT_BAT_SYNC;
    msg.content.data[0] = mode;

    if (mode == 1) //使能电池电量上报
    {
        uint32 cur_value;

        cur_value = bat_val * (BATTERY_REPORT_MAX - BATTERY_REPORT_MIN) / BATTERY_GRADE_MAX + BATTERY_REPORT_MIN;

        msg.content.data[1] = cur_value;
    }

    send_sync_msg_inner(APP_ID_BTENGINE, &msg, NULL, 0);
}
void com_kill_timer(int* timer_id)
{
    int tmp_id;
    tmp_id = (int) *timer_id;
    if (tmp_id != -1)
    {
        kill_app_timer_inner(APP_TIMER_GROUP_COMMON, tmp_id);
        *timer_id = -1;
    }
}

void send_vol_to_phone_do(uint8 phone_volume)
{
    msg_apps_t msg;
    btengine_volume_t set_vol;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SET_VOLUME_SYNC;
    //存放输入参数的地址
    set_vol.volume = phone_volume;
    set_vol.need_sync = 1;
    msg.content.addr = &set_vol;
    //发送同步消息
    send_sync_msg_inner(APP_ID_BTENGINE, &msg, NULL, 0);
}

//发送命令给bt engine同步更新当前音量到远端
//mode:0-更新a2dp，1-更新hFP
void send_vol_to_phone(uint8 mode, uint8 g_volume)
{
    uint8 tmp_vol;
    if (mode == 0)
    {
        tmp_vol = ((g_volume * AVRCP_VOLUME_MAX) / VOLUME_VALUE_MAX);
        //先降一级音量，再设置当前音量主要是为了规避在同步音量时,
        //虽然音量已经同步过去，但是进度条并没有同步更新的问题
        if (tmp_vol < AVRCP_VOLUME_MAX)
        {
            send_vol_to_phone_do(tmp_vol+1);
        }
        else
        {
            send_vol_to_phone_do(tmp_vol-1);
        }
        send_vol_to_phone_do(tmp_vol);
    }
    else
    {
        tmp_vol = (g_volume * HFP_VOLUME_MAX) / VOLUME_VALUE_CALL_MAX;
        send_vol_to_phone_do(tmp_vol);
    }
}

void a2dp_volume_timer_proc(void)
{
    uint8 cur_volume = com_get_sound_volume_inner();

    com_kill_timer(&bt_a2dp_info.a2dp_timer_id);
    if ((bt_info.status == STATUS_A2DP_PLAY) &&
            ((bt_info.service_connect & AVRCP_CONNECTED_MY) != 0))
    {
        libc_printf("\n a2dp vol Update %d\n", cur_volume);

        send_vol_to_phone(0, cur_volume);
    }

    bt_a2dp_info.a2dp_start_flag = 2;
}

void a2dp_volume_delay_update(void)
{
    com_kill_timer(&bt_a2dp_info.a2dp_timer_id);
    bt_a2dp_info.a2dp_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL,
    Volume_Delay_Update_Time, a2dp_volume_timer_proc);
}

void a2dp_state_deal(uint8 a2dp_state)
{
    if (bt_a2dp_info.avrcp_volume_sync_enable == TRUE)
    {
        if (a2dp_state == STATUS_A2DP_PLAY)
        {
            if (bt_a2dp_info.a2dp_start_flag == 0)
            {
                bt_a2dp_info.a2dp_start_flag = 1;
                a2dp_volume_delay_update();
            }
        }
        else
        {
            bt_a2dp_info.a2dp_start_flag = 0;
            com_kill_timer(&bt_a2dp_info.a2dp_timer_id);
        }
    }
}

void hfp_volume_timer_proc(void)
{
    uint8 cur_volume = com_get_sound_volume_inner();

    //只有打出电话或正在通电话时才进行音量同步
    //否则会出现没有真正同步到音量的情况
    //在来电时，进行音量同步是无效的
    if((bt_info.status==STATUS_CALL_OUTGOING)||(bt_info.status==STATUS_CALL_HFP))
    {
        com_kill_timer(&bt_hfp_info.hfp_timer_id);
        if ((bt_info.enter_hfp_flag == 1) &&
            ((bt_info.service_connect & HFP_CONNECTED) != 0))
        {
            libc_printf("\nhfp vol ap->phone %d", cur_volume);
            send_vol_to_phone(1, cur_volume);
        }
    }

    bt_hfp_info.hfp_start_flag = 2;
}

void hfp_vol_delay_update(void)
{
    com_kill_timer(&bt_hfp_info.hfp_timer_id);
    bt_hfp_info.hfp_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 2000, hfp_volume_timer_proc);
}

void hfp_state_deal(uint8 hfp_flag)
{
    if (bt_hfp_info.hfp_volume_sync_enable == TRUE)
    {
        if (hfp_flag == 1)
        {
            if (bt_hfp_info.hfp_start_flag == 0)
            {
                bt_hfp_info.hfp_start_flag = 1;
                hfp_vol_delay_update();
            }
        }
        else
        {
            bt_hfp_info.hfp_start_flag = 0;
            com_kill_timer(&bt_hfp_info.hfp_timer_id);
        }
    }
}
//同步音箱端音量到远端
void com_syn_vol_to_phone(uint8 volume)
{

    if ((bt_info.status == STATUS_A2DP_PLAY)
        && (com_get_config_default_inner(BTMANAGER_ENABLE_AVRCP_VOLUME_SYNC) == 1))
    {
        send_vol_to_phone(0, volume);
    }
    if ((bt_info.enter_hfp_flag == 1)
        && (com_get_config_default_inner(BTMANAGER_ENABLE_HFP_VOLUME_SYNC) == 1))
    {
        send_vol_to_phone(1, volume);
    }
}

void check_hfp_ap_switch(void)
{
    if (get_cur_app_id() != APP_ID_BTCALL)
    {
        if ((bt_info.status == STATUS_CALL_INCOMING)
            || (bt_info.status == STATUS_CALL_OUTGOING)
            || (bt_info.status == STATUS_CALL_HFP)
            || (bt_info.status == STATUS_CALL_PHONE)
            || (bt_info.status == STATUS_SIRI))
        {
            uint16 msg_type = MSG_SWITCH_FRONT_APP;
            sys_mq_send(MQ_ID_HOTPLUG, &msg_type);
        }
    }
}
//电池电量同步到远端设备处理
void sync_battery_deal(void)
{
    uint32 cur_timer;
    if ((bt_info.status == STATUS_WAIT_PAIRED) || (bt_info.status == STATUS_NONE)\

            || ((bt_info.service_connect & HFP_CONNECTED) == 0))
    {
        if (g_com_bt_env.at_bat_value != 0xff)
        {
            g_com_bt_env.at_bat_value = 0xff;
            g_com_bt_env.at_bat_status = 0;
            g_com_bt_env.at_timer = 0;
            g_com_bt_env.hfp_linked = 0;
        }
        return;
    }
    cur_timer = sys_get_ab_timer();
    if (g_com_bt_env.hfp_linked == 0)
    {
        g_com_bt_env.hfp_linked = 1;
        g_com_bt_env.at_bat_status = AT_BAT_NULL;
    }
    //电池电量上报
    if (g_com_bt_env.hfp_linked == 1)
    {
        if (g_com_bt_env.at_bat_status == AT_BAT_NULL)
        {
            com_bt_battery_report(0, 0);
            g_com_bt_env.at_bat_status = AT_BAT_ENABLE;
            g_com_bt_env.at_timer = cur_timer;
        }
        else if (g_com_bt_env.at_bat_status == AT_BAT_ENABLE)
        {
            if ((cur_timer - g_com_bt_env.at_timer) >= 1000)
            {
                g_com_bt_env.at_bat_status = AT_BAT_UPDATE;
            }
        }
        else
        {
            uint8 bat_value = get_bat_value_inner();

            if (bat_value != g_com_bt_env.at_bat_value)
            {
                g_com_bt_env.at_bat_value = bat_value;

                com_bt_battery_report(1, g_com_bt_env.at_bat_value);
            }
        }
    }
}
void com_bt_event_loop_inner(void)
{
    uint16 msg_type;

    com_get_btengine_status_inner(NULL);

#if (SUPPORT_ONLY_BT_TEST == 0)
    check_hfp_ap_switch();
#endif
    hfp_state_deal(bt_info.enter_hfp_flag);
    a2dp_state_deal(bt_info.status);
    sync_battery_deal();

    if ((g_com_bt_env.ble_connect_flag == FALSE) && (bt_info.ble_connect_flag == TRUE))
    {
        bt_manager_blespp_data_trans_create_ble_profile();
    }
    else if ((g_com_bt_env.ble_connect_flag == TRUE) && (bt_info.ble_connect_flag == FALSE))
    {
        bt_manager_blespp_data_trans_destroy_ble_profile();
    }
    g_com_bt_env.ble_connect_flag = bt_info.ble_connect_flag;

    if (((g_com_bt_env.service_connect_status & SPP_CONNECTED) == 0)
            && ((bt_info.service_connect & SPP_CONNECTED) != 0))
    {
        bt_manager_blespp_data_trans_create_spp_profile();
    }
    else if (((g_com_bt_env.service_connect_status & SPP_CONNECTED) != 0)
            && ((bt_info.service_connect & SPP_CONNECTED) == 0))
    {
        bt_manager_blespp_data_trans_destroy_spp_profile();
    }

    bt_manager_blespp_data_trans_deal_loop();

    g_com_bt_env.last_status = bt_info.status;

    //等待获取手机时间
#ifdef SUPPORT_READ_PHONE_TIME
    if (g_com_bt_env.read_phone_clock_flag == TRUE)
    {
        g_com_bt_env.read_phone_clock_count++;
        if (g_com_bt_env.read_phone_clock_count >= 20)
        {
            bt_read_phone_time_t phone_time;
            com_read_phone_clock(&phone_time);
            #if 0
            libc_printf("read phone clock:\n");
            libc_printhex(&phone_time, sizeof(bt_read_phone_time_t), 0);
            #endif
            if ((phone_time.replay == REQ_DATA_INVALID) || (phone_time.replay == BT_NO_READY))
            {

            }
            else
            {
                if (phone_time.replay == RESP_DATA_VALID)
                {
                    //set date & time
                    rtc_set_date(&phone_time.date);
                    rtc_set_time(&phone_time.time);
                }
                else //cmd not support
                {

                }

                g_com_bt_env.read_phone_clock_flag = FALSE;
            }

            g_com_bt_env.read_phone_clock_count = 0;
            g_com_bt_env.read_phone_clock_ot++;
            if (g_com_bt_env.read_phone_clock_ot >= 5)
            {
                libc_printf("read phone clock fail!\n");
                g_com_bt_env.read_phone_clock_flag = FALSE;
            }
        }
    }
#endif

    if (g_com_bt_env.service_connect_status != bt_info.service_connect)
    {
        //蓝牙物理连接状态发生
        msg_type = MSG_BT_CON_CHANGED;
        sys_mq_send(MQ_ID_SYS, &msg_type);

#ifdef SUPPORT_READ_PHONE_TIME
        //HFP刚连接起来就获取一次手机时间
        if (((g_com_bt_env.service_connect_status & HFP_CONNECTED) == 0)
            && ((bt_info.service_connect & HFP_CONNECTED) != 0))
        {
            com_read_phone_clock_req();
            g_com_bt_env.read_phone_clock_flag = TRUE;
            g_com_bt_env.read_phone_clock_count = 0;
            g_com_bt_env.read_phone_clock_ot = 0;
        }
#endif

        g_com_bt_env.service_connect_status = bt_info.service_connect;
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO

    if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
    {
        if (g_volume_infor.tws_sync_volume_flag == TWS_SYNC_VOL_DELAY)
        {
            if ((sys_get_ab_timer() - g_volume_infor.tws_sync_volume_delay_timer) >= TWS_SYNC_VOL_DELAY_TIME)
            {
                if (g_volume_infor.tws_sync_volume_delay != g_volume_infor.tws_sync_volume_value)
                {
                    com_tws_m2s_set_vol_val_cmd(g_volume_infor.tws_sync_volume_delay);
                    g_volume_infor.tws_sync_volume_value = g_volume_infor.tws_sync_volume_delay;
                }
                g_volume_infor.tws_sync_volume_flag = TWS_SYNC_VOL_RIGHT_NOW;
                g_volume_infor.tws_sync_volume_delay_timer = sys_get_ab_timer();
            }
        }
        else if (g_volume_infor.tws_sync_volume_flag == TWS_SYNC_VOL_RIGHT_NOW)
        {
            if ((sys_get_ab_timer() - g_volume_infor.tws_sync_volume_delay_timer) >= TWS_SYNC_VOL_DELAY_TIME)
            {
                g_volume_infor.tws_sync_volume_flag = TWS_SYNC_VOL_NULL;
            }
        }
    }

    //处理组队角色状态的变化
    change_tws_role_inner(bt_info.dev_role);
    if (bt_info.dev_role != g_com_bt_env.last_dev_role)
    {
        if (bt_info.dev_role != NORMAL_DEV)
        {
            com_tws_pair_init(bt_info.dev_role);
        }
        else
        {
            com_tws_pair_exit(g_com_bt_env.last_dev_role);
        }
    }

    g_com_bt_env.last_tws_pair_status = bt_info.tws_pair_status;
    g_com_bt_env.last_dev_role = bt_info.dev_role;

    //处理 TWS 同步命令事件
    if (get_tws_role_inner() == TWS_MASTER)
    {
        com_tws_sync_cmd_loop_deal_m();
    }
    else if (get_tws_role_inner() == TWS_SLAVE)
    {
        com_tws_slave_send_event_loop_deal();

        com_tws_sync_cmd_loop_deal_s();
    }

#endif
}

//有可能蓝牙马上关闭，common还没来得及完成tws pair exit，所以必须在此确认
void com_bt_check_tws_pair_exit(void)
{
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (NORMAL_DEV != g_com_bt_env.last_dev_role)
    {
        if (get_tws_role_inner() != NORMAL_DEV)
        {
            libc_printf_warning("tws disconnect abnormal!");
        }

        com_tws_pair_exit(g_com_bt_env.last_dev_role);

        change_tws_role_inner(NORMAL_DEV);
        g_com_bt_env.last_dev_role = NORMAL_DEV;
        g_com_bt_env.last_tws_pair_status = TWS_IDLE;
    }
#endif
}

