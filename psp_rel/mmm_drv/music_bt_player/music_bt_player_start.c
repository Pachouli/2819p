/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м����ʼ���Ŵ��������Ƹ�ʱ����Ƶ�������ģ�ֻ�е����������嵽
 *       �㹻ʱ��ʱ�ŻῪ����Ƶ����������������������ֹͣ���ź��������ָ�Ҳ��Ҫ
 *       �߿�ʼ���Ŵ������̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"
#include "music_bt_player_tws_if.h"
#include "sysdef.h"

uint8 __section__(".bss.pcm_adc_buffer") PCM_ADC_BUF_ADDR[PCM_ADC_BUF_LEN/2]; //DMA Block��һ�㣬��ʱ���Զ�һ��

uint8 __section__(".bss.input_buffer") PCM_INPUT_BUF_ADDR[PCM_INPUT_BUF_LEN];

uint8 __section__(".stack.ap_high_stack_ptos")ap_high_stk_ptos[1];//��̬����
extern uint16 _AP_HIGH_STK_SIZE;

#define DRV_NAME_SIZE 12
static const char dec_sbc_name[DRV_NAME_SIZE] = "dec_sbc.al";
static const char aduiopp_name[DRV_NAME_SIZE] = "audiopp.al";

static const char adc_channel_name[DRV_NAME_SIZE] = "adc_chan.al";
static const char enc_sbc_name[DRV_NAME_SIZE] = "enc_sbc.al";

int32 creat_decode_thread(music_bt_player_t *music_player)
{
    pthread_param_t player_thread_para;

    player_thread_para.start_rtn = (void *)player_loop;
    player_thread_para.arg = (void *) music_player;
    player_thread_para.ptos = (OS_STK *) ap_high_stk_ptos;
    player_thread_para.stk_size = (uint32)&_AP_HIGH_STK_SIZE;

    /* �����м���̲߳��������� */
    if (libc_pthread_create(&player_thread_para, AP_BACK_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        music_player = NULL;
        return -1;
    }

    return 0;
}

/* �������ֽ����� */
int32 start_music_decoder(music_bt_player_t *music_player)
{
    int32 ret = 0;
    sbc_open_para_t sbc_open_para;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if ((p_tws_info->dev_role == TWS_MASTER)
        && (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN))
    {
        sbc_enc_open_para_t sbc_enc_open_para;

        //����SBC������
        if ((int32) sys_drv_install(SP_PRE_GROUP, 0, enc_sbc_name) == -1)
        {
            while (1);
        }
        else
        {
            //�򿪽����
            sbc_enc_open_para.sbc_type = SBC_TYPE_SBC;
            sbc_enc_open_para.samplerate = SBC_FREQ_44100;
            sbc_enc_open_para.bitpool = 0x35;
            ret = sbc_encoder_cmd(&music_player->sbc_encoder_handle, AE_CMD_OPEN, &sbc_enc_open_para);
            if ((ret == -1) || (music_player->sbc_encoder_handle == NULL))
            {
                libc_printf_error("sbc enc open fail!\n");
                while (1);
            }
        }

        //��ADCͨ�������ò�����������ADCͨ��
        if ((int32) sys_drv_install(DRV_GROUP_CHANNEL, 0, adc_channel_name) == -1)
        {
            while (1);
        }
        else
        {
            adc_channel_buffer_setting_t adc_chan_buffer_set;

            adc_channel_cmd(&(music_player->adc_channel_handle), ADC_CHANNEL_OPEN, NULL);
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SETTING, &(music_player->music_setting.adc_chan_set));

            adc_chan_buffer_set.adc_chan_buffer_1k = PCM_ADC_BUF_ADDR;
            adc_chan_buffer_set.pcm_adc_block_size = PCM_ADC_BUF_BLOCK/2; //DMA Block��һ�㣬��ʱ���Զ�һ��
            adc_chan_buffer_set.adc_chan_cache = PCM_INPUT_BUF_ADDR;
            adc_chan_buffer_set.adc_chan_cache_len = AUX_TWS_PCM_INPUT_BUF_LEN;
            adc_chan_buffer_set.mic_chan_cache = 0;
            adc_chan_buffer_set.mic_chan_cache_len = 0;
            adc_chan_buffer_set.mic_aux_temp_buffer_256 = 0;
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SET_BUFFER, &adc_chan_buffer_set);

            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_PIPE, &(music_player->p_input_buf_r));
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_START, NULL);
        }
    }
#endif

