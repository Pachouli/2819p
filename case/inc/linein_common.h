/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������LINEIN������غ궨�壬ö�����ͣ��ṹ�嶨��ȣ�ΪLINEIN����ǰ̨�����湲��
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __LINEIN_COMMON_H__
#define __LINEIN_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

#define SUPPORT_AUX_AA_STEREO  1 //ʹ���Ƿ�AIN-PAֱͨʵ��������

#define AUX_MODE_ADDA          0 //�Ȼ�ϣ�ADC�����ִ�����DAC��ȥ����֧��������
#define AUX_MODE_AA_STEREO     1 //AIN-PAֱͨ������

#define ENTER_AUX_CHAN         0 //AUXͨ��

#define ENTER_MIC_CHAN_C       1 //MICͨ�� TF
#define ENTER_MIC_CHAN_U       2 //MICͨ�� U��

#define ENTER_SPDIF_CHAN       3 //SPDIFͨ��
#define ENTER_I2SRX_CHAN       4 //I2SRXͨ��
#define ENTER_HDMI_CHAN        5 //HDMIͨ��
#define ENTER_HDMI1_CHAN       6 //HDMI1ͨ��
#define ENTER_HDMI2_CHAN       7 //HDMI2ͨ��
#define ENTER_ARC_CHAN         8 //ARCͨ��

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
    /*! ֹͣ */
    StopSta = 0,
    /*! ��ͣ */
    PauseSta,
    /*! ���� */
    PlaySta,
} play_status_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */
    play_status_e play_status;
    /*! ¼��״̬ */
    record_status_e record_sta;
    /*! ����������ʱ�䣬����һ��ʱ�����Խ���ʡ��ģʽ */
    int32 lowpw_time;
    /*! ��ǰ��¼��ʱ��*/
    int32 rec_time;
} lineineg_status_t;

#endif
