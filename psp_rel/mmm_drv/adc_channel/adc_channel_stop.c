/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ADCͨ��ֹͣ�ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "adc_channel.h"

int32 adc_channel_adc_close(adc_channel_info_t *p_ac_info)
{
    if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_AUX)
    {
        return adc_channel_data_input_if_aux_exit(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_MIC)
    {
        return adc_channel_data_input_if_mic_exit(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_MIC_AUX)
    {
        return adc_channel_data_input_if_mic_aux_exit(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_I2SRX)
    {
        return adc_channel_data_input_if_i2srx_exit(p_ac_info);
    }
    else if (p_ac_info->adc_chan_set.data_input_if == ADC_SOURCE_SPDIFRX)
    {
        return adc_channel_data_input_if_spdifrx_exit(p_ac_info);
    }

    return 0;
}

/* �������ֽ����� */
int32 adc_channel_stop(adc_channel_info_t *p_ac_info)
{
    int32 ret = 0;

    //ֹͣAUDIO Ӳ������
    if (p_ac_info->g_aout_init_flag == 1)
    {
        adc_channel_adc_close(p_ac_info);
        p_ac_info->g_aout_init_flag = 0;
    }

    p_ac_info->adc_chan_buf_w.read_ptr = 0;
    p_ac_info->adc_chan_buf_w.write_ptr = 0;
    p_ac_info->adc_chan_buf_w.length = 0;

    //״̬�޸�Ϊ STOP
    p_ac_info->ac_status = ADC_CHANNEL_STA_STOP;

    return ret;
}
