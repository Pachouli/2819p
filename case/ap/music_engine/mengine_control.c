/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ���沥��״̬����
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_status_deal(void)
 * \����״̬����,����ǰ���ֲ���״̬
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

    //��ȡ��ǰ����״̬
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
        //�����������ֵ
        com_update_sound_energy(NULL);
    }

    g_eg_status_p->lowpw_time = g_mp_status.music_lowpw_time;

    //���뵭���������ڲ���״̬
    if ((g_mp_status.status == MMM_MP_ENGINE_FADEIN) || (g_mp_status.status == MMM_MP_ENGINE_FADEOUT))
    {
        g_mp_status.status = MMM_MP_ENGINE_PLAYING;
    }

    //��ŵ�ǰ�Ĳ���ʱ��ͱ�����
    if ((g_eg_status_p->play_status != PauseSta) && (g_eg_status_p->play_status != StopSta))
    {
        g_eg_playinfo_p->cur_time = (uint32) g_mp_status.cur_time;
    }

    switch (g_mp_status.status)
    {
        case MMM_MP_ENGINE_PLAYING: //��������

        //ESD RESET ʵʱ�ϵ㱸��
        _get_breakpoint();

        if ((g_eg_status_p->fast_status == FFPlaySta) || (g_eg_status_p->fast_status == FBPlaySta)) //�����ȡ��
        {
            //�Ѿ��ָ���������
            if (g_eg_status_p->play_status == PlaySta)
            {
                _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);
                g_eg_status_p->fast_status = FBPlay_Null;
            }
        }
        break;

        case MMM_MP_ENGINE_PLAYEND: //ֹͣ

        //sd �ļ�����ֻ��Ҫֹͣ
        if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
        {
            _stop (STOP_NORMAL); //ֹͣ����,���ļ��ر�
            break;
        }

        //�������ŵ�β
        if (g_eg_status_p->play_status == PlaySta)
        {
            //�������ŵ�β
            switch_retval = _mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
        }
        else if (g_eg_status_p->play_status == PlayFast)
        {
            //�����βֹͣ����˵�ͷֹͣ��ֹͣ�����
            _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);

            if (g_eg_status_p->fast_status == FFPlaySta)
            {
                //�����β�л�����һ����������
                switch_retval = _mengine_file_switch(STOP_NORMAL, FORCE_SWITCH_NEXT, PLAY_NORMAL);
                g_eg_status_p->fast_status = FFPlayTailSta;
            }
            else
            {
                //���˵�ͷ�ӵ�ǰ��ͷ��ʼ����
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

        case MMM_MP_ENGINE_ERROR: //����

        //�������״̬
        _get_errno_no();
        //�رս����м��
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
 * \���湦�ܴ�����
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
        //��ѯ��ǰ����״̬
        if (mp_handle != NULL)
        {
            mengine_status_deal();
        }

        //��ȡ��������ϸ
        if (get_app_msg(THIS_APP_TYPE, &cur_msg_val) == TRUE)
        {
            switch (cur_msg_val.msg.type)
            {
                /*��ȡ��Ϣ���*/
                case MSG_MENGINE_GET_PLAYMODE_SYNC: //��ȡѭ��ģʽ+shuffle
                mengine_get_playmode(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_FILEPATH_SYNC: //��ȡ�ļ�location��Ϣ
                mengine_get_filepath(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_STATUS_SYNC: //��ȡ��ǰ����״̬
                mengine_get_status(&cur_msg_val);
                break;

                case MSG_MENGINE_GET_PLAYINFO_SYNC: //��ȡ��ǰ������Ϣ
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
            //������������Ϊ���ļ���Ч��ȡ����Ч��ʶ
            if (1 == g_eg_cfg_p->bk_infor_invalid_flag)
            {
                g_eg_cfg_p->bk_infor_invalid_flag = 0;
                mengine_vram_write(g_eg_cfg_p->location.plist_location.disk);
            }
        }

        if (mengine_retval == RESULT_APP_QUIT)
        {
            //�˳�Ӧ�ùرմ���
            _stop (STOP_NORMAL);
            break;
        }

        //ESD RESET ʵʱ�ϵ㱸��
        g_p_esd_bk_info->play_status = g_eg_status_p->play_status;

        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }

    //�����������ֵ
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
 * \�ظ�ͬ����Ϣ
 * \param[in]    void  msg_ptr ��Ϣ�ṹ��
 * \param[in]    bool  ret_vals ����¼������� TRUE �ɹ� FALSE ʧ��
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR ����
 * \ingroup      mengine_control.c
 * \note  �ظ���Ϣʱ������ret_vals�Ľ��������Ӧ��ɹ���ʧ��
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

    //�ظ�ͬ����Ϣ(�����ź���)
    libc_sem_post(msg_ptr->sem);
}

