/*! \cond MMM_MR_API */
/*******************************************************************************
 *                              5110
 *                            Module: music
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao     2011-02-01 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mmm_mr.h
 * \brief    定义音乐录音中间件提供的接口及参数
 * \author   jpliao
 * \version 1.0
 * \date  2011/02/01
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_MR_H__
#define __MMM_MR_H__

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
#include <record_module_extern.h>
#endif

/*!
 * \brief
 *      当前codec的状态,即mmm_mr_status_t中status取值
 */
typedef enum
{
    /*初始状态*/
    MMM_MR_ENGINE_INIT = 0,
    /*就绪状态*/
    MMM_MR_ENGINE_READY,
    /*! 编码中 */
    MMM_MR_ENGINE_ENCODING,
    /*! 暂停  */
    MMM_MR_ENGINE_PAUSED,
    /*! 编码器停止 */
    MMM_MR_ENGINE_STOPPED,
    /*! 编码错误 */
    MMM_MR_ENGINE_ERROR,
} mmm_mr_engine_t;

//监听 monitor 状态机
typedef enum
{
    /*! 初始状态 */
    MMM_MR_ENGINE_MNT_INIT = 0,
    /*! 停止监听 */
    MMM_MR_ENGINE_MNT_PAUSED,
    /*! 监听淡入 */
    MMM_MR_ENGINE_MNT_FADEIN,
    /*! 监听中 */
    MMM_MR_ENGINE_MNT_PLAYING,
    /*! 监听淡出  */
    MMM_MR_ENGINE_MNT_FADEOUT,
    /*! 监听错误 */
    MMM_MR_ENGINE_MNT_ERROR,
} mmm_mr_engine_mnt_t;

/*!
 * \brief
 *      录音中间件提供给应用层的命令字
 */
typedef enum
{
    /*! 打开 */
    MMM_MR_OPEN = 0,
    /*! 关闭 */
    MMM_MR_CLOSE,
    /*! 打开声音输入 */
    MMM_MR_AIN_OPEN,
    /*! 关闭声音输入 */
    MMM_MR_AIN_CLOSE,

    /*! 开始录音 */
    MMM_MR_START,
    /*! 停止录音 */
    MMM_MR_STOP,
    /*! 设置录音文件 */
    MMM_MR_SET_FILE,
    /*! 关闭录音文件 */
    MMM_MR_CLEAR_FILE,
    /*! 设置录音参数 */
    MMM_MR_AUDIO_PARAM,
    /*! 获取编码状态 */
    MMM_MR_GET_STATUS,
    /*! 获取错误号 */
    MMM_MR_GET_ERRORNO,
    /*! 单线程测试接口 */
    MMM_MR_SINGLE_THREAD,

	/*! 设置降噪参数: 0，不降噪，1-10为降噪级别，越大降噪越厉害*/
	MMM_MR_SET_DENOISE,
	/*! 设置自动增益控制参数：0，无需自动增益控制，1: 控制在-6b左右*/
	MMM_MR_SET_AGC,

} mmm_mr_cmd_t;

/*!
 * \brief
 *      录音中间件提供给应用层的回调函数命令字
 */
typedef enum
{
    /*! 写入文件头 */
    MMM_MR_CB_FIRST_HEAD_WRITE = 0x0,
    /*! 大文件分曲 */
    MMM_MR_CB_TRACK_ON,
    /*! 磁盘写满 */
    MMM_MR_CB_DISK_FULL,
    /*! 命令个数 */
    MMM_MR_CB_MAX,


} mmm_mr_cb_cmd_t;



/*!
 * \brief
 *      定义录音源的类型
 */
typedef enum
{
    /*! MIC */
    AI_SOURCE_MIC,
    /*! FM */
    AI_SOURCE_FM,
    /*! LINEIN */
    AI_SOURCE_LINEIN,
    /*! 不选择输入 */
    AI_SOURCE_NULL,
} mmm_ai_type_t;

