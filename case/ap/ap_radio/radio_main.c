/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

/* Radio UI 任务栈*/
INT8U prio = AP_FRONT_LOW_PRIO;

//各波段最大频点
const uint32 max_freq[] = { MAX_US, MAX_JP, MAX_EU };

//各波段最小频点
const uint32 min_freq[] = { MIN_US, MIN_JP, MIN_EU };

//radio ui 的配置项信息
radio_config_t g_radio_config;

//引擎状态
FM_Drv_Status_t g_engine_status;

//引擎状态变量结构体指针
fmeg_status_t g_fmeg_status;
play_status_e g_last_fmeg_status;

/* 指向当前所播放的电台列表表首*/
uint32 *pfreq;

//标识当前所处应用场景
radio_scene_e g_radio_scene;

//备份进行半自动搜台前的频率
uint32 startfreq;

//进入应用模式
uint8 g_enter_param;

//语音提示
bool need_tts = TRUE;

uint32 g_radio_search_th;

uint8 g_freq_tts_sec_ids[10];

//自动搜台试听
bool  g_radio_seek_audition_enable;
uint8 g_radio_seek_audition_time;

uint32 g_number_input_freq; //数字按键输入的频点
uint32 g_number_input_timer; //8S超时定时器
uint8 g_number_input_high; //数字按键输入的高数字
uint8 g_number_input_keys; //在收听场景，已输入几个数字按键，中间插入Next等按键会被清0

#ifdef FM_THRESHOLD_TEST
int16 __section__(".FM_TEST_BSS") fm_threshold_values[512]; //门限值记录
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_load_cfg(void)
 * \载入应用初始化信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_load_cfg(void)
{
    //获取门限值
    com_get_config_struct(RADIO_SEEK_THRESHOLD, (uint8 *)(&g_radio_search_th), sizeof(g_radio_search_th));

    g_radio_seek_audition_enable = (uint8) com_get_config_default(RADIO_SEEK_AUDITION_ENABLE);
    g_radio_seek_audition_time = (uint8) com_get_config_default(RADIO_SEEK_AUDITION_TIME);

    //获取radio AP 配置信息
    if (sys_vm_read(&g_radio_config, VM_AP_RADIO, sizeof(radio_config_t)) < 0)
    {
        g_radio_config.magic = 0xffff;
    }
    if (g_radio_config.magic != VRAM_MAGIC(VM_AP_RADIO))
    {
        g_radio_config.magic = VRAM_MAGIC(VM_AP_RADIO);

        //三个波段预设电台列表清零
        libc_memset(g_radio_config.Auto_tab, 0, MAX_STATION_COUNT * 4);

        g_radio_config.band_mode = (radio_band_e) com_get_config_default(RADIO_AP_BAND_SEL);
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
        g_radio_config.FMStatus.station_count = 0;

        sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));
    }

    /* 指针指向当前播放的频率表首*/
    pfreq = &g_radio_config.Auto_tab[0];
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_save_cfg(void)
{
    /*保存VM变量*/
    com_save_sys_comval();
    sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_app_init(void)
 * \RadioUI 应用信息初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_app_init(void)
{
    //读取配置信息 (包括vm信息)
    radio_load_cfg();

    //初始化applib库，前台AP
    applib_init(APP_ID_RADIO, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_app_exit(void)
 * \退出app的功能函数 保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //配置信息回写VRAM
    radio_save_cfg();

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);

    return TRUE;
}

void _kill_engine(void)
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

void _create_engine(void)
{
    msg_apps_t msg;

    //创建radio 后台引擎进程
    msg.type = MSG_CREAT_APP_SYNC;
    msg.para[0] = APP_ID_FMENGINE;
    msg.para[1] = FM_EG_ENTER_NORMAL;
    msg.content.addr = NULL;
    //发送同步消息
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_dispatch(uint8 enter_mode)
 * \场景调度器
 * \param[in]    enter_mode 进入模式
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       返回到何处
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radio_scene_dispatch(uint8 enter_mode)
{
    app_result_e scene_result = RESULT_NULL;
    bool first_flag = TRUE;
    g_radio_scene = FM_SCENE_PLAYING;

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_RADIO);

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(radio_app_msg_callback);

    /* radioUI 场景循环*/
    while (g_radio_scene != FM_SCENE_EXIT)
    {
        switch (g_radio_scene)
        {
            /* 进入RADIO 播放场景*/
            case FM_SCENE_PLAYING:
            scene_result = radio_scene_playing(first_flag);
            first_flag = FALSE;
            if (scene_result == RESULT_RADIO_AUTO_SEARCH)
            {
                g_radio_scene = FM_SCENE_AUTO_SEARCH;
            }
            else if (scene_result == RESULT_RADIO_MANUAL_UP_SEARCH)
            {
                g_radio_scene = FM_SCENE_MANUAL_UP_SEARCH;
            }
            else if (scene_result == RESULT_RADIO_MANUAL_DOWN_SEARCH)
            {
                g_radio_scene = FM_SCENE_MANUAL_DOWN_SEARCH;
            }
            else
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            break;

            /* FM自动搜台场景 */
            case FM_SCENE_AUTO_SEARCH:
            scene_result = radio_scene_auto_search(DIR_UP);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            /* FM手动搜台场景 */
            case FM_SCENE_MANUAL_UP_SEARCH:
            scene_result = radio_scene_play_search_deal(DIR_UP);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            /* FM手动搜台场景 */
            case FM_SCENE_MANUAL_DOWN_SEARCH:
            scene_result = radio_scene_play_search_deal(DIR_DOWN);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            default:
            //退出场景调度
            g_radio_scene = FM_SCENE_EXIT;
            break;
        }
    }

    _kill_engine();

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    return scene_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const void *argv[])
 * \radioUI app 入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e retval;
    bool result;
    //进入模式
    uint8 enter_mode = (uint8) (uint32) argv[0];

    g_enter_param = enter_mode;
    result = radio_app_init(); //初始化
    if (result == FALSE)
    {
        //初始化失败，切换到下一个应用
        retval = RESULT_NEXT_FUNCTION;
    }
    else
    {
        _kill_engine();
        com_set_mute (FALSE); //解除静音

        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
        com_speaker_on(SPK_PLAY);

        //切换为AB类功放，以降低干扰，提升FM收听效果
        com_speaker_ab_d_switch(PA_SWITCH_AB);

        retval = radio_scene_dispatch(enter_mode); //场景调度器

        //切换为D类功放，以降低功耗
        com_speaker_ab_d_switch(PA_SWITCH_D);
    }

    com_ap_switch_deal(retval);
    radio_app_exit(); //退出

    return retval;
}

