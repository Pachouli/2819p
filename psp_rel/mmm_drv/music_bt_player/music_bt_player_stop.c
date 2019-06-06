/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м��ֹͣ���Ŵ������������ݲ�������ֹͣ���Ŵ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"
#include "sysdef.h"

#define DRV_NAME_SIZE 12
static const char dec_sbc_name[DRV_NAME_SIZE] = "dec_sbc.al";
static const char aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";

void destroy_decode_thread(music_bt_player_t *music_player)
{
    music_player->thread_terminate_flag = TRUE;

    while (music_player->thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}

int32 stop_music_decoder_fadeout_start(music_bt_player_t *music_player, bool pause)
{
    music_player->fade_flag = 2;
    libc_printf_info("fadeout:%d\n", music_player->music_setting.fade_out_time);
    update_PP_para_info(music_player);

    if (pause == TRUE)
    {
        music_player->music_status.status = MMM_BP_ENGINE_FADEOUT_PAUSE;
    }
    else
    {
        music_player->music_status.status = MMM_BP_ENGINE_FADEOUT;
    }

    return 0;
}

int32 stop_music_decoder_fadeout_wait_cp(music_bt_player_t *music_player)
{
    uint32 fade_out_timeout;

#ifdef BT_MMM_DEBUG_INFO
    libc_printf("wait pcm buf empty\n");
#endif

    fade_out_timeout = sys_get_ab_timer();
    //�ȴ�PCM_BUF�ĵ�����Ƶȫ���������
    while ((music_player->g_bp_pcm_buf_pending & 0x02) == 0)
    {
        //ͨ�����PCM BUF�в����������ж��Ƿ�Ϊ��
        if (get_pcmbuf_length() == 0)
        {
            break;
        }

        //���500ms���������ˣ�Ӧ���ǳ�������
        if ((sys_get_ab_timer() - fade_out_timeout) > 500)
        {
            libc_printf_warning("fadeout overtime!!\n");
            break;
        }
    }

#ifdef BT_MMM_DEBUG_INFO
    libc_printf("pcm buf empty\n");
#endif

    return 0;
}

int32 stop_music_decoder_fadeout(music_bt_player_t *music_player)
{
    int32 ret = 0;
    uint16 fade_out_time_bk = music_player->music_setting.fade_out_time;

    //��������
    if (music_player->buffer_time_length < fade_out_time_bk)
    {
        music_player->music_setting.fade_out_time = music_player->buffer_time_length;
        if (music_player->music_setting.fade_out_time < 5)
        {
            music_player->music_setting.fade_out_time = 5;
        }
    }

    stop_music_decoder_fadeout_start(music_player, TRUE);

    music_player->music_setting.fade_out_time = fade_out_time_bk;

    while (1)
    {
        if ((music_player->music_status.status == MMM_BP_ENGINE_PAUSE)
            || (music_player->music_status.status == MMM_BP_ENGINE_ERROR))
        {
#if (SUPPORT_SOFT_KEYTONE == 1)
            //�ȴ��������������
            while ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
            {
                libc_printf("wait keytone stop!\n");
                keytone_playend_deal(music_player);
            }
            if (music_player->kt_play_enable == TRUE)
            {
                keytone_stop(music_player);
            }
#endif

            ret = stop_music_decoder_fadeout_wait_cp(music_player);

            break;
        }

        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }

    return ret;
}

/* �������ֽ����� */
int32 stop_music_decoder(music_bt_player_t *music_player)
{
    int32 ret = 0;

    //������ڲ��ţ����������������ȴ���������
    if (music_player->music_status.status == MMM_BP_ENGINE_PLAYING)
    {
        ret = stop_music_decoder_fadeout(music_player);
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    //�ȴ��������������
    while ((music_player->kt_play_enable == TRUE) && (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
    {
        libc_printf("wait keytone stop!\n");
        keytone_playend_deal(music_player);
    }
    if (music_player->kt_play_enable == TRUE)
    {
        keytone_stop(music_player);
    }
#endif

    //���ԣ�����״̬Ӧ��ΪREADY��PAUSE��ERROR
    if ((music_player->music_status.status != MMM_BP_ENGINE_READY)
        && (music_player->music_status.status != MMM_BP_ENGINE_PAUSE)
        && (music_player->music_status.status != MMM_BP_ENGINE_ERROR))
    {
        libc_printf_error("STOP CMD status NO READY&PAUSE&ERROR!\n");
        while (1);
    }

    //�����̲߳�����Ҫ���У����ٽ����߳�
    music_player->is_player_loop_enable = FALSE;
    destroy_decode_thread(music_player);

    //ֹͣAUDIO Ӳ�����������ٽ����ź����ȣ�
    stop_music_decoder_sub(music_player, TRUE);

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        libc_sem_destroy(&(p_tws_info->start_dec_sem));
        p_tws_info->start_dec_sem = NULL;
    }
#endif

    //�ر�������Ч��
    ret = post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
    music_player->audiopp_handle = NULL;
    if (ret != 0)
    {
        libc_printf_error("PP_CMD_CLOSE fail!!\n");
    }

    //ж��������Ч��
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    //�رս����
    ret = sbc_decoder_cmd(music_player->audiodec_handle, AD_CMD_CLOSE, (uint32) NULL);
    music_player->audiodec_handle = NULL;

    //ж�ؽ����
    sys_drv_uninstall (CODEC_GROUP);

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == BP_WORK_MODE_MIC)
    {
        //�ر�ADCͨ��
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_STOP, NULL);
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_CLOSE, NULL);
        sys_drv_uninstall(DRV_GROUP_CHANNEL);
    }
#endif

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1))
    //ֹͣ������˳�¼��
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta != MMM_RECORD_STOP))
    {
        ret = stop_music_record(music_player);
    }
#endif

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if ((p_tws_info->dev_role == TWS_MASTER)
        && (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN))
    {
        //�ر�ADCͨ��
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_STOP, NULL);
        adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_CLOSE, NULL);
        sys_drv_uninstall(DRV_GROUP_CHANNEL);

        //�ر�SBC����
        sbc_encoder_cmd(music_player->sbc_encoder_handle, AE_CMD_CLOSE, NULL);
        sys_drv_uninstall(SP_PRE_GROUP);
    }
#endif

    //״̬�޸�Ϊ MMM_BP_ENGINE_STOPPED��STOP���̽���
    music_player->music_status.status = MMM_BP_ENGINE_STOPPED;

    return ret;
}

//�м���ֶ���Ҳ����øýӿ�
int32 stop_music_decoder_sub(music_bt_player_t *music_player, bool stop)
{
    int32 ret = 0;

    //ֹͣAUDIO Ӳ������
    if (music_player->g_aout_init_flag == 1)
    {
        mmm_bp_aout_close(music_player);
        music_player->g_aout_init_flag = 0;
    }

    //���ٽ����ź���
    if (music_player->bt_dec_sem != NULL)
    {
        libc_sem_post(music_player->bt_dec_sem);
        libc_sem_destroy(&(music_player->bt_dec_sem));
        music_player->bt_dec_sem = NULL;
    }

    music_player->g_bp_pcm_buf_int_en_flag = FALSE;
    music_player->g_bp_pcm_buf_pending = 0;
    music_player->g_start_sub_pre_flag = 0;
    music_player->is_sending_to_pcm_buf_flag = FALSE;
    music_player->send_to_pcm_buf_count = 0;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        ret = tws_stop_music_decoder_sub(music_player, stop);
    }
#endif

    return ret;
}
