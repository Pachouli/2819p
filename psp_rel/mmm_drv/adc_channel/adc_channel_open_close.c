/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ADCͨ���򿪺͹رա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "adc_channel.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ƶ�����м��
 * \param[in]   param  �����ֲ����м���Ĳ�����Ŀǰ����Ϊ��
 * \return       �������
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *adc_channel_open(void *param)
{
    adc_channel_info_t *p_adc_channel_info = &g_adc_channel_info;

    libc_printf_info("ADC Channel OPEN!\n");

    //OPEN֮��״̬�л�ΪOPEN
    p_adc_channel_info->ac_status = ADC_CHANNEL_STA_OPEN;

    return (void *) p_adc_channel_info;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �ر����ֲ����м��
 * \param[in]   handel  �򿪵����ֲ����м���������
 * \param[out]  param �ر��м�����豣�����ݵ�ָ�룬Ŀǰ����Ϊ��
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 adc_channel_close(adc_channel_info_t *p_adc_channel_info, void *param)
{
    libc_printf_info("ADC Channel CLOSE!\n");

    //CLOSE֮��״̬�л�ΪCLOSE
    p_adc_channel_info->ac_status = ADC_CHANNEL_STA_CLOSE;

    return 0;
}
