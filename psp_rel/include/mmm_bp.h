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
 * \brief    ����LINEIN��RADIO��USB Speaker�����м��ṩ�Ľӿڼ�����
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

//#define DEC_ENABLE_TRUE_WIRELESS_STEREO //�����Ƿ�֧��TWS
#define SUPPORT_TWS_EXCPT_RESET  1 //�Ƿ�֧��TWS�쳣�ָ�
#define SUPPORT_MMM_BT_RECORD_FUNC  0 //�Ƿ�֧��¼������

/*! \cond MMM_MP_API */

/*!
 * \brief
 *      ���ֲ����ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! �������м�� */
    MMM_BP_OPEN = 0,
    /*! �ر������м�� */
    MMM_BP_CLOSE,

    /*! ���ò��Ų������������ͨ��ģʽ������ʱ�䡢����ʱ��� */
    MMM_BP_SET_MUSIC_SETTING,

    /*! ���� */
    MMM_BP_PLAY,
    /*! ֹͣ */
    MMM_BP_STOP,

    /*! ��ȡ����״̬ */
    MMM_BP_GET_STATUS,
    /*! ��ȡ������Ϣ�� */
    MMM_BP_GET_ERRORNO,

    /*! ������Ч */
    MMM_BP_SET_EQ,
    /*! ���û�ȡ��Ƶ������ֵ */
    MMM_BP_SET_FREQ_POINT,

    /*! ���������� */
    MMM_BP_KEYTONE_PLAY,

    /*! ����TWS����ģʽ�Ͳ�����û�����þ��ǵ��䲥��ģʽ���ýӿ�Ӧ���� MMM_BP_SET_MUSIC_SETTING ֮�����ϵ��� */
    MMM_BP_SET_TWS_MODE,
    /*! ����LINEIN TWS ADC SBC ENCODE Send frame �ص����� */
    MMM_BP_SET_SEND_FRAME_CBK,

    /*! ����¼���ļ���� */
    MMM_BP_RECORDING_SET_FS,
    /*! ���ñ������ */
    MMM_BP_RECORDING_SET_ENCODE,
    /*! ����¼�������BTPLAYδSTART����ôRECORDING��ȵ�BTPLAY STARTʱ��START */
    MMM_BP_RECORDING_START,
    /*! �ر�¼�������BTPLAY��STOP����ôRECORDING���Զ�STOP */
    MMM_BP_RECORDING_STOP,
    /*! ��ȡ¼�����״̬ */
    MMM_BP_RECORDING_GET_STATUS,
} mmm_bp_cmd_t;

/*!
 *  \brief
 *      ���ֲ��������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_BP_ENGINE_STOPPED = 0,
    /*! ׼������״̬�����嵽�㹻���ݺ������������� */
    MMM_BP_ENGINE_READY,
    /*! ��ͣ״̬ */
    MMM_BP_ENGINE_PAUSE,
    /*! ���ڵ��룬������ɺ��л�Ϊ����״̬ */
    MMM_BP_ENGINE_FADEIN,
    /*! ����״̬ */
    MMM_BP_ENGINE_PLAYING,
    /*! ���ݲ����������������ڵ�����������ɺ��л�ΪREADY״̬ */
    MMM_BP_ENGINE_FADEOUT,
    /*! STOP�����������������ڵ�����������ɺ��л�ΪPAUSE״̬ */
    MMM_BP_ENGINE_FADEOUT_PAUSE,
    /*! ����״̬ */
    MMM_BP_ENGINE_ERROR,
    /*! δOPEN״̬��δ֪״̬ */
    MMM_BP_ENGINE_NULL,
} mmm_bp_engine_t;

typedef enum
{
    /* ��ͨ���� */
    BP_WORK_MODE_NORMAL = 0,
    /* ���� */
    BP_WORK_MODE_TWS = 1,
    /* ����+MIC */
    BP_WORK_MODE_MIC = 2,
    /* ����+MIC */
    BP_WORK_MODE_TWS_MIC = 3,
} bp_work_mode_e;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ����״̬�����mmm_bp_engine_t */
    uint8 status;
    /*! ����������״̬����� mmm_ktp.h �� mmm_ktp_engine_t ���� */
    uint8 ktp_status;
    /*! ���䶪������ */
    bool lost_pkg_too_more_flag;

    /*! ��ǰ����ֵ-���ֵ*/
    short bt_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short bt_cur_energy_rms;
    /*! ��ǰ����ֵ-���ֵ*/
    short mic_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short mic_cur_energy_rms;
    /*! ��ǰ��Ƶ������ֵ */
    short freq_point_energy[10];
    /*! ����������ʱ�� */
    int bt_lowpw_time;
    /*! ADC CHANNEL ���� */
    adc_energy_info_t adc_energy;
} mmm_bp_status_t;

typedef enum
{
    MMM_SOURCE_AVDTP_STREAM = 0,
    MMM_SOURCE_ADC_CHAN = 1,
} mmm_data_source_e;

/*!
 *  \brief
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    /*! ������Դͷ AVDTP STREAM/AUX CHANNEL */
    int data_source;
    /*! ADC CHANNEL���� */
    adc_chanel_setting_t adc_chan_set;

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

    /*! �����Ƹ�����������ֵ�����ֵ����0��ʾ��ʹ��������� */
    int bt_lowpw_th_max;
    /*! �����Ƹ�����������ֵ����Чֵ����0��ʾ��ʹ��������� */
    int bt_lowpw_th_rms;
} music_bt_setting_t;

//TWSģʽ������ز���
typedef struct
{
    /*! TWS��ɫ�����仹�Ǹ��䣻���䲥������Normal */
    dev_role_e dev_role;
    /*! TWS����λ�ã����仹�����䣻���䲥������˫���� */
    dev_spk_pos_e dev_spk;
    /*! ��ȡ����ʱ��� */
    f_get_bt_clk tws_get_bt_clk;
    /*! ����ͬ������������ʱ��� */
    f_set_bt_clk tws_set_bt_clk;
    /*! ����ͬ�������ص����� */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! �Ƿ���AVDTP���ݰ���ʧ */
    f_has_avdtp_pkg_lost tws_has_avdtp_pkg_lost;
} music_bt_tws_setting_t;

#define AUDIO_MMM_MP_CMD_ENTRY      (*((uint32*)(MMM_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     ����API����api.h
 */
#define mmm_bp_cmd(a,b,c)           ((int32(*)(void *handel, mmm_bp_cmd_t cmd, uint32 args))AUDIO_MMM_MP_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
