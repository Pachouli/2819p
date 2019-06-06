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
 * \file     mmm_mp.h
 * \brief    定义音乐播放中间提供的接口及参数
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_MP_H__
#define __MMM_MP_H__

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

#endif

/*! \cond MMM_MP_API */

/*! vps级别设置范围为[-vps_max_level，vps_max_level]，负数为慢放，正数为快放，0表示vps关闭*/
#define VPS_MAX_LEVEL               8

#define SUPPORT_APE_TYPE   1
#define SUPPORT_FLAC_TYPE  1

#define SUPPORT_EXTLIB_TTS_PLAY 0  //是否支持extlib歌曲播放

#define SUPPORT_LOOP_PLAY             0   //是否支持无缝播放功能
#define SUPPORT_VFS_LOOP_PLAY         0   //是否支持用户磁盘无缝播放功能
#define SUPPORT_SDLIB_LOOP_PLAY       0   //是否支持SD区无缝播放
#define SUPPORT_RES_DIR_LOOP_PLAY     0   //是否支持内置磁盘资源文件无缝播放
#define SUPPORT_EXTLIB_LOOP_PLAY      1   //是否支持外置NORFLASH无缝播放

/*!
 *  \brief
 *  音乐播放错误号，在调用接口返回错误或者中间件（引擎）状态为错误时有效
 */
typedef enum
{
    /*! 打开文件失败 */
    MMM_MP_ERR_OPEN_FILE = 0x10001,
    /*! 文件格式不支持 */
    MMM_MP_ERR_FILE_NOT_SUPPORT = 0x10002,
    /*! 文件被损坏*/
    MMM_MP_ERR_FILE_DAMAGED = 0x10003,
    /*! 解码出错 */
    MMM_MP_ERR_DECODER_ERROR = 0x10004,
    /*! DRM无license */
    MMM_MP_ERR_NO_LICENSE = 0x10005,
    /*! DRM时钟错误 */
    MMM_MP_ERR_SECURE_CLOCK = 0x10006,
    /*! DRM时钟错误 */
    MMM_MP_ERR_LICENSE_INFO = 0x10007,
    /*! 无效操作 */
    MMM_MP_ERR_CMD_ILLEGAL = 0x10008,
    /*! 命令字不存在 */
    MMM_MP_ERR_CMD_NOTEXIST = 0x10009,
    /*! seek返回时间在期望时间之后 */
    MMM_MP_ERR_SEEK_BEHIND = 0x1000A,
    /*! parser加载错误 */
    MMM_MP_ERR_PARSER_LOAD = 0x1000B,
    /*! decoder加载错误 */
    MMM_MP_ERR_DECODER_LOAD = 0x1000C,
    /*! 创建fifo错误 */
    MMM_MP_ERR_FIFO_CREATE = 0x1000D,
    /*! 解码时间超过总时间 */
    MMM_MP_ERR_TIME_OVERFLOW = 0x1000E,
    /*! 输入源和输出设备不匹配 */
    MMM_MP_ERR_IO_MISMATCH = 0x1000F,
    /*! 快进到文件尾不停止 */
    MMM_MP_ERR_FF_END = 0x10010,
    /*! 快进到文件头不停止 */
    MMM_MP_ERR_FB_START = 0x10011,
    /*! parser出错 */
    MMM_MP_ERR_PARSER_ERROR = 0x10012,
    /*! 输出设备支持出错 */
    MMM_MP_ERR_DEVICE_ERROR = 0x10013,
    /*! 读磁盘错误*/
    MMM_MP_ERR_DISK_ERROR = 0x10014,
    /*! 未知错误 */
    MMM_MP_ERR_UNKNOWN = 0x100FF
} mmm_mp_err_t;
/*!
 * \brief
 *      音乐播放提供给应用层的命令字
 */