/*!
 *  \brief
 *     音乐输出的设置信息
 */
typedef struct
{
    /*! 输入来源 ，参见 mmm_ai_type_t*/
    int input_source;
    /*! 是否监听，0: 不监听, 1:监听 */
    int listening_flag;
    /*! 输出方式， 0:非直驱，1:直驱 */
    int drive_mode;
    /*! 输入音源增益级别，0-7,推荐值：FM/LINE IN 2, MIC IN 7; step 1.5db*/
    int input_gain;
    /*! ADC增益级别，0-0x3f,推荐值：FM/LINE IN 0x2f, MIC IN 0x33; step 1.5db*/
    int adc_gain;
    /*! 数字增益级别，0,1,2, 零：disable，非零: enable，step 6db */
    int digital_gain;
} mmm_mr_ain_setting_t;

/*!
 *  \brief
 *     音频编码的参数设置
 *
 * \li     wav只需设采样率。比特率计算公式为：ADPCM: 采样率*4*2，PCM：采样率*16*2
 * \li     mp3录音参数设置一览表
 * \par
 <div class="generic_text_mapping_table">
 * <table>
 * <tr>
 * <th>质量</th>
 * <th>可选通道数</th>
 * <th>可选采样率</th>
 * <th>可选比特率</th>
 * </tr>
 * <tr>
 * <td>低</td>
 * <td>1</td>
 * <td>8k,11025,12k,16k</td>
 * <td>[8,64]</td>
 * </tr>
 * <tr>
 * <td>中</td>
 * <td>1,2</td>
 * <td>22050,24k</td>
 * <td>[64,128]</td>
 * </tr>
 * <tr>
 * <td>高</td>
 * <td>1,2</td>
 * <td>32k,44100,48k</td>
 * <td>[64,192]</td>
 * </tr>
 * </table>
 * </div>
 */
typedef struct
{
    /*! 音频编码的参数*/
    record_module_encode_para_t encode_para;

    /*! 采样频率如8=8Khz，22=22.05Khz，44=44.1Khz等*/
    int sample_rate;
    /*! 控制模式：0:正常录音, 1:声控模式, 2:自动分曲 */
    int vad_mode;
    /*! 静音检测门限值[0,128],用于确定门限值，为零表示采用初始录音的噪声平均值，非零则-0.5dB一级, 默认值 64 */
    int vad_threshold;
    /*! 静音检测延迟时间,单位ms, 默认值 3000 */
    int vad_delay;
    /*! 降噪级别参数: 0，不降噪，1-10为降噪级别，越大降噪越厉害 */
    int denoise_level;
    /*! 音频输出通道选择，见 typestruct.h 中的定义 */
    int aud_output_chan;
    /*! 淡入时间，单位毫秒 */
    int fade_in_time;
    /*! 淡出时间，单位毫秒 */
    int fade_out_time;
    /*! 是否后台蓝牙模式 */
    int is_bt_inback;
} mmm_mr_audio_param_t;

/*!
 * \brief
 *      录音中间件返回给应用层的状态信息
 */
typedef struct
{
    /*! 当前的录音时间*/
    int time;
    /*! 当前codec的状态*/
    int status;
    /*! 输出当前实时能量级别,暂时不支持*/
    int energy_level;
    /*! 当前是否是静音，在静音检测功能开启时有效, 零：有声，非零：静音*/
    int silence_flag;
    /*! 在出错状态时返回的错误号*/
    int err_no;
    /*! 当前录音写到文件中的数据量，单位：512字节即1个page*/
    int write_pages;
    /*! 当前录音文件句柄首簇信息*/
    uint32 start_cluster;
} mmm_mr_status_t;


#define AUDIO_MMM_MR_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define mmm_mr_cmd(a,b,c)           ((int32(*)(void *handel, mmm_mr_cmd_t cmd, uint32 args))AUDIO_MMM_MR_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif
#endif // __MMM_MR_H__
