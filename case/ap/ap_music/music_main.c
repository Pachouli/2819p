/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌前台应用  前台应用初始化及主函数
 ********************************************************************************/

#include "app_music.h"

//globle variable

//进入music的方式
uint8 g_enter_mode;
//连续错误歌曲数
uint16 g_error_num = 0;

//向前切
uint16 g_switch_prev = 0;

//全出错切换盘标志
bool g_all_error_switch = FALSE;

//文件路径来源信息保存，播放时候用
file_path_info_t g_file_path_info;

//当前播放状态信息
mengine_status_t g_music_status;
play_status_e last_play_status;

//当前播放时间和比特率信息
mengine_playinfo_t g_play_info;

//music总时间信息
mmm_mp_file_info_t g_music_fileinfo;
//music ui的配置项信息
music_config_t g_music_config;

//当前歌曲总时间
uint32 g_total_time;

//当前播放时间
uint32 g_cur_time;

//前一次记录播放时间
uint32 g_prev_time;

uint8 last_minute, last_col;

//主动切歌到第几首
uint16 g_manul_switch_song_seq;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
uint8 g_fname_buffer[32];
bool  g_fname_scroll_flag;
bool  g_scroll_flag;
#endif

#define  MODE_MAX_COUNTER          (9)

const mode2disk_t g_mode2disk[MODE_MAX_COUNTER] =
{
    { PARAM_MUSIC_CARD_DIR,        DISK_H },
    { PARAM_MUSIC_UHOST_DIR,       DISK_U },
    { PARAM_MUSIC_RES_DIR,         DISK_C },
    { PARAM_MUSIC_CARD_REPLAY,     DISK_H },
    { PARAM_MUSIC_UHOST_REPLAY,    DISK_U },
    
    { PARAM_MUSIC_UHOST_DIR1_PLAY, DISK_U },
    { PARAM_MUSIC_UHOST_DIR2_PLAY, DISK_U },
    { PARAM_MUSIC_CARD_DIR1_PLAY,  DISK_H },
    { PARAM_MUSIC_CARD_DIR2_PLAY,  DISK_H },
};

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
    //AP_MUSIC_PRIO music 变量获取
    if (sys_vm_read(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t)) < 0)
    {
        g_music_config.magic = 0xffff;
    }
    if (g_music_config.magic != VRAM_MAGIC(VM_AP_MUSIC))
    {
        g_music_config.magic = VRAM_MAGIC(VM_AP_MUSIC);

        //eq mode
        g_music_config.eq_type = EQ_NOR;

        //repeate mode
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        //按键个数
        g_music_config.card_open = (uint8) com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY);
    }
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
    sys_vm_write(&g_music_config, VM_AP_MUSIC, sizeof(g_music_config));
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
    applib_init(APP_ID_MUSIC, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    //初始化文件选择器 后台进行此操作

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
    //退出应用时确保采样率为44KHz
    set_dac_rate(44);

    //save config 写vram

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //执行applib库的注销操作
    applib_quit(THIS_APP_TYPE);

    //save config 写vram
    _save_cfg();

    return TRUE;
}

void _filter_sd_in_out_msg(void)
{
    msg_apps_type_e type;
    uint8 i = 0;
    //clr msf queue
    while (i < 5)
    {
        //sys_mq_flush(MQ_ID_HOTPLUG);
        if (sys_mq_receive(MQ_ID_HOTPLUG, (void *) &type) == 0)
        {
            if ((type != MSG_SD_IN) && (type != MSG_SD_OUT))
            {
                sys_mq_send(MQ_ID_HOTPLUG, (void *) &type);
            }
        }
        else
        {
            break;
        }

        i++;
    }
}

int8 set_play_disk_info(uint8 enter_mode)
{
    uint8 i=0;

    for(i=0;i<MODE_MAX_COUNTER;i++)
    {
        if(g_mode2disk[i].enter_mode == enter_mode)
        {
            break;
        }
    }

    if(i == MODE_MAX_COUNTER)
    {
        libc_printf_error("invalid enter mode!");
        return -1;
    }

    g_file_path_info.file_path.dirlocation.disk = g_mode2disk[i].play_disk;
    return 0;
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
    g_enter_mode = (uint8) (uint32) argv[0];

    _filter_sd_in_out_msg();

    //续播
    if(0 != set_play_disk_info(g_enter_mode))
    {
        retval = RESULT_NEXT_FUNCTION;
        goto endpro;
    }

    //初始化
    if (_app_init() == TRUE)
    {
        retval = music_scene_playing();
        g_p_global_var->display_time = g_cur_time;
    }
    else
    {
        //提示出错(显示ERRO)
        retval = RESULT_NEXT_FUNCTION;
    }

    endpro:
    com_ap_switch_deal(retval);

    //退出
    _app_deinit();

    return retval;
}
