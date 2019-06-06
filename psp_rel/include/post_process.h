/*******************************************************************************
 *                              5003
 *                            Module: musicdec
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-18 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     audio_decoder_lib_dev.h
 * \brief    ��Ƶ���������Ҫ��ͷ�ļ�
 * \author   kkli
 * \version 1.0
 * \date  2008/09/18
 *******************************************************************************/
#ifndef __PROST_PROCESS_H__
#define __PROST_PROCESS_H__

#include <psp_includes.h>
#include <audio_resample.h>

#define SUPPORT_OK_MIC              0   //�Ƿ�֧�ֿ���OK MIC����
#define ENABLE_OK_MIC_FAST          1   //�Ƿ�ʹ�ܿ��ٵ���ʱģʽ��Ƶ���ܵ���ߣ����Ļ��һЩ

#define SUPPORT_SOFT_KEYTONE        0   //�Ƿ�֧��������������Ƶ����

#define EQ_NUM_BANDS                14  //2����ͨ�˲���9����Ƶ��Ƶ�㣬����3�����ʹ��

#define POST_PROCESS_FRAME_SAMPLE   128 //DAE Block �̶�Ϊ 128��������(��)

#define OK_MIC_DATA_FLUSH     0 //�������
#define OK_MIC_DATA_BUFFER    1 //��������
#define OK_MIC_DATA_READ      2 //��ȡ����

//noise gate
#define NG_ENABLE             0
#define NG_THRESHOLD          -750
#define NG_ATTACK_TIME        1000
#define NG_RELEASE_TIME       10
#define NG_RATIO              5
#define NG_SET0_THRESHOLD     0

typedef struct
{
    /*����Ƶ�㣨���߸�ͨ�˲����Ľ�ֹƵ�ʣ�����λHz*/
    short cutoff;
    /*Qֵ����λ0.1���Ƽ���Χ��3��30*/
    short q;
    /*���棬��λ0.1db���Ƽ���Χ��-240��+120*/
    short gain;
    /*�˲������ͣ�0����Ч��1�ǳ���EQ�˲�����2�Ǹ�ͨ�˲���������ֵΪreserve*/
    short type;
} peq_band_t;

typedef struct
{
    short          threshold;
    unsigned short detect_attact_time;
    unsigned short detect_release_time;
    unsigned short attact_time;
    unsigned short release_time;
    unsigned short cutoff_freq;
    unsigned short q;
    unsigned short type;
} drc_band_t;

#define OUTPUT_CHANNEL_NORMAL      0 //������Ĭ�ϳ�ʼ��Ϊ����
#define OUTPUT_CHANNEL_L_R_SWITCH  1 //������������ L = R0, R = L0
#define OUTPUT_CHANNEL_L_R_MERGE   2 //���������ϲ���������������������� L = R = (L0 + R0) / 2
#define OUTPUT_CHANNEL_L_R_ALL_L   3 //������������������� L = R = L0
#define OUTPUT_CHANNEL_L_R_ALL_R   4 //������������������� L = R = R0

#define KT_MIX_MODE_NONE    0 //��Ӱ�첥��ͨ��
#define KT_MIX_MODE_MIX     1 //��ϵ�����ͨ��
#define KT_MIX_MODE_REPLACE 2 //�滻����ͨ��

/*for CASE*/
typedef struct
{
    uint16 magic;

    int16 precut;

    bool aset_mode;
    bool bypass;
    bool peq_enable;
    bool limiter_enable;
    bool kt_limiter_enable;
    bool drc_enable;
    uint8 drc_mode;
    uint8 peq_version;
    int16 soft_volume;
    int16 kt_soft_volume;//ok mic
    int16 pre_softvol_gain;
    int16 post_softvol_gain;
    int16 drc_precut;

    int16 limiter_threshold;
    uint16 limiter_attack_time;
    uint16 limiter_release_time;

    peq_band_t peq_bands[EQ_NUM_BANDS];
    uint8 music_eq_id[EQ_NUM_BANDS];

    uint8 ok_mic_mute;
    uint8 kt_peq_band_num;//speech or ok mic
    int16 kt_limiter_threshold;//ok mic
    uint16 kt_limiter_attack_time;//ok mic
    uint16 kt_limiter_release_time;//ok mic

    uint16 drc_crossover;
    drc_band_t drc_bands[2];

    uint8 mic_mix_mode_l_chan;
    uint8 mic_mix_mode_r_chan;

    bool   echo_enable;
    uint8  echo_ratio_index;
    uint16 echo_delay;
    uint16 echo_ratio;
    uint16 echo_lowpass_fc1;
    uint16 echo_lowpass_fc2;

    bool freq_shift_enable;

    bool voice_removal_enable;

    bool   ducking_enable;
    uint16 ducking_detect_time;
    uint16 ducking_attack_time;
    uint16 ducking_release_time;
    short  ducking_side_chain_threshold;
    short  ducking_main_chain_target_gain;
} dae_config_t;

typedef struct
{
    bool  freq_point_enable;
    uint8 freq_point_num;
    short freq_point[10];
} freq_point_config_t;

#define MIC_MODE_DISABLE    0 //��ֹMICͨ��
#define MIC_MODE_BASIC      1 //����˫ͨ������������ͨ����Ҫ���Ⱥ�MIC PEQ����
#define MIC_MODE_OK_MIC     2 //����OK����

#define ECHO_BUF_SIZE          0x1000
#define ECHO_TAIL_BUF_SIZE     0xa00
#define FREQ_SHIFT_BUF_SIZE    0x590

typedef int32 (*post_process_update_ops)(void *p_info);
typedef struct
{
    post_process_update_ops update_ops;
    dae_config_t *p_dae_cfg;
    freq_point_config_t *p_fp_config;
    void * p_pp_p_info;
    uint8 *lch_buffer;
    uint8 *rch_buffer;
    uint8 *mic_lch_buffer;
    uint8 *mic_rch_buffer;
    uint8 *echo_buf[6];
    uint8 *freq_shift_buf;
    uint16 frame_sample;
    uint8  sample_rate_khz;
    uint8  aud_output_chan;
    uint8  fade_flag;
    uint8  mic_mode        : 2;
    uint8  idle_mode       : 1; //����ģʽ�����ྲ��������ЧBypass
    uint8  dae_update_flag : 1;
    uint8  ng_enable       : 1; //Noise gate enable
    uint16 fade_in_time;
    uint16 fade_out_time;
    uint16 ret_dae_freq; //���ز���
} post_process_detail_t;

#endif  // __PROST_PROCESS_H__
