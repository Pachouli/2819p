/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：读卡器应用入口，会对usb的vid，pid等信息进行设置，定时扫描卡插入拔出状态，
 * 循环处理usb驱动状态及消息。
 * 作者：
 ********************************************************************************/


#include "ap_udevice.h"
#define USB_VID_PID_LENGHTH                6//前两字节存放十六进制标志符0x，后4字节用来表示vid/pid
#define USB_VENDOR_LENGTH                  8
#define USB_PRODUCT_NAME_LENGTH            16
#define USB_INQUIRY_INFORMATION_FOB_LENGTH 52


udisk_status_t g_udisk_status;
uint8 s_support_phy_card;
bool  s_need_sd_icon_twinkle;
bool  s_sd_icon_display;

void card_detect_func(void);
void cardreader_app_timer_handle(void);

/*全局变量定义*/

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

const key_map_t cardreader_key_map_list[] =
{
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_MODE_SWITCH },

    /*! 结束标志 */
    { { KEY_NULL_NO_SHORTCUT, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * 获取USB相关的配置信息
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void get_usb_info(void)
{
    uint8 info_p[52];
    uint8 ret;
    libc_memset(info_p,0x0,52);

    //set vid
    ret=com_get_config_struct(USB_VID, info_p, USB_VID_PID_LENGHTH);
    if(ret!=FALSE)
    {
        udisk_set_usb_data( DEVICE_VID, info_p);
    }

    //set pid
    ret=com_get_config_struct(USB_CARDREADER_PID, info_p, USB_VID_PID_LENGHTH);
    if(ret!=FALSE)
    {
        udisk_set_usb_data(DEVICE_PID,info_p);
    }

     // set manufacture name
    ret=com_get_config_struct(INF_USB_VENDOR, info_p, USB_VENDOR_LENGTH);
    if(ret!=FALSE)
    {
        udisk_set_usb_data(DEVICE_MANUFNAME,info_p);
    }

    //set product name
    ret=com_get_config_struct(INF_CARDREADER_PRODUCT_NAME, info_p, USB_PRODUCT_NAME_LENGTH);
    if(ret!=FALSE)
    {
        udisk_set_usb_data(DEVICE_PRODNAME,info_p);
    }

    //set device fob information
    ret=com_get_config_struct(INF_INQUIRY_INFORMATION_FOB, info_p, USB_INQUIRY_INFORMATION_FOB_LENGTH);
    if(ret!=FALSE)
    {
        udisk_set_usb_data(DEVICE_FOB_INFO,info_p);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 获取私有消息的处理
 * \param[in]    msg MainMenu 的处理的各种私有消息的回调函数
 * \param[out]   none
 * \return       app_result_e
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
app_result_e udevice_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;

    /*同步消息处理*/
    switch (pri_msg->msg.type)
    {
        case MSG_LINEIN_IN:
        //读卡器应用下忽略LINEIN热拔插
        break;

        default:
        ret_result = com_message_box(pri_msg->msg.type);
        break;
    }

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
}

/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e cur_event;
    private_msg_t pri_msg;
    uint32 irq_temp;
    uint8 last_card_rw_flag = 0;
    uint32 card_rw_idle_count = 0;
    //定时器id
    int8 timer_id_t = -1;
    card_state_e last_card_state = get_card_state();

    s_support_phy_card = 0;
#if (SUPPORT_CARD_HARDWARE == 1)
    s_support_phy_card = com_get_config_default(SETTING_HARDWARE_SUPPORT_CARD);
#endif

    timer_id_t = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 200, cardreader_app_timer_handle);

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(udevice_msg_callback);

    while (1)
    {
        if (get_card_state() != last_card_state)
        {
            if (get_card_state() == CARD_STATE_CARD_IN)
            {
                led_display_icon(LCD_SD, TRUE);
                s_sd_icon_display = TRUE;
            }
            else
            {
                led_display_icon(LCD_SD, FALSE);
                s_sd_icon_display = FALSE;
            }
            last_card_state = get_card_state();
            s_need_sd_icon_twinkle = FALSE;
        }

        if (sys_usb_detect() == FALSE)
        {
            result = RESULT_NEXT_FUNCTION;
            break;
        }

        udisk_get_status(&g_udisk_status);

        //1:拔线，2：挂起;3-切换到ADFU状态
        if ((g_udisk_status.usb_state == 0x01) || (g_udisk_status.usb_state == 0x02) || (0 != g_udisk_status.SwitchToAdfuFlag))
        {
            break;
        }

        if (1 == g_udisk_status.install_card_flag) //卡读写出错
        {
            irq_temp = sys_local_intc_save(1<<IRQ_TIMER3);

            udisk_deal_card_out();
            g_udisk_status.install_card_flag = 0;

            sys_local_intc_restore(irq_temp);
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &cur_event, cardreader_key_map_list) == TRUE)
            {
                switch (cur_event)
                {
                    case EVENT_MODE_SWITCH:
                    if (card_rw_idle_count >= 10)
                    {
                        result = RESULT_NEXT_FUNCTION;
                    }
                    break;

                    default:
                    break;
                }
            }
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                result = udevice_msg_callback(&pri_msg);
            }
        }

        if (result == RESULT_ENTER_ALARM)
        {
            libc_printf_info("CARDREADER MODE ALARM AUTO SNOOZE!\n");
            com_rtc_update_alarm(ALARM_UPDATE_SNOOZE);
            result = RESULT_NULL;
        }

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

        if (g_udisk_status.card_rw_flag != 0)
        {
            if (last_card_rw_flag == 0)
            {
                //读写时快闪
                com_led_twinkle_init (LED_ID_STATUS, LED_SCENE_MAIN, PWM_PERIOD_200MS, TK_SEL_PWM);
                if (get_card_state() == CARD_STATE_CARD_IN)
                {
                    s_need_sd_icon_twinkle = TRUE;
                }
            }
            card_rw_idle_count = 0;
        }
        else
        {
            if (last_card_rw_flag != 0)
            {
                //停止读写LED灯常亮
                com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
                if (get_card_state() == CARD_STATE_CARD_IN)
                {
                    led_display_icon(LCD_SD, TRUE);
                    s_sd_icon_display = TRUE;
                }
                else
                {
                    led_display_icon(LCD_SD, FALSE);
                    s_sd_icon_display = FALSE;
                }
                s_need_sd_icon_twinkle = FALSE;
            }
            card_rw_idle_count++;
        }

        last_card_rw_flag = g_udisk_status.card_rw_flag;

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    if (timer_id_t != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, timer_id_t);
        timer_id_t = -1;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*读入VM变量*/
    _read_var();
    libc_memset(&g_udisk_status,0x00,sizeof(udisk_status_t));
    /*初始化applib库，前台AP*/
    applib_init(APP_ID_UDISK, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 2);

}

