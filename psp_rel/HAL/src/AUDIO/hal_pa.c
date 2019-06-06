
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

pa_setting_t g_pa_sett;

extern void init_dac_digital(void);
extern void init_dac_analog(bool dacl_mute, bool dacr_mute);

/*******************************************************************************************/

void dac_send_max_level(void)
{
    uint8 i;
    uint32 reg_bak;
    reg_bak = *((REG32)(DAC_DIGCTL));
    *((REG32)(DAC_DIGCTL)) |= (1<<DAC_DIGCTL_DAFIS); //dac fifo input from cpu
    while ((*((REG32)(DAC_STAT)) & 0xf) != 8); //wait empty
    for (i = 0; i < 8; i++)
    {
        *((REG32)(DAC_DAT_FIFO)) = 0x7fff0000;
    }
    *((REG32)(DAC_DIGCTL)) = reg_bak;
}

void dac_send_zero(void)
{
    uint8 i;
    uint32 reg_bak;
    reg_bak = *((REG32)(DAC_DIGCTL));
    *((REG32)(DAC_DIGCTL)) |= (1<<DAC_DIGCTL_DAFIS); //dac fifo input from cpu
    while((*((REG32)(DAC_STAT)) & 0xf) != 8); //wait empty
    for (i = 0; i < 8; i++)
    {
        *((REG32)(DAC_DAT_FIFO)) = 0x00000000;
    }
    *((REG32)(DAC_DIGCTL)) = reg_bak;
}

void dac_send_ramp_level(void)
{
    uint32 reg_bak;
    uint16 num,stepsize;
    int16 level;
    num = 20000;
    stepsize = 0xffff/num;
    level = 0x7fff;

    reg_bak = *((REG32)(DAC_DIGCTL));
    *((REG32)(DAC_DIGCTL)) |= (1<<DAC_DIGCTL_DAFIS); //dac fifo input from cpu
    while (num > 0)
    {
        while(((*((REG32)(DAC_STAT))) & 0x10)); //full wait
        *((REG32)(DAC_DAT_FIFO)) = (level<<16);
        level -= stepsize;
        num--;
    }
    *((REG32)(DAC_DIGCTL)) = reg_bak;
}

void enable_pa_antipop(pa_setting_t *pa_sett)
{
    if (pa_sett->pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DPBML); //dac playback l disable

        *((REG32)(DAC_ANACTL)) |= ((1<<DAC_ANACTL_PAENL));//pa l&r enable
    }
    if (pa_sett->par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) &= ~(1 << DAC_ANACTL_DPBMR); //dac playback r disable
        *((REG32)(DAC_ANACTL)) |= ((1<<DAC_ANACTL_PAENR));//pa l&r enable
    }

    dac_send_max_level();
    g_p_sys_hal_cbks->hal_udelay(1000);

    *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_P2IB);

    *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_ATPLP2);//pa anti-pop loop2 enable
    if (pa_sett->pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_ATP2CEL);//left ramp connect enable
    }
    if (pa_sett->par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_ATP2CER);//right ramp connect enable
    }

    dac_send_ramp_level();
    g_p_sys_hal_cbks->hal_sleep(41); //delay 400ms

    if (pa_sett->pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAOSENL);//left pa outstage enable
    }
    if (pa_sett->par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAOSENR);//right pa outstage enable
    }
    g_p_sys_hal_cbks->hal_udelay(1000);

    if (pa_sett->pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_ATP2CEL));//left ramp connect disable
    }
    if (pa_sett->par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_ATP2CER));//right ramp connect disable
    }

    *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_ATPLP2); //pa anti-pop loop2 disable

    dac_send_zero();
    g_p_sys_hal_cbks->hal_udelay(1000);

    if (pa_sett->pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DPBML); //dac playback l enable
    }
    if (pa_sett->par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DPBMR); //dac playback l enable
    }
}

void disable_pa_antipop(void)
{
    //antipop流程中 disable PA 之前需要将VOL设置为最大，否则可能出现较大POP声
    *((REG32)(DAC_VOL)) = 0X4000;

    *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_PAOSENL)|(1<<DAC_ANACTL_PAOSENR));//left&right pa outstage disable

    *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_ATPLP2);//pa anti-pop loop2 disable

    *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_PAENL)|(1<<DAC_ANACTL_PAENR));    //pa l&r disable

    *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_DPBML)|(1<<DAC_ANACTL_DPBMR));    //dac playback l&r disable

    *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAVDC); //pa anti-pop discharge enable

    dac_send_max_level();
    g_p_sys_hal_cbks->hal_udelay(5000);

    if (g_pa_sett.pal_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_ATP2CEL);//left ramp connect enable
    }
    if (g_pa_sett.par_output == TRUE)
    {
        *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_ATP2CER);//right ramp connect enable
    }

    dac_send_ramp_level();
    g_p_sys_hal_cbks->hal_sleep(41); //delay 400ms

    *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_ATP2CEL)|(1<<DAC_ANACTL_ATP2CER));//left&right ramp connect disable
    *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAVDC); //pa anti-pop discharge disable
}

/*******************************************************************************************/

