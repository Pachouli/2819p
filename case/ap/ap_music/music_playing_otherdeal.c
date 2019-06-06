/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌前台应用  前台应用播放异常处理
 ********************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_disk_out_deal(uint8 out_disk)
 * \拔盘操作
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_playing_diskdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_disk_out_deal(uint8 out_disk)
{
    app_result_e result = RESULT_NULL;

    //获取文件路径
    music_get_filepath (&g_file_path_info);
    if (out_disk == g_file_path_info.file_path.dirlocation.disk)
    {
        //clr error number
        music_clear_error();
        result = play_key_mode();
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_disk_switch_deal(uint8 dest_disk)
 * \切换盘符
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_playing_diskdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_disk_switch_deal(uint8 dest_disk)
{
    app_result_e result = RESULT_NULL;

    if (music_switch_disk(dest_disk) == TRUE)
    {
        //set play mode
        music_set_playmode(g_music_config.repeat_mode);
        music_get_filepath (&g_file_path_info);
    }

    return result;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_disk_switch_deal(uint8 dest_disk)
 * \切换盘符
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_playing_diskdeal.c
 * \note
 */
/*******************************************************************************/
bool music_check_disk_in(uint8 disk_type)
{
    bool bret = TRUE;

    if (disk_type == DISK_H)
    {
        bret = sys_card_detect();
    }
    else
    {
        if (disk_type == DISK_U)
        {
            bret = sys_uhost_detect();
        }
        else
        {
            bret = FALSE;
        }
    }

    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_eror_deal(eg_err_e error_number)
 * \出错处理(切换歌曲)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e play_eror_deal(eg_err_e error_number)
{
    app_result_e err_ret = RESULT_NULL;

    bool ret_val = FALSE;

    bool disk_out = FALSE;

    uint8 disk_flag;

    //清除错误号
    music_clear_error();

    //获取文件路径
    music_get_filepath(&g_file_path_info);

    disk_flag = g_file_path_info.file_path.dirlocation.disk;

    if (music_check_disk_in(disk_flag) == FALSE)
    {
        disk_out = TRUE;
    }

    if ((error_number == EG_ERR_DISK_OUT) || (error_number == EG_ERR_NO_FILE) || (disk_out == TRUE))
    {
        /*增加卡检测，可支持卡快速插拔*/
        if (disk_flag == DISK_H)
        {
            uint16 msg = 0;
            msg = MSG_SYS_SD_OUT;
            sys_mq_send(MQ_ID_HOTPLUG, &msg);
            libc_printf_info("<eg get msg:sd out!>err_code:%d f:%d\n",error_number,disk_out);
        }

        if(disk_flag == DISK_U)
        {
            libc_printf_info("<eg uhost out!>err_code:%d f:%d\n",error_number,disk_out);
        }
        //防止系统消息处理完之后,显示出现错乱
        err_ret = RESULT_NEXT_FUNCTION;
    }
    else
    {
        uint16 file_total_number;

        //其他情况是解码出错
        g_error_num++;
        music_get_filepath(&g_file_path_info);
        file_total_number = g_file_path_info.file_path.dirlocation.file_total_disk;

        if ((file_total_number != 0) && (g_error_num >= file_total_number))
        {
            //切换到下一个应用
            err_ret = RESULT_NEXT_FUNCTION;
        }
        else
        {
            if (g_switch_prev == 1)
            {
                //切换到上一曲
                ret_val = music_play_prev();
            }
            else
            {
                //切换到下一曲
                ret_val = music_play_next();
            }
            if (ret_val == TRUE)
            {
                music_play();
            }
            else
            {
                err_ret = RESULT_NULL;
            }
        }
    }
    return err_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_switch_disk(uint8 disk_flag)
 * \切换播放的磁盘
 * \param[in]    disk_flag 磁盘盘符 para1
 * \param[in]    para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_switch_disk(uint8 disk_flag)
{
    bool bret;

    bret = music_send_msg(&disk_flag, NULL, MSG_MENGINE_SWITCH_DISK_SYNC);

    //切换盘符打开静音
    com_set_mute (FALSE);

    return bret;
}
