
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

extern uint8 __map_sr2reg_48(uint8 sr);
extern uint8 __map_sr2reg_44(uint8 sr);

/*adc_rate 8 - 48*/
void hal_audio_set_i2srx_rate(uint8 i2srx_rate)
{
    uint32 i2srx_div_val = 0xff;
    uint32 aps_val;

    i2srx_div_val = __map_sr2reg_48(i2srx_rate);
    if (i2srx_div_val == 0xff)
    {
        i2srx_div_val = __map_sr2reg_44(i2srx_rate);
        if (i2srx_div_val == 0xff)
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

    //set audio pll clk
    *((REG32)(AUDIO_PLL_CTL)) &= (~AUDIO_PLL_CTL_APS_MASK);
    *((REG32)(AUDIO_PLL_CTL)) |= (aps_val << AUDIO_PLL_CTL_APS_SHIFT);

    //I2SRx clock divisor
    *((REG32)(CMU_I2SCLK)) &= (~CMU_I2SCLK_I2SCLKDIV_MASK);
    *((REG32)(CMU_I2SCLK)) |= (i2srx_div_val << CMU_I2SCLK_I2SCLKDIV_SHIFT); //set i2sclk
}

void hal_audio_enable_i2srx(i2srx_setting_t *i2srx_sett)
{
    //i2s clk set
    *((REG32)(CMU_I2SCLK)) &= ~CMU_I2SCLK_I2SRXCLKSEL_MASK;
    *((REG32)(CMU_I2SCLK)) |= (0<<CMU_I2SCLK_I2SRXCLKSEL_SHIFT); //iisrx defaul set to i2s_clk
    hal_audio_set_i2srx_rate(i2srx_sett->sample_rate);

    //i2srx clock gating enable
    *((REG32)(CMU_DEVCLKEN)) |= (1 << CMU_DEVCLKEN_I2SRXCLKEN);

    //iisrx mode set
    *((REG32)(I2S_RXCTL)) &= ~(1<<I2S_RXCTL_I2S_RXMODE);
    *((REG32)(I2S_RXCTL)) |= (i2srx_sett->i2srx_work_mode<<I2S_RXCTL_I2S_RXMODE);//I2SRX select master/slave mode

    *((REG32)(I2S_RXCTL)) |= (1<<I2S_RXCTL_I2S_RX_SMCLK);
    if ((i2srx_sett->i2srx_work_mode == I2SRX_WORK_SLAVE) && (i2srx_sett->i2srx_slave_mclk_use_inner == 1))
    {
        *((REG32)(I2S_RXCTL)) &= ~(1<<I2S_RXCTL_I2S_RX_SMCLK);
    }

    *((REG32)(I2S_RXCTL)) &= (~I2S_RXCTL_I2S_RXWIDTH_MASK);
    *((REG32)(I2S_RXCTL)) |= (i2srx_sett->i2srx_width<<I2S_RXCTL_I2S_RXWIDTH_SHIFT);//DATA width

    *((REG32)(I2S_RXCTL)) &= (~I2S_RXCTL_I2S_RXMODELSEL_MASK);
    *((REG32)(I2S_RXCTL)) |= (i2srx_sett->i2srx_data_mode<<I2S_RXCTL_I2S_RXMODELSEL_SHIFT);//RX data model

    //iisrx fifo set
    *((REG32)(I2S_RXFIFOCTL)) |= (0x01<<I2S_RXFIFOCTL_RXFOS_SHIFT) ; //I2SRX FIFO output select DMA,and input defaul from i2srx
    *((REG32)(I2S_RX_STA))    |= (1<<I2S_R0_STA_RXFIP);             //clean irq pending
    *((REG32)(I2S_RXFIFOCTL)) |= (1<<I2S_RXFIFOCTL_RXFFDE);       //enable rx fifo full drq
    *((REG32)(I2S_RXFIFOCTL)) &= ~(1<<I2S_RXFIFOCTL_RXFRT);       //reset rx fifo
    *((REG32)(I2S_RXFIFOCTL)) |= (1<<I2S_RXFIFOCTL_RXFRT);        //normal rx fifo

    //iisrx enable
    *((REG32)(I2S_RXCTL))  |=  (1<<I2S_RXCTL_I2S_RXEN);          //i2s RX enable
}

void hal_audio_disable_i2srx(void)
{
    //iisrx disable
    *((REG32)(I2S_RXCTL)) &= ~(1<<I2S_RXCTL_I2S_RXEN);          //i2s RX disale
}
