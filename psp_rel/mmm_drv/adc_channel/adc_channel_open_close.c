/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道打开和关闭。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    打开音频播放中间件
 * \param[in]   param  打开音乐播放中间件的参数，目前设置为空
 * \return       操作句柄
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *adc_channel_open(void *param)
{
    adc_channel_info_t *p_adc_channel_info = &g_adc_channel_info;

    libc_printf_info("ADC Channel OPEN!\n");

    //OPEN之后状态切换为OPEN
    p_adc_channel_info->ac_status = ADC_CHANNEL_STA_OPEN;

    return (void *) p_adc_channel_info;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭音乐播放中间件
 * \param[in]   handel  打开的音乐播放中间件操作句柄
 * \param[out]  param 关闭中间件所需保存数据的指针，目前设置为空
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 adc_channel_close(adc_channel_info_t *p_adc_channel_info, void *param)
{
    libc_printf_info("ADC Channel CLOSE!\n");

    //CLOSE之后状态切换为CLOSE
    p_adc_channel_info->ac_status = ADC_CHANNEL_STA_CLOSE;

    return 0;
}
