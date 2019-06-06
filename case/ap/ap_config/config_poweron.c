/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机应用检测开机选项，确定开机第一个应用。
 * 作者：cailizhen
 ********************************************************************************/

#include "ap_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  过滤开机的消息，同时对消息进行处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_poweron_option(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    uint8 onoff_hit, i;
    uint8 hot_det_max_cnt;
    uint32 start_hot_det_timer;

    //看门狗溢出唤醒，一般是ESD导致代码跑飞异常reset
    if ((g_config_poweron_info.wakeup_para.wdog == 1)
        && (com_get_config_default(SETTING_SYS_RESTART_BREAKPOINT_ENABLE) == TRUE)
        && (g_p_esd_bk_info->flag == ESD_BK_FLAG) && (g_p_esd_bk_info->app_id != APP_ID_CONFIG))
    {
        libc_printf_info("ESD RESET TO:%d\n", g_p_esd_bk_info->app_id);
        g_p_esd_bk_info->reset_flag = 1;
    }
    else
    {
        g_p_esd_bk_info->reset_flag = 0;
    }

    start_hot_det_timer = sys_get_ab_timer();
    libc_printf_info("START HOT @%d\n", start_hot_det_timer);

    hot_det_max_cnt = 5; //20*5=100ms
    if (com_ap_switch_check(APP_FUNC_PLAYUHOST) == TRUE)
    {
        hot_det_max_cnt = 20; //20*20=400ms
    }

    for (i = 0; i < hot_det_max_cnt; i++)
    {
        sys_hot_detect_handle();

        while (get_app_msg(THIS_APP_TYPE, &private_msg) == TRUE)
        {
            libc_printf("poweron msg:0x%x\n", private_msg.msg.type);

            if ((private_msg.msg.type == MSG_SD_IN) || (private_msg.msg.type == MSG_UH_IN))
            {
                clear_fs_vram_info();
                if (private_msg.msg.type == MSG_UH_IN)
                {
                    libc_printf("UHOST IN @%d\n", sys_get_ab_timer());
                }
            }

            if (private_msg.msg.type == MSG_RTCALARM)
            {
                g_config_poweron_info.wakeup_para.alarm = 1;
            }

            if (private_msg.msg.type == MSG_LOW_POWER)
            {
                result = RESULT_LOW_POWER;
                break;
            }
        }

        if (i >= 5)
        {
            if ((get_uhost_state() == UHOST_IN)
                || (get_cable_state() == CABLE_STATE_CABLE_IN)
                || (get_adaptor_state() == ADAPTOR_STATE_CABLE_IN))
            {
                break;
            }

            if ((sys_get_ab_timer() - start_hot_det_timer) > 400) //max 400ms
            {
                break;
            }
        }

        sys_os_time_dly(2);
    }

    libc_printf("QUIT HOT @%d\n", sys_get_ab_timer());

    //允许usb检测初始化时reset控制器，在此之前，USB/Uhost检测切换不用延时；在此之后，切换到Uhost必须延时150ms以上
    hal_set_usb_detect(USB_DETECT_EN_RESET);

    //如果是ESD RESET则不要进行其他规则检测
    if (g_p_esd_bk_info->reset_flag == 1)
    {
        return RESULT_ESD_RESET;
    }

    //检测开机按键，并过滤
    while (1)
    {
        onoff_hit = hal_pmu_read_onoff_key();
        if (onoff_hit == FALSE)
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            com_filter_key_hold();
            break;
        }

        sys_os_time_dly(1);
    }

#if (SUPPORT_UHOST_UPGRADE == 1)
    if (get_uhost_state() == UHOST_IN)
    {
        result = config_upgrade_entry(DISK_U);
        if (result == RESULT_ENTER_UHOST_UPGRADE)
        {
            return result;
        }
    }
#endif

    if (get_card_state() == CARD_STATE_CARD_IN)
    {
#if (SUPPORT_CARD_UPGRADE == 1)
        result = config_upgrade_entry(DISK_H);
        if (result == RESULT_ENTER_CARD_UPGRADE)
        {
            return result;
        }
#endif

        if ((uint8) com_get_config_default(SETTING_AUTOTEST_IN_CARD_ENABLE) == 1)
        {
            result = config_autotest_entry(DISK_H);
            if (result == RESULT_TESTAP_CARD_ENTER)
            {
                return result;
            }
        }
    }

