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
 * \brief    ����LINEIN��RADIO��USB Speaker�����м��ṩ�Ľӿڼ�����
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
#include <record_module_extern.h>

#endif

#define SUPPORT_MMM_PCM_RECORD_FUNC  1 //�Ƿ�֧��¼������

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �������м�� */
    MMM_PP_OPEN = 0,
    /*! �ر������м�� */
    MMM_PP_CLOSE,

    /*! ���ò��Ų���������AINͨ���������ʡ����ͨ��ģʽ������ʱ�䡢����ʱ��� */
    MMM_PP_SET_MUSIC_SETTING,

    /*! ���� */
    MMM_PP_PLAY,
    /*! ֹͣ */
    MMM_PP_STOP,

    /*! ��ȡ����״̬ */
    MMM_PP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_PP_GET_ERRORNO,

    /*! ������Ч */
    MMM_PP_SET_EQ,
    /*! ���û�ȡ��Ƶ������ֵ */
    MMM_PP_SET_FREQ_POINT,

    /*! ���������� */
    MMM_PP_KEYTONE_PLAY,

    /*! ����¼���ļ���� */
    MMM_PP_RECORDING_SET_FS,
    /*! ���ñ������ */
    MMM_PP_RECORDING_SET_ENCODE,
    /*! ����¼�������BTPLAYδSTART����ôRECORDING��ȵ�BTPLAY STARTʱ��START */
    MMM_PP_RECORDING_START,
    /*! �ر�¼�������BTPLAY��STOP����ôRECORDING���Զ�STOP */
    MMM_PP_RECORDING_STOP,
    /*! ��ȡ¼�����״̬ */
    MMM_PP_RECORDING_GET_STATUS,

    /*! �رռ�����Ĭ�Ͽ���������������TRUE��ʾ�رռ�����FALSE��ʾ�ָ����� */
    MMM_PP_CLOSE_MONITORING,

    /*! ���ÿ���ģʽ��Ĭ�Ϸǿ���ģʽ */
    MMM_PP_SET_IDLE_MODE,
} mmm_pp_cmd_t;
/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_PP_ENGINE_STOPPED = 0,
    /*! ׼������״̬�����嵽�㹻���ݺ������������� */
    MMM_PP_ENGINE_READY,
    /*! ��ͣ״̬ */
    MMM_PP_ENGINE_PAUSE,
    /*! ����״̬ */
    MMM_PP_ENGINE_PLAYING,
    /*! ���ڵ��룬������ɺ��л�Ϊ����״̬ */
    MMM_PP_ENGINE_FADEIN,
    /*! ���ݲ����������������ڵ�����������ɺ��л�ΪREADY״̬ */
    MMM_PP_ENGINE_FADEOUT,
    /*! STOP�����������������ڵ�����������ɺ��л�ΪPAUSE״̬ */
    MMM_PP_ENGINE_FADEOUT_PAUSE,
    /*! ����״̬ */
    MMM_PP_ENGINE_ERROR,
    /*! δOPEN״̬��δ֪״̬ */
    MMM_PP_ENGINE_NULL,
} mmm_pp_engine_t;

typedef enum
{
    /* ��ͨ���� */
    PP_WORK_MODE_NORMAL = 0,
    /* OK MIC���� */
    PP_WORK_MODE_MIC = 1,
    /* KEYTONE���� */
    PP_WORK_KEYTONE = 2,
} pp_work_mode_e;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_pp_engine_t */
    int status;
    adc_energy_info_t adc_energy;
    /*! ��ǰ��Ƶ������ֵ */
    short freq_point_energy[10];
    /*! ����������״̬����� mmm_ktp.h �� mmm_ktp_engine_t ���� */
    int ktp_status;
} mmm_pp_status_t;

#define PCM_SOURCE_AUX  0
#define PCM_SOURCE_USB  1

/*!
 *  \brief
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    adc_chanel_setting_t adc_chan_set;
    /*! PCMԴѡ��0��ʾAUX��1��ʾFM��2��ʾUSB */
    int pcm_source;

    /*! Ĭ�ϲ����ʣ���λ��KHz��Ĭ��ֵ44 */
    int default_samplerate;
    /*! Ĭ��ͨ������Ĭ��ֵ2 */
    int default_channels;

    /*! ��Ƶ���ͨ��ѡ�񣬼� typestruct.h �еĶ��� */
    int aud_output_chan;

    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;
    /*! ��ʼ������ʱ����λ���� */
    int start_delay_time;

} music_pcm_setting_t;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     ����API����api.h
 */
#define mmm_pp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_pp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
