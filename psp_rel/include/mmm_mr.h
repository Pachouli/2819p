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
 * \brief    ��������¼���м���ṩ�Ľӿڼ�����
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
 *      ����ϵͳͷ�ļ�
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
 *      ��ǰcodec��״̬,��mmm_mr_status_t��statusȡֵ
 */
typedef enum
{
    /*��ʼ״̬*/
    MMM_MR_ENGINE_INIT = 0,
    /*����״̬*/
    MMM_MR_ENGINE_READY,
    /*! ������ */
    MMM_MR_ENGINE_ENCODING,
    /*! ��ͣ  */
    MMM_MR_ENGINE_PAUSED,
    /*! ������ֹͣ */
    MMM_MR_ENGINE_STOPPED,
    /*! ������� */
    MMM_MR_ENGINE_ERROR,
} mmm_mr_engine_t;

//���� monitor ״̬��
typedef enum
{
    /*! ��ʼ״̬ */
    MMM_MR_ENGINE_MNT_INIT = 0,
    /*! ֹͣ���� */
    MMM_MR_ENGINE_MNT_PAUSED,
    /*! �������� */
    MMM_MR_ENGINE_MNT_FADEIN,
    /*! ������ */
    MMM_MR_ENGINE_MNT_PLAYING,
    /*! ��������  */
    MMM_MR_ENGINE_MNT_FADEOUT,
    /*! �������� */
    MMM_MR_ENGINE_MNT_ERROR,
} mmm_mr_engine_mnt_t;

/*!
 * \brief
 *      ¼���м���ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �� */
    MMM_MR_OPEN = 0,
    /*! �ر� */
    MMM_MR_CLOSE,
    /*! ���������� */
    MMM_MR_AIN_OPEN,
    /*! �ر��������� */
    MMM_MR_AIN_CLOSE,

    /*! ��ʼ¼�� */
    MMM_MR_START,
    /*! ֹͣ¼�� */
    MMM_MR_STOP,
    /*! ����¼���ļ� */
    MMM_MR_SET_FILE,
    /*! �ر�¼���ļ� */
    MMM_MR_CLEAR_FILE,
    /*! ����¼������ */
    MMM_MR_AUDIO_PARAM,
    /*! ��ȡ����״̬ */
    MMM_MR_GET_STATUS,
    /*! ��ȡ����� */
    MMM_MR_GET_ERRORNO,
    /*! ���̲߳��Խӿ� */
    MMM_MR_SINGLE_THREAD,

	/*! ���ý������: 0�������룬1-10Ϊ���뼶��Խ����Խ����*/
	MMM_MR_SET_DENOISE,
	/*! �����Զ�������Ʋ�����0�������Զ�������ƣ�1: ������-6b����*/
	MMM_MR_SET_AGC,

} mmm_mr_cmd_t;

/*!
 * \brief
 *      ¼���м���ṩ��Ӧ�ò�Ļص�����������
 */
typedef enum
{
    /*! д���ļ�ͷ */
    MMM_MR_CB_FIRST_HEAD_WRITE = 0x0,
    /*! ���ļ����� */
    MMM_MR_CB_TRACK_ON,
    /*! ����д�� */
    MMM_MR_CB_DISK_FULL,
    /*! ������� */
    MMM_MR_CB_MAX,


} mmm_mr_cb_cmd_t;



/*!
 * \brief
 *      ����¼��Դ������
 */
typedef enum
{
    /*! MIC */
    AI_SOURCE_MIC,
    /*! FM */
    AI_SOURCE_FM,
    /*! LINEIN */
    AI_SOURCE_LINEIN,
    /*! ��ѡ������ */
    AI_SOURCE_NULL,
} mmm_ai_type_t;

/*!
 *  \brief
 *     ���������������Ϣ
 */
