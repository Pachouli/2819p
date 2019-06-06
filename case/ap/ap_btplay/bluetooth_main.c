/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

/*全局变量定义*/

uint8 last_disp_status = STATUS_NONE;
uint8 g_disp_status = STATUS_NONE;
bool g_need_draw;
int8 display_timer_id = -1;
bool display_flag;
tts_play_ret_e g_tts_enter_ret;
uint8 g_bt_tws_enable;

bool g_esd_reset_ignore_enter_tts_flag;
bool g_esd_reset_ignore_wait_tts_flag;
bool g_esd_reset_ignore_link_tts_flag;
bool g_esd_reset_need_play_flag;

uint8 g_avrcp_autoplay_cnt = 0; //连接成功后自动发送avrcp播放的次数
uint8 g_avrcp_autoplay_flag = 0; //可以发送avrcp play自动播放
uint8 g_avrcp_autoplay_try = 0; //发送avrcp play的次数
uint8 g_send_avrcp_play_tick = 0; //延时2秒发送avrcp play计时
uint8 g_fake_play_tick = 0; //为了过滤某些手机回连时有2S的伪start状态
uint8 g_btplay_tick_250ms = 0;
int8 tick_timer_id = -1;

btengine_info_t g_btplay_bt_info;
btplay_info_t g_btplay_info;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 读取应用VM全局变量,初始化全局变量
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
 *    applib初始化
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

    /*初始化applib库，前台AP*/
    applib_init(APP_ID_BTPLAY, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 4);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    g_bt_tws_enable = FALSE;
    if (com_get_config_default(BTMANAGER_ENABLE_TWS) == TRUE)
    {
        g_bt_tws_enable = TRUE;
    }
#endif

    //ESD RESET 不要播报TTS
    if (g_p_esd_bk_info->reset_flag == 1)
    {
        g_esd_reset_ignore_enter_tts_flag = TRUE;
        g_esd_reset_ignore_wait_tts_flag = TRUE;
        if ((g_p_esd_bk_info->bt_info_state != STATUS_WAIT_PAIRED)
            && (g_p_esd_bk_info->bt_info_state != STATUS_NONE))
        {
            g_esd_reset_ignore_link_tts_flag = TRUE;
        }
        if (g_p_esd_bk_info->play_status == BTPLAY_PLAY)
        {
            g_esd_reset_need_play_flag = TRUE;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib退出
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    com_save_sys_comval();

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);
}

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //查看当前引擎是否为music
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //注销radio后台
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

void _create_btplay_engine(void)
{
    msg_apps_t msg;
    bool tws_mode_tbd = FALSE; //待确定TWS应用模式

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((g_btplay_bt_info.dev_role == TWS_MASTER) && (g_p_global_var->slave_btplay_flag == TRUE))
    {
        tws_mode_tbd = TRUE;
    }
#endif

    //无消息内容
    msg.para[0] = APP_ID_BTPLAY_EG;
    msg.para[1] = (SOURCE_BT_A2DP<<4) | tws_mode_tbd;
    msg.content.addr = NULL;

    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}

app_result_e bt_set_volume(uint8 param)
{
    msg_apps_t msg;
    btengine_volume_t set_vol;

    //消息类型(即消息名称)
    msg.type = MSG_BTENGINE_SET_VOLUME_SYNC;
    //存放输入参数的地址
    set_vol.volume = param;
    set_vol.need_sync = 1;
    msg.content.addr = &set_vol;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return RESULT_NULL;
}

void tts_play_create_btengine_hook(void)
{
    com_creat_bt_engine(0);
}

void init_tts_play_and_create_btengine(void)
{
    #if 1 //在播报TTS启动后装载btengine，能够兼顾到尽快播报TTS和加快蓝牙就绪
    //播报“进入蓝牙”
    enter_bt_tts_play(tts_play_create_btengine_hook);
    //如果没有TTS播报就不会创建bt engine，需要显式调用创建bt engine的接口
    if (get_bt_state() == BT_STATE_NONE)
    {
        com_creat_bt_engine(0);
    }
    #else
    enter_bt_tts_play(NULL);
    com_creat_bt_engine(0);
    #endif
}

