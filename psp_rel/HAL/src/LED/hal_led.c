
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define SEG_LED_MODE_SEL   12 //matrixled_7com_cc
#define SEG_LED_CUR_TYPE   1  //0-GPIO for FPGA, 1-Restrict Current
#define SEG_LED_DZ_SEL     0  //0-disable, 1~7 1/32 ~ 7/32 dead zone

#ifdef ENABLE_GPIO_LED_DISPLAY

#define COM_NUM  7

//IO PIN
uint8 led_io_index[8];
//定时器用以写入到gpio中的值
uint8 seg_value_reg[8];
//当前pin脚时序序号(0-6)，共7个pin脚
uint8 com_cnt;

void com_cycle(unsigned char com_num)
{
    uint32 dat0_temp, dat1_temp;
    uint32 io_ctl_reg;
    uint8 i;

    //先将所有IO口拉高，防止出现鬼影
    dat0_temp = act_readl(GPIO_ODAT0);
    dat1_temp = act_readl(GPIO_ODAT1);
    for (i = 0; i < COM_NUM; i++)
    {
        if (led_io_index[i] < 32)
        {
            dat0_temp |= (1<<led_io_index[i]);
        }
        else
        {
            dat1_temp |= (1<<(led_io_index[i]-32));
        }
    }
    act_writel(dat0_temp, GPIO_ODAT0);
    act_writel(dat1_temp, GPIO_ODAT1);

    //刷新LED管子
    for (i = 0; i < COM_NUM; i++)
    {
        io_ctl_reg = GPIO0_CTL + led_io_index[i]*4;
        if (((seg_value_reg[com_num]>>i)&0x1) || (com_num == i))
        {
            act_writel((act_readl(io_ctl_reg) & (~(0x000000cf))) | 0x41, io_ctl_reg);
        }
        else
        {
            act_writel((act_readl(io_ctl_reg) & (~(0x000000cf))) | 0x01, io_ctl_reg);
        }

        if (seg_value_reg[com_num] & (1<<i))
        {
            if (led_io_index[i] < 32)
            {
                dat0_temp |= (1<<led_io_index[i]);
            }
            else
            {
                dat1_temp |= (1<<(led_io_index[i]-32));
            }
        }
        else
        {
            if (led_io_index[i] < 32)
            {
                dat0_temp &= ~(1<<led_io_index[i]);
            }
            else
            {
                dat1_temp &= ~(1<<(led_io_index[i]-32));
            }
        }
    }
    act_writel(dat0_temp, GPIO_ODAT0);
    act_writel(dat1_temp, GPIO_ODAT1);
}

void hal_timer3_isr(void)
{
    act_writel(act_readl(T3_CTL), T3_CTL);

    com_cycle(com_cnt);
    com_cnt++;
    if (com_cnt >= COM_NUM)
    {
        com_cnt = 0;
    }
}

void hal_timer3_init(uint32 t3_period_us)
{
    uint32 latch;
    uint32 hz = 1000*1000/t3_period_us;

    //Enable Timer clock
    act_writel(act_readl(CMU_DEVCLKEN) | (1 << CMU_DEVCLKEN_TIMER3CLKEN), CMU_DEVCLKEN);

    act_writel(0, T3_CTL);

    act_writel(act_readl(T3_CTL), T3_CTL);//clear pending

    // clk of timer0/1 is HOSC(ck24M),not to care the adjust frequency
    latch = (26 * 1000000 + (hz >> 1))/hz;
    act_writel(latch, T3_VAL);

    g_p_sys_hal_cbks->hal_request_irq(IRQ_TIMER3, hal_timer3_isr);

    // enable counter, reload, irq
    act_writel((1<<T3_CTL_En)|(1<<T3_CTL_RELO)|(1<<T3_CTL_ZIEN), T3_CTL);
}

void hal_timer3_exit(void)
{
    // disable timer3
    act_writel(0, T3_CTL);
    //disable Timer clock
    act_writel(act_readl(CMU_DEVCLKEN) & ~(1 << CMU_DEVCLKEN_TIMER3CLKEN), CMU_DEVCLKEN);

    g_p_sys_hal_cbks->hal_free_irq(IRQ_TIMER3);
}

#endif


