/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道开放命令API入口。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

adc_channel_info_t g_adc_channel_info;

const uint32 __section__(".rodata.entry") adc_channel_cmd0[] =
{
    (uint32) adc_channel_cmd_inner,
};

/*! \cond MMM_PP_API */

int32 adc_channel_cmd_inner(void *handel, adc_channel_cmd_e cmd, uint32 args)
{
    adc_channel_info_t *p_adc_channel_info = NULL;
    int32 ret = 0;

    if (cmd != ADC_CHANNEL_OPEN)
    {
        p_adc_channel_info = (adc_channel_info_t *) handel;
    }

    if ((g_adc_channel_info.ac_status == ADC_CHANNEL_STA_CLOSE) && (cmd != ADC_CHANNEL_OPEN))
    {
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case ADC_CHANNEL_OPEN:
        p_adc_channel_info = (adc_channel_info_t *)adc_channel_open((void*)args);
        if (p_adc_channel_info == NULL)
        {
            *((adc_channel_info_t **) handel) = NULL;
            ret = -1;
            break;
        }
        *((adc_channel_info_t **) handel) = p_adc_channel_info;
        break;

        case ADC_CHANNEL_SETTING:
        libc_memcpy(&(p_adc_channel_info->adc_chan_set), (void*)args, sizeof(adc_chanel_setting_t));
        break;

        case ADC_CHANNEL_SET_BUFFER:
        libc_memcpy(&(p_adc_channel_info->adc_chan_buf_set), (void*)args, sizeof(adc_channel_buffer_setting_t));
        break;

        case ADC_CHANNEL_SET_RESAMPLE_BUFFER:
        {
            adc_channel_resample_buffer_setting_t *p_rs_buff_sett = (adc_channel_resample_buffer_setting_t *)(void* )args;

            p_adc_channel_info->resample_left_chan_buf.read_ptr = 0;
            p_adc_channel_info->resample_left_chan_buf.write_ptr = 0;
            p_adc_channel_info->resample_left_chan_buf.length = 0;
            p_adc_channel_info->resample_left_chan_buf.raw_data = p_rs_buff_sett->resample_left_ch_buff;
            p_adc_channel_info->resample_left_chan_buf.capacity = p_rs_buff_sett->resample_left_ch_len;

            p_adc_channel_info->resample_right_chan_buf.read_ptr = 0;
            p_adc_channel_info->resample_right_chan_buf.write_ptr = 0;
            p_adc_channel_info->resample_right_chan_buf.length = 0;
            p_adc_channel_info->resample_right_chan_buf.raw_data = p_rs_buff_sett->resample_right_ch_buff;
            p_adc_channel_info->resample_right_chan_buf.capacity = p_rs_buff_sett->resample_right_ch_len;

            p_adc_channel_info->resample_split_buff = p_rs_buff_sett->resample_split_buff;

            *(p_rs_buff_sett->ret_feedback_data_cbk) = adc_channel_feedback_data_func;

            p_adc_channel_info->resample_enable = TRUE;
        }
        break;

        case ADC_CHANNEL_RESAMPLE_START_BUF_DATA:
        p_adc_channel_info->start_buffer_data_flag = TRUE;
        break;

        case ADC_CHANNEL_START:
        ret = adc_channel_start(p_adc_channel_info);
        break;

        case ADC_CHANNEL_STOP:
        ret = adc_channel_stop(p_adc_channel_info);
        break;

        case ADC_CHANNEL_GET_PIPE:
        {
            buffer_rw_t **pp_buffer_rw = (buffer_rw_t **)(void *)args;
            (*pp_buffer_rw) = &(p_adc_channel_info->adc_chan_buf_w);
        }
        break;

        case ADC_CHANNEL_GET_MIC_PIPE:
        {
            buffer_rw_t **pp_buffer_rw = (buffer_rw_t **)(void *)args;
            (*pp_buffer_rw) = &(p_adc_channel_info->mic_chan_buf_w);
        }
        break;

        case ADC_CHANNEL_GET_RESAMPLE_LCH_PIPE:
        {
            buffer_rw_t **pp_buffer_rw = (buffer_rw_t **)(void *)args;
            (*pp_buffer_rw) = &(p_adc_channel_info->resample_left_chan_buf);
        }
        break;
        case ADC_CHANNEL_GET_RESAMPLE_RCH_PIPE:
        {
            buffer_rw_t **pp_buffer_rw = (buffer_rw_t **)(void *)args;
            (*pp_buffer_rw) = &(p_adc_channel_info->resample_right_chan_buf);
        }
        break;

        case ADC_CHANNEL_GET_ENERGY:
        {
            adc_energy_info_t *p_adc_energy_info = (adc_energy_info_t *)(void *)args;
            libc_memcpy(p_adc_energy_info, &(p_adc_channel_info->adc_energy), sizeof(adc_energy_info_t));
        }
        break;

        case ADC_CHANNEL_GET_SAMPLERATE:
        {
            uint32 temp_flag = sys_local_intc_save(0x01);
            get_sample_rate_ret_t *p_samplerate_ret = (uint32 *)(void *)args;
            p_samplerate_ret->need_update = p_adc_channel_info->sample_rate_need_update;
            p_samplerate_ret->need_resample = p_adc_channel_info->need_resample;
            p_samplerate_ret->sample_rate_ok = p_adc_channel_info->sample_rate_ok_flag;
            p_samplerate_ret->sample_rate_khz = p_adc_channel_info->sample_rate_khz;
            p_samplerate_ret->resample_src_khz = p_adc_channel_info->resample_src_khz;
            sys_local_intc_restore(temp_flag);
        }
        break;

        case ADC_CHANNEL_CLOSE:
        //非法状态
        if ((p_adc_channel_info->ac_status != ADC_CHANNEL_STA_STOP)
            && (p_adc_channel_info->ac_status != ADC_CHANNEL_STA_OPEN))
        {
            ret = -1;
            break;
        }

        ret = adc_channel_close(handel, NULL);
        break;

        default:
        libc_printf_warning("ADC CHANNEL cmd not support:%d\n", cmd);
        ret = -1;
        break;
    }

    sys_os_sched_unlock();

    return ret;
}

int32 adc_channel_init(void)
{
    g_adc_channel_info.ac_status = ADC_CHANNEL_STA_CLOSE;

    return 0;
}

int32 adc_channel_exit(void)
{
    g_adc_channel_info.ac_status = ADC_CHANNEL_STA_CLOSE;

    return 0;
}

module_init (adc_channel_init)
module_exit (adc_channel_exit)

/*! \endcond */