void init_tts_play(void)
{
    //播报“进入蓝牙”
    enter_bt_tts_play(NULL);

    if (g_disp_status == STATUS_WAIT_PAIRED)
    {
        bt_tts_play();
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
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
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;

    /*ap初始化*/
    _app_init();

    last_disp_status = STATUS_NONE;

    //确保杀死其他引擎
    _kill_conflict_engine();

    com_set_mute (FALSE); //解除静音

    //如果不支持后台蓝牙，闹钟起闹后需要播报进入蓝牙和等待连接的TTS
    if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK) && (get_bt_state() == BT_STATE_NONE))
    {
        g_p_global_var->bg_app_sta = BG_STATUS_NULL;
    }

    if (g_p_global_var->bg_app_sta == BG_STATUS_BACK)
    {
        g_p_global_var->bg_app_sta = BG_STATUS_NULL;
        com_get_btengine_status(&g_btplay_bt_info);

        last_disp_status = g_disp_status = g_btplay_bt_info.status;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        last_tws_pair_status = g_btplay_bt_info.tws_pair_status;
        last_tws_dev_role = g_btplay_bt_info.dev_role;
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //TWS 副箱刷新显示
        if ((g_btplay_bt_info.tws_pair_status == TWS_PAIRED) && (g_btplay_bt_info.dev_role == TWS_SLAVE))
        {
            g_btplay_info.status = BTPLAY_STOP;
            bt_ui_display_tws(TRUE);
        }
        else
#endif
        {
            bt_ui_display(TRUE);
        }
    }
    else
    {
        last_disp_status = STATUS_NONE;

        //创建 bt engine
        if (get_bt_state() == BT_STATE_NONE)
        {
            bt_ui_display(TRUE);

#if (SUPPORT_FAST_PREVIEW == 0)
            init_tts_play_and_create_btengine();
#endif
        }
        else
        {
            com_get_btengine_status(&g_btplay_bt_info);
            g_disp_status = g_btplay_bt_info.status;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
            last_tws_pair_status = g_btplay_bt_info.tws_pair_status;
            last_tws_dev_role = g_btplay_bt_info.dev_role;
            //解除“禁止回连手机”
            {
                tws_dev_reconn_phone_policy_t param;
                param.tws_paired_src_reconn_phone_enable = 1;
                param.cancel_cur_create_connection = 0;
                com_btengine_connect_deal(CON_DEAL_SET_TWS_RECONN_PHONE_POLICY, &param);
            }
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //TWS 副箱刷新显示
            if ((g_btplay_bt_info.tws_pair_status == TWS_PAIRED) && (g_btplay_bt_info.dev_role == TWS_SLAVE))
            {
                g_btplay_info.status = BTPLAY_STOP;
                bt_ui_display_tws(TRUE);
            }
            else
#endif
            {
                bt_ui_display(TRUE);
            }

#if (SUPPORT_FAST_PREVIEW == 0)
            init_tts_play();
            last_disp_status = g_disp_status;
#endif
        }
    }

#if (SUPPORT_FAST_PREVIEW == 1)
    //等待快速预览模式是否切换到其他应用，如果有切换的话，就马上跳转
    while (1)
    {
        fast_preview_status_e fp_sta;
        fp_sta = com_fast_preview_get_status();
        if (fp_sta != FAST_PREVIEW_STA_NULL)
        {
            if (fp_sta == FAST_PREVIEW_STA_RUNNING)
            {
                need_flush_key = TRUE;
            }
            sys_os_time_dly(1);
        }
        else
        {
            private_msg_t pri_msg;
            input_gui_msg_t input_msg;
            uint8 i;

            if (need_flush_key == TRUE)
            {
                while (get_gui_msg(&input_msg) == TRUE)
                {
                    ; //flush key msg
                }
            }

            for (i = 0; i < 5; i++)
            {
                if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
                {
                    result = bt_message_deal(&pri_msg);
                    if ((result != RESULT_NULL) && (result != RESULT_REDRAW))
                    {
                        need_switch_ap = TRUE;
                        break;
                    }
                }
                else
                {
                    break;
                }

                sys_os_time_dly(1);
            }

            break;
        }
    }

    if (need_switch_ap == TRUE)
    {
        goto fast_switch_ap_exit;
    }

    if (get_bt_state() == BT_STATE_NONE)
    {
        init_tts_play_and_create_btengine();
    }
    else
    {
        init_tts_play();
        last_disp_status = g_disp_status;
    }
#endif

    bt_set_volume(com_get_sound_volume());

    //创建 BTPLAY 引擎
    _create_btplay_engine();

    //获取并更新一次状态，保证 g_btplay_bt_info 有效
    com_get_btengine_status(&g_btplay_bt_info);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //主箱处理对箱应用模式和单箱应用模式切换
    if (g_btplay_bt_info.dev_role == TWS_MASTER)
    {
        if (g_p_global_var->slave_btplay_flag == TRUE)
        {
            tws_switch_tws_mode(TWS_MODE_BT_TWS);
            btplay_tws_conform_mode();
        }
    }
    else if (g_btplay_bt_info.dev_role == TWS_SLAVE)
    {
        //副箱进入BTPLAY模式
        com_tws_slave_update_btplay(TRUE);
    }
#endif

#ifdef DOUBLE_CLICK_KEY_TEST
    com_double_click_enable(TRUE, KEY_F1, KEY_NULL);
#endif

    result = get_message_loop();

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //非关机导致的切换应用
    if ((result != RESULT_POWER_OFF) && (result != RESULT_LOW_POWER))
    {
        //主箱处理对箱应用模式和单箱应用模式切换
        if (g_btplay_bt_info.dev_role == TWS_MASTER)
        {
            if (get_tws_mode() == TWS_MODE_BT_TWS)
            {
                tws_switch_tws_mode(TWS_MODE_SINGLE);
            }
        }
        else if (g_btplay_bt_info.dev_role == TWS_SLAVE)
        {
            //副箱退出BTPLAY模式，需要等待解码停下来并切换为单箱应用模式，才可以退出
            com_tws_slave_update_btplay(FALSE);
        }
    }
#endif

#ifdef DOUBLE_CLICK_KEY_TEST
    com_double_click_enable(FALSE, KEY_NULL, KEY_NULL);
#endif

    fast_switch_ap_exit:

    //create new app
    com_ap_switch_deal(result);

    _app_exit();

    return result;
}