typedef enum
{
    /*! 分块处理以提高效率 */

    /*! 打开音乐中间件 */
    MMM_MP_OPEN = 0,
    /*! 关闭音乐中间件 */
    MMM_MP_CLOSE,

    /*! 设置音乐播放配置信息 */
    MMM_MP_SET_MUSIC_SETTING,
    /*! 获取音乐播放配置信息 */
    MMM_MP_GET_MUSIC_SETTING,
    /*! 设置快进退 */
    MMM_MP_SET_FFB,
    /*! 设置快进退步长 */
    MMM_MP_FFB_STEP,
    /*! 设置快进退播放时间 */
    MMM_MP_FFB_PLAY_TIME,
    /*! 设置快进退间隔 */
    MMM_MP_FFB_WAIT_TIME,

    /*! 设置播放文件 */
    MMM_MP_SET_FILE,
    /*! 清除播放文件 */
    MMM_MP_CLEAR_FILE,
    /*! 获取媒体信息 */
    MMM_MP_MEDIA_INFO,
    /*! 设置断点播放信息 */
    MMM_MP_SET_BREAK_POINT,
    /*! 获取断点播放信息 */
    MMM_MP_GET_BREAK_POINT,
    /*! 播放 */
    MMM_MP_PLAY,
    /*! 停止 */
    MMM_MP_STOP,

    /*! 获取播放状态 */
    MMM_MP_GET_STATUS,
    /*! 获取错误信息号 */
    MMM_MP_GET_ERRORNO,

    /*! 设置音效 */
    MMM_MP_SET_EQ,
    /*! 设置获取多频点能量值 */
    MMM_MP_SET_FREQ_POINT,

    /*! 播放软按键音 */
    MMM_MP_KEYTONE_PLAY,
    /*! 设置无缝播放 */
    MMM_MP_SET_LOOP_PLAY,
} mmm_mp_cmd_t;
/*!
 *  \brief
 *      音乐播放引擎的状态
 */
typedef enum
{
    /*! 停止状态 */
    MMM_MP_ENGINE_STOPPED = 0,
    /*! 文件设置成功 */
    MMM_MP_ENGINE_FILESET,
    /*! 播放状态 */
    MMM_MP_ENGINE_PLAYING,
    /*! 暂停状态 */
    MMM_MP_ENGINE_PAUSED,
    /*! 播放到头或播放到尾 */
    MMM_MP_ENGINE_PLAYEND,
    /*! 正在淡入，淡入完成后切换为播放状态 */
    MMM_MP_ENGINE_FADEIN,
    /*! 正在淡出，淡出完成后切换为暂停状态 */
    MMM_MP_ENGINE_FADEOUT,
    /*! 出错状态 */
    MMM_MP_ENGINE_ERROR,
    /*! 中间件刚加载未初始化，未知状态 */
    MMM_MP_ENGINE_NULL
} mmm_mp_engine_t;

typedef enum
{
    /* 普通播放 */
    MP_WORK_MODE_NORMAL = 0,
    /* OK MIC播放 */
    MP_WORK_MODE_MIC = 1,
} mp_work_mode_e;

typedef struct
{
    uint16 * sec_ids;
    uint8 sec_cnt;
} mmm_mp_tts_para_t;

#define FS_TYPE_VFS     0
#define FS_TYPE_SDFILE  1
#define FS_TYPE_RESFILE 2
#define FS_TYPE_EXTLIB  3

/*!
 *  \brief
 *     打开文件所需的参数信息
 */
typedef struct
{
    /*! 0: vfs, 1: sd file system */
    uint8 fs_type;
    /*! 0: not tts, 1: tts */
    uint8 is_tts;
    /*! 0: stereo or mono, 1: only mono */
    uint8 only_mono;
    /*! for vfs open only */
    uint32 file_mount_id;
    /*! file name */
    char *file_name;

    /*! tts para */
    mmm_mp_tts_para_t *p_tts_para;
} mmm_mp_fs_para_t;

/*!
 *  \brief
 *     音乐文件信息
 */
typedef struct
{
    /*! 音乐总时间,以毫秒为单位*/
    int total_time;
    /*! 音乐比特率，以Kbps为单位*/
    int bitrate;
    /*! 音乐采样率，以KHz为单位*/
    int sample_rate;
    /*! 采样率，单位Hz */
    int sample_rate_hz;
    /*! 音乐声道数*/
    int channels;
    /*! drm类型，详见mmm_drm_type_t */
    int drm_type;
    /*! 格式类型 */
    int music_type;
    /*! 样本点位数 */
    int bps;
    /*! high ape */
    int high_ape;
} mmm_mp_file_info_t;

/*!
 *  \brief
 *     音乐播放实时信息
 */
