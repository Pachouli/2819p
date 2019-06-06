/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌前台应用  与引擎交互接口功能实现
 ********************************************************************************/


#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \获取引擎相关参数函数
 * \param[in]    input_param 输入参数指针
 * \param[in]    msg_type  消息类型
 * \param[out]   output_param 输出参数指针
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    //存放输入参数的地址
    msg.content.addr = input_param;

    //存放获取参数的地址
    reply.content = output_param;

    //消息类型(即消息名称)
    msg.type = msg_type;

    //发送同步消息
    bret = send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \获取当前播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    bool bret;
    bret = music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \获取当前播放状态
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t *play_status)
{
    bool bret;
    bret = music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* loop_mode)
 * \获取当前播放模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8 *loop_mode)
{
    bool bret;
    bret = music_send_msg(NULL, loop_mode, MSG_MENGINE_GET_PLAYMODE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filename(music_file_name_t *music_file_name)
 * \获取当前文件名称
 * \param[in]    music_file_name 文件名结构体缓冲区
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filename(music_file_name_t *music_file_name)
{
    bool bret;
    bret = music_send_msg(NULL, music_file_name, MSG_MENGINE_GET_FILENAME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_playmode(uint8 playmode)
 * \设置循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret;
    bret = music_send_msg(&playmode, NULL, MSG_MENGINE_SET_PLAYMODE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_volume(uint8 volume)
 * \设置音量
 * \param[in]    volume 音量等级
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_set_volume(uint8 volume)
{
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play(void)
 * \开始播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_pause(void)
 * \暂停播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_pause(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_resume(void)
 * \恢复播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_resume(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_stop(void)
 * \停止播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STOP_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_forward(void)
 * \快进
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_forward(void)
{
    bool bret;
    //消息类型(即消息名称)快进
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FFWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_backward(void)
 * \快退
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_backward(void)
{
    bool bret;
    //消息类型(即消息名称)快退
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FBWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_cancel_ffb(void)
 * \取消快进退
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_cancel_ffb(void)
{
    bool bret;
    //消息类型(即消息名称)取消快进退
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CANCEL_FFB_SYNC);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_next(void)
 * \播放下一曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_play_next(void)
{
    bool bret;

    //发送同步消息
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_NEXT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_prev(void)
 * \播放上一曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_play_prev(void)
{
    bool bret;

    //发送同步消息
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_PREV_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bbool music_clear_error(void)
 * \清除错误号
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_clear_error(void)
{
    bool bret = TRUE;

    //发送同步消息
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CLEAR_ERR_SYNC);

    return bret;
}

//获取文件信息，包括文件总时间，波特率，采样率等
bool music_get_fileinfo(mmm_mp_file_info_t *fileinfo)
{
    return music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_sortnumber(uint16 destnum)
 * \设置排序后的文件序号
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_get_disknumber(uint16 *disk_num)
{
    bool bret;
    bret = music_send_msg(NULL, disk_num, MSG_MENGINE_GET_DISKNUMBER_SYNC);
    return bret;
}

