/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 组队初始化和断开组队销毁处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

bool com_tws_pair_init(dev_role_e dev_role)
{
    libc_printf_info("tws pair init\n");

    //挂载共享内存
    g_p_tws_ui_m2s_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_UI_M2S);
    if (g_p_tws_ui_m2s_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_UI_M2S mount fail!!\n");
        while (1);
    }

    g_p_tws_ui_s2m_pipe = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_TWS_UI_S2M);
    if (g_p_tws_ui_s2m_pipe == NULL)
    {
        libc_printf_error("SHARE_MEM_ID_TWS_UI_S2M mount fail!!\n");
        while (1);
    }

    if (dev_role == TWS_MASTER)
    {
        //清除收到副箱的按键队列
        sys_mq_flush (MQ_ID_GUI_TEST);

        //主箱支持对箱应用的模式
        if ((get_cur_func_id() == APP_FUNC_BTPLAY) || (get_cur_func_id() == APP_FUNC_PLAYLINEIN))
        {
            uint32 wait_update_btplay_timer = sys_get_ab_timer();

            //等待接收副箱的BTPLAY状态更新，以决定是否进入对箱应用模式
            g_p_global_var->update_btplay_flag = FALSE;
            while (1)
            {
                com_tws_sync_cmd_loop_deal_m();

                if (g_p_global_var->update_btplay_flag == TRUE)
                {
                    break;
                }

                if (sys_get_ab_timer() - wait_update_btplay_timer > 500)
                {
                    libc_printf_warning("wait slave update btplay overtime!\n");
                    break;
                }
            }

            //副箱处于BTPLAY模式，可以马上进入对箱应用模式（蓝牙推歌TWS）
            if (g_p_global_var->slave_btplay_flag == TRUE)
            {
                if (get_cur_func_id() == APP_FUNC_BTPLAY)
                {
                    com_tws_m2s_switch_tws_mode_cmd_inner(TWS_MODE_BT_TWS);
                }
                else
                {
                    com_tws_m2s_switch_tws_mode_cmd_inner(TWS_MODE_AUX_TWS);
                }
            }
        }
    }
    else
    {
        g_tws_slave_ui_info.btplay_update = 1;
        g_tws_slave_ui_info.is_btplay = (get_cur_func_id() == APP_FUNC_BTPLAY);
        if (com_tws_s2m_update_btplay_inner(g_tws_slave_ui_info.is_btplay) == TRUE)
        {
            g_tws_slave_ui_info.btplay_update = 2;
            g_tws_slave_ui_info.btplay_timer = sys_get_ab_timer();
        }
    }

    g_com_env_var.dev_role = dev_role;

    return TRUE;
}

bool com_tws_pair_exit(dev_role_e last_dev_role)
{
    libc_printf_info("tws pair exit\n");

    if (g_p_tws_ui_m2s_pipe != NULL)
    {
        flush_tws_cmd_buffer_rw(g_p_tws_ui_m2s_pipe);
        g_p_tws_ui_m2s_pipe = NULL;
    }
    if (g_p_tws_ui_s2m_pipe != NULL)
    {
        flush_tws_cmd_buffer_rw(g_p_tws_ui_s2m_pipe);
    }

    //清除收到副箱的按键队列
    sys_mq_flush (MQ_ID_GUI_TEST);
    g_p_global_var->update_btplay_flag = FALSE;
    g_p_global_var->slave_btplay_flag = FALSE;
    libc_memset(&g_tws_slave_ui_info, 0x00, sizeof(g_tws_slave_ui_info));
    change_tws_mode_inner(TWS_MODE_SINGLE);
    com_btengine_switch_avdtp_source(AVDTP_SOURCE_BT_A2DP);
    com_tws_mode_exit(last_dev_role);

    return TRUE;
}

bool com_tws_mode_init(dev_role_e dev_role)
{
    if (dev_role == TWS_MASTER)
    {
        uint8 cur_volume;

        com_tws_m2s_set_vol_limit_cmd(g_volume_infor.phy_limit_current);
        cur_volume = com_get_sound_volume_inner();
        com_tws_m2s_set_vol_val_cmd(cur_volume);
        g_volume_infor.tws_sync_volume_value = cur_volume; //初始化

#if (TWS_POS_MODE == TWS_POS_MODE_NEW_MASTER_LEFT)
        //初次组队为主箱，或者从副箱变为主箱，默认为左箱
        if (g_com_env_var.dev_role != TWS_MASTER)
        {
            g_com_env_var.spk_pos = TWS_SPK_LEFT;
        }
#elif (TWS_POS_MODE == TWS_POS_MODE_ALWAYS_MASTER_LEFT)
        //主箱始终左箱，不可切换左右箱
        g_com_env_var.spk_pos = TWS_SPK_LEFT;
#endif

        //确定对箱位置
        if (g_com_env_var.spk_pos == TWS_SPK_STEREO)
        {
            change_tws_spk_pos_inner(TWS_SPK_LEFT);
        }
        else
        {
            change_tws_spk_pos_inner(g_com_env_var.spk_pos);
        }
        //并确定副箱的对箱位置，与主箱相反
        if (get_tws_spk_pos_inner() == TWS_SPK_LEFT)
        {
            com_tws_m2s_switch_pos_cmd_inner(TWS_SPK_RIGHT);
        }
        else
        {
            com_tws_m2s_switch_pos_cmd_inner(TWS_SPK_LEFT);
        }
    }
    else
    {
        g_tws_slave_ui_info.bat_val = g_app_info_state.bat_value;
        if (com_tws_s2m_update_bat(g_tws_slave_ui_info.bat_val) == FALSE)
        {
            g_tws_slave_ui_info.bat_val_update = TRUE;
        }

        //SYS CLOCK锁定为SYS_CLK_ON_IDLE
        com_adjust_sys_clk(SYS_CLK_ON_IDLE, TRUE);
    }

    return TRUE;
}

bool com_tws_mode_exit(dev_role_e last_dev_role)
{
    if (last_dev_role == TWS_SLAVE)
    {
        //SYS CLOCK解除锁定
        com_adjust_sys_clk(SYS_CLK_ON_IDLE, FALSE);
    }

    g_volume_infor.tws_sync_volume_flag = TWS_SYNC_VOL_NULL;
    g_app_info_state.slave_bat_value = BATTERY_GRADE_MAX;

    return TRUE;
}

#endif
