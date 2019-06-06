/*******************************************************************************
 *                              5110
 *                            Module: musicdec
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kkli     2011-3-18 17:18     1.0             build this file
*******************************************************************************/
/*!
 * \file     music_info.h
 * \brief    定义音频解码插件输出的歌曲信息结构
 * \author   jpliao
 * \version 1.0
 * \date  2011/03/18
*******************************************************************************/
#ifndef __MUSIC_INFO_H__
#define __MUSIC_INFO_H__


/*!
 *  \brief
 *      歌曲类型
 */
typedef enum
{
    UNSUPPORT_TYPE = 0,
    MP3_TYPE,
    WMA_TYPE,
    WAV_TYPE,
    APE_TYPE,
    FLAC_TYPE,
}music_type_t;

/*!
 * \brief
 *      音频解码插件库返回的歌曲信息结构
 */
typedef struct
{
    /*! 歌曲类型music_type_t  */
    int music_type;
    /*! 总时间，单位ms */
    int total_time;
    /*! 平均比特率，单位kbps */
    int avg_bitrate;
    /*! 采样率，单位kHz */
    int sample_rate;
    /*! 采样率，单位Hz */
    int sample_rate_hz;
    /*! 声道数 */
    int channels;
    /*! 样本点位数 */
    int bps;
    /*! 每帧样本点数 */
    int block_size;
    /*! 每帧样本点数 mp3 frame/wma packet/ogg page */
    int frame_size;
    /*! 子类型 */
    int sub_format;
    /*! drm标记 */
    int drm_flag;
    /*! drm key buffer*/
    void *key_buf;
    /*! 文件长度，单位byte */
    unsigned int file_len;
    /*! 文件头部长度，单位byte */
    unsigned int file_hdr_len;
    /*! storage句柄 */
    void *storage_io;
    /*! 可用数据空间起始位置 */
    void *data_buf_addr;
    /*! 可用数据空间长度 */
    int data_buf_len;
    /*! 音频解码所需的输入参数，结构由特定格式约定 */
    void *buf;
    /*! 音频解码模式 */
    int decode_mode;
    /*! 音频解码输出位宽 */
    int output_bps;
} music_info_t;

/*!
 * \brief
 *      音频解码插件库返回的歌曲断点信息结构
 *      WAV/MP3/WMA等格式断点所在文件位置和其对应的时间应该是精确的
 */
typedef struct
{
    /*! 断点位置的时间，单位ms */
    int bp_time_offset;
    /*! 断点位置在文件中的位置，单位字节 */
    unsigned int bp_file_offset;
    /*! 断点播放所需要的辅助信息*/
    /*! 比如章节号码适用于Audible  */
    int bp_info_ext;
} music_bp_info_t;

/*!
 * \brief
 *      音频解码插件库返回的解码状态信息结构
 */
typedef struct
{
    /*! 解码状态代码，由特定格式自行约定，0代表正常 */
    int status;
    /*! 当前解码时间，单位ms */
    int cur_time;
    /*! 当前比特率，单位kbps */
    int cur_bitrate;
    /*! 当前能量级别 */
    int cur_energy;
    /*! 当前帧解码结果输出数据结构的指针 */
    void *aout;
    /*! 当前文件位置，单位byte */
    unsigned int cur_file_offset;
    /*! 当前解码消耗的数据，单位byte */
    int bytes_used;
    /*! 当前解码断点信息 */
    music_bp_info_t music_bp_info;
} music_decode_info_t;


/*!
 * \brief
 *      音频编码插件库返回的编码状态信息结构
 */
typedef struct
{
    /*! 解码状态代码，由特定格式自行约定，0代表正常 */
    int status;
    /*! 当前解码时间，单位ms */
    int cur_time;
    /*! 当前能量级别 */
    int cur_energy;
    /*! 当前静音标记 */
    int silence_flag;
    /*! 当前PCM数据结构的指针 */
    void *ain;
    /*! 当前帧数据结构的指针 */
    void *aout;
    /*! 当前编码的数据，单位byte */
    int bytes_used;
    /*! 编码帧输出长度，单位byte */
    int bytes_enced;
} music_encode_info_t;

//for SBC Decode
typedef struct
{
    void *input;//usually char
    int input_len;//usually bytes
    int decode_len;//usually bytes, included skipped bytes
    int frame_len;//usually bytes, only frame true length bytes
    void *output;//usually short, L,R,L,R
    int output_len;//usually bytes
    int output_bps;//usually 16
    int status;//reserve
    int cur_time;
    int cur_bitrate;
    int cur_energy;
} music_frame_info_t;

typedef struct
{
    int enable_mute;
    int mute_pcm_abs_limit;
    int mute_pcm_abs_avg_limit;
    int mute_pcm_samples_limit;
} mute_info_t;

#define SBC_TYPE_SBC  0
#define SBC_TYPE_MSBC 1

typedef struct
{
    int sbc_type;  //0 sbc, 1 msbc
    mute_info_t mute_info;
} sbc_open_para_t;

#define SBC_FREQ_16000		0x00
#define SBC_FREQ_32000		0x01
#define SBC_FREQ_44100		0x02
#define SBC_FREQ_48000		0x03
typedef struct
{
    int sbc_type;  //0 sbc, 1 msbc
    int samplerate;
    int bitpool;
} sbc_enc_open_para_t;

#define SBC_CH_MODE_MONO          0x00
#define SBC_CH_MODE_DUAL_CHANNEL  0x01
#define SBC_CH_MODE_STEREO        0x02
#define SBC_CH_MODE_JOINT_STEREO  0x03

#endif // __MUSIC_INFO_H__