typedef struct
{
    /*! ������Դ ���μ� mmm_ai_type_t*/
    int input_source;
    /*! �Ƿ������0: ������, 1:���� */
    int listening_flag;
    /*! �����ʽ�� 0:��ֱ����1:ֱ�� */
    int drive_mode;
    /*! ������Դ���漶��0-7,�Ƽ�ֵ��FM/LINE IN 2, MIC IN 7; step 1.5db*/
    int input_gain;
    /*! ADC���漶��0-0x3f,�Ƽ�ֵ��FM/LINE IN 0x2f, MIC IN 0x33; step 1.5db*/
    int adc_gain;
    /*! �������漶��0,1,2, �㣺disable������: enable��step 6db */
    int digital_gain;
} mmm_mr_ain_setting_t;

/*!
 *  \brief
 *     ��Ƶ����Ĳ�������
 *
 * \li     wavֻ��������ʡ������ʼ��㹫ʽΪ��ADPCM: ������*4*2��PCM��������*16*2
 * \li     mp3¼����������һ����
 * \par
 <div class="generic_text_mapping_table">
 * <table>
 * <tr>
 * <th>����</th>
 * <th>��ѡͨ����</th>
 * <th>��ѡ������</th>
 * <th>��ѡ������</th>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1</td>
 * <td>8k,11025,12k,16k</td>
 * <td>[8,64]</td>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1,2</td>
 * <td>22050,24k</td>
 * <td>[64,128]</td>
 * </tr>
 * <tr>
 * <td>��</td>
 * <td>1,2</td>
 * <td>32k,44100,48k</td>
 * <td>[64,192]</td>
 * </tr>
 * </table>
 * </div>
 */
typedef struct
{
    /*! ��Ƶ����Ĳ���*/
    record_module_encode_para_t encode_para;

    /*! ����Ƶ����8=8Khz��22=22.05Khz��44=44.1Khz��*/
    int sample_rate;
    /*! ����ģʽ��0:����¼��, 1:����ģʽ, 2:�Զ����� */
    int vad_mode;
    /*! �����������ֵ[0,128],����ȷ������ֵ��Ϊ���ʾ���ó�ʼ¼��������ƽ��ֵ��������-0.5dBһ��, Ĭ��ֵ 64 */
    int vad_threshold;
    /*! ��������ӳ�ʱ��,��λms, Ĭ��ֵ 3000 */
    int vad_delay;
    /*! ���뼶�����: 0�������룬1-10Ϊ���뼶��Խ����Խ���� */
    int denoise_level;
    /*! ��Ƶ���ͨ��ѡ�񣬼� typestruct.h �еĶ��� */
    int aud_output_chan;
    /*! ����ʱ�䣬��λ���� */
    int fade_in_time;
    /*! ����ʱ�䣬��λ���� */
    int fade_out_time;
    /*! �Ƿ��̨����ģʽ */
    int is_bt_inback;
} mmm_mr_audio_param_t;

/*!
 * \brief
 *      ¼���м�����ظ�Ӧ�ò��״̬��Ϣ
 */
typedef struct
{
    /*! ��ǰ��¼��ʱ��*/
    int time;
    /*! ��ǰcodec��״̬*/
    int status;
    /*! �����ǰʵʱ��������,��ʱ��֧��*/
    int energy_level;
    /*! ��ǰ�Ƿ��Ǿ������ھ�����⹦�ܿ���ʱ��Ч, �㣺���������㣺����*/
    int silence_flag;
    /*! �ڳ���״̬ʱ���صĴ����*/
    int err_no;
    /*! ��ǰ¼��д���ļ��е�����������λ��512�ֽڼ�1��page*/
    int write_pages;
    /*! ��ǰ¼���ļ�����״���Ϣ*/
    uint32 start_cluster;
} mmm_mr_status_t;


#define AUDIO_MMM_MR_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     ����API����api.h
 */
#define mmm_mr_cmd(a,b,c)           ((int32(*)(void *handel, mmm_mr_cmd_t cmd, uint32 args))AUDIO_MMM_MR_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif
#endif // __MMM_MR_H__
