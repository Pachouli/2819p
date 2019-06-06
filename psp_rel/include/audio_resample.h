/*******************************************************************************/
/*!
 * \file     audio_resample.h
 * \brief    ��Ƶ�ز���
 * \author   zhanguoliang
 * \version  1.0
 * \date     2018-9-23
 *******************************************************************************/

#ifndef __AUDIO_RESMPLE_H__
#define __AUDIO_RESMPLE_H__

#define SUPPORT_AUDIO_RESAMPLE   0 //�Ƿ�֧���ز������ܣ����Թ̶�ת��Ϊ44.1KHz/48KHz���

#define RESAMPLE_TEMP_SAMPLES    835 //�ز�����ʱ����������������ĳЩ������ת������һЩ��
#define RESAMPLE_TABLE_ELEMENTS  312 //�ز���ϵ����Ԫ����Ŀ  �����ĳЩ������ת������һЩ��

//�ز�����Ϣ
typedef struct
{
    void *resample_handle_left;     //�������ز������
    void *resample_handle_right;    //�������ز������
    uint32 convert_input_rate_hz;   //�ز������������
    uint32 convert_output_rate_hz;  //�ز�����������ʣ����ݸ�ֵ����dac_fifo��Ƶ��
    uint16 expected_input_sample_cnt;  //�ز���ת����������Ҫ������뻺��Ĳ�����Ե���������ֵ���ݲ�ͬ�Ĳ�����֮���ת������ͬ������Ҫ�鿴�ز���ת����
    uint16 expected_output_sample_cnt; //�ز���ת��������������С
    uint16 cur_resample_input_len;     //��ǰ�ز����������е����������Ե�����
    bool enable_resample;              //���������ļ������ʺ��ز���ӳ���ȷ���Ƿ�ʹ���ز���ת����FALSE��ʾ��ʹ�ܣ�TRUE��ʾʹ��
} audio_resample_info_t;

//�ز�������ӳ������ڶ����ز�����������������ʺ�ת��Ҫ���ӳ���ϵ
typedef struct
{
    uint32 input_rate_hz;       //�ز������������hz
    uint32 output_rate_hz;      //�ز������������hz
    uint16 input_samples;       //�ز���ת������Ҫ����������������
    uint16 output_samples;      //�ز���ת������������������
} resample_op_map_t;

#endif

