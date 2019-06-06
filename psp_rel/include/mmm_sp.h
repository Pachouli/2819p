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
#ifndef __MMM_SP_H__
#define __MMM_SP_H__

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
#include <mmm_sp_common.h>
#include <post_process.h>
#include <mmm_ktp.h>
#include <record_module_extern.h>

#endif

#define SUPPORT_MMM_SPEECH_RECORD_FUNC  0 //�Ƿ�֧��¼������

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �������м�� */
    MMM_SP_OPEN = 0,
    /*! �ر������м�� */
    MMM_SP_CLOSE,

    /*! ���ò��Ų���������MICͨ���������ʵ� */
    /*! ����PLC���� */
    /*! ����PRE AGC���� */
    /*! ����POST AGC���� */
    /*! ����AEC���� */
    MMM_SP_SET_SPEECH_SETTING_EXT,

    /*! ���� */
    MMM_SP_PLAY,
    /*! ֹͣ */
    MMM_SP_STOP,

    /*! ����ASQT DUMP BUFFER */
    MMM_SP_SET_ASQT_DUMP_BUF,

    /*! ��ȡ����״̬ */
    MMM_SP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_SP_GET_ERRORNO,

    /*! ������Ч */
    MMM_SP_SET_EQ,

    /*! ���������� */
    MMM_SP_KEYTONE_PLAY,

    /*! ����HFP Connected������������ʼ��������ʱ�� */
    MMM_SP_SET_HFP_CONNECTED,

    /*! ����¼���ļ���� */
    MMM_SP_RECORDING_SET_FS,
    /*! ���ñ������ */
    MMM_SP_RECORDING_SET_ENCODE,
    /*! ����¼�������BTPLAYδSTART����ôRECORDING��ȵ�BTPLAY STARTʱ��START */
    MMM_SP_RECORDING_START,
    /*! �ر�¼�������BTPLAY��STOP����ôRECORDING���Զ�STOP */
    MMM_SP_RECORDING_STOP,
    /*! ��ȡ¼�����״̬ */
    MMM_SP_RECORDING_GET_STATUS,
} mmm_sp_cmd_t;

/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_SP_ENGINE_STOPPED = 0,
    /*! ׼��״̬���Ѿ���ʼԤ���� */
    MMM_SP_ENGINE_READY,
    /*! ����״̬ */
    MMM_SP_ENGINE_PLAYING,
    /*! ����״̬ */
    MMM_SP_ENGINE_ERROR,
    /*! δOPEN״̬��δ֪״̬ */
    MMM_SP_ENGINE_NULL,
} mmm_sp_engine_t;

typedef enum
{
    /* ��ͨ��绰 */
    SP_WORK_MODE_NORMAL = 0,
    /* OK MIC ����ģʽ */
    SP_WORK_MODE_MIC = 1,
} sp_work_mode_e;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_sp_engine_t */
    int status;
    /*! ����������״̬����� mmm_ktp.h �� mmm_ktp_engine_t ���� */
    int ktp_status;
    /*! ����ֵ */
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
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    /*! MIC OP Gain��0-7�ֱ��ʾ26db��30db��31.5db��33db��34.5db��36db��37.5db��39db */
    int8 mic_op_gain;
    /*! MIC ADC Gain��0-3�ֱ��ʾ0db��0db��6db��12db */
    int8 mic_adc_gain;

    /*! ��Ƶ���ͨ��ѡ�񣬼� typestruct.h �еĶ��� */
    int8 aud_output_chan;

    /*! �����ʣ���λ��KHz��Ĭ��ֵ8 */
    int8 default_samplerate;
    /*! ����ͨ����1��ʾ��������2��ʾ��������3��ʾ˫���� */
    int8 default_channels;

    /*! ������˥����0-18��ʾ˥��0db ~ -18db */
    int8 right_weak;
    /*! ������˥����0-18��ʾ˥��0db ~ -18db */
    int8 left_weak;

    /*! �Ƿ����SCO OUT BUFFER */
    bool fill_sco_out;

    /*! �Ƿ���ҪSCO����Ԥ���� */
    bool use_sco_data_predeal;
    /*! �Ƿ���ҪSCO���ݺ��� */
    bool use_sco_data_postdeal;
    /*! SCO�������ͣ�0��ʾCVSD��1��ʾMSBC */
    uint8 sco_codec_type;
} speech_setting_t;

