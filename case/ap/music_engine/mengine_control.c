/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 引擎播放状态处理
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_status_deal(void)
 * \引擎状态处理,处理当前音乐播放状态
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void mengine_status_deal(void)
{
    switch_result_e switch_retval = SWITCH_NO_ERR;
    sound_energy_t tmp_sound_energy;

    //获取当前播放状态
    if (_get_mmm_status() == FALSE)
    {
        return ;
    }

    change_ktp_status(g_mp_status.ktp_status);

    if ((g_mp_status.status == MMM_MP_ENGINE_PLAYING)
        || (g_mp_status.status == MMM_MP_ENGINE_FADEIN)
        || (g_mp_status.status == MMM_MP_ENGINE_FADEOUT))
    {
        tmp_sound_energy.play_cur_energy = g_mp_status.music_cur_energy;
        tmp_sound_energy.play_cur_energy_rms = g_mp_status.music_cur_energy_rms;
        tmp_sound_energy.mic_cur_energy = g_mp_status.mic_cur_energy;
        tmp_sound_energy.mic_cur_energy_rms = g_mp_status.mic_cur_energy_rms;
        libc_memcpy(tmp_sound_energy.freq_point_energy, g_mp_status.freq_point_energy, sizeof(tmp_sound_energy.freq_point_energy));
        com_update_sound_energy(&tmp_sound_energy);
    }
    else
    {
        //清除声音能量值
        com_update_sound_energy(NULL);
    }

    g_eg_status_p->lowpw_time = g_mp_status.music_lowpw_time;

    //淡入淡出当作正在播放状态
    if ((g_mp_status.status == MMM_MP_ENGINE_FADEIN) || (g_mp_status.status == MMM_MP_ENGINE_FADEOUT))
    {
        g_mp_status.status = MMM_MP_ENGINE_PLAYING;
    }

    //存放当前的播放时间和比特率
    if ((g_eg_status_p->play_status != PauseSta) && (g_eg_status_p->play_status != StopSta))
    {
        g_eg_playinfo_p->cur_time = (uint32) g_mp_status.cur_time;
    }

    switch (g_mp_status.status)
    {
        case MMM_MP_ENGINE_PLAYING: //正常播放

        //ESD RESET 实时断点备份
        _get_breakpoint();

        if ((g_eg_status_p->fast_status == FFPlaySta) || (g_eg_status_p->fast_status == FBPlaySta)) //快进退取消
        {
            //已经恢复正常播放
            if (g_eg_status_p->play_status == PlaySta)
            {
                _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);
                g_eg_status_p->fast_status = FBPlay_Null;
            }
        }
        break;

        case MMM_MP_ENGINE_PLAYEND: //停止

        //sd 文件播放只需要停止
        if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
        {
            _stop (STOP_NORMAL); //停止播放,将文件关闭
            break;
        }

        //正常播放到尾
        if (g_eg_status_p->play_status == PlaySta)
        {
            //正常播放到尾
            switch_retval = _mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
        }
        else if (g_eg_status_p->play_status == PlayFast)
        {
            //快进到尾停止或快退到头停止后，停止快进退
            _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);

            if (g_eg_status_p->fast_status == FFPlaySta)
            {
                //快进到尾切换到下一曲正常播放
                switch_retval = _mengine_file_switch(STOP_NORMAL, FORCE_SWITCH_NEXT, PLAY_NORMAL);
                g_eg_status_p->fast_status = FFPlayTailSta;
            }
            else
            {
                //快退到头从当前曲头开始播放
                switch_retval = _stop(STOP_NORMAL);
                switch_retval = _play(PLAY_NORMAL);
                g_eg_status_p->fast_status = FBPlayHeadSta;
            }
        }
        else
        {
            ; //do nothing
        }

        break;

        case MMM_MP_ENGINE_ERROR: //出错

        //保存出错状态
        _get_errno_no();
        //关闭解码中间件
        _stop (STOP_NORMAL);

        break;

        default:
        break;
    }
}


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
app_result_e mengine_control_block(void)
{
    private_msg_t cur_msg_val;
    app_result_e mengine_retval = RESULT_NULL;

    while (1)
    {
        //查询当前播放状态
        if (mp_handle != NULL)
        {
            mengine_status_deal();
        }

        //获取并处理详细
        if (get_app_msg(THIS_APP_TYPE, &cur_msg_val) == TRUE)
        {
            switch (cur_msg_val.msg.type)
            {
                /*获取信息相关*/
                case MSG_MENGINE_GET_PLAYMODE_SYNC: //获取循环模式+shuffle
                mengine_get_playmode(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_FILEPATH_SYNC: //获取文件location信息
                mengine_get_filepath(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_STATUS_SYNC: //获取当前播放状态
                mengine_get_status(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_PLAYINFO_SYNC: //获取当前播放信息
                mengine_get_playinfo(&cur_msg_val);
                break;

                default:
                mengine_retval = mengine_message_done_bank(&cur_msg_val);
                break;
            }
        }

        if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
        {
            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            change_engine_state(ENGINE_STATE_PLAYING);
            //播放起来则认为该文件有效，取消无效标识
            if (1 == g_eg_cfg_p->bk_infor_invalid_flag)
            {
                g_eg_cfg_p->bk_infor_invalid_flag = 0;
                mengine_vram_write(g_eg_cfg_p->location.plist_location.disk);
            }
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            //退出应用关闭处理
            _stop (STOP_NORMAL);
            break;
        }

        //ESD RESET 实时断点备份
        g_p_esd_bk_info->play_status = g_eg_status_p->play_status;

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }

    //清除声音能量值
    com_update_sound_energy(NULL);

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

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
void mengine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
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

