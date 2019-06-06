
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define RTC_ADJUST_CLOCK 610

rtc_alarm_isr g_rtc_alarm_isr;

void hal_rtc_flush(void)
{
    uint32 i;

    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//qac
    }
    for (i = 0; i < 0x400; i++)
    {
        CLEAR_WATCH_DOG;
    }
}

void hal_rtc_phy_init(void)
{
    uint32 temp_val;

    temp_val = act_readl(HCL_4Hz);
    temp_val &= ~(1<<HCL_4Hz_HOSC_DIV_EN);
    temp_val &= ~(0x7fff<<16);
    temp_val |= (RTC_ADJUST_CLOCK<<16);//HOSC_ADJ_DAT
    temp_val |= (1<<31);//HOSC_ADJ_EN
    temp_val |= (1<<HCL_4Hz_HOSC_CAL_EN);
    act_writel(temp_val, HCL_4Hz);
    hal_rtc_flush();

    act_writel((act_readl(RTC_CTL) & ~(1<<RTC_CTL_HCL_SELECT)), RTC_CTL);
    hal_rtc_flush();
}

void hal_rtc_set_time(time_t *time)
{
    uint32 time_value = 0;

    act_writel((act_readl(RTC_CTL) & ~(1<<RTC_CTL_CAL_EN)), RTC_CTL);
    hal_rtc_flush();
    time_value |= ((uint32)(time->hour)) << 16;
    time_value |= ((uint32)(time->minute)) << 8;
    time_value |= ((uint32)(time->second));
    act_writel(time_value, RTC_DHMS);
    hal_rtc_flush();
    act_writel((act_readl(RTC_CTL) | (1<<RTC_CTL_CAL_EN)), RTC_CTL);
    hal_rtc_flush();
}

void hal_rtc_get_time(time_t *time)
{
    time->hour = (uint8)((act_readl(RTC_DHMS) & 0x1f0000)>>16);
    time->minute = (uint8)((act_readl(RTC_DHMS) & 0x3f00)>>8);
    time->second = (uint8)(act_readl(RTC_DHMS) & 0x3f);
    if (time->second == 0)
    {
        //需要防止过0错误，比如在获取min时时间是00:00:59，min=00；
        //然后在获取sec时，时钟突然跳到00:01:00，sec=0；导致时间变为00:00:00
        //解决方法就是在second==0时，再读一次hour和minute
        time->hour = (uint8)((act_readl(RTC_DHMS) & 0x1f0000)>>16);
        time->minute = (uint8)((act_readl(RTC_DHMS) & 0x3f00)>>8);
    }
}

//TODO 闰年设置
void hal_rtc_set_date(date_t *date)
{
    uint32 date_value = 0;

    act_writel((act_readl(RTC_CTL) & ~(1<<RTC_CTL_CAL_EN)), RTC_CTL);
    hal_rtc_flush();

    if (date->year < 2000)
    {
        date->year = 2000;
    }
    date_value |= ((uint32)(date->year - 2000)) << 16;
    date_value |= ((uint32)(date->month)) << 8;
    date_value |= (uint32)(date->day);
    act_writel(date_value, RTC_YMD);
    hal_rtc_flush();
    act_writel((act_readl(RTC_CTL) | (1<<RTC_CTL_CAL_EN)), RTC_CTL);
    hal_rtc_flush();
}

void hal_rtc_get_date(date_t *date)
{
    date->year = (uint16)(((act_readl(RTC_YMD) & 0x7f0000)>>16) + 2000);
    date->month = (uint8)((act_readl(RTC_YMD) & 0xf00)>>8);
    date->day = (uint8)(act_readl(RTC_YMD) & 0x1f);
}

void hal_rtc_alarm_isr(void)
{
    act_writel((act_readl(RTC_CTL)), RTC_CTL);
    hal_rtc_flush();

    g_rtc_alarm_isr();
}

void hal_rtc_set_alarm_time(time_t *time)
{
    uint32 time_value = 0;

    time_value |= ((uint32)(time->hour)) << 16;
    time_value |= ((uint32)(time->minute)) << 8;
    time_value |= ((uint32)(time->second));
    act_writel(time_value, RTC_DHMSALM);
    act_writel(0x12348765, RTC_ACCESS);
    hal_rtc_flush();
}

void hal_rtc_get_alarm_time(time_t *time)
{
    time->hour = (uint8)((act_readl(RTC_DHMSALM) & 0x1f0000)>>16);
    time->minute = (uint8)((act_readl(RTC_DHMSALM) & 0x3f00)>>8);
    time->second = (uint8)(act_readl(RTC_DHMSALM) & 0x3f);
}

void hal_rtc_enable_alarm(rtc_alarm_isr alarm_isr)
{
    if (g_rtc_alarm_isr == NULL)
    {
        g_rtc_alarm_isr = alarm_isr;

        //正在开机过程中闹钟时间到了
        if ((act_readl(RTC_CTL) & (1<<RTC_CTL_ALIP)) != 0)
        {
            g_rtc_alarm_isr();
        }

        g_p_sys_hal_cbks->hal_request_irq(IRQ_RTC, hal_rtc_alarm_isr);

        act_writel((act_readl(RTC_CTL) | (1<<RTC_CTL_CAL_EN) | (1<<RTC_CTL_ALIE)), RTC_CTL);
        hal_rtc_flush();
    }
}

void hal_rtc_disable_alarm(void)
{
    act_writel((act_readl(RTC_CTL) & ~(1<<RTC_CTL_ALIE)), RTC_CTL);
    hal_rtc_flush();
    g_p_sys_hal_cbks->hal_free_irq(IRQ_RTC);
    g_rtc_alarm_isr = NULL;
}

void hal_rtc_get_status(rtc_status_t *p_rtc_status)
{
    p_rtc_status->calendar_en = ((act_readl(HCL_4Hz) & (1<<HCL_4Hz_HOSC_CAL_EN)) != 0);
    p_rtc_status->alarm_en    = ((act_readl(RTC_CTL) & (1<<RTC_CTL_ALIE)) != 0);
}

void hal_rtc_bakdata_write(uint8 bak_idx, uint32 bak_val)
{
    uint32 rtc_bak_addr = RTC_BAK0 + bak_idx*4;

    act_writel(bak_val, rtc_bak_addr);

    hal_rtc_flush();
}

uint32 hal_rtc_bakdata_read(uint8 bak_idx)
{
    uint32 rtc_bak_addr = RTC_BAK0 + bak_idx*4;
    return act_readl(rtc_bak_addr);
}
