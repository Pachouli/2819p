/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话前台应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

/*全局变量定义*/

uint8 last_disp_status = STATUS_NONE;
uint8 g_disp_status = STATUS_NONE;
bool last_sco_created_flag;

btengine_info_t g_btcall_bt_info;

btcall_info_t g_btcall_info;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

bool g_chf_cag_exch_enable;

bool g_speech_play_flag;

bool g_speech_peq_flag;

void *callring_handle;
callring_status_e g_cr_status;
uint32 g_cr_timer;
bool callring_wait_sco_flag; //来电后等待2S钟看看有无SCO来电铃声，有就不用播放默认来电铃声了
uint32 g_wait_sco_timer; //来电后等待2S计时器；也就是刚进入通话场景的时间戳
uint8 ignore_default_ring_flag; //忽略播放默认铃声的标记
uint8 callin_hint_mode; //来电提示模式
uint8 callring_space_time; //默认来电铃声间隙时长，单位100ms
uint8 callring_pb_space_time; //电话本播报间隔时间，单位1S
bool g_pb_need_init = TRUE; //是否需要初始化
phonebook_play_status_e g_pb_play_status; //电话本播报状态
uint32 g_pb_break_timer; //等待间隔时间

#if (SUPPORT_ASQT_TEST == 1)

asqt_state_e g_asqt_state = ASQT_STATUS_CONFIG;
PC_curStatus_e g_pc_status;
uint32 g_pc_state_timer;
uint8 g_pc_state_count;
uint8 g_pc_pa_volume = 0xff;

asqt_configs_t g_asqt_configs;
asqt_dump_buf_t g_asqt_dump_buf;
asqt_dump_tag_t g_asqt_dump_tag;
buffer_rw_t *p_sco_input_buf;
uint16 g_dump_data_index;

buffer_rw_t g_sco_in_cache;
bool need_load_full;

asqt_config_t g_asqt_config;
uint8 g_asqt_config_count;

#endif

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
void _read_var(void)
{
    g_chf_cag_exch_enable = (uint8) com_get_config_default(BTCALL_FRONT_CHF_CAG_EXCH_ENABLE);
    callin_hint_mode = (uint8) com_get_config_default(BTCALL_CALLIN_HINT_MODE);
    callring_space_time = (uint8) com_get_config_default(BTCALL_CALLIN_RING_SPACE_TIME);
    callring_pb_space_time = (uint8) com_get_config_default(BTCALL_CALLIN_PB_SPACE_TIME);
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
void _app_init(void)
{
    /*读入VM变量*/
    _read_var();

    /*初始化applib库，前台AP*/
    applib_init(APP_ID_BTCALL, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

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
    //退出应用时确保采样率为44KHz
    set_dac_rate(44);

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

void _create_btcall_engine(void)
{
    msg_apps_t msg;

    //无消息内容
    msg.para[0] = APP_ID_BTCALL_EG;
    msg.para[1] = 0;
    msg.content.addr = NULL;

    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
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
    uint32 wait_status_update_timer;
    bool is_asqt_mode = FALSE;

    /*ap初始化*/
    _app_init();

    _kill_conflict_engine();

    //接通电话，进入通话场景
    com_set_sound_volume_mode(VOLUME_MODE_CALL, VOLUME_GAIN_CALL);

    com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    com_speaker_on(SPK_PLAY);
    change_app_state (APP_STATE_PLAYING);
    //关闭TTS播报
    //NOTE!!不能开启TTS，否则会和默认来电铃声及电话本播报相冲突!!
    com_tts_state_play_control(TTS_CTRL_IGNORE);

#if (SUPPORT_ASQT_TEST == 1)
    //加载BT ENGINE
    if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
    {
        //关闭按键音
        keytone_set_on_off (FALSE);

        com_creat_bt_engine(0);
        sys_os_time_dly(300); //等待3S钟蓝牙稳定下来
        is_asqt_mode = TRUE;
    }
#endif

    //创建 BTCALL 引擎
    _create_btcall_engine();

    result = get_message_loop();

#if (SUPPORT_ASQT_TEST == 1)
    //重启BT ENGINE
    if (is_asqt_mode == TRUE)
    {
        _kill_conflict_engine();
        com_close_bt_engine();
        com_creat_bt_engine(0);
        sys_os_time_dly(300); //等待3S钟蓝牙稳定下来
    }
#endif

    //恢复TTS播报
    com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
    change_app_state (APP_STATE_NO_PLAY);
    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    //断开连接需要播报“断开连接”TTS
    wait_status_update_timer = sys_get_ab_timer();
    while (1)
    {
        if ((sys_get_ab_timer() - wait_status_update_timer) >= 250)
        {
            break;
        }

        com_get_btengine_status(&g_btcall_bt_info);
        if (g_btcall_bt_info.status == STATUS_WAIT_PAIRED)
        {
            //语音播报蓝牙断开&等待连接
            tts_play_ret_e temp_tts_ret;
            temp_tts_ret = com_tts_state_play(TTS_BT_DISCON, 0);
            if ((temp_tts_ret == TTS_PLAY_RET_BY_KEY) || (temp_tts_ret == TTS_PLAY_RET_BY_SYS))
            {
                //如果播报“蓝牙断开”被终止了，那么等待连接就不再播报了
                break;
            }
            com_tts_state_play(TTS_WAIT_BT_CON, 0);
            break;
        }
    }

    com_ap_switch_deal(result);

    _app_exit();

    return 0;
}
