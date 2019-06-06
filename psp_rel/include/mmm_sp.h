/*! \cond MMM_MP_API*/
/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao    2010-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mmm_pp.h
 * \brief    定义LINEIN、RADIO、USB Speaker播放中间提供的接口及参数
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_SP_H__
#define __MMM_SP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      加载系统头文件
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#include <mmm_sp_common.h>
#include <post_process.h>
#include <mmm_ktp.h>
#include <record_module_extern.h>

#endif

#define SUPPORT_MMM_SPEECH_RECORD_FUNC  0 //是否支持录音功能

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      音乐播放提供给应用层的命令字
 */
typedef enum
{
    /*! 打开音乐中间件 */
    MMM_SP_OPEN = 0,
    /*! 关闭音乐中间件 */
    MMM_SP_CLOSE,

    /*! 设置播放参数，包括MIC通道、采样率等 */
    /*! 设置PLC参数 */
    /*! 设置PRE AGC参数 */
    /*! 设置POST AGC参数 */
    /*! 设置AEC参数 */
    MMM_SP_SET_SPEECH_SETTING_EXT,

    /*! 播放 */
    MMM_SP_PLAY,
    /*! 停止 */
    MMM_SP_STOP,

    /*! 设置ASQT DUMP BUFFER */
    MMM_SP_SET_ASQT_DUMP_BUF,

    /*! 获取播放状态 */
    MMM_SP_GET_STATUS,
    /*! 获取错误信息号 */
    MMM_SP_GET_ERRORNO,

    /*! 设置音效 */
    MMM_SP_SET_EQ,

    /*! 播放软按键音 */
    MMM_SP_KEYTONE_PLAY,

    /*! 设置HFP Connected，回声消除开始计算收敛时间 */
    MMM_SP_SET_HFP_CONNECTED,

    /*! 设置录音文件句柄 */
    MMM_SP_RECORDING_SET_FS,
    /*! 设置编码参数 */
    MMM_SP_RECORDING_SET_ENCODE,
    /*! 启动录音，如果BTPLAY未START，那么RECORDING会等到BTPLAY START时再START */
    MMM_SP_RECORDING_START,
    /*! 关闭录音，如果BTPLAY先STOP，那么RECORDING会自动STOP */
    MMM_SP_RECORDING_STOP,
    /*! 获取录音相关状态 */
    MMM_SP_RECORDING_GET_STATUS,
} mmm_sp_cmd_t;

/*!
 *  \brief
 *      音乐播放引擎的状态
 */
typedef enum
{
    /*! 停止状态 */
    MMM_SP_ENGINE_STOPPED = 0,
    /*! 准备状态，已经开始预处理 */
    MMM_SP_ENGINE_READY,
    /*! 播放状态 */
    MMM_SP_ENGINE_PLAYING,
    /*! 出错状态 */
    MMM_SP_ENGINE_ERROR,
    /*! 未OPEN状态，未知状态 */
    MMM_SP_ENGINE_NULL,
} mmm_sp_engine_t;

typedef enum
{
    /* 普通打电话 */
    SP_WORK_MODE_NORMAL = 0,
    /* OK MIC 练唱模式 */
    SP_WORK_MODE_MIC = 1,
} sp_work_mode_e;

/*!
 *  \brief
 *     音乐播放实时信息
 */
typedef struct
{
    /*! 引擎状态，详见mmm_sp_engine_t */
    int status;
    /*! 软按键音播放状态，详见 mmm_ktp.h 的 mmm_ktp_engine_t 定义 */
    int ktp_status;
    /*! 能量值 */
    short play_cur_energy;
    short play_cur_energy_rms;
    short mic_cur_energy;
    short mic_cur_energy_rms;
} mmm_sp_status_t;

#define SPEECH_CHAN_ONLY_RIGHT   1
#define SPEECH_CHAN_ONLY_LEFT    2
#define SPEECH_CHAN_RIGHT_LEFT   3

/*!
 *  \brief
 *     音乐播放的设置信息
 */
