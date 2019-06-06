
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

extern pa_setting_t g_pa_sett;

static const uint8 sample_rate_tbl_48[8] =
{ 96, 48, 32, 24, 16, 12, 8, 0 };
static const uint8 sample_rate_tbl_44[8] =
{ 88, 44, 0, 22, 0, 11, 0, 0 };

uint8 __map_sr2reg_48(uint8 sr)
{
    uint8 i;

    for (i = 0; i < 8; i++)
    {
        if (sample_rate_tbl_48[i] == sr)
        {
            return i;
        }
    }

    return 0xff;
}

uint8 __map_sr2reg_44(uint8 sr)
{
    uint8 i;

    for (i = 0; i < 8; i++)
    {
        if (sample_rate_tbl_44[i] == sr)
        {
            return i;
        }
    }

    return 0xff;
}

void init_dac_digital(void)
{
    //dac digital set
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~((1<<DAC_DIGCTL_ENDITH)))) | (0x1<<DAC_DIGCTL_ENDITH); //dac dith enable
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_OSRDA_MASK))) | (0x0<<DAC_DIGCTL_OSRDA_SHIFT); //dac osr set 32x
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_INTFRS_MASK))) | (0x2<<DAC_DIGCTL_INTFRS_SHIFT); //dac cic filter set 4x

    //dac fifo set
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(1 << DAC_DIGCTL_DACHNUM))) | (0x0<<DAC_DIGCTL_DACHNUM); //dac channel select stereo
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(1 << DAC_DIGCTL_DAFIS))) | (0 << DAC_DIGCTL_DAFIS); //dac fifo input from cpu
    *((REG32)(DAC_DIGCTL)) = (*((REG32)(DAC_DIGCTL))) | (1 << DAC_DIGCTL_DAFEDE); //dac fifo empty drq enable
    if ((*((REG32)(DAC_DIGCTL)) & (1<<DAC_DIGCTL_DDEN)) == 0)
    {
        *((REG32)(DAC_DIGCTL)) = (*((REG32)(DAC_DIGCTL))) & (~(1 << DAC_DIGCTL_DAFRT)); //dac fifo reset
        *((REG32)(DAC_DIGCTL)) = (*((REG32)(DAC_DIGCTL))) | (1 << DAC_DIGCTL_DAFRT); //dac fifo normal
    }
    //pcm_buf set
    *((REG32)(PCM_BUF_STAT)) = (*((REG32)(PCM_BUF_STAT))) | (1 << PCM_BUF_STAT_PCMBEIP); //clear pacm_buf empty pending

    *((REG32)(DAC_DIGCTL)) |= (1 << DAC_DIGCTL_DDEN); //dac digital enable
}

void init_dac_analog(bool dacl_mute, bool dacr_mute)
{
    *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_BIASEN); //ALL DAC&PA bias enable
    if (dacl_mute == FALSE)
    {
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DAENL); //dac analog l enable
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DPBML); //dac playback l enable
    }
    else
    {
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DAENL); //dac analog l disable
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DPBML); //dac playback l disable
    }
    if (dacr_mute == FALSE)
    {
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DAENR); //dac analog r enable
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DPBMR); //dac playback r enable
    }
    else
    {
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DAENR); //dac analog r disable
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DPBMR); //dac playback r disable
    }
}

/*return value: 8-48*/
uint8 hal_audio_get_dac_rate(void)
{
    uint32 temp1,temp2;
    uint8 ret = 0;

    if (g_pa_sett.aout_mode != AOUT_MODE_I2STX_ONLY)
    {//I2STX clock 与 ADDA clock同步
        temp1 = *((REG32)(CMU_ADDACLK)) & CMU_ADDACLK_DACCLKDIV_MASK;
    }
    else
    {
        temp1 = *((REG32)(CMU_I2SCLK)) & CMU_I2SCLK_I2SCLKDIV_MASK;
    }
    temp2 = (*((REG32)(AUDIO_PLL_CTL)) & AUDIO_PLL_CTL_APS_MASK) >> AUDIO_PLL_CTL_APS_SHIFT;
    if (temp2 < 8)
    {
        /*44.1kHz 系列对应22M*/
        ret = sample_rate_tbl_44[temp1];
    }
    else
    {
        /*48kHz 系列对应24M*/
        ret = sample_rate_tbl_48[temp1];
    }

    return ret;
}

