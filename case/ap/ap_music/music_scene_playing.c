/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌前台应用  前台应用播放场景初始化及退出
 ********************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_open_engine(mengine_enter_mode_e start_mode)
 * \装载引擎,如果引擎存在，则不再安装
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //msg_reply_t temp_reply;

    //无消息内容
    msg.para[0] = APP_ID_MENGINE;
    if(DISK_C == g_file_path_info.file_path.dirlocation.disk)
    {
        msg.para[1] = (uint8) (start_mode);
    }
    else
    {
        msg.para[1] = (uint8) (start_mode | g_file_path_info.file_path.dirlocation.disk);
    }

    if (get_need_update_vm() == 1)
    {
        msg.para[1] |= 0x08; //bit3表示要清除断点信息
    }
    msg.content.addr = NULL;

    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    if (bret == TRUE)
    {
        set_need_update_vm(0);
    }

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_close_engine(uint8 engine_id)
 * \关闭引擎
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_close_engine(uint8 engine_id)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //注销后台，后台engine_id
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP;
    msg.para[0] = engine_id;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}

//快速预览模式会使用该接口来预览，代码硬拷贝到 com_fast_preview_ui.c 里
//disk_index: 0-SD卡,1-Uhost,2-NOR
void music_show_welcome_ui(uint8 disk_index)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    if (disk_index == 0)
    {
        led_display_icon(LCD_SD, TRUE);
    }
    else if (disk_index == 1)
    {
        led_display_icon(LCD_USB, TRUE);
    }

    //显示4个0
    led_display(NUMBER1, 0, TRUE);
    led_display(NUMBER2, 0, TRUE);
    led_display(NUMBER3, 0, TRUE);
    led_display(NUMBER4, 0, TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_init(void)
 * \播放场景初始化
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_init(void)
{
    bool ret_val = TRUE;
    //for open engine param
    uint8 open_mode, tts_id;
    uint8 disk_index;
    app_result_e result = RESULT_NULL;
    bool need_switch_ap = FALSE;
    bool need_flush_key = FALSE;

    //from tools
    com_set_mute(FALSE); //解除静音

    if (get_engine_type() != ENGINE_NULL)
    {
        //注销radio后台
        music_close_engine(APP_ID_THEENGINE);
    }

    if (g_enter_mode == PARAM_MUSIC_CARD_REPLAY)
    {
        //tts_id = TTS_CARD_REPLAY;
        tts_id = TTS_REPLAY;
        open_mode = ENTER_RECORD_PLAY;

        disk_index = 0;
    }
    else if (g_enter_mode == PARAM_MUSIC_UHOST_REPLAY)
    {
        //tts_id = TTS_UHOST_REPLAY;
        tts_id = TTS_REPLAY;
        open_mode = ENTER_RECORD_PLAY;

        disk_index = 1;
    }
    else
    {
        if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
        {
            tts_id = TTS_PLAY_SDCARD;
            open_mode = ENTER_MUSIC_START;

            disk_index = 0;
        }
        else if (g_file_path_info.file_path.dirlocation.disk == DISK_U)
        {
            tts_id = TTS_PLAY_UHOST;
            open_mode = ENTER_MUSIC_START;

            disk_index = 1;
        }
        else //if (g_file_path_info.file_path.dirlocation.disk == DISK_C)
        {
            tts_id = TTS_PLAY_MDISK;
            open_mode = ENTER_MDISK_PLAY;
            disk_index = 2;
        }
    }

    if (g_p_global_var->fp_switch_jump_welcome == FALSE)
    {
        music_show_welcome_ui(disk_index);
    }
    g_p_global_var->fp_switch_jump_welcome = FALSE;

    //从后台应用返回恢复时间点
    if (g_p_global_var->bg_app_sta == BG_STATUS_BACK)
    {
        g_cur_time = g_p_global_var->display_time;
    }

#if (SUPPORT_FAST_PREVIEW == 1)

    if (get_bt_state() == BT_STATE_WORK)
    {
        //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

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
            private_msg_t music_msg;
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
                if (get_app_msg(THIS_APP_TYPE, &music_msg) == TRUE)
                {
                    result = music_message_deal(&music_msg);
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

    //ESD RESET 不要播报TTS
    if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
    {
        //play tts
        com_tts_state_play(tts_id, 0);
    }

#else

    //ESD RESET 不要播报TTS
    if ((g_p_global_var->bg_app_sta != BG_STATUS_BACK) && (g_p_esd_bk_info->reset_flag == 0))
    {
        //play tts
        com_tts_state_play(tts_id, 0);
    }

    if (get_bt_state() == BT_STATE_WORK)
    {
        //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
        if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
        {
            com_close_bt_engine();
        }
    }

#endif

    com_speaker_on(SPK_PLAY);

    //创建MUSIC引擎
    ret_val = music_open_engine(open_mode);

    if (ret_val == TRUE)
    {
        //check_file:
        //获取状态信息
        music_get_status(&g_music_status);

        //set volume
        com_reset_sound_volume(SET_VOL_BY_SYSTEM);

        if (g_music_status.err_status == EG_ERR_NO_FILE)
        {
            //没有卡和U盘，或则盘上没有可播放的音乐文件则提示出错
            ret_val = FALSE;
        }
    }

    if (ret_val == TRUE)
    {
        music_set_playmode(g_music_config.repeat_mode);
    }

    if (ret_val == FALSE)
    {
        result = RESULT_NEXT_FUNCTION;
    }

    fast_switch_ap_exit:

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_exit(void)
 * \退出播放场景
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_exit(void)
{
    if (get_engine_type() != ENGINE_NULL)
    {
        //注销radio后台
        music_close_engine(APP_ID_THEENGINE);
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(void)
 * \播放场景处理
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_playing(void)
{
    app_result_e ret_vals;
    uint8 disk_in_play;

    //场景初始化 初始化文件选择器，获取UI需显示的数据等
    ret_vals = _scene_play_init();

    if ((ret_vals != RESULT_NULL) && (ret_vals != RESULT_REDRAW))
    {
        goto ret_deal;
    }

    //连续错误歌曲数
    g_error_num = 0;

    //当前播放曲目号
    music_get_filepath(&g_file_path_info); //获取文件路径

    //初始显示曲目号
    _playing_show_seq(g_file_path_info.file_path.dirlocation.file_num);
    g_manul_switch_song_seq = g_file_path_info.file_path.dirlocation.file_num;

    disk_in_play = g_file_path_info.file_path.dirlocation.disk;
    //不是从clock返回,则一定是插入磁盘、mode键等方式进入music,则需要重新扫描磁盘

    //场景循环
    ret_vals = _playing_loop_deal();

    ret_deal:

    //调用场景退出函数
    _scene_play_exit();
    //deal result ret 如果是录音播放,则对于进入下一个的处理变成回到上一次播放
    if (ret_vals == RESULT_NEXT_FUNCTION)
    {
        //other play mode need return to last play
        if ((g_enter_mode != PARAM_MUSIC_CARD_DIR) && (g_enter_mode != PARAM_MUSIC_UHOST_DIR) && (g_enter_mode != PARAM_MUSIC_RES_DIR))
        {
            ret_vals = RESULT_LASTPLAY;
        }
    }

    return ret_vals;
}