#ifdef FCC_TEST_MODE
    {
        uint32 *p_bqb_flag = (uint32 *)(ESD_BK_INFOR_ADDR + 0x4c);

        extern void config_fcc_test_entry(void);

        if (*p_bqb_flag != 0x12345678)
        {
            config_fcc_test_entry();
        }
        *p_bqb_flag = 0;
    }
#endif

    //检查是否开机进入DUT测试模式
    com_enter_duttest_check();

    if (result != RESULT_LOW_POWER)
    {
#if (SUPPORT_TIME_ALARM == 1)
        if (g_config_poweron_info.wakeup_para.alarm == 1)
        {
            result = RESULT_ENTER_ALARM;
        }
        else
#endif
        {
            if (get_cable_state() == CABLE_STATE_CABLE_IN)
            {
                //带USB数据线开机进入读卡器模式
                if (com_ap_switch_check(APP_FUNC_UDISK) == TRUE)
                {
                    result = RESULT_USB_TRANS;
                }
                //如果不支持读卡器模式，但是支持空闲充电模式，那么则进入空闲充电模式
                else if (com_get_config_default(SETTING_APP_SUPPORT_CHARGE_LOOP) == 1)
                {
                    result = RESULT_ENTER_CHARGE_LOOP;
                }
            }
            //带USB充电适配线开机进入空闲充电模式
            else if (get_adaptor_state() == ADAPTOR_STATE_CABLE_IN)
            {
                //带USB充电线开机可进入充电空闲模式，以支持充电指示灯控制
                if (com_get_config_default(SETTING_APP_SUPPORT_CHARGE_LOOP) == 1)
                {
                    result = RESULT_ENTER_CHARGE_LOOP;
                }
            }
        }
    }
    else
    {
        if (g_config_poweron_info.wakeup_para.alarm == 1)
        {
            libc_printf_info("LOWPOWER ALARM AUTO SNOOZE!\n");
            com_rtc_update_alarm(ALARM_UPDATE_SNOOZE);
        }
    }

    return result;
}

void config_poweron_dispatch(app_result_e result)
{
    bool from_power_off = FALSE;

    //等待 IC PA 初始化完成
    while (get_ic_pa_inited() == FALSE)
    {
        sys_os_time_dly(1);
    }

    //初始化PA Volume，作为系统音量
    com_reset_sound_volume(SET_VOL_BY_SYSTEM);

    if (result == RESULT_ESD_RESET)
    {
        //恢复RESET之前的系统环境，包括音量等
        com_restore_sys_comval_by_reset();
    }
    else
    {
        if ((result == RESULT_POWER_OFF) || (result == RESULT_LOW_POWER) || (result == RESULT_ERROR))
        {
            if (result == RESULT_LOW_POWER)
            {
                //电量不足
                com_tts_state_play(TTS_BATTERY_LOW, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
                sys_os_time_dly(50);
            }
            config_power_off(); //断电关机，不再返回
        }

        if (result == RESULT_ENTER_CHARGE_LOOP)
        {
            //进入充电空闲模式，以支持充电指示灯控制
            result = config_charge_loop(CHARGE_MODE_ON);
            from_power_off = TRUE;
            if ((result == RESULT_LOW_POWER) || (result == RESULT_POWER_OFF))
            {
                config_power_off(); //断电关机，不再返回
            }
        }

        //进入U盘模式也不需要不报“开机”
        //闹钟响应不需要播报“开机”
        if ((result != RESULT_USB_TRANS) &&
            (result != RESULT_ENTER_ALARM) &&
            (result != RESULT_ENTER_CARD_UPGRADE) &&
            (result != RESULT_ENTER_UHOST_UPGRADE))
        {
            com_tts_state_play(TTS_POWERON, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
        }
    }

#if (SUPPORT_RECORD_FUNC == 1)
    //录音断电后文件头恢复
    com_record_recover_record_file();
#endif

    if (result == RESULT_ENTER_ALARM)
    {
        g_p_global_var->alarm_poweron_flag = TRUE;
    }

#if (SUPPORT_HDMI_MODULE == 1)
    if (from_power_off == FALSE)
    {
        hdmi_power_config(TRUE);
    }

    sys_adjust_clk(FREQ_222M, SET_LOCK_CLK);
#endif

    com_ap_switch_deal(result);
}
