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
 * \file     mmm_bp.h
 * \brief    定义LINEIN、RADIO、USB Speaker播放中间提供的接口及参数
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_BP_H__
#define __MMM_BP_H__

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

//#define DEC_ENABLE_TRUE_WIRELESS_STEREO //解码是否支持TWS
#define SUPPORT_TWS_EXCPT_RESET  1 //是否支持TWS异常恢复
#define SUPPORT_MMM_BT_RECORD_FUNC  0 //是否支持录音功能

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      音乐播放提供给应用层的命令字
 */
typedef enum
{
    /*! 打开音乐中间件 */
    MMM_BP_OPEN = 0,
    /*! 关闭音乐中间件 */
    MMM_BP_CLOSE,

    /*! 设置播放参数，包括输出通道模式、淡入时间、淡出时间等 */
    MMM_BP_SET_MUSIC_SETTING,

    /*! 播放 */
    MMM_BP_PLAY,
    /*! 停止 */
    MMM_BP_STOP,

    /*! 获取播放状态 */
    MMM_BP_GET_STATUS,
    /*! 获取错误信息号 */
    MMM_BP_GET_ERRORNO,

    /*! 设置音效 */
    MMM_BP_SET_EQ,
    /*! 设置获取多频点能量值 */
    MMM_BP_SET_FREQ_POINT,

    /*! 播放软按键音 */
    MMM_BP_KEYTONE_PLAY,

    /*! 设置TWS播放模式和参数，没有设置就是单箱播放模式；该接口应放在 MMM_BP_SET_MUSIC_SETTING 之后马上调用 */
    MMM_BP_SET_TWS_MODE,
    /*! 设置LINEIN TWS ADC SBC ENCODE Send frame 回调函数 */
    MMM_BP_SET_SEND_FRAME_CBK,

    /*! 设置录音文件句柄 */
    MMM_BP_RECORDING_SET_FS,
    /*! 设置编码参数 */
    MMM_BP_RECORDING_SET_ENCODE,
    /*! 启动录音，如果BTPLAY未START，那么RECORDING会等到BTPLAY START时再START */
    MMM_BP_RECORDING_START,
    /*! 关闭录音，如果BTPLAY先STOP，那么RECORDING会自动STOP */
    MMM_BP_RECORDING_STOP,
    /*! 获取录音相关状态 */
    MMM_BP_RECORDING_GET_STATUS,
} mmm_bp_cmd_t;

/*!
 *  \brief
 *      音乐播放引擎的状态
 */
typedef enum
{
    /*! 停止状态 */
    MMM_BP_ENGINE_STOPPED = 0,
    /*! 准备播放状态，缓冲到足够数据后启动播放流程 */
    MMM_BP_ENGINE_READY,
    /*! 暂停状态 */
    MMM_BP_ENGINE_PAUSE,
    /*! 正在淡入，淡入完成后切换为播放状态 */
    MMM_BP_ENGINE_FADEIN,
    /*! 播放状态 */
    MMM_BP_ENGINE_PLAYING,
    /*! 数据不足启动淡出，正在淡出，淡出完成后切换为READY状态 */
    MMM_BP_ENGINE_FADEOUT,
    /*! STOP命令启动淡出，正在淡出，淡出完成后切换为PAUSE状态 */
    MMM_BP_ENGINE_FADEOUT_PAUSE,
    /*! 出错状态 */
    MMM_BP_ENGINE_ERROR,
    /*! 未OPEN状态，未知状态 */
    MMM_BP_ENGINE_NULL,
} mmm_bp_engine_t;

typedef enum
{
    /* 普通单箱 */
    BP_WORK_MODE_NORMAL = 0,
    /* 对箱 */
    BP_WORK_MODE_TWS = 1,
    /* 单箱+MIC */
    BP_WORK_MODE_MIC = 2,
    /* 对箱+MIC */
    BP_WORK_MODE_TWS_MIC = 3,
} bp_work_mode_e;

/*!
 *  \brief
 *     音乐播放实时信息
 */
typedef struct
{
    /*! 引擎状态，详见mmm_bp_engine_t */
    uint8 status;
    /*! 软按键音播放状态，详见 mmm_ktp.h 的 mmm_ktp_engine_t 定义 */
    uint8 ktp_status;
    /*! 主箱丢包过多 */
    bool lost_pkg_too_more_flag;

    /*! 当前能量值-最大值*/
    short bt_cur_energy;
    /*! 当前能量值-平均值*/
    short bt_cur_energy_rms;
    /*! 当前能量值-最大值*/
    short mic_cur_energy;
    /*! 当前能量值-平均值*/
    short mic_cur_energy_rms;
    /*! 当前多频点能量值 */
    short freq_point_energy[10];
    /*! 低能量持续时间 */
    int bt_lowpw_time;
    /*! ADC CHANNEL 能量 */
    adc_energy_info_t adc_energy;
} mmm_bp_status_t;

typedef enum
{
    MMM_SOURCE_AVDTP_STREAM = 0,
    MMM_SOURCE_ADC_CHAN = 1,
} mmm_data_source_e;

/*!
 *  \brief
 *     音乐播放的设置信息
 */
typedef struct
{
    /*! 数据流源头 AVDTP STREAM/AUX CHANNEL */
    int data_source;
    /*! ADC CHANNEL参数 */
    adc_chanel_setting_t adc_chan_set;

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

    /*! 蓝牙推歌低能量检测阈值（最大值）；0表示不使用这个条件 */
    int bt_lowpw_th_max;
    /*! 蓝牙推歌低能量检测阈值（有效值）；0表示不使用这个条件 */
    int bt_lowpw_th_rms;
} music_bt_setting_t;

//TWS模式播放相关参数
typedef struct
{
    /*! TWS角色，主箱还是副箱；单箱播放则是Normal */
    dev_role_e dev_role;
    /*! TWS箱体位置，左箱还是右箱；单箱播放则是双声道 */
    dev_spk_pos_e dev_spk;
    /*! 获取蓝牙时间戳 */
    f_get_bt_clk tws_get_bt_clk;
    /*! 设置同步触发的蓝牙时间戳 */
    f_set_bt_clk tws_set_bt_clk;
    /*! 设置同步触发回调函数 */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! 是否发生AVDTP数据包丢失 */
    f_has_avdtp_pkg_lost tws_has_avdtp_pkg_lost;
} music_bt_tws_setting_t;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define mmm_bp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_bp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
