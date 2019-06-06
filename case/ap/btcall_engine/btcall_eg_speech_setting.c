/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话各算法模块参数配置接口。
 * 作者：cailizhen
 ********************************************************************************/

#include  "app_btcall_engine.h"

#define QCONST32(x,bits) ((unsigned int)(.5+(x)*(float)(((unsigned int)1)<<(bits))))

//有效值转换为峰值，假定人声的峰值因数为15db
const int16 agc_level_tab[] =
{
    5691, //0 = -30db ~ -40db
    6386, //1 = -29db
    7165, //2 = -28db
    8039, //3 = -27db
    9020, //4 = -26db
    10120, //5 = -25db
    11355, //6 = -24db
    12741, //7 = -23db
    14295, //8 = -22db
    16039, //9 = -21db
    17996, //10 = -20db
    20191, //11 = -19db
    22655, //12 = -18db
    25419, //13 = -17db
    28520, //14 = -16db
    32000, //15 = -15db ~ -12db
};

const int32 agc_level_tab_fix[] =
{
    32768,  //0 = 0db
    34716,  //1 = 0.5db
    36781,  //2 = 1db
    38968,  //3 = 1.5db
    41285,  //4 = 2db
    43740, //5 = 2.5db
    46341, //6 = 3db
    49097, //7 = 3.5db
    52016, //8 = 4db
    55109, //9 = 4.5db
    58386, //10 = 5db
    61858, //11 = 5.5db
    65536, //12 = 6db
    69433, //13 = 6.5db
    73562, //14 = 7db
    77936, //15 = 7.5db
};

void audio_setting_init_prms(speech_setting_t *p_audio_sett, asqt_configs_t *p_asqt_cfg)
{
    asqt_sv_configs_t *p_asqt_sv_cfg = (asqt_sv_configs_t *) p_asqt_cfg;

    if (p_asqt_sv_cfg != NULL)
    {
        p_audio_sett->mic_op_gain = p_asqt_sv_cfg->mic_op_gain;
        p_audio_sett->mic_adc_gain = p_asqt_sv_cfg->mic_adc_gain;
        p_audio_sett->aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
        p_audio_sett->default_samplerate = 8;
        p_audio_sett->default_channels = p_asqt_sv_cfg->dac_enable;
        p_audio_sett->right_weak = 0;
        p_audio_sett->left_weak = 0;
        if (p_asqt_sv_cfg->dac_enable == SPEECH_CHAN_RIGHT_LEFT)
        {
            if ((p_asqt_sv_cfg->dac_weak >= 1) && (p_asqt_sv_cfg->dac_weak <= 18))
            {
                p_audio_sett->left_weak = p_asqt_sv_cfg->dac_weak;
            }
            else if ((p_asqt_sv_cfg->dac_weak >= 33) && (p_asqt_sv_cfg->dac_weak <= 50))
            {
                p_audio_sett->right_weak = p_asqt_sv_cfg->dac_weak - 32;
            }
        }

        if (p_asqt_sv_cfg->sv_prms.analysis_mode == 1)
        {
            p_audio_sett->fill_sco_out = TRUE;
        }
        else
        {
            p_audio_sett->fill_sco_out = FALSE;
        }
    }
    else
    {
#if (SUPPORT_OK_MIC == 1)
        p_audio_sett->mic_op_gain = (int8) com_get_config_default(OK_MIC_MICIN_OP_GAIN);
        p_audio_sett->mic_adc_gain = (int8) com_get_config_default(OK_MIC_MICIN_ADC_GAIN);
        p_audio_sett->aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
#else
        p_audio_sett->mic_op_gain = (int8) com_get_config_default(BTCALL_MICIN_OP_GAIN);
        p_audio_sett->mic_adc_gain = (int8) com_get_config_default(BTCALL_MICIN_ADC_GAIN);
        p_audio_sett->aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
#endif
        p_audio_sett->default_samplerate = 8;
        p_audio_sett->default_channels = (int8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_ENABLE);
        p_audio_sett->right_weak = 0;
        p_audio_sett->left_weak = 0;
        if (p_audio_sett->default_channels == SPEECH_CHAN_RIGHT_LEFT)
        {
            int8 dac_weak_sel = (int8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_WEAK_SEL);
            int8 dac_weak_val = (int8) com_get_config_default(BTCALL_SPEAKER_CHANNEL_WEAK_VAL);

            if (dac_weak_sel == 2)
            {
                p_audio_sett->left_weak = dac_weak_val;
            }
            else if (dac_weak_sel == 1)
            {
                p_audio_sett->right_weak = dac_weak_val;
            }
        }

        p_audio_sett->fill_sco_out = TRUE;
    }
}

