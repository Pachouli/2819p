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
 * \brief    音频解码库所需要的头文件
 * \author   kkli
 * \version 1.0
 * \date  2008/09/18
 *******************************************************************************/
#ifndef __PROST_PROCESS_H__
#define __PROST_PROCESS_H__

#include <psp_includes.h>
#include <audio_resample.h>

#define SUPPORT_OK_MIC              0   //是否支持卡拉OK MIC处理
#define ENABLE_OK_MIC_FAST          1   //是否使能快速低延时模式，频率跑到最高，功耗会高一些

#define SUPPORT_SOFT_KEYTONE        0   //是否支持软按键音及短音频播放

#define EQ_NUM_BANDS                14  //2个高通滤波，9个倍频程频点，再留3个灵活使用

#define POST_PROCESS_FRAME_SAMPLE   128 //DAE Block 固定为 128个样本对(点)

#define OK_MIC_DATA_FLUSH     0 //清空数据
#define OK_MIC_DATA_BUFFER    1 //缓存数据
#define OK_MIC_DATA_READ      2 //读取数据

//noise gate
#define NG_ENABLE             0
#define NG_THRESHOLD          -750
#define NG_ATTACK_TIME        1000
#define NG_RELEASE_TIME       10
#define NG_RATIO              5
#define NG_SET0_THRESHOLD     0

typedef struct
{
    /*中心频点（或者高通滤波器的截止频率），单位Hz*/
    short cutoff;
    /*Q值，单位0.1，推荐范围是3到30*/
    short q;
    /*增益，单位0.1db，推荐范围是-240到+120*/
    short gain;
    /*滤波器类型：0是无效，1是常规EQ滤波器，2是高通滤波器，其它值为reserve*/
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

#define OUTPUT_CHANNEL_NORMAL      0 //正常，默认初始化为正常
#define OUTPUT_CHANNEL_L_R_SWITCH  1 //左右声道互换 L = R0, R = L0
#define OUTPUT_CHANNEL_L_R_MERGE   2 //左右声道合并后输出到左声道和右声道 L = R = (L0 + R0) / 2
#define OUTPUT_CHANNEL_L_R_ALL_L   3 //左右声道都输出左声道 L = R = L0
#define OUTPUT_CHANNEL_L_R_ALL_R   4 //左右声道都输出右声道 L = R = R0

#define KT_MIX_MODE_NONE    0 //不影响播放通道
#define KT_MIX_MODE_MIX     1 //混合到播放通道
#define KT_MIX_MODE_REPLACE 2 //替换播放通道

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

#define MIC_MODE_DISABLE    0 //禁止MIC通道
#define MIC_MODE_BASIC      1 //基础双通道，比如蓝牙通话需要喇叭和MIC PEQ处理
#define MIC_MODE_OK_MIC     2 //卡拉OK功能

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
    uint8  idle_mode       : 1; //空闲模式，伴奏静音并且音效Bypass
    uint8  dae_update_flag : 1;
    uint8  ng_enable       : 1; //Noise gate enable
    uint16 fade_in_time;
    uint16 fade_out_time;
    uint16 ret_dae_freq; //返回参数
} post_process_detail_t;

#endif  // __PROST_PROCESS_H__