int32 hal_audio_enable_pa(pa_setting_t *pa_sett)
{
    g_p_sys_hal_cbks->hal_memcpy(&g_pa_sett, pa_sett, sizeof(pa_setting_t));

    hal_audio_phy_init();
    *((REG32)(CMU_MEMCLKSEL)) |= (1 << CMU_MEMCLKSEL_PCMRAMCLKSEL); //PCM RAM switch to DAC clock
    //clock gating
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_DACCLKEN); //dac clock gating enable

    init_dac_digital();

    if (pa_sett->aout_mode != AOUT_MODE_I2STX_ONLY)
    {
        *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_BIASEN); //ALL DAC&PA bias enable
        if (pa_sett->pal_output == TRUE)
        {
            *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DAENL); //dac analog l enable
        }
        if (pa_sett->par_output == TRUE)
        {
            *((REG32)(DAC_ANACTL)) |= (1 << DAC_ANACTL_DAENR); //dac analog r enable
        }

        //DAC_BIAS set
        *((REG32)(DAC_BIAS)) = 0xb0046 | (pa_sett->pa_swing << DAC_BIAS_PALRSW);

        if ((g_pa_sett.antipop == FALSE) || (pa_sett->direct_drive_enable == TRUE))
        {
            uint32 dac_anactl_val = *((REG32)(DAC_ANACTL));

            if (pa_sett->pal_output == TRUE)
            {
                dac_anactl_val |= (1 << DAC_ANACTL_DPBML); //dac playback l enable
                dac_anactl_val |= (1<<DAC_ANACTL_PAENL); //pa l enable
                dac_anactl_val |= (1<<DAC_ANACTL_PAOSENL);//left pa outstage enable
            }
            else
            {
                dac_anactl_val &= ~(1 << DAC_ANACTL_DPBML); //dac playback l disable
                dac_anactl_val &= ~(1<<DAC_ANACTL_PAENL); //pa l disable
                dac_anactl_val &= ~(1<<DAC_ANACTL_PAOSENL);//left pa outstage disable
            }
            if (pa_sett->par_output == TRUE)
            {
                dac_anactl_val |= (1 << DAC_ANACTL_DPBMR); //dac playback l enable
                dac_anactl_val |= (1<<DAC_ANACTL_PAENR); //pa r enable
                dac_anactl_val |= (1<<DAC_ANACTL_PAOSENR);//right pa outstage enable
            }
            else
            {
                dac_anactl_val &= ~(1 << DAC_ANACTL_DPBMR); //dac playback l disable
                dac_anactl_val &= ~(1<<DAC_ANACTL_PAENR); //pa r disable
                dac_anactl_val &= ~(1<<DAC_ANACTL_PAOSENR);//right pa outstage disable
            }

            if (pa_sett->direct_drive_enable == TRUE)
            {
                dac_anactl_val |= (1 << DAC_ANACTL_OPVROEN); //OPVRO enable
                dac_anactl_val &= ~(0x07<<DAC_ANACTL_OPVROOSIQ);
                dac_anactl_val |= (0x06 << DAC_ANACTL_OPVROOSIQ); //OPVROOSIQ
            }

            *((REG32)(DAC_ANACTL)) = dac_anactl_val;

            if (pa_sett->direct_drive_enable == TRUE)
            {
                *((REG32)(DAC_BIAS)) = (*((REG32)(DAC_BIAS)) & ~(DAC_BIAS_OPVROIB_MASK)) | (0x0<<DAC_BIAS_OPVROIB_SHIFT);
            }
        }
        else
        {
            enable_pa_antipop(pa_sett);
        }

        //DAC clock divisor - sr=48k
        *((REG32) (CMU_ADDACLK)) &= (~CMU_ADDACLK_DACCLKDIV_MASK);
        *((REG32) (CMU_ADDACLK)) |= (1 << CMU_ADDACLK_DACCLKDIV_SHIFT);
    }

    //I2STX配置
    if (pa_sett->aout_mode != AOUT_MODE_PA_ONLY)
    {
        hal_audio_i2stx_phy_init();
    }

    return HAL_OK;
}

int32 hal_audio_disable_pa(void)
{
    if (g_pa_sett.aout_mode != AOUT_MODE_I2STX_ONLY)
    {
        if (g_pa_sett.antipop == FALSE)
        {
            *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_DPBML)|(1<<DAC_ANACTL_DPBMR)); //dac playback l&r disable
            *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_PAOSENL)|(1<<DAC_ANACTL_PAOSENL));//left&right pa outstage disable
            *((REG32)(DAC_ANACTL)) &= ~((1<<DAC_ANACTL_PAENL)|(1<<DAC_ANACTL_PAENR));//pa l&r disable
        }
        else
        {
            disable_pa_antipop();
        }
    }

    hal_audio_phy_exit();

    return HAL_OK;
}

int32 hal_audio_mute_pa(bool mute)
{
    if (g_pa_sett.aout_mode == AOUT_MODE_I2STX_ONLY)
    {
        return HAL_OK;
    }

    if (mute == TRUE)
    {
        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_DPBML); //dac playback l disable
        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAENL); //pa l disable
        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAOSENL);//left pa outstage disable

        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_DPBMR); //dac playback r disable
        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAENR); //pa r disable
        *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAOSENR);//right pa outstage disable
    }
    else
    {
        if (g_pa_sett.pal_output == TRUE)
        {
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_DPBML); //dac playback l enable
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAENL); //pa l enable
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAOSENL);//left pa outstage enable
        }
        else
        {
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_DPBML); //dac playback l disable
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAENL); //pa l disable
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAOSENL);//left pa outstage disable
        }
        if (g_pa_sett.par_output == TRUE)
        {
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_DPBMR); //dac playback r enable
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAENR); //pa r enable
            *((REG32)(DAC_ANACTL)) |= (1<<DAC_ANACTL_PAOSENR);//right pa outstage enable
        }
        else
        {
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_DPBMR); //dac playback r disable
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAENR); //pa r disable
            *((REG32)(DAC_ANACTL)) &= ~(1<<DAC_ANACTL_PAOSENR);//right pa outstage disable
        }
    }

    return HAL_OK;
}