/******************************************************************************/
/*!
 * \par  Description:
 *      applib退出
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    /*保存VM变量*/
    com_save_sys_comval();

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);
}

/******************************************************************************/
/*!
 * \par  Description:
 *  杀死后台引擎
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
static void udevice_close_engine(void)
{
    msg_apps_t msg;

    if (get_engine_type() != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;

        msg.para[0] = APP_ID_THEENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e result = RESULT_NULL;
    int sys_clk_backup;

    /*ap初始化*/
    _app_init();

    if (usb_is_cable_plug() != FALSE)
    {
        udevice_close_engine();

        //进入USB读卡器应用点亮LED指示灯
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        //LED数码管显示
        led_clear_screen(CLR_ALL);
        led_display(NUMBER1, 'U', TRUE);
        led_display(NUMBER2, 'S', TRUE);
        led_display(NUMBER3, 'B', TRUE);
        if (get_card_state() == CARD_STATE_CARD_IN)
        {
            led_display_icon(LCD_SD, TRUE);
            s_sd_icon_display = TRUE;
            s_need_sd_icon_twinkle = FALSE;
        }

        //ESD RESET 不要播报TTS
        if (g_p_esd_bk_info->reset_flag == 0)
        {
            com_tts_state_play(TTS_ENTER_UDISK, 0);
        }

        //关闭TTS播报和按键音
        com_tts_state_play_control(TTS_CTRL_IGNORE);
        keytone_set_on_off (FALSE);

        change_app_state (APP_STATE_PLAYING);

        sys_clk_backup = com_adjust_sys_clk(FREQ_108M, TRUE);

        //设置卡磁盘卷标
#if (SUPPORT_CARD_HARDWARE == 1)
        if ((com_get_config_default(INF_UDISK_LABEL_ENABLE) != 0)
            && (com_get_config_default(SETTING_HARDWARE_SUPPORT_CARD) != 0))
        {
            set_disk_label();
        }
#endif

        if ((int32) sys_drv_install(DRV_GROUP_USB_DEVICE, 0, "udisk.drv") == -1)
        {
            while (1);
        }

        get_usb_info();

        udisk_device_init();

#if (SUPPORT_CARD_HARDWARE == 1)
        if(com_get_config_default(SETTING_HARDWARE_SUPPORT_CARD) != 0)
        {
            card_detect_func();
        }
#endif

        result = get_message_loop();

        udisk_device_exit();

        sys_drv_uninstall (DRV_GROUP_USB_DEVICE);

        change_app_state (APP_STATE_NO_PLAY);

        com_adjust_sys_clk(sys_clk_backup, FALSE);

        //恢复TTS播报和按键音
        com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
        keytone_set_on_off(TRUE);

        if (g_udisk_status.SwitchToAdfuFlag == 0x01)
        {
            sys_mdelay(200);
            udisk_switch_to_adfu();
        }
        else if (g_udisk_status.SwitchToAdfuFlag == 0x02)
        {
            sys_mdelay(200);
            result = com_stub_tools_connect();
        }
    }
    else
    {
        result = RESULT_NULL;
    }

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    if (result == RESULT_NEXT_FUNCTION) //跳到默认应用
    {
        result = RESULT_NULL;
    }

    //进入U盘之前不播报开机，从U盘退出时需要开机，表示进入音箱功能
    //com_tts_state_play(TTS_POWERON, 0);

    com_ap_switch_deal(result);

    _app_exit();

    //u盘数据更新标志
    //set_need_update_vm(1);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      卡检测函数
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void card_detect_func(void)
{
    uint32 irq_temp = sys_local_intc_save(1<<IRQ_TIMER3);

    if (TRUE == sys_card_detect())
    {
        change_card_state (CARD_STATE_CARD_IN);
        if (g_udisk_status.card_exist_flag == 0)
        {
            udisk_card_cap_check();
        }
    }
    else
    {
        udisk_deal_card_out();
        change_card_state (CARD_STATE_CARD_NULL);
    }

    sys_local_intc_restore(irq_temp);
}

void cardreader_app_timer_handle(void)
{
    if (s_support_phy_card == 1)
    {
        card_detect_func();
    }

    if (s_need_sd_icon_twinkle == TRUE)
    {
        if (s_sd_icon_display == TRUE)
        {
            led_display_icon(LCD_SD, FALSE);
            s_sd_icon_display = FALSE;
        }
        else
        {
            led_display_icon(LCD_SD, TRUE);
            s_sd_icon_display = TRUE;
        }
    }
}