void hal_led_phy_init(void *p_led_phy_para)
{
#ifdef ENABLE_GPIO_LED_DISPLAY
    led_io_phy_para_t *p_io_ctrl_phy_para = (led_io_phy_para_t *) p_led_phy_para;
    uint8 i;
    for (i = 0; i < COM_NUM; i++)
    {
        led_io_index[i] = p_io_ctrl_phy_para->led_io_pin[i] - PAD_GPIO0;
    }
    hal_led_adjust_light(p_io_ctrl_phy_para->light_level);
    hal_timer3_init(1400); //70HZ
#else
    led_phy_para_t *p_led_ctrl_phy_para = (led_phy_para_t *) p_led_phy_para;
    uint8 seg_led_mode_sel = SEG_LED_MODE_SEL + p_led_ctrl_phy_para->common_mode;

    act_writel((act_readl(CMU_DEVCLKEN)&(~(0x1<<CMU_DEVCLKEN_SEGLCDCLKEN)))|(1<<CMU_DEVCLKEN_SEGLCDCLKEN), CMU_DEVCLKEN); //Eable LED_LCD clock
    act_writel((act_readl(CMU_SEGLCDCLK) | (0x1<<CMU_SEGLCDCLK_CLKSEL)), CMU_SEGLCDCLK); //HOSC
    act_writel((act_readl(CMU_SEGLCDCLK) & (~CMU_SEGLCDCLK_CLKDIV0_MASK)) | (0x01<<CMU_SEGLCDCLK_CLKDIV1) | (0x01<<CMU_SEGLCDCLK_CLKDIV0_SHIFT), CMU_SEGLCDCLK); //512div*2div /32/COM
    g_p_sys_hal_cbks->hal_udelay(1000);

    act_writel(act_readl(MRCR)&(~(0x1<<MRCR_SEG_LCD_LED_RESET)), MRCR);
    act_writel(act_readl(MRCR)|(0x1<<MRCR_SEG_LCD_LED_RESET), MRCR);
    g_p_sys_hal_cbks->hal_udelay(1000);

    act_writel(0, SEG_DISP_CTL);
    act_writel(((SEG_LED_DZ_SEL<<SEG_DISP_CTL_LED_COM_DZ_SHIFT)|(1<<SEG_DISP_CTL_SEGOFF)|(1<<SEG_DISP_CTL_LCD_OUT_EN)|seg_led_mode_sel), SEG_DISP_CTL);
    act_writel((p_led_ctrl_phy_para->light_level | (SEG_LED_CUR_TYPE<<4)), SEG_BIAS_EN);
    if (SEG_LED_CUR_TYPE == 1)
    {
        act_writel(0xff, SEG_RC_EN);
        act_writel((act_readl(VOUT_CTL) | (1<<VOUT_CTL_SEG_LED_EN)), VOUT_CTL);
    }
#endif
}

void hal_led_phy_exit(void)
{
#ifdef ENABLE_GPIO_LED_DISPLAY
    hal_timer3_exit();
#else
    act_writel((act_readl(SEG_DISP_CTL) & (~0xA0)), SEG_DISP_CTL);
    act_writel(act_readl(CMU_DEVCLKEN)&(~(0x1<<CMU_DEVCLKEN_SEGLCDCLKEN)), CMU_DEVCLKEN);
    act_writel(act_readl(MRCR)&(~(0x1<<MRCR_SEG_LCD_LED_RESET)), MRCR);
#endif
}

#ifndef ENABLE_GPIO_LED_DISPLAY
void hal_led_refresh(void)
{
   act_writel((act_readl(SEG_DISP_CTL) | (1<<4)), SEG_DISP_CTL);
   while ((act_readl(SEG_DISP_CTL)&(1<<4)) != 0)//wait_refresh_compelet
   {
   }
}
#endif

void hal_led_display(uint32 *p_seg_disp_data)
{
#ifdef ENABLE_GPIO_LED_DISPLAY
    uint32 cpu_sr;
    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    g_p_sys_hal_cbks->hal_memcpy(seg_value_reg,p_seg_disp_data,sizeof(seg_value_reg));

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
#else
    act_writel(*(p_seg_disp_data+0), SEG_DISP_DATA0);
    act_writel(*(p_seg_disp_data+1), SEG_DISP_DATA1);
    hal_led_refresh();
#endif
}

void hal_led_adjust_light(uint8 light_level)
{
#ifdef ENABLE_GPIO_LED_DISPLAY
    uint32 cpu_sr;
    uint32 io_ctl_reg;
    uint8 i;
    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    light_level &= (0x07);
    for (i = 0; i < COM_NUM; i++)
    {
        io_ctl_reg = GPIO0_CTL + led_io_index[i]*4;
        act_writel((act_readl(io_ctl_reg) & (~(0x00007000))) | (light_level<<12), io_ctl_reg);
    }

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
#else
    uint32 seg_bias_en_value;

    seg_bias_en_value = act_readl(SEG_BIAS_EN);
    seg_bias_en_value &= ~(SEG_BIAS_EN_LED_SEG_BIAS_MASK);
    seg_bias_en_value |= light_level;
    act_writel(seg_bias_en_value, SEG_BIAS_EN);
#endif
}