void plc_init_prms(plc_para_t *p_plc_para, asqt_configs_t *p_asqt_cfg)
{
    asqt_sv_configs_t *p_asqt_sv_cfg = (asqt_sv_configs_t *) p_asqt_cfg;

    if (p_asqt_sv_cfg != NULL)
    {

    }

    //丢包补偿模块开关，1：打开，0：关闭
    p_plc_para->plc_enable = (int) com_get_config_default(BTCALL_PLC_ENABLE);
}

void preagc_init_prms(agc_para_t *p_agc_para, asqt_configs_t *p_asqt_cfg)
{
    asqt_sv_configs_t *p_asqt_sv_cfg = (asqt_sv_configs_t *) p_asqt_cfg;
    int temp_value;

    if (p_asqt_sv_cfg != NULL)
    {
        p_agc_para->agc_enable = p_asqt_sv_cfg->pre_agc_enable;
        temp_value = p_asqt_sv_cfg->pre_agc_level;
    }
    else
    {
        //预处理AGC使能:预处理自动增益控制使能，用于放大对方的声音，1：打开，0：关闭
        p_agc_para->agc_enable = (int) com_get_config_default(BTCALL_AGC_PRETREAT_ENABLE);

        //预处理AGC目标值:预处理自动增益控制目标值，默认设置为10 = -20db。
        temp_value = (int) com_get_config_default(BTCALL_AGC_PRETREAT_LEVEL);
    }

    //按照手机免提标准进行设置， 12000 ~ 32000
    if (com_get_config_default(BTCALL_AGC_PRETREAT_MODE) == 0)
    {
        p_agc_para->agc_mode = 2;//自动增益
    }
    else
    {
        p_agc_para->agc_mode = 5;//固定增益
    }

    if (p_agc_para->agc_mode == 2)
    {
        if ((temp_value >= 0) && (temp_value <= 15))
        {
            temp_value = agc_level_tab[temp_value];
        }
        else if ((temp_value < agc_level_tab[0]) || (temp_value > agc_level_tab[15]))
        {
            libc_printf_warning("agc_level invalid! %d\n", temp_value);
            temp_value = agc_level_tab[10];
        }
    }
    else
    {
        if ((temp_value < 0) || (temp_value > 15))
        {
            temp_value = 8;
        }
        temp_value = agc_level_tab_fix[temp_value];
    }
    p_agc_para->agc_level = temp_value;

    /*放大是最大的增益，零：disable，非零：增益上限值*/
    p_agc_para->minimum_gain = (int) QCONST32(0.5f, 15);
    /*缩小时的最小增益，零：disable，非零：增益下限值*/
    p_agc_para->maximum_gain = (int) QCONST32(22.624f, 15);
    /*噪声门槛值，范围200-1000，推荐值400*/
    p_agc_para->noise_gate_threshold = 100;
}

void postagc_init_prms(agc_para_t *p_agc_para, asqt_configs_t *p_asqt_cfg)
{
    asqt_sv_configs_t *p_asqt_sv_cfg = (asqt_sv_configs_t *) p_asqt_cfg;
    int temp_value;

    if (p_asqt_sv_cfg != NULL)
    {
        p_agc_para->agc_enable = p_asqt_sv_cfg->post_agc_enable;
        temp_value = p_asqt_sv_cfg->post_agc_level;
    }
    else
    {
        //后端AGC使能:预处理自动增益控制使能，用于放大对方的声音，1：打开，0：关闭
        p_agc_para->agc_enable = (int) com_get_config_default(BTCALL_AGC_POSTREAT_ENABLE);

        //后端AGC目标值:预处理自动增益控制目标值，默认设置为10 = -20db。
        temp_value = (int) com_get_config_default(BTCALL_AGC_POSTREAT_LEVEL);
    }

    //按照手机免提标准进行设置， 12000 ~ 32000
    if (com_get_config_default(BTCALL_AGC_POSTREAT_MODE) == 0)
    {
        p_agc_para->agc_mode = 2;//自动增益
    }
    else
    {
        p_agc_para->agc_mode = 5;//固定增益
    }

    if (p_agc_para->agc_mode == 2)
    {
        if ((temp_value >= 0) && (temp_value <= 15))
        {
            temp_value = agc_level_tab[temp_value];
        }
        else if ((temp_value < agc_level_tab[0]) || (temp_value > agc_level_tab[15]))
        {
            libc_printf_warning("agc_level invalid! %d\n", temp_value);
            temp_value = agc_level_tab[10];
        }
    }
    else if (p_agc_para->agc_mode == 5)
    {
        if ((temp_value < 0) || (temp_value > 15))
        {
            temp_value = 8;
        }
        temp_value = agc_level_tab_fix[temp_value];
    }
    p_agc_para->agc_level = temp_value;

    /*放大是最大的增益，零：disable，非零：增益上限值*/
    p_agc_para->minimum_gain = (int) QCONST32(0.5f, 15);
    /*缩小时的最小增益，零：disable，非零：增益下限值*/
    p_agc_para->maximum_gain = (int) QCONST32(8.0f, 15);
    /*噪声门槛值，范围200-1000，推荐值400*/
    p_agc_para->noise_gate_threshold = 800;
}