void hal_audio_set_dac_rate_do(uint8 dac_rate)
{
    uint32 dac_div_val = 0xff;
    uint32 aps_val;
    uint32 kt_overtime;

    //等待按键音结束后才能调采样率
    kt_overtime = g_p_sys_hal_cbks->hal_get_ab_timer_us();
    while (1)
    {
        if (hal_audio_get_key_tone_status() == 0)
        {
            break;
        }

        if ((g_p_sys_hal_cbks->hal_get_ab_timer_us() - kt_overtime) > 400*1000)
        {
            *((REG32)(KT_CTL)) &= ~(1 << KT_CTL_SKT);
            g_p_sys_hal_cbks->hal_printf("keytone play overtime error!!");
            break;
        }
    }

    dac_div_val = __map_sr2reg_48(dac_rate);
    if (dac_div_val == 0xff)
    {
        dac_div_val = __map_sr2reg_44(dac_rate);
        if (dac_div_val == 0xff)
        {
            g_p_sys_hal_cbks->hal_printf("invalid sample rate!");
            return;
        }

        aps_val = 0x04; //fast 0%
    }
    else
    {
        aps_val = 0x0C; //fast 0%
    }

    if (dac_rate == 8)
    {
        *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_OSRDA_MASK))) | (0x1<<DAC_DIGCTL_OSRDA_SHIFT); //dac osr set 64x
        *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_INTFRS_MASK))) | (0x3<<DAC_DIGCTL_INTFRS_SHIFT); //dac cic filter set 8x
    }
    else
    {
        *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_OSRDA_MASK))) | (0x0<<DAC_DIGCTL_OSRDA_SHIFT); //dac osr set 32x
        *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(DAC_DIGCTL_INTFRS_MASK))) | (0x2<<DAC_DIGCTL_INTFRS_SHIFT); //dac cic filter set 4x
    }

    //set audio pll clk
    *((REG32)(AUDIO_PLL_CTL)) &= (~AUDIO_PLL_CTL_APS_MASK);
    *((REG32)(AUDIO_PLL_CTL)) |= (aps_val << AUDIO_PLL_CTL_APS_SHIFT);

    //DAC clock divisor
    *((REG32)(CMU_ADDACLK)) &= (~CMU_ADDACLK_DACCLKDIV_MASK);
    *((REG32)(CMU_ADDACLK)) |= (dac_div_val << CMU_ADDACLK_DACCLKDIV_SHIFT);

    if (g_pa_sett.aout_mode != AOUT_MODE_PA_ONLY)
    {
        //I2STX clock divisor
        *((REG32) (CMU_I2SCLK)) &= (~CMU_I2SCLK_I2SCLKDIV_MASK);
        *((REG32) (CMU_I2SCLK)) |= (dac_div_val << CMU_I2SCLK_I2SCLKDIV_SHIFT);
    }
}

void hal_audio_set_dac_rate(uint8 dac_rate)
{
    if (hal_audio_get_dac_rate() == dac_rate)
    {
        /*采样率跟之前的一致则无需重复设置*/
        return;
    }

    hal_audio_set_dac_rate_do(dac_rate);
}

void hal_audio_wait_dacfifo_empty(void)
{
    while (1)
    {
        //wait dac fifo empty 否则PCM_BUF_STAT读可能会出错
        if (((*((REG32)(DAC_STAT))) & (1 << DAC_STAT_DAFEIP)) != 0)
        {
            break;
        }
    }

    *((REG32)(DAC_STAT)) = (1 << DAC_STAT_DAFEIP);
}

