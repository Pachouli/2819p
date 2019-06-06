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
#ifndef __MMM_PP_H__
#define __MMM_PP_H__

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
#include <post_process.h>
#include <mmm_ktp.h>
#include <adc_channel_extern.h>
#include <record_module_extern.h>

#endif

#define SUPPORT_MMM_PCM_RECORD_FUNC  1 //是否支持录音功能

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      音乐播放提供给应用层的命令字
 */
typedef enum
{
    /*! 打开音乐中间件 */
    MMM_PP_OPEN = 0,
    /*! 关闭音乐中间件 */
    MMM_PP_CLOSE,

    /*! 设置播放参数，包括AIN通道、采样率、输出通道模式、淡入时间、淡出时间等 */
    MMM_PP_SET_MUSIC_SETTING,

    /*! 播放 */
    MMM_PP_PLAY,
    /*! 停止 */
    MMM_PP_STOP,

    /*! 获取播放状态 */
    MMM_PP_GET_STATUS,
    /*! 获取错误信息号 */
    MMM_PP_GET_ERRORNO,

    /*! 设置音效 */
    MMM_PP_SET_EQ,
    /*! 设置获取多频点能量值 */
    MMM_PP_SET_FREQ_POINT,

    /*! 播放软按键音 */
    MMM_PP_KEYTONE_PLAY,

    /*! 设置录音文件句柄 */
    MMM_PP_RECORDING_SET_FS,
    /*! 设置编码参数 */
    MMM_PP_RECORDING_SET_ENCODE,
    /*! 启动录音，如果BTPLAY未START，那么RECORDING会等到BTPLAY START时再START */
    MMM_PP_RECORDING_START,
    /*! 关闭录音，如果BTPLAY先STOP，那么RECORDING会自动STOP */
    MMM_PP_RECORDING_STOP,
    /*! 获取录音相关状态 */
    MMM_PP_RECORDING_GET_STATUS,

    /*! 关闭监听，默认开启监听，参数：TRUE表示关闭监听，FALSE表示恢复监听 */
    MMM_PP_CLOSE_MONITORING,

    /*! 设置空闲模式，默认非空闲模式 */
    MMM_PP_SET_IDLE_MODE,
} mmm_pp_cmd_t;
/*!
 *  \brief
 *      音乐播放引擎的状态
 */
typedef enum
{
    /*! 停止状态 */
    MMM_PP_ENGINE_STOPPED = 0,
    /*! 准备播放状态，缓冲到足够数据后启动播放流程 */
    MMM_PP_ENGINE_READY,
    /*! 暂停状态 */
    MMM_PP_ENGINE_PAUSE,
    /*! 播放状态 */
    MMM_PP_ENGINE_PLAYING,
    /*! 正在淡入，淡入完成后切换为播放状态 */
    MMM_PP_ENGINE_FADEIN,
    /*! 数据不足启动淡出，正在淡出，淡出完成后切换为READY状态 */
    MMM_PP_ENGINE_FADEOUT,
    /*! STOP命令启动淡出，正在淡出，淡出完成后切换为PAUSE状态 */
    MMM_PP_ENGINE_FADEOUT_PAUSE,
    /*! 出错状态 */
    MMM_PP_ENGINE_ERROR,
    /*! 未OPEN状态，未知状态 */
    MMM_PP_ENGINE_NULL,
} mmm_pp_engine_t;

typedef enum
{
    /* 普通播放 */
    PP_WORK_MODE_NORMAL = 0,
    /* OK MIC播放 */
    PP_WORK_MODE_MIC = 1,
    /* KEYTONE播放 */
    PP_WORK_KEYTONE = 2,
} pp_work_mode_e;

/*!
 *  \brief
 *     音乐播放实时信息
 */
typedef struct
{
    /*! 引擎状态，详见mmm_pp_engine_t */
    int status;
    adc_energy_info_t adc_energy;
    /*! 当前多频点能量值 */
    short freq_point_energy[10];
    /*! 软按键音播放状态，详见 mmm_ktp.h 的 mmm_ktp_engine_t 定义 */
    int ktp_status;
} mmm_pp_status_t;

#define PCM_SOURCE_AUX  0
#define PCM_SOURCE_USB  1

/*!
 *  \brief
 *     音乐播放的设置信息
 */
typedef struct
{
    adc_chanel_setting_t adc_chan_set;
    /*! PCM源选择，0表示AUX，1表示FM，2表示USB */
    int pcm_source;

    /*! 默认采样率，单位：KHz，默认值44 */
    int default_samplerate;
    /*! 默认通道数，默认值2 */
    int default_channels;

    /*! 音频输出通道选择，见 typestruct.h 中的定义 */
    int aud_output_chan;

    /*! 淡入时间，单位毫秒 */
    int fade_in_time;
    /*! 淡出时间，单位毫秒 */
    int fade_out_time;
    /*! 开始播放延时，单位毫秒 */
    int start_delay_time;

} music_pcm_setting_t;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define mmm_pp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_pp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