#if (SUPPORT_OK_MIC == 1)
    if (music_player->work_mode == BP_WORK_MODE_MIC)
    {
        //��MICͨ�������ò�����������ADCͨ��
        if ((int32) sys_drv_install(DRV_GROUP_CHANNEL, 0, adc_channel_name) == -1)
        {
            while (1);
        }
        else
        {
            adc_channel_buffer_setting_t adc_chan_buffer_set;

            adc_channel_cmd(&(music_player->adc_channel_handle), ADC_CHANNEL_OPEN, NULL);
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SETTING, &(music_player->music_setting.adc_chan_set));

            adc_chan_buffer_set.adc_chan_buffer_1k = PCM_ADC_BUF_ADDR;
            adc_chan_buffer_set.pcm_adc_block_size = PCM_ADC_BUF_BLOCK/2; //DMA Block��һ�㣬��ʱ���Զ�һ��
            adc_chan_buffer_set.adc_chan_cache = 0;
            adc_chan_buffer_set.adc_chan_cache_len = 0;
            adc_chan_buffer_set.mic_chan_cache = PCM_INPUT_BUF_ADDR;
            adc_chan_buffer_set.mic_chan_cache_len = PCM_INPUT_BUF_LEN;
            adc_chan_buffer_set.mic_aux_temp_buffer_256 = 0;
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_SET_BUFFER, &adc_chan_buffer_set);

            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_GET_MIC_PIPE, &(music_player->p_mic_input_buf_r));
            adc_channel_cmd(music_player->adc_channel_handle, ADC_CHANNEL_START, NULL);
        }
    }
#endif

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1))
    //�������ź��Զ�����¼��
    if ((music_player->record_enable == TRUE) && (music_player->mmm_record_sta == MMM_RECORD_STOP))
    {
        ret = start_music_record(music_player);
    }
#endif

    //���ؽ����
    ret = (int32) sys_drv_install(CODEC_GROUP, 0, dec_sbc_name);
    if (ret == -1)
    {
        goto start_music_decoder_fail_ret_0;
    }
    //�򿪽����
    sbc_open_para.sbc_type = SBC_TYPE_SBC;
    sbc_open_para.mute_info.enable_mute = FALSE;
    sbc_open_para.mute_info.mute_pcm_abs_limit = 0;
    sbc_open_para.mute_info.mute_pcm_abs_avg_limit = 0;
    sbc_open_para.mute_info.mute_pcm_samples_limit = 0;
    ret = sbc_decoder_cmd(&music_player->audiodec_handle, AD_CMD_OPEN, &sbc_open_para);
    if ((ret == -1) || (music_player->audiodec_handle == NULL))
    {
        libc_printf_error("sbc open fail!\n");
        goto start_music_decoder_fail_ret_1;
    }

    //����������Ч��
    ret = (int32) sys_drv_install(SOUND_EFFECT_GROUP, 0, aduiopp_name);
    if (ret == -1)
    {
        goto start_music_decoder_fail_ret_2;
    }

    //��������Ч��
    ret = post_process_ops(&(music_player->audiopp_handle), PP_CMD_OPEN, 0);
    if ((ret == -1) || (music_player->audiopp_handle == NULL))
    {
        libc_printf_error("audiopp open fail!!\n");
        goto start_music_decoder_fail_ret_3;
    }

    //���������߳�
    if (creat_decode_thread(music_player) == -1)
    {
        ret = -1;
        goto start_music_decoder_fail_ret_4;
    }

    //״̬�л�Ϊ READY
    music_player->music_status.status = MMM_BP_ENGINE_READY;
    music_player->is_player_loop_enable = TRUE;

    music_player->ok_mic_start_flag = OK_MIC_DATA_FLUSH;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        libc_sem_init(&(p_tws_info->start_dec_sem), 0);
    }
#endif

    return ret;


    start_music_decoder_fail_ret_4:
    //�ر�������Ч��
    post_process_ops(music_player->audiopp_handle, PP_CMD_CLOSE, 0);
    music_player->audiopp_handle = NULL;

    start_music_decoder_fail_ret_3:
    //ж��������Ч��
    sys_drv_uninstall (SOUND_EFFECT_GROUP);

    start_music_decoder_fail_ret_2:
    //�رս����
    sbc_decoder_cmd(music_player->audiodec_handle, AD_CMD_CLOSE, (uint32) NULL);
    music_player->audiodec_handle = NULL;

    start_music_decoder_fail_ret_1:
    //ж�ؽ����
    sys_drv_uninstall (CODEC_GROUP);

    start_music_decoder_fail_ret_0:

    return ret;
}

