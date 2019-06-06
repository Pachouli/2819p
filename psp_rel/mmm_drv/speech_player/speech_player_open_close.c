/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ���м���������򿪺͹رա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ƶ�����м��
 * \param[in]   param  �����ֲ����м���Ĳ�����Ŀǰ����Ϊ��
 * \return       �������
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *mmm_sp_open(void *param)
{
    speech_player_t *speech_player = speech_player_global;

    sys_adjust_clk(FREQ_102M, SET_DECODE_CLK);

    /* ��ʼ��settting */
    speech_player->speech_setting.mic_op_gain = 0;
    speech_player->speech_setting.mic_adc_gain = 0;
    speech_player->speech_setting.default_samplerate = 8;
    speech_player->speech_setting.default_channels = 3;
    speech_player->speech_setting.right_weak = 0;
    speech_player->speech_setting.left_weak = 0;
    speech_player->pre_high_thres = PRE_PROCESS_BUF_HIGH_THRES_DEFAULT*32; //16KHz
    speech_player->pre_mean_thres = PRE_PROCESS_BUF_MEAN_THRES_DEFAULT*32; //16KHz
    speech_player->speech_setting.fill_sco_out = TRUE;


    speech_player->g_p_speech_plc_para = sys_malloc(sizeof(plc_para_t));
    speech_player->g_p_speech_pre_agc_para = sys_malloc(sizeof(agc_para_t));
    speech_player->g_p_speech_post_agc_para = sys_malloc(sizeof(agc_para_t));

    speech_player->g_p_speech_aec_prms = sys_malloc(sizeof(actionsaec_prms_t));
    if (speech_player->g_p_speech_aec_prms == NULL)
    {
        libc_printf("g_p_speech_aec_prms\n");
        while (1);
    }

    //��ʼ��Input Buffer������
    speech_player->sco_input_buf = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_SCO_INPIPE);
    if (speech_player->sco_input_buf == NULL)
    {
        while (1);
    }

    //��ʼ��Output Buffer������
    speech_player->sco_output_buf = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_SCO_OUTPIPE);
    if (speech_player->sco_output_buf == NULL)
    {
        while (1);
    }

    //��ʼ��MIC Input Buffer
    speech_player->mic_input_buf.raw_data = (uint8 *) (MIC_INPUT_BUF_ADDR);
    speech_player->mic_input_buf.capacity = MIC_INPUT_BUF_LEN;
    speech_player->mic_input_buf.length = 0;
    speech_player->mic_input_buf.write_ptr = 0;
    speech_player->mic_input_buf.read_ptr = 0;

    //��ʼ��PRE Process Buffer
    speech_player->pre_process_buf.raw_data = (uint8 *) (PRE_PROCESS_BUF_ADDR);
    speech_player->pre_process_buf.capacity = PRE_PROCESS_BUF_LEN;
    speech_player->pre_process_buf.length = 0;
    speech_player->pre_process_buf.write_ptr = 0;
    speech_player->pre_process_buf.read_ptr = 0;
    libc_memcpy(&(speech_player->post_process_buf), &(speech_player->pre_process_buf), sizeof(buffer_rw_t));

    speech_player->sp_post_buffer = SP_POST_BUFFER_ADDR;
    speech_player->sp_post_buffer_remain = 0;
    speech_player->sp_pre_buffer = SP_PRE_BUFFER_ADDR;
    speech_player->sp_pre_buffer_remain = 0;
    speech_player->sco_pkg_flag = TRUE;

    speech_player->agc_predeal_buf = PRE_AGC_DEAL_BUF_ADDR;

    get_audio_aps_info(&(speech_player->audio_aps_info));

    speech_player->buffer_need_reset = 1;
    speech_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

    speech_player->work_mode = (sp_work_mode_e)(uint32) param;

    //OPEN��״̬�л�ΪSTOPPED
    speech_player->speech_status.status = MMM_SP_ENGINE_STOPPED;

    return (void *) speech_player;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر����ֲ����м��
 * \param[in]   handel  �򿪵����ֲ����м���������
 * \param[out]  param �ر��м�����豣�����ݵ�ָ�룬Ŀǰ����Ϊ��
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 mmm_sp_close(speech_player_t *speech_player, void *param)
{
    if (speech_player->p_asqt_dump_buf != NULL)
    {
        sys_free(speech_player->soc_input_buf_asqt_tempbuf);
        speech_player->soc_input_buf_asqt_tempbuf = NULL;
    }

#if (SUPPORT_SOFT_KEYTONE == 1)
    if (speech_player->kt_play_enable == TRUE)
    {
        keytone_stop(speech_player);
    }
#endif

    sys_adjust_clk(FREQ_0M, SET_DECODE_CLK);

    sys_free(speech_player->g_p_speech_plc_para);
    speech_player->g_p_speech_plc_para = NULL;

    sys_free(speech_player->g_p_speech_pre_agc_para);
    speech_player->g_p_speech_pre_agc_para = NULL;

    sys_free(speech_player->g_p_speech_post_agc_para);
    speech_player->g_p_speech_post_agc_para = NULL;

    sys_free(speech_player->g_p_speech_aec_prms);
    speech_player->g_p_speech_aec_prms = NULL;

    //CLOSE֮��״̬�л�ΪNULL
    speech_player->speech_status.status = MMM_SP_ENGINE_NULL;

    return 0;
}