void actaec_init_sv_prms(actionsaec_prms_t *sv_prms, asqt_configs_t *p_asqt_cfg, bool att_flag)
{
    asqt_sv_configs_t *p_asqt_sv_cfg = (asqt_sv_configs_t *) p_asqt_cfg;

    if (p_asqt_sv_cfg != NULL)
    {
        libc_memcpy(sv_prms, &(p_asqt_sv_cfg->sv_prms), sizeof(actionsaec_prms_t));
        //回声尾长，单位 ms，可支持32ms，64ms，96ms
        sv_prms->tail_length = (int) com_get_config_default(BTCALL_AEC_TAIL_LENGTH);

        if (sv_prms->tail_length > 64)
        {
            sv_prms->tail_length = 64; //运算性能不够，ASQT模式下最多只能支持64ms尾长
        }
    }
    else
    {
        sv_prms->frame_size = 16; //帧长时间，单位ms，目前仅支持 16ms 帧长
        sv_prms->sampling_rate = 8000; //采样率，单位Hz，目前仅支持 8000Hz 采样率

        // aec prms
        //aec模块开关，1：打开，0：关闭
        sv_prms->aec_enable = (int) com_get_config_default(BTCALL_AEC_MODULE_ENABLE);
        if (att_flag == TRUE)
        {
            sv_prms->aec_enable = FALSE; //自动化测试模式关闭AEC，防止半双工模式会被MIC声音过滤掉
        }
        //回声尾长，单位 ms，可支持32ms，64ms，96ms，128ms
        sv_prms->tail_length = (int) com_get_config_default(BTCALL_AEC_TAIL_LENGTH);

        //非线性处理子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为 1
        sv_prms->nlp_enable = (int) com_get_config_default(BTCALL_AEC_NLP_ENABLE);
        //非线性处理程度，整数0~15,越大代表非线性处理越厉害
        sv_prms->nlp_aggressiveness = (int) com_get_config_default(BTCALL_AEC_NLP_AGGRESSIVENESS);
        //双讲近端语音保留程度，整数0~15，越大表示双讲是近端语音保留越多，但是可能会带来单讲的回声，需找个折中值。
        sv_prms->dt_threshold = (int) com_get_config_default(BTCALL_AEC_DT_THRESHOLD);

        //残留回声抑制子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为1
        sv_prms->aes_enable = (int) com_get_config_default(BTCALL_AEC_AES_ENABLE);
        //回声抑制子模块单讲回声抑制比，单位dB， 0~40
        sv_prms->echo_suppress = (int) com_get_config_default(BTCALL_AEC_ECHO_SUPPRESS);
        //回声抑制子模块双讲回声抑制比，单位dB， 0~40
        sv_prms->echo_suppress_avtive = (int) com_get_config_default(BTCALL_AEC_ECHO_SUPPRESS_ACTIVE);

        //ns prms
        //降噪开关，1：打开，0：关闭
        sv_prms->ans_enable = (int) com_get_config_default(BTCALL_AEC_ANS_ENABLE);
        //噪声抑制比，单位dB， 0~25
        sv_prms->noise_suppress = (int) com_get_config_default(BTCALL_AEC_NOISE_SUPPRESS);

        //cng prms
        //舒适噪声开关，1：打开，0：关闭
        sv_prms->cng_enable = (int) com_get_config_default(BTCALL_AEC_CNG_ENABLE);
        //舒适噪声水平，单位 dB Fs, 一般应小于-45 dB；
        //实际水平比该值大5db左右，所以这里-5；对应默认值为-45
        sv_prms->noise_floor = (int) (int16) com_get_config_default(BTCALL_AEC_NOISE_FLOOR);

        //打电话模式
        sv_prms->analysis_mode = 1;
        //单双工模式选择，0表示全双工，1表示半双工
        sv_prms->half_mode = (int) (int16) com_get_config_default(BTCALL_AEC_HALF_FULL_MODE);
    }
}

