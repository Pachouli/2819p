/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入前台应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_linein.h"

//globle variable

//当前播放状态信息
lineineg_status_t g_linein_status;
play_status_e g_last_linein_status;
bool g_linein_tws_flag;

uint8 g_linein_play_mode;

#if (SUPPORT_RECORD_FUNC == 1)
uint32 rec_time_bak;//截至上一次暂停录音时间，每次恢复录音，中间件上报的录音时间都是从0开始；暂停录音时更新
uint32 rec_time_cur;//本次恢复录音的时间戳；暂停录音时清0
uint32 rec_time_display;//最近显示录音时间

//0-全盘扫描;1-32M;2-64M;3-128M;4-256M;5-512M;6-1024M(1G);7-1536M(1.5G);8-2048M(2G)
const uint32 rec_scan_space_size_map[9] =
    { 0xffffffff, 0x10000, 0x20000, 0x40000, 0x80000 ,0x100000,0x200000,0x300000,0x400000};
#endif

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
#if (SUPPORT_RECORD_FUNC == 1)
    sys_global_info_t* p_sys_global_info;
    p_sys_global_info = sys_get_system_info(GET_SYS_GLOBAL_DATA_INFO);
    p_sys_global_info->fs_scan_space = rec_scan_space_size_map[(uint8) com_get_config_default(RECORD_SCAN_MAX_SPACE)];
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*保存VM变量*/
    com_save_sys_comval();
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  初始化文件选择器时设置默认盘符为主盘，文件类型为COMMONDIR
 * \li  如果后台有music播放或从browser等进入，同步location
 * \li  music ui默认没有盘符记忆，从playlist进入和退出，盘符写入browser VM变量
 */
/*******************************************************************************/
bool _app_init(void)
{
    //读取配置信息 (包括vm信息)
    _load_cfg();

    //初始化applib库，前台AP
    applib_init(APP_ID_LINEIN, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    g_linein_tws_enable = FALSE;
    if ((com_get_config_default(BTMANAGER_ENABLE_TWS) == TRUE)
        && (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 1))
    {
        if ((g_linein_play_mode == LINEIN_MODE_AUX) && (com_get_config_default(LINEIN_ENABLE_TWS_MODE) == TRUE))
        {
            g_linein_tws_enable = TRUE;
        }
        #if (0)
        if (g_linein_play_mode == LINEIN_MODE_SPDIF)
        {
            g_linein_tws_enable = TRUE;
        }
        #endif
    }
#endif

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \退出app的功能函数 保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //执行applib库的注销操作
    applib_quit(THIS_APP_TYPE);

    //save config 写vram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e retval;

    g_linein_play_mode = (uint8) (uint32) argv[0];

    //初始化
    if (_app_init() == TRUE)
    {
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
        com_speaker_on(SPK_PLAY);
        retval = scene_linein_play();
    }
    else
    {
        //提示出错(显示ERRO)
        retval = RESULT_NEXT_FUNCTION;
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_linein_tws_enable == TRUE)
    {
        if ((com_ap_switch_ask_next_func(retval) != APP_FUNC_BTCALL) && (com_ap_switch_ask_next_func(retval) != APP_FUNC_ALARM))
        {
            tws_dev_reconn_phone_policy_t param;
            param.tws_paired_src_reconn_phone_enable = 1;
            param.cancel_cur_create_connection = 0;
            com_btengine_connect_deal(CON_DEAL_SET_TWS_RECONN_PHONE_POLICY, &param);
        }
    }
#endif

    com_ap_switch_deal(retval);

    //退出
    _app_deinit();

    return 0;
}