static const uint16 freq_valus[] =
{
    0, 4, 8, 14, 26, 30, 36, 42, 48, 54, 60, 66,72, 78, 84, 90, 96, 102,
    108, 114, 120, 126, 132, 138, 144, 150, 156, 162 ,168,
    174, 180, 186, 192,198, 204, 210, 216, 222,
};
static uint8 _get_freq_index(uint32 freq_value)
{
    uint8 index;

    for (index = 0; index < sizeof(freq_valus)/sizeof(uint16); index++)
    {
        if (freq_value <= freq_valus[index])
        {
            break;
        }
    }

    if (index < FREQ_26M)
    {
        index = FREQ_26M;
    }

    return index;
}

void set_music_decoder_clk(music_bt_player_t *music_player)
{
    uint16 set_freq;

    if (music_player->audiopp_handle == NULL)
    {
        return;
    }

    set_freq = music_player->dae_freq + music_player->decode_freq + music_player->encode_freq;
    if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
    {
        set_freq += 30; //���� Cache Miss
    }
    else
    {
        set_freq += 10; //���� Cache Miss
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_FAST == 1))
    if (music_player->work_mode == BP_WORK_MODE_MIC)
    {
        set_freq += 222;
    }
#endif

    sys_adjust_clk(_get_freq_index(set_freq), SET_DECODE_CLK);
}

//Ԥ����ʼ���ţ��Լ���������ʼ���ŵ�ʱ��
int32 start_music_decoder_sub_pre(music_bt_player_t *music_player)
{
    int32 ret = 0;

    if (music_player->g_dae_pm_init_flag == 0)
    {
        music_player->g_dae_pm_init_flag = 1;

        //����DAE����������DAE����������
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);

        //���� DECODE_CLK
#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
        if (p_tws_info->dev_role != NORMAL_DEV)
        {
            music_player->decode_freq = 25; //TWS����������Ϊ35MIPS��ȥ��SBC�����Rcode������������5M
        }
        else
#endif
        {
            music_player->decode_freq = 20; //����������Ϊ30MIPS��ȥ��SBC�����Rcode������������5M
        }

        if (music_player->g_sbc_channel_mode == SBC_CH_MODE_JOINT_STEREO)
        {
            music_player->decode_freq += 8; //joint stereo ģʽ����������8M�����ֲ���cache miss
            libc_printf("joint stereo\n");
        }

        if (music_player->music_setting.default_samplerate == 48)
        {
            music_player->decode_freq += 4; //48KHz����������������1/10
        }

        if (music_player->music_setting.data_source == MMM_SOURCE_ADC_CHAN)
        {
            music_player->decode_freq += 15; //���뼰ADC����������Ϊ20M
        }

        set_music_decoder_clk(music_player);
    }

    //����������Ч�����ȽϺ�ʱ���ʷ���Ԥ�����������Խ�ʡ������ʼ���ŵ�ʱ��
    music_player->fade_flag = 1;
    music_player->dae_update_flag = 1;
    update_PP_para_info(music_player);

    //��ʼ��AUDIO DAC��Ӳ����ʼ����    �ŵ�SBC�����ò����ʺ�������AOUT
    if (music_player->g_aout_init_flag == 0)
    {
        mmm_bp_aout_open(music_player);
        music_player->g_aout_init_flag = 1;
    }

    //���� AUDIO APS �Լ���������ػ���
    set_audio_aps (music_player->audio_aps_info.fast_aps);
    music_player->audio_aps = music_player->audio_aps_info.fast_aps;
    music_player->adjust_sample_counter = 0;
    music_player->adjust_sample_high_counter = 0;
    music_player->buffer_time_min = (uint16) -1;
    music_player->bt_zero_frame = 0;
    music_player->bt_lowpw_frame = 0;

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO
    if (p_tws_info->dev_role != NORMAL_DEV)
    {
        ret = tws_start_music_decoder_sub_pre(music_player);
    }
#endif

    return ret;
}

//ĳЩ��Ҫ�ȵ�������ȷ������ܳ�ʼ���Ĺ���
//�м���ֶ�����ָ�����Ҳ����øýӿ�
int32 start_music_decoder_sub(music_bt_player_t *music_player)
{
    int32 ret = 0;

    //���ԣ�����״̬Ӧ��ΪREADY
    if (music_player->music_status.status != MMM_BP_ENGINE_READY)
    {
        libc_printf_error("START SUB status NO READY!\n");
        while (1);
    }

    //�Խ����߳���������
    music_player->music_status.status = MMM_BP_ENGINE_FADEIN;
    libc_printf_info("fadein:%d\n", music_player->music_setting.fade_in_time);

    music_player->decode_pp_frame_total = 0;
    music_player->decode_pp_frame_index = 0;

    return ret;
}

