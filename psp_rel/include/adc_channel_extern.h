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
 * \file     adc_channel_extern.h
 * \brief    ����LINEIN��RADIO��USB Speaker�����м��ṩ�Ľӿڼ�����
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __ADC_CHANNEL_EXTERN_H__
#define __ADC_CHANNEL_EXTERN_H__

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

#endif

/*! \cond MMM_MP_API */

//PCM - AUX/FM ����ҪADC��Ӧ�ý�������ռ�
#define PCM_ADC_BUF_BLOCK   (512) //�պõ�����128�������㣬�պù�������Ч����һ֡
//NOTE��������POST_PROCESS_FRAME_SAMPLE��2��������������
#define PCM_ADC_BUF_LEN     (PCM_ADC_BUF_BLOCK * 2)

typedef struct
{
    bool  need_update : 1;
    bool  need_resample : 1;
    bool  sample_rate_ok : 1;
    uint8 sample_rate_khz; //�ز���Ŀ�Ĳ�����
    uint8 resample_src_khz; //�ز���ԭʼ������
} get_sample_rate_ret_t;

/*!
 * \brief
 *      ADCͨ���ṩ��Ӧ�ò��������
 */
typedef enum
{
    /*! ��ADCͨ�� */
    ADC_CHANNEL_OPEN = 0,
    /*! �ر�ADCͨ�� */
    ADC_CHANNEL_CLOSE,

    /*! ����ADCͨ������������AINͨ���������ʵ� */
    ADC_CHANNEL_SETTING,

    /*! ����ADCͨ�� */
    ADC_CHANNEL_START,
    /*! ֹͣADCͨ�� */
    ADC_CHANNEL_STOP,

    /*! ��ȡADCͨ������ */
    ADC_CHANNEL_GET_PIPE,
    ADC_CHANNEL_GET_MIC_PIPE,
    ADC_CHANNEL_GET_RESAMPLE_LCH_PIPE,
    ADC_CHANNEL_GET_RESAMPLE_RCH_PIPE,

    /*! ��ȡADC���� */
    ADC_CHANNEL_GET_ENERGY,

    /*! ��ȡ�����ʣ���λHz */
    ADC_CHANNEL_GET_SAMPLERATE,

    /*! ���û����� */
    ADC_CHANNEL_SET_BUFFER,
    /*! �����ز��������� */
    ADC_CHANNEL_SET_RESAMPLE_BUFFER,
    /*! ��ʼ�ز����������ݣ��յ�������֮ǰ�ǹ������ݵ� */
    ADC_CHANNEL_RESAMPLE_START_BUF_DATA,
} adc_channel_cmd_e;

/*!
 *  \brief
 *      ADCͨ��״̬
 */
typedef enum
{
    ADC_CHANNEL_STA_CLOSE = 0,
    ADC_CHANNEL_STA_OPEN,
    ADC_CHANNEL_STA_START,
    ADC_CHANNEL_STA_STOP,
    ADC_CHANNEL_STA_ERROR,
} adc_channel_status_e;

/*!
 *  \brief
 *     ���ֲ���ʵʱ��Ϣ
 */
typedef struct
{
    /*! ��ǰ����ֵ-���ֵ*/
    short adc_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short adc_cur_energy_rms;
    /*! ADC����������ʱ�䣬��λms */
    uint32 adc_lowpw_time;
    /*! ��ǰ����ֵ-���ֵ*/
    short mic_cur_energy;
    /*! ��ǰ����ֵ-ƽ��ֵ*/
    short mic_cur_energy_rms;
} adc_energy_info_t;

/* ��������ӿ����� */
#define ADC_SOURCE_AUX     0
#define ADC_SOURCE_MIC     1
#define ADC_SOURCE_I2SRX   2
#define ADC_SOURCE_SPDIFRX 3
#define ADC_SOURCE_HDMIARC 4 //ֻ��PCM Player�м�����ã�ת��ΪSPDIFRX���ݸ�ADC Channelģ��
#define ADC_SOURCE_MIC_AUX 5

