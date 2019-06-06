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
 * \brief    �������ֲ����м��ṩ�Ľӿڼ�����
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
 *      ����ϵͳͷ�ļ�
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

/*! vps�������÷�ΧΪ[-vps_max_level��vps_max_level]������Ϊ���ţ�����Ϊ��ţ�0��ʾvps�ر�*/
#define VPS_MAX_LEVEL               8

#define SUPPORT_APE_TYPE   1
#define SUPPORT_FLAC_TYPE  1

#define SUPPORT_EXTLIB_TTS_PLAY 0  //�Ƿ�֧��extlib��������

#define SUPPORT_LOOP_PLAY             0   //�Ƿ�֧���޷첥�Ź���
#define SUPPORT_VFS_LOOP_PLAY         0   //�Ƿ�֧���û������޷첥�Ź���
#define SUPPORT_SDLIB_LOOP_PLAY       0   //�Ƿ�֧��SD���޷첥��
#define SUPPORT_RES_DIR_LOOP_PLAY     0   //�Ƿ�֧�����ô�����Դ�ļ��޷첥��
#define SUPPORT_EXTLIB_LOOP_PLAY      1   //�Ƿ�֧������NORFLASH�޷첥��

/*!
 *  \brief
 *  ���ֲ��Ŵ���ţ��ڵ��ýӿڷ��ش�������м�������棩״̬Ϊ����ʱ��Ч
 */
typedef enum
{
    /*! ���ļ�ʧ�� */
    MMM_MP_ERR_OPEN_FILE = 0x10001,
    /*! �ļ���ʽ��֧�� */
    MMM_MP_ERR_FILE_NOT_SUPPORT = 0x10002,
    /*! �ļ�����*/
    MMM_MP_ERR_FILE_DAMAGED = 0x10003,
    /*! ������� */
    MMM_MP_ERR_DECODER_ERROR = 0x10004,
    /*! DRM��license */
    MMM_MP_ERR_NO_LICENSE = 0x10005,
    /*! DRMʱ�Ӵ��� */
    MMM_MP_ERR_SECURE_CLOCK = 0x10006,
    /*! DRMʱ�Ӵ��� */
    MMM_MP_ERR_LICENSE_INFO = 0x10007,
    /*! ��Ч���� */
    MMM_MP_ERR_CMD_ILLEGAL = 0x10008,
    /*! �����ֲ����� */
    MMM_MP_ERR_CMD_NOTEXIST = 0x10009,
    /*! seek����ʱ��������ʱ��֮�� */
    MMM_MP_ERR_SEEK_BEHIND = 0x1000A,
    /*! parser���ش��� */
    MMM_MP_ERR_PARSER_LOAD = 0x1000B,
    /*! decoder���ش��� */
    MMM_MP_ERR_DECODER_LOAD = 0x1000C,
    /*! ����fifo���� */
    MMM_MP_ERR_FIFO_CREATE = 0x1000D,
    /*! ����ʱ�䳬����ʱ�� */
    MMM_MP_ERR_TIME_OVERFLOW = 0x1000E,
    /*! ����Դ������豸��ƥ�� */
    MMM_MP_ERR_IO_MISMATCH = 0x1000F,
    /*! ������ļ�β��ֹͣ */
    MMM_MP_ERR_FF_END = 0x10010,
    /*! ������ļ�ͷ��ֹͣ */
    MMM_MP_ERR_FB_START = 0x10011,
    /*! parser���� */
    MMM_MP_ERR_PARSER_ERROR = 0x10012,
    /*! ����豸֧�ֳ��� */
    MMM_MP_ERR_DEVICE_ERROR = 0x10013,
    /*! �����̴���*/
    MMM_MP_ERR_DISK_ERROR = 0x10014,
    /*! δ֪���� */
    MMM_MP_ERR_UNKNOWN = 0x100FF
} mmm_mp_err_t;
/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �ֿ鴦�������Ч�� */

    /*! �������м�� */
    MMM_MP_OPEN = 0,
    /*! �ر������м�� */
    MMM_MP_CLOSE,

    /*! �������ֲ���������Ϣ */
    MMM_MP_SET_MUSIC_SETTING,
    /*! ��ȡ���ֲ���������Ϣ */
    MMM_MP_GET_MUSIC_SETTING,
    /*! ���ÿ���� */
    MMM_MP_SET_FFB,
    /*! ���ÿ���˲��� */
    MMM_MP_FFB_STEP,
    /*! ���ÿ���˲���ʱ�� */
    MMM_MP_FFB_PLAY_TIME,
    /*! ���ÿ���˼�� */
    MMM_MP_FFB_WAIT_TIME,

    /*! ���ò����ļ� */
    MMM_MP_SET_FILE,
    /*! ��������ļ� */
    MMM_MP_CLEAR_FILE,
    /*! ��ȡý����Ϣ */
    MMM_MP_MEDIA_INFO,
    /*! ���öϵ㲥����Ϣ */
    MMM_MP_SET_BREAK_POINT,
    /*! ��ȡ�ϵ㲥����Ϣ */
    MMM_MP_GET_BREAK_POINT,
    /*! ���� */
    MMM_MP_PLAY,
    /*! ֹͣ */
    MMM_MP_STOP,

    /*! ��ȡ����״̬ */
    MMM_MP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_MP_GET_ERRORNO,

    /*! ������Ч */
    MMM_MP_SET_EQ,
    /*! ���û�ȡ��Ƶ������ֵ */
    MMM_MP_SET_FREQ_POINT,

    /*! ���������� */
    MMM_MP_KEYTONE_PLAY,
    /*! �����޷첥�� */
    MMM_MP_SET_LOOP_PLAY,
} mmm_mp_cmd_t;
/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_MP_ENGINE_STOPPED = 0,
    /*! �ļ����óɹ� */
    MMM_MP_ENGINE_FILESET,
    /*! ����״̬ */
    MMM_MP_ENGINE_PLAYING,
    /*! ��ͣ״̬ */
    MMM_MP_ENGINE_PAUSED,
    /*! ���ŵ�ͷ�򲥷ŵ�β */
    MMM_MP_ENGINE_PLAYEND,
    /*! ���ڵ��룬������ɺ��л�Ϊ����״̬ */
    MMM_MP_ENGINE_FADEIN,
    /*! ���ڵ�����������ɺ��л�Ϊ��ͣ״̬ */
    MMM_MP_ENGINE_FADEOUT,
    /*! ����״̬ */
    MMM_MP_ENGINE_ERROR,
    /*! �м���ռ���δ��ʼ����δ֪״̬ */
    MMM_MP_ENGINE_NULL
} mmm_mp_engine_t;

