#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

extern void udelay(UINT32 us);

uint32 bCMU_LRADCCLK = 0, bGPIO7_CTL = 0, bCMU_DEVCLKEN = 0;

/*
  GPIO7 用于ADFU KEY 检测，使用LRADC1  0x01
  gpio_pu:0 : 外部上拉  1:内部上拉
*/
void mbrec_key_scan_init(uint8 adc_sel,uint8 gpio_pu)
{
    bCMU_LRADCCLK = act_readl(CMU_LRADCCLK);
    bCMU_DEVCLKEN = act_readl(CMU_DEVCLKEN);

    act_writel((1 << CMU_DEVCLKEN_LRADCCLKEN) | act_readl(CMU_DEVCLKEN), CMU_DEVCLKEN);
    act_writel(0x02, CMU_LRADCCLK);
    if ((adc_sel & 0x01) != 0)
    {
        act_writel(act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_LRADC1_EN),PMUADC_CTL);
    }

    udelay(2000);
    if (gpio_pu == 1)
    {
        act_writel(act_readl(LRADC_RES_SET) & (~(1 << LRADC_RES_SET_R_SEL)),LRADC_RES_SET);//lradc1内部上拉
    }
    else
    {
        act_writel(act_readl(LRADC_RES_SET) | (1 << LRADC_RES_SET_R_SEL),LRADC_RES_SET);//外部上拉
    }

    if ((adc_sel & 0x01) != 0)
    {
        bGPIO7_CTL = act_readl(GPIO7_CTL);
        act_writel((1 << GPIO7_CTL_AD_Select) | act_readl(GPIO7_CTL), GPIO7_CTL);
        udelay(2000);
    }
}

/*
  mode:
  0: check adfu key
  1: check card upgrade key
*/
uint32 read_key_value(void)
{
    return (act_readl(LRADC1_DATA) & LRADC1_DATA_LRADC1_MASK);
}

void exit_mbrec_key(void)
{
    act_writel(bGPIO7_CTL, GPIO7_CTL);
    act_writel(bCMU_LRADCCLK, CMU_LRADCCLK);
    act_writel(bCMU_DEVCLKEN, CMU_DEVCLKEN);
}