void hal_audio_enable_dac(dac_setting_t *dac_sett)
{
    uint32 chnum;

    *((REG32)(CMU_MEMCLKSEL)) |= (1 << CMU_MEMCLKSEL_PCMRAMCLKSEL); //PCM RAM switch to DAC clock
    //clock gating
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_DACCLKEN); //dac clock gating enable

    init_dac_digital();

    if (g_pa_sett.aout_mode != AOUT_MODE_I2STX_ONLY)
    {
        init_dac_analog(dac_sett->dacl_mute, dac_sett->dacr_mute);
    }

    hal_audio_set_dac_rate_do(dac_sett->sample_rate);

    if (dac_sett->mono == FALSE)
    {
        chnum = 0x0 << DAC_DIGCTL_DACHNUM;
        *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(1 << DAC_DIGCTL_DALRMIX)))
                | (0x0 << DAC_DIGCTL_DALRMIX); //dac left mix right
    }
    else
    {
        chnum = 0x1 << DAC_DIGCTL_DACHNUM;
    }
    *((REG32)(DAC_DIGCTL)) = ((*((REG32)(DAC_DIGCTL))) & (~(1 << DAC_DIGCTL_DACHNUM))) | chnum; //dac channel select stereo or mono

    hal_audio_set_audio_aps(dac_sett->aps);
}

void hal_audio_disable_dac(void)
{
    //dac fifo empty drq disable
    *((REG32)(DAC_DIGCTL)) &= (~(1 << DAC_DIGCTL_DAFEDE));
    //if ((*((REG32)(DAC_DIGCTL)) & (1<<DAC_DIGCTL_DDEN)) == 0)
    //{
    //    //DAC FIFO reset
    //    *((REG32)(DAC_DIGCTL)) &= (~(1 << DAC_DIGCTL_DAFRT));
    //}
    //dac dith disable
    //*((REG32)(DAC_DIGCTL)) &= (~(1<<DAC_DIGCTL_ENDITH));
    //dac digital disable 按键音也要从DAC DIGITAL播放出来，所以不能关掉
    //*((REG32)(DAC_DIGCTL)) &= (~(1<<DAC_DIGCTL_DDEN));
}

void hal_audio_i2stx_phy_init(void)
{
    uint32 temp;

    if (g_pa_sett.i2stx_width != I2STX_WIDTH_16BIT)
    {
        //RAM4 switch to DAC
        if ((act_readl(CMU_MEMCLKSEL) & CMU_MEMCLKSEL_RAM4CLKSEL_MASK) == (0 << CMU_MEMCLKSEL_RAM4CLKSEL_SHIFT))
        {
            g_p_sys_hal_cbks->hal_memset((void *)0x9fc1f000, 0, 0x1000); //ram4 addr = 0x9fc1f000, size = 4kB
        }
        act_writel((act_readl(CMU_MEMCLKSEL) & ~(CMU_MEMCLKSEL_RAM4CLKSEL_MASK)) | (2 << CMU_MEMCLKSEL_RAM4CLKSEL_SHIFT), CMU_MEMCLKSEL);
    }

    //I2STX Clock SRC
    temp = *((REG32) (CMU_I2SCLK));
    temp &= ~(CMU_I2SCLK_I2STXCLKSEL_MASK);
    temp |= (0<<CMU_I2SCLK_I2STXCLKSEL_SHIFT); //iistx set to i2s_clk
    //I2STX clock divisor - sr=48k
    temp &= (~CMU_I2SCLK_I2SCLKDIV_MASK);
    temp |= (1 << CMU_I2SCLK_I2SCLKDIV_SHIFT);
    *((REG32) (CMU_I2SCLK)) = temp;

    //i2stx clock gating enable
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_I2STXCLKEN);
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_I2SRXCLKEN);

    *((REG32) (I2S_TXCTL)) &= (~(1<<I2S_TXCTL_I2S_TXMODE));
    *((REG32) (I2S_TXCTL)) |= (g_pa_sett.i2stx_work_mode<<I2S_TXCTL_I2S_TXMODE);
    *((REG32) (I2S_TXCTL)) &= (~I2S_TXCTL_I2S_TXWIDTH_MASK);
    *((REG32) (I2S_TXCTL)) |= (g_pa_sett.i2stx_width<<I2S_TXCTL_I2S_TXWIDTH_SHIFT);
    *((REG32) (I2S_TXCTL)) &= (~I2S_TXCTL_I2S_TXMODELSEL_MASK);
    *((REG32) (I2S_TXCTL)) |= (g_pa_sett.i2stx_data_mode<<I2S_TXCTL_I2S_TXMODELSEL_SHIFT);
    *((REG32) (I2S_TXCTL)) |= (1 << I2S_TXCTL_I2S_TXEN);
}
