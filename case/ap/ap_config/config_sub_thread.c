/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机应用使能IC内部PA，这是耗时较长的初始化任务，为了提升开机体验使用子线程
 *       并行初始化；其他耗时较长的初始化任务也可以放在这里执行。
 * 作者：cailizhen
 ********************************************************************************/

#include "ap_config.h"

uint8 __section__(".stack.ap_high_stack_ptos")ap_high_stk_ptos[1];//动态数组
extern uint16 _AP_HIGH_STK_SIZE;

void *open_pa(void *para)
{
    pa_setting_t pa_sett;
    aud_output_channel_e aud_output_chan;

    //初始化关闭外部功放
    com_speaker_off(SPK_SYS);

    pa_sett.aout_mode = (audio_output_mode_e) com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_MODE);
    if (pa_sett.aout_mode != AOUT_MODE_PA_ONLY)
    {
        pad_phy_para_t phy_para;

        set_pad_function(I2STX_BCLK_PIN, I2STX_BCLK_FUNC, NULL, 0);
        set_pad_function(I2STX_LRCLK_PIN, I2STX_LRCLK_FUNC, NULL, 0);
        set_pad_function(I2STX_MCLK_PIN, I2STX_MCLK_FUNC, NULL, 0);
        set_pad_function(I2STX_DOUT_PIN, I2STX_DOUT_FUNC, NULL, 0);

        pa_sett.i2stx_work_mode = I2STX_WORK_MASTER; //I2STX只能作Master，否则无法微调Clock，会导致播歌断音
        pa_sett.i2stx_width = I2STX_EFF_WIDTH;
        pa_sett.i2stx_data_mode = I2STX_DATA_MODE;
    }

    //打开PA，PA开机后常开
    pa_sett.pa_swing = (pa_swing_mode_e) com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_PA_SWING);
    aud_output_chan = (aud_output_channel_e) com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    pa_sett.direct_drive_enable = com_get_config_default(SETTING_HARDWARE_EARPHONE_DDV_ENABLE);
    pa_sett.pal_output = TRUE;
    pa_sett.par_output = TRUE;
    if ((aud_output_chan == AUD_OUTPUT_MIX_LEFT_CHAN) || (aud_output_chan == AUD_OUTPUT_LEFT_CHAN))
    {
        pa_sett.par_output = FALSE;
    }
    if ((aud_output_chan == AUD_OUTPUT_MIX_RIGHT_CHAN) || (aud_output_chan == AUD_OUTPUT_RIGHT_CHAN))
    {
        pa_sett.pal_output = FALSE;
    }

    //是否使用耳机输出，使用耳机输出开启PA时需要给PA充电耗时较长，关闭PA时需要给PA放电耗时也较长
    pa_sett.antipop = (bool) com_get_config_default(SETTING_HARDWARE_EARPHONE_ENABLE);

    enable_pa(&pa_sett);

    //默认为D类功放，以降低功耗
    com_speaker_ab_d_switch(PA_SWITCH_D);

    if (com_get_config_default(SETTING_HARDWARE_EXTERN_PA_MUTE_ENABLE) == FALSE)
    {
        com_speaker_on(SPK_SYS);
    }
    else
    {
        //仅使能DC升压，使能DC升压使用软件PWM需要锁掉度，最好放在开机过程中进行
        com_speaker_on(SPK_EN_DCDC);
    }

    set_ic_pa_inited(TRUE);

    libc_printf_info("PA open thread quit @%d\n", sys_get_ab_timer());

    /*注意：libc_pthread_exit无法返回*/
    libc_pthread_exit();
    return NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    创建PA Anti-pop子线程
 * \param[in]   param
 * \return       操作句柄
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *pa_thread_open(void *param)
{
    pthread_param_t g_pa_thread;

    pthread_param_t *p_thread_param = &g_pa_thread;

    //初始化创建线程的参数
    p_thread_param->start_rtn = open_pa;
    p_thread_param->arg = param;
    p_thread_param->ptos = (OS_STK *) ap_high_stk_ptos;
    p_thread_param->stk_size = (uint32)&_AP_HIGH_STK_SIZE;

    //禁止调度
    sys_os_sched_lock();

    libc_printf_info("Create PA open thread @%d\n", sys_get_ab_timer());
    //创建PA Anti-pop子线程
    if (libc_pthread_create(p_thread_param, AP_FRONT_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        libc_printf_error("Create PA open thread Fail!!\n");
        sys_os_sched_unlock();
        return NULL;
    }
    sys_os_sched_unlock();
    return NULL;
}