typedef struct
{
    /*PLC ʹ��*/
    int plc_enable;
} plc_para_t;

typedef struct
{
    /*agcʹ��*/
    int agc_enable;
    /*Ŀ����ȣ�4000-24000���Ƽ�ֵ8000*/
    int agc_level;
    /*��չ��������δʵ��*/
    int agc_mode;
    /*�Ŵ����������棬�㣺disable�����㣺��������ֵ*/
    int minimum_gain;
    /*��Сʱ����С���棬�㣺disable�����㣺��������ֵ*/
    int maximum_gain;
    /*�����ż�ֵ����Χ200-1000���Ƽ�ֵ400*/
    int noise_gate_threshold;
} agc_para_t;

typedef struct
{
    int frame_size; //֡��ʱ�䣬��λms��Ŀǰ��֧�� 16ms ֡��
    int sampling_rate; //�����ʣ���λHz��Ŀǰ��֧�� 8000Hz ������
    int aec_enable; //aecģ�鿪�أ�1���򿪣�0���ر�
    int tail_length; //����β������λ ms������Ϊ16�ı���,Ŀǰ��֧�� 32 ms
    int nlp_enable; //�����Դ�����ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ 1
    int nlp_aggressiveness; //�����Դ���̶ȣ�����0~15,Խ���������Դ���Խ����
    int dt_threshold; //˫���������������̶ȣ�����0~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ��
    int aes_enable; //��������������ģ�鿪�أ�1���򿪣�0���رգ�Ϊ 1 ʱ���뱣֤ aec_enable Ϊ1
    int echo_suppress; //����������ģ�鵥���������Ʊȣ���λdB�� 0~40
    int echo_suppress_avtive; //����������ģ��˫���������Ʊȣ���λdB�� 0~40
    int ans_enable; //���뿪�أ�1���򿪣�0���ر�
    int noise_suppress; //�������Ʊȣ���λdB�� 0~25
    int cng_enable; //�����������أ�1���򿪣�0���ر�
    int noise_floor; //��������ˮƽ����λ dB Fs, һ��ӦС��-45 dB
    int analysis_mode; //����ģʽ��0��ʾģ���绰ģʽ��1��ʾ��绰ģʽ
    int half_mode; //��˫��ģʽ��0��ʾȫ˫����1��ʾ��˫��
} actionsaec_prms_t; //NOTE������� actionsaec_struct.h �� sv_prms_t �ṹ�嶨��һ��

//�������ò���
typedef struct
{
    int mic_op_gain; //MICģ�����棺0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db��Ĭ��Ϊ0
    int mic_adc_gain; //MIC�������棺0: 0dB(bypass)��1: 0dB(bypass)��2: 6dB(x2)��3: 12dB(x4)
    int dac_enable; //DACʹ�ܣ�1��ʾ��������2��ʾ��������3��ʾ˫������Ĭ��Ϊ1
    int dac_weak; //����˥����0~18db��1-18��˥��1db-18db����ʾ�����ȣ�33-50��˥��1db-18db����ʾ�����ȣ�Ĭ��Ϊ0
    int volume_call_gain; //��绰�������棬��λ-1db��0~9 ��ʾ 0db ~ -9db
    int pre_agc_enable; //ǰ��AGCʹ�����Ĭ��Ϊ1
    int pre_agc_level; //ǰ��AGC level��12000 - 32000��Ĭ��20000

    int post_agc_enable; //���AGCʹ�����Ĭ��Ϊ1
    int post_agc_level; //���AGC level��12000 - 32000��Ĭ��20000

    actionsaec_prms_t sv_prms; //AEC ����
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
 *     �ӿں�������
 *     ����API����api.h
 */
#define mmm_sp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_sp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