/* �û����� */
#define ADC_USER_LINEIN    0
#define ADC_USER_BTPLAY    1
#define ADC_USER_HDMI_ARC  2 //HDMI ARC��ʱ��Ҫ�ﵽ100ms����

/*!
 *  \brief
 *     ���ֲ��ŵ�������Ϣ
 */
typedef struct
{
    /*! ��������ӿ����� */
    uint8 data_input_if;
    /*! Ĭ�ϲ����ʣ���λ��KHz��Ĭ��ֵ44 */
    uint8 default_samplerate;

    /*! AIN����ģʽ��0��ʾAUXL0��AUXR0���������룬1��ʾAUXL1��AUXR1���������룬2��ʾAUXL0��AUXL1������룬
        3��ʾAUXR0��AUXR1������룬4��ʾAUXL0��AUXR0������룬5��ʾIC���Ϻ�AUXL0���룬
        6��ʾIC���Ϻ�AUXR0���룬7��ʾIC���Ϻ�AUXL1���룬8��ʾIC���Ϻ�AUXR1���� */
    uint8 ain_input_mode;
    /*! AIN OP Gain */
    uint8 ain_op_gain;
    /*! AIN ADC Gain */
    uint8 ain_adc_gain;
    /*! �û����ͣ��˻�Ϊ����PCM Player�м��ʹ�� */
    uint8 user_type;

    /*! �����MIC���룬0��ʾMIC0��1��ʾMIC1��2��ʾMIC0+MIC1 */
    uint8 micin_input_mode;
    /*! MIC IN OP Gain */
    uint8 micin_op_gain;
    /*! MIC IN ADC Gain */
    uint8 micin_adc_gain;

    uint8 i2srx_work_mode : 1; //��ɫ��0-����ɫ��1-�ӽ�ɫ
    uint8 i2srx_slave_mclk_use_inner : 1; //I2SRX��ΪSlaveʹ��ʱ��֧�ֲ�������96KHz�����£����Բ��ý�MCLK����ʡһ��PIN
    uint8 i2srx_width : 2; //��Чλ��0-16bit��1-20bit��2-24bit
    uint8 i2srx_data_mode : 2; //���ݴ���ģʽ��0-i2sģʽ��1-����룬2-�Ҷ���

    /*! LINEIN�����������ֵ�����ֵ��������һ��ʱ�������ΪAUX����Դֹͣ������ʡ��ػ��ȣ�0��ʾ��ʹ��������� */
    short adc_lowpw_th_max;
    /*! LINEIN�����������ֵ����Чֵ��������һ��ʱ�������ΪAUX����Դֹͣ������ʡ��ػ��ȣ�0��ʾ��ʹ��������� */
    short adc_lowpw_th_rms;
} adc_chanel_setting_t;

typedef struct
{
    /*! adc channel ���л����������м��ָ�� */
    uint8 *adc_chan_buffer_1k;
    uint8 *adc_chan_cache;
    uint8 *mic_chan_cache;
    uint8 *mic_aux_temp_buffer_256;
    uint16 adc_chan_cache_len;
    uint16 mic_chan_cache_len;
    uint16 pcm_adc_block_size; //DMA Bufferһ�룬������ճ��ȣ����512�ֽ�=1k/2
} adc_channel_buffer_setting_t;

//����ز���Ҫ�����ݷ�����adc_channel������������ģ�Ͳ���
typedef bool (*adc_chan_feedback_data_cbk)(short *buf, uint16 length);
typedef struct
{
    uint8 *resample_left_ch_buff;
    uint8 *resample_right_ch_buff;
    uint8 *resample_split_buff;
    uint16 resample_left_ch_len;
    uint16 resample_right_ch_len;
    adc_chan_feedback_data_cbk *ret_feedback_data_cbk;
} adc_channel_resample_buffer_setting_t;

#define ADC_CHANNEL_CMD_ENTRY       (*((uint32*)(AUDIO_CHANNEL_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     ����API����api.h
 */
#define adc_channel_cmd(a,b,c)      ((int32(*)(void *handel, adc_channel_cmd_e cmd, uint32 args))ADC_CHANNEL_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