typedef struct
{
    /*! MIC OP Gain，0-7分别表示26db，30db，31.5db，33db，34.5db，36db，37.5db，39db */
    int8 mic_op_gain;
    /*! MIC ADC Gain，0-3分别表示0db，0db，6db，12db */
    int8 mic_adc_gain;

    /*! 音频输出通道选择，见 typestruct.h 中的定义 */
    int8 aud_output_chan;

    /*! 采样率，单位：KHz，默认值8 */
    int8 default_samplerate;
    /*! 喇叭通道，1表示右声道，2表示左声道，3表示双声道 */
    int8 default_channels;

    /*! 右声道衰减，0-18表示衰减0db ~ -18db */
    int8 right_weak;
    /*! 左声道衰减，0-18表示衰减0db ~ -18db */
    int8 left_weak;

    /*! 是否填充SCO OUT BUFFER */
    bool fill_sco_out;

    /*! 是否需要SCO数据预处理 */
    bool use_sco_data_predeal;
    /*! 是否需要SCO数据后处理 */
    bool use_sco_data_postdeal;
    /*! SCO编码类型，0表示CVSD，1表示MSBC */
    uint8 sco_codec_type;
} speech_setting_t;

typedef struct
{
    /*PLC 使能*/
    int plc_enable;
} plc_para_t;

typedef struct
{
    /*agc使能*/
    int agc_enable;
    /*目标幅度：4000-24000，推荐值8000*/
    int agc_level;
    /*扩展参数，暂未实现*/
    int agc_mode;
    /*放大是最大的增益，零：disable，非零：增益上限值*/
    int minimum_gain;
    /*缩小时的最小增益，零：disable，非零：增益下限值*/
    int maximum_gain;
    /*噪声门槛值，范围200-1000，推荐值400*/
    int noise_gate_threshold;
} agc_para_t;

typedef struct
{
    int frame_size; //帧长时间，单位ms，目前仅支持 16ms 帧长
    int sampling_rate; //采样率，单位Hz，目前仅支持 8000Hz 采样率
    int aec_enable; //aec模块开关，1：打开，0：关闭
    int tail_length; //回声尾长，单位 ms，必须为16的倍数,目前仅支持 32 ms
    int nlp_enable; //非线性处理子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为 1
    int nlp_aggressiveness; //非线性处理程度，整数0~15,越大代表非线性处理越厉害
    int dt_threshold; //双讲近端语音保留程度，整数0~15，越大表示双讲是近端语音保留越多，但是可能会带来单讲的回声，需找个折中值。
    int aes_enable; //残留回声抑制子模块开关，1：打开，0：关闭，为 1 时必须保证 aec_enable 为1
    int echo_suppress; //回声抑制子模块单讲回声抑制比，单位dB， 0~40
    int echo_suppress_avtive; //回声抑制子模块双讲回声抑制比，单位dB， 0~40
    int ans_enable; //降噪开关，1：打开，0：关闭
    int noise_suppress; //噪声抑制比，单位dB， 0~25
    int cng_enable; //舒适噪声开关，1：打开，0：关闭
    int noise_floor; //舒适噪声水平，单位 dB Fs, 一般应小于-45 dB
    int analysis_mode; //测试模式，0表示模拟打电话模式，1表示打电话模式
    int half_mode; //半双工模式，0表示全双工，1表示半双工
} actionsaec_prms_t; //NOTE：必须跟 actionsaec_struct.h 的 sv_prms_t 结构体定义一致

//界面设置参数
typedef struct
{
    int mic_op_gain; //MIC模拟增益：0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db，默认为0
    int mic_adc_gain; //MIC数字增益：0: 0dB(bypass)，1: 0dB(bypass)，2: 6dB(x2)，3: 12dB(x4)
    int dac_enable; //DAC使能：1表示右声道，2表示左声道，3表示双声道，默认为1
    int dac_weak; //声道衰减，0~18db；1-18（衰减1db-18db）表示左喇叭，33-50（衰减1db-18db）表示右喇叭，默认为0
    int volume_call_gain; //打电话音量增益，单位-1db，0~9 表示 0db ~ -9db
    int pre_agc_enable; //前端AGC使能与否，默认为1
    int pre_agc_level; //前端AGC level，12000 - 32000，默认20000

    int post_agc_enable; //后端AGC使能与否，默认为1
    int post_agc_level; //后端AGC level，12000 - 32000，默认20000

    actionsaec_prms_t sv_prms; //AEC 参数
} asqt_sv_configs_t;

typedef struct
{
	speech_setting_t  speech_setting;
	plc_para_t        plc_para;
	agc_para_t        pre_agc_para;
	agc_para_t        post_agc_para;
	actionsaec_prms_t aec_prms;
} speech_setting_ext_t;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define mmm_sp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_sp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