typedef enum
{
    /* ��ͨ���� */
    MP_WORK_MODE_NORMAL = 0,
    /* OK MIC���� */
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
 *     ���ļ�����Ĳ�����Ϣ
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
 *     �����ļ���Ϣ
 */
typedef struct
{
    /*! ������ʱ��,�Ժ���Ϊ��λ*/
    int total_time;
    /*! ���ֱ����ʣ���KbpsΪ��λ*/
    int bitrate;
    /*! ���ֲ����ʣ���KHzΪ��λ*/
    int sample_rate;
    /*! �����ʣ���λHz */
    int sample_rate_hz;
    /*! ����������*/
    int channels;
    /*! drm���ͣ����mmm_drm_type_t */
    int drm_type;
    /*! ��ʽ���� */
    int music_type;
    /*! ������λ�� */
    int bps;
    /*! high ape */
    int high_ape;
} mmm_mp_file_info_t;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_mp_engine_t */
    int status;
    /*! ����Ĵ���ţ����mmm_mp_err_t */
    int err_no;
    /*! �Զ�AB����ʱ��״̬�� ���mmm_mp_ab_t*/
    int ab_play_flag;
    /*! ��ǰ�Ĳ���ʱ�䣬��λ���� */
    int cur_time;
    /*! ��ǰ�����ʣ���λkbps */
    int cur_bitrate;
    /*! ��ǰ�������� */
    int cur_energy;
    /*! ��ǰ����ֵ-���ֵ*/
    short music_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short music_cur_energy_rms;
    /*! ��ǰ����ֵ-���ֵ*/
    short mic_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short mic_cur_energy_rms;
    /*! ��ǰ��Ƶ������ֵ */
    short freq_point_energy[10];
    /*! ����������ʱ�� */
    int music_lowpw_time;
    /*! Todo spectrum*/
    /*! ����������״̬����� mmm_ktp.h �� mmm_ktp_engine_t ���� */
    int ktp_status;
} mmm_mp_status_t;
/*!
 *  \brief
 *     ���ֲ�������
 */
typedef struct
{
    /*! parser�ĵ�ʱ�� */
    int parser_elapsed;
    /*! decoder�ĵ�ʱ�� */
    int dec_elapsed;
    /*! ��ȡ�ļ��ĵ�ʱ�� */
    int io_elapsed;
    /*! ������ģ���format_check */
    int other_elapsed;
} mmm_mp_performance_t;

/*!
 *  \brief
 *     ���ֲ��Ŷϵ���Ϣ
 *     ��music_bp_info_tһ��
 */
typedef struct
{
    /*! �ϵ�λ�õ�ʱ�䣬��λms */
    int bp_time_offset;
    /*! �ϵ�λ�����ļ��е�λ�ã���λ�ֽ� */
    int bp_file_offset;
    /*! �ϵ㲥������Ҫ�ĸ�����Ϣ*/
    /*! �����½ں���������Audible  */
    int bp_info_ext;
} mmm_mp_bp_info_t;

/*!
 *  \brief
 *     ���������������Ϣ
 */
typedef struct
{
    /*! Ĭ�ϲ����ʣ���λ��KHz��Ĭ��ֵ44 */
    int default_samplerate;
    /*! Ĭ��ͨ������Ĭ��ֵ2 */
    int default_channels;
} mmm_mp_aout_setting_t;

/*!
 *  \brief
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    /*! ϵͳĬ�ϲ����������и������ã������ýӿڣ�ԭ����ֻ����һ��*/
    mmm_mp_aout_setting_t aout_setting;

    /*! ADC CHANNEL���� */
    adc_chanel_setting_t adc_chan_set;

    /*! ��Ƶ���ͨ��ѡ�񣬼� typestruct.h �еĶ��� */
    int aud_output_chan;

    /*! ���뵭�� */
    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;

    /*! ����� */
    /*! ����˲�������λ���� */
    int ffb_step;
    /*! �������ʱ����λ���� */
    int ffb_delay;
    /*! ����˲���ʱ�䣬��λ���� */
    int ffb_play_time;

    /*! �����Ƹ�����������ֵ�����ֵ����0��ʾ��ʹ��������� */
    int music_lowpw_th_max;
    /*! �����Ƹ�����������ֵ����Чֵ����0��ʾ��ʹ��������� */
    int music_lowpw_th_rms;

    /*! �洢�����Ƿ�Uhost��Uhost������Card��һ�㣬��Ҫ��Ƶ */
    int is_uhost;
  
} music_setting_t;

/*!
 *  \brief
 *      ��������
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
 *     �ӿں�������
 *     ����API����api.h
 */
#define mmm_mp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_mp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