typedef struct
{
    /*! 引擎状态，详见mmm_mp_engine_t */
    int status;
    /*! 出错的错误号，详见mmm_mp_err_t */
    int err_no;
    /*! 自动AB播放时的状态， 详见mmm_mp_ab_t*/
    int ab_play_flag;
    /*! 当前的播放时间，单位毫秒 */
    int cur_time;
    /*! 当前比特率，单位kbps */
    int cur_bitrate;
    /*! 当前能量级别 */
    int cur_energy;
    /*! 当前能量值-最大值*/
    short music_cur_energy;
    /*! 当前能量值-平均值*/
    short music_cur_energy_rms;
    /*! 当前能量值-最大值*/
    short mic_cur_energy;
    /*! 当前能量值-平均值*/
    short mic_cur_energy_rms;
    /*! 当前多频点能量值 */
    short freq_point_energy[10];
    /*! 低能量持续时间 */
    int music_lowpw_time;
    /*! Todo spectrum*/
    /*! 软按键音播放状态，详见 mmm_ktp.h 的 mmm_ktp_engine_t 定义 */
    int ktp_status;
} mmm_mp_status_t;
/*!
 *  \brief
 *     音乐播放性能
 */
typedef struct
{
    /*! parser耗的时间 */
    int parser_elapsed;
    /*! decoder耗的时间 */
    int dec_elapsed;
    /*! 读取文件耗的时间 */
    int io_elapsed;
    /*! 其它损耗，如format_check */
    int other_elapsed;
} mmm_mp_performance_t;

/*!
 *  \brief
 *     音乐播放断点信息
 *     跟music_bp_info_t一致
 */
typedef struct
{
    /*! 断点位置的时间，单位ms */
    int bp_time_offset;
    /*! 断点位置在文件中的位置，单位字节 */
    int bp_file_offset;
    /*! 断点播放所需要的辅助信息*/
    /*! 比如章节号码适用于Audible  */
    int bp_info_ext;
} mmm_mp_bp_info_t;

/*!
 *  \brief
 *     音乐输出的设置信息
 */
typedef struct
{
    /*! 默认采样率，单位：KHz，默认值44 */
    int default_samplerate;
    /*! 默认通道数，默认值2 */
    int default_channels;
} mmm_mp_aout_setting_t;

/*!
 *  \brief
 *     音乐播放的设置信息
 */
typedef struct
{
    /*! 系统默认参数，对所有歌曲适用，有设置接口，原则上只设置一次*/
    mmm_mp_aout_setting_t aout_setting;

    /*! ADC CHANNEL参数 */
    adc_chanel_setting_t adc_chan_set;

    /*! 音频输出通道选择，见 typestruct.h 中的定义 */
    int aud_output_chan;

    /*! 淡入淡出 */
    /*! 淡入时间，单位毫秒 */
    int fade_in_time;
    /*! 淡出时间，单位毫秒 */
    int fade_out_time;

    /*! 快进退 */
    /*! 快进退步长，单位毫秒 */
    int ffb_step;
    /*! 快进退延时，单位毫秒 */
    int ffb_delay;
    /*! 快进退播放时间，单位毫秒 */
    int ffb_play_time;

    /*! 蓝牙推歌低能量检测阈值（最大值）；0表示不使用这个条件 */
    int music_lowpw_th_max;
    /*! 蓝牙推歌低能量检测阈值（有效值）；0表示不使用这个条件 */
    int music_lowpw_th_rms;

    /*! 存储介质是否Uhost，Uhost读数比Card慢一点，需要提频 */
    int is_uhost;
  
} music_setting_t;

/*!
 *  \brief
 *      歌曲类型
 */
typedef enum
{
    MUSIC_UNSUPPORT_TYPE = 0,
    MUSIC_MP3_TYPE,
    MUSIC_WMA_TYPE,
    MUSIC_WAV_TYPE,
    MUSIC_FLAC_TYPE,
    MUSIC_APE_TYPE,
    MUSIC_OGG_TYPE,
    MUSIC_AAX_TYPE,
    MUSIC_AUD_TYPE,
    MUSIC_AAC_TYPE,
    MUSIC_AIN_TYPE,
    MUSIC_ACT_TYPE,
} music_file_type_e;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define mmm_mp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_mp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
