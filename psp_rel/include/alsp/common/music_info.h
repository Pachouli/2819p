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
 * \brief    ������Ƶ����������ĸ�����Ϣ�ṹ
 * \author   jpliao
 * \version 1.0
 * \date  2011/03/18
*******************************************************************************/
#ifndef __MUSIC_INFO_H__
#define __MUSIC_INFO_H__


/*!
 *  \brief
 *      ��������
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
 *      ��Ƶ�������ⷵ�صĸ�����Ϣ�ṹ
 */
typedef struct
{
    /*! ��������music_type_t  */
    int music_type;
    /*! ��ʱ�䣬��λms */
    int total_time;
    /*! ƽ�������ʣ���λkbps */
    int avg_bitrate;
    /*! �����ʣ���λkHz */
    int sample_rate;
    /*! �����ʣ���λHz */
    int sample_rate_hz;
    /*! ������ */
    int channels;
    /*! ������λ�� */
    int bps;
    /*! ÿ֡�������� */
    int block_size;
    /*! ÿ֡�������� mp3 frame/wma packet/ogg page */
    int frame_size;
    /*! ������ */
    int sub_format;
    /*! drm��� */
    int drm_flag;
    /*! drm key buffer*/
    void *key_buf;
    /*! �ļ����ȣ���λbyte */
    unsigned int file_len;
    /*! �ļ�ͷ�����ȣ���λbyte */
    unsigned int file_hdr_len;
    /*! storage��� */
    void *storage_io;
    /*! �������ݿռ���ʼλ�� */
    void *data_buf_addr;
    /*! �������ݿռ䳤�� */
    int data_buf_len;
    /*! ��Ƶ�������������������ṹ���ض���ʽԼ�� */
    void *buf;
    /*! ��Ƶ����ģʽ */
    int decode_mode;
    /*! ��Ƶ�������λ�� */
    int output_bps;
} music_info_t;

/*!
 * \brief
 *      ��Ƶ�������ⷵ�صĸ����ϵ���Ϣ�ṹ
 *      WAV/MP3/WMA�ȸ�ʽ�ϵ������ļ�λ�ú����Ӧ��ʱ��Ӧ���Ǿ�ȷ��
 */
typedef struct
{
    /*! �ϵ�λ�õ�ʱ�䣬��λms */
    int bp_time_offset;
    /*! �ϵ�λ�����ļ��е�λ�ã���λ�ֽ� */
    unsigned int bp_file_offset;
    /*! �ϵ㲥������Ҫ�ĸ�����Ϣ*/
    /*! �����½ں���������Audible  */
    int bp_info_ext;
} music_bp_info_t;

/*!
 * \brief
 *      ��Ƶ�������ⷵ�صĽ���״̬��Ϣ�ṹ
 */
typedef struct
{
    /*! ����״̬���룬���ض���ʽ����Լ����0�������� */
    int status;
    /*! ��ǰ����ʱ�䣬��λms */
    int cur_time;
    /*! ��ǰ�����ʣ���λkbps */
    int cur_bitrate;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! ��ǰ֡������������ݽṹ��ָ�� */
    void *aout;
    /*! ��ǰ�ļ�λ�ã���λbyte */
    unsigned int cur_file_offset;
    /*! ��ǰ�������ĵ����ݣ���λbyte */
    int bytes_used;
    /*! ��ǰ����ϵ���Ϣ */
    music_bp_info_t music_bp_info;
} music_decode_info_t;


/*!
 * \brief
 *      ��Ƶ�������ⷵ�صı���״̬��Ϣ�ṹ
 */
typedef struct
{
    /*! ����״̬���룬���ض���ʽ����Լ����0�������� */
    int status;
    /*! ��ǰ����ʱ�䣬��λms */
    int cur_time;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! ��ǰ������� */
    int silence_flag;
    /*! ��ǰPCM���ݽṹ��ָ�� */
    void *ain;
    /*! ��ǰ֡���ݽṹ��ָ�� */
    void *aout;
    /*! ��ǰ��������ݣ���λbyte */
    int bytes_used;
    /*! ����֡������ȣ���λbyte */
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
