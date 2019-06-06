/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话引擎应用主循环，查询中间件状态并响应状态变化，获取应用消息并响应。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_btcall_engine.h"

void btcall_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*更新当前录音文件大小到vram*/
void btcall_engine_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void btcall_engine_recording_check_status(void)
{
    bool ret ;
    //更新录音信息到vram
    if (g_btcall_info.record_sta == RECORD_STA_RECORDING)
    {
        btcall_engine_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_btcall_info.record_sta != RECORD_STA_RECORDING))
    {
        //从play到pause(录音暂停时，需要强制更新时间)
        libc_printf("record pause , update record file now\n");
        btcall_engine_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_btcall_info.record_sta;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_control_block(void)
 * \引擎功能处理函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
app_result_e btcall_engine_control_block(void)
{
    private_msg_t cur_msg;
    app_result_e mengine_retval = RESULT_NULL;
    bool ret_vals;
    bool need_set_hfp_connect = FALSE;
    uint8 last_hfp_status;

    g_btcall_info.status = BTCALL_STOP;

    com_get_btengine_status(&btcall_bt_info);
    last_hfp_status = btcall_bt_info.status;
    if (last_hfp_status == STATUS_CALL_HFP)
    {
        need_set_hfp_connect = TRUE;
    }

    while (1)
    {
        //中间件状态查询及处理
        if (sp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;
                change_ktp_status(sp_status.ktp_status);

                tmp_sound_energy.play_cur_energy = sp_status.play_cur_energy;
                tmp_sound_energy.play_cur_energy_rms = sp_status.play_cur_energy_rms;
                tmp_sound_energy.mic_cur_energy = sp_status.mic_cur_energy;
                tmp_sound_energy.mic_cur_energy_rms = sp_status.mic_cur_energy_rms;
                com_update_sound_energy(&tmp_sound_energy);

                if (sp_status.status == MMM_SP_ENGINE_ERROR)
                {
                    //检测到错误，暂时没有这样的状态返回  TODO
                }

            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module出错
                    g_btcall_info.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }

        com_get_btengine_status(&btcall_bt_info);

        if ((last_hfp_status != STATUS_CALL_HFP) && (btcall_bt_info.status == STATUS_CALL_HFP))
        {
            need_set_hfp_connect = TRUE;
        }
        last_hfp_status = btcall_bt_info.status;

        if ((sp_handle != NULL) && (need_set_hfp_connect == TRUE))
        {
            if (mmm_sp_cmd(sp_handle, MMM_SP_SET_HFP_CONNECTED, 0) == 0)
            {//设置成功
                need_set_hfp_connect = FALSE;
            }
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
        btcall_engine_recording_check_status();
#endif

        //获取并处理详细
        if (get_app_msg(THIS_APP_TYPE, &cur_msg) == TRUE)
        {
            switch (cur_msg.msg.type)
            {
                /*获取BTCALL引擎状态*/
                case MSG_BTCALLEG_GET_STATUS_SYNC:
                {
                    libc_memcpy((btcall_info_t *)(cur_msg.msg.content.addr), &g_btcall_info, sizeof(btcall_info_t));
                    btcall_engine_reply_msg(&cur_msg, TRUE);
                }
                break;

#if (SUPPORT_SOFT_KEYTONE == 1)
                /*请求播放软按键音*/
                case MSG_SOFT_KEYTONE_PLAY_SYNC:
                {
                    bool ret_val = FALSE;
                    if (sp_handle != NULL)
                    {
                        if (mmm_sp_cmd(sp_handle, MMM_SP_KEYTONE_PLAY, (uint8 *)(cur_msg.msg.content.addr)) == 0)
                        {
                            change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                            ret_val = TRUE;
                        }
                    }
                    btcall_engine_reply_msg(&cur_msg, ret_val);
                }
                break;
#endif

                /*设置参数相关*/
                case MSG_SET_EQ_SYNC: //设置eq +eq curve data
                if (sp_handle != NULL)
                {
                    mmm_sp_cmd(sp_handle, MMM_SP_SET_EQ, 0);
                }
                btcall_engine_reply_msg(&cur_msg, TRUE);
                break;

                case MSG_BTCALLEG_SPEECH_PLAY_SYNC:
                {
                    btcall_speech_play_para_t *p_play_para = (btcall_speech_play_para_t *)(cur_msg.msg.content.addr);

                    libc_memcpy(&g_play_para_backup, p_play_para, sizeof(btcall_speech_play_para_t));

                    g_btcall_info.status = BTCALL_PLAY;
                    ret_vals = _play(g_play_para_backup.oncall, g_play_para_backup.att_flag, \
                            g_play_para_backup.p_asqt_config, g_play_para_backup.p_asqt_dump_buf);

                    btcall_engine_reply_msg(&cur_msg, ret_vals);
                }
                break;

                case MSG_BTCALLEG_SPEECH_STOP_SYNC:
                {
                    bool oncall = cur_msg.msg.content.data[0];

                    g_btcall_info.status = BTCALL_STOP;
                    ret_vals = _stop(oncall);
                    need_set_hfp_connect = FALSE;

                    btcall_engine_reply_msg(&cur_msg, ret_vals);
                }
                break;

                case MSG_APP_QUIT: //退出app
                //exit_msg_loop();
                mengine_retval = RESULT_APP_QUIT;
                break;

                case MSG_TTS_START_SYNC: //准备进入TTS播报
                mengine_retval = btcall_engine_tts_start(&cur_msg);
                break;

                case MSG_TTS_STOP_SYNC: //TTS播报完成
                mengine_retval = btcall_engine_tts_stop(&cur_msg);
                break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
                case MSG_REC_START_SYNC:
                mengine_retval = btcall_engine_record_start(&cur_msg);
                break;

                case MSG_REC_STOP_SYNC:
                mengine_retval = btcall_engine_record_stop(&cur_msg);
                break;
#endif

                default:
                libc_printf_warning("btcall engine invalid msg!0x%x\n", cur_msg.msg.type);
                btcall_engine_reply_msg(&cur_msg, FALSE);
                break;
            }
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            break;
        }

        //ESD RESET实时断点备份
        g_p_esd_bk_info->play_status = 0; //清0确保不会进入蓝牙播歌

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }

    //清除声音能量值
    com_update_sound_energy(NULL);

    g_btcall_info.status = BTCALL_STOP;

    return mengine_retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_reply_msg(void* msg_ptr, bool ret_vals)
 * \回复同步消息
 * \param[in]    void  msg_ptr 消息结构体
 * \param[in]    bool  ret_vals 相关事件处理结果 TRUE 成功 FALSE 失败
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR 忽略
 * \ingroup      mengine_control.c
 * \note  回复消息时，根据ret_vals的结果，设置应答成功或失败
 */
/*******************************************************************************/
void btcall_engine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
{
    if (ret_vals == TRUE)
    {
        msg_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        msg_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //回复同步消息(发送信号量)
    libc_sem_post(msg_ptr->sem);
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_start(void* msg_ptr)
 * \开始语音播报,做好互斥准备
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口后，要保证不再使用 0x37000 以上的空间，直到调用 mengine_tts_stop 为止
 */
/*******************************************************************************/
app_result_e btcall_engine_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (sp_handle != NULL)
    {
        //先暂停播放
        ret_vals = _stop(g_play_para_backup.oncall);
        if (ret_vals == TRUE)
        {
            g_stop_by_tts_flag = TRUE;
        }
    }

    btcall_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_stop(void* msg_ptr)
 * \语音播报完成,可恢复备份
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口，表示 0x37000 以上空间可以完全由music应用支配
 */
/*******************************************************************************/
app_result_e btcall_engine_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        //恢复播放
        ret_vals = _play(g_play_para_backup.oncall, g_play_para_backup.att_flag, \
                g_play_para_backup.p_asqt_config, g_play_para_backup.p_asqt_dump_buf);

        g_stop_by_tts_flag = FALSE;
    }

    btcall_engine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_btcall_info.record_sta == RECORD_STA_RECORDING) || (g_btcall_info.record_sta == RECORD_STA_PAUSE))
    	&& (g_record_param.file_handle != NULL))
    {
        return TRUE; //已经创建
    }

    //创建文件
    common_err_ret = com_record_disk_init(disk);
    if (common_err_ret == REC_FS_ERR_NONE)
    {
        common_err_ret = com_record_create_file(FILE_FIRST, &g_record_param);
    }
    if (common_err_ret != REC_FS_ERR_NONE)
    {
        libc_printf_error("common_err_ret = %d\n", common_err_ret);
        com_record_disk_exit();
        return FALSE;
    }

    return TRUE;
}

bool _record_close_file(void)
{
    if (g_btcall_info.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e btcall_engine_record_start(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_btcall_info.status == BTCALL_STOP)
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_create_file();
        if (ret_vals == TRUE)
        {
            ret_vals = _record_start(1, RECORD_SOURCE_LR_MIX);
            if (ret_vals == TRUE)
            {
                g_btcall_info.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_btcall_info.record_sta = RECORD_STA_ERROR;
        }
    }
    btcall_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

app_result_e btcall_engine_record_stop(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_btcall_info.status == BTCALL_STOP)
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_btcall_info.record_sta = RECORD_STA_STOP;
    }

    btcall_engine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

#endif
