/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RTC AiP8563_H ģ����������ʵ�֡�
 * ���ߣ�wangli
 ********************************************************************************/

#include "ext_rtc_inner.h"
#include "rtc_AiP8563.h"

#if (SUPPORT_EXT_RTC == 1)

ext_rtc_alarm_isr g_ext_rtc_alram_isr = NULL;
ext_rtc_alarm_isr alarm_inner_isr = NULL;

/*********************************************************************************
 * \         int RTC_WriteReg(uint8 RegAddr, uint8 Data)
 * \
 * \Description : д�Ĵ���
 * \
 * \Arguments :  RegAddr:Ҫд�ļĴ���;Data:Ҫд��ļĴ���ֵ
 * \Returns :   0 success -1 fail
 * \
 * \Notes :
 * \
*********************************************************************************/
int RTC_WriteReg(uint8 RegAddr, uint8 Data)
{
    uint8 i;
    uint32 flags;
    int result = 0;
    uint8 tempbuf[2];
    uint8 ret = 0;

    //register addr
    tempbuf[0] = RegAddr;
    //register data
    tempbuf[1] = Data;

    for (i = 0; i < 10; i++)
    {
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//���ж�
#else
        sys_os_sched_lock();
#endif
        ret = twi_write_bytes(TWI_MASTER_SIM, rtc_twi_master_idx, tempbuf, I2C_WRITE_ADDR, 2, TWI_READ_MODE_NO);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        sys_local_intc_restore(flags);//���ж�
#else
        sys_os_sched_unlock();
#endif

        //send 3 byte to fm module
        if (ret != 0)
        {
            break;
        }
    }

    if (i == 10)
    {
        result = -1;
    }

    if (result == -1)
    {
        libc_printf_warning("RTC_WriteReg fail!\n");
    }

    return result;
}
/*********************************************************************************
 * \           uint8 RTC_ReadReg(uint8 RegAddr)
 * \
 * \Description : ���Ĵ�����ֵ
 * \
 * \Arguments :  RegAddr:Ҫ���ļĴ���
 * \Returns :   �����ļĴ���ֵ
 * \
 * \Notes :
 * \
*********************************************************************************/
uint8 RTC_ReadReg(uint8 RegAddr)
{
    uint8 i;
    uint8 Data = 0;
    uint32 flags;
    uint8 tempbuf[2];
    uint8 ret = 0;

    for (i = 0; i < 10; i++)
    {
        tempbuf[0] = RegAddr;

#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//���ж�
#else
        sys_os_sched_lock();
#endif
        ret = twi_write_bytes(TWI_MASTER_SIM, rtc_twi_master_idx, tempbuf, I2C_WRITE_ADDR, 1, TWI_READ_MODE_YES);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
        sys_local_intc_restore(flags);//���ж�
#else
        sys_os_sched_unlock();
#endif

        if (ret != 0)
        {
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
            flags = sys_local_intc_save(0x01|(1<<IRQ_TIMER3));//���ж�
#else
            sys_os_sched_lock();
#endif
            ret = twi_read_bytes(TWI_MASTER_SIM, rtc_twi_master_idx, tempbuf, I2C_READ_ADDR, 1);
#if (TWI_MASTER_MULTY_CALLERS_AND_INT_CALL == 1)
            sys_local_intc_restore(flags);//���ж�
#else
            sys_os_sched_unlock();
#endif

            if (ret != 0)
            {
                Data = tempbuf[0];
                break;
            }
        }
    }

    if (i == 10)
    {
        libc_printf_warning("RTC_ReadReg fail!\n");
    }

    return Data;
}



/**********************************************************************
 * \int RTC_SetRegBit(uint8 reg, uint8 bitMask, uint8 data_val)
 * \Description: set register specified bit
 * \
 * \Parameters:
 * \reg:        register that will be set
 * \bitMask:    mask specified bit of register
 * \data_val:    data will be set for specified bit
 * \Return Value:0 success; -1 fail
 * \None
 **********************************************************************/
int RTC_SetRegBit(uint8 RegAddr, uint8 bitMask, uint8 data_val)
{
    uint8 result;
    //read register
    result = RTC_ReadReg(RegAddr);
    //mask bits clear
    result &= (uint8) (~bitMask);
    //mask bits set
    result |= (data_val & bitMask);

    return RTC_WriteReg(RegAddr, result);
}

/*********************************************************************************
 * \
     unsigned char RTC_BinToBcd2(unsigned char BINValue)
 * \
 * \Description : ��BINת��ΪBCD
 * \
 * \Arguments :  ����ʮ����
 * \Returns :   BCD��ʽ
 * \
 * \Notes :
 * \
*********************************************************************************/

static uint8 RTC_BinToBcd(uint8 Bin_Val)
{
    uint8 bcdhigh = 0;

    while (Bin_Val >= 10)
    {
        bcdhigh++;
        Bin_Val -= 10;
    }

    return ((uint8)(bcdhigh << 4) | Bin_Val);
}


/*********************************************************************************
 * \
         unsigned char RTC_Bcd2ToBin(unsigned char BCDValue)
 * \
 * \Description :  ��BCDת��ΪBIN
 * \
 * \Arguments :  ����BCD
 * \Returns :   ʮ���Ƹ�ʽ
 * \
 * \Notes :
 * \
*********************************************************************************/

static uint8 RTC_BcdToBin(uint8 BCD_Val)
{
    uint8 tmp = 0;

    tmp = ((BCD_Val & 0xF0) >> 0x04) * 10;
    return (tmp + (BCD_Val & 0x0F));
}

void sRTC_uninit(void)
{
    uint8 temp = 0;

    temp = RTC_ReadReg(CTL_STA_1);  //��ȡ����/״̬�Ĵ���1
    temp |= (1<<5) ;  //ֹͣ����

    RTC_WriteReg(CTL_STA_1, temp);
}

uint8 sRTC_Check(void)
{
    uint8 test_val;
    uint8 Time_Count;
    if(RTC_ReadReg(TIMER_CTL)&0x80)
    {
        RTC_WriteReg(TIMER_CTL, (uint8)(~(1<<7)));
        Time_Count = RTC_ReadReg(TIMER_VAL);
    }
    RTC_WriteReg(TIMER_VAL, 0x55);
    for(test_val = 0; test_val<10; test_val++)
    {
        ;
    }
    test_val = RTC_ReadReg(TIMER_VAL);
    if(Time_Count != 0)
    {
        RTC_WriteReg(TIMER_VAL, Time_Count);
        RTC_WriteReg(TIMER_CTL, (1<<7));
    }
    if(test_val != 0x55)
    {
        return 1;//ng
    }
    return 0;//ok
}

void sRTC_SetMode(uint8 Mode)
{
    uint8 temp = 0;

    temp = RTC_ReadReg(CTL_STA_1);  //��ȡ�Ĵ���ֵ
    if (Mode == 0x06)  //EXT_CLK����ģʽ
    {
        temp |= (1<<7);
    }
    else if (Mode == 0x05)
    {
        temp &= ~(1<<7);
        temp &= ~(1<<3);  //��Դ��λ����ʧЧ
    }

    RTC_WriteReg(CTL_STA_1, temp);
}

void sRTC_SetPowerReset(uint8 NewState)
{
    uint8 TestC = 0;

    TestC = RTC_ReadReg(CTL_STA_1);  //��ȡ�Ĵ���ֵ
    TestC &= ~(1<<3);  //���֮ǰ����
    if (NewState == 0x08)  //��λ������Ч
    {
        TestC |= 0x08;
    }
    else if (NewState == 0x09)
    {
        TestC &= ~0x08;  //ʧЧ����ͨģʽ��ֵ�߼�0����ʧЧ
    }

    RTC_WriteReg(CTL_STA_1, TestC);
}

void sRTC_set_Time(time_t* time)
{
    RTC_WriteReg(SECOND,       RTC_BinToBcd(time->second));
    RTC_WriteReg(MINUTE,       RTC_BinToBcd(time->minute));
    RTC_WriteReg(HOUR,         RTC_BinToBcd(time->hour));
}

void sRTC_get_Time(time_t* time)
{
    time->hour =    RTC_BcdToBin( RTC_ReadReg(HOUR));
    time->minute =  RTC_BcdToBin( RTC_ReadReg(MINUTE));
    time->second =  RTC_BcdToBin( RTC_ReadReg(SECOND));
    if(time->second == 0)
    {
        //��Ҫ��ֹ��0���󣬱����ڻ�ȡminʱʱ����00:00:59��min=00��
        //Ȼ���ڻ�ȡsecʱ��ʱ��ͻȻ����00:01:00��sec=0������ʱ���Ϊ00:00:00
        //�������������second==0ʱ���ٶ�һ��hour��minute
        time->hour =    RTC_BcdToBin( RTC_ReadReg(HOUR));
        time->minute =  RTC_BcdToBin( RTC_ReadReg(MINUTE));
    }
}

void sRTC_set_Date(date_t* date)
{
    uint8 century;
    uint8 year;

    //����  year month day
    //��
    RTC_WriteReg(DAY,RTC_BinToBcd(date->day));
    century = (date->year>=2000) ? 0 : 1;

    //�� + ����
    RTC_WriteReg(MONTH_CENTURY, (RTC_BinToBcd(date->month) | century<<CENTURY_OFFSET));

    //���
    year = (date->year>=2000)? date->year-2000 : date->year-1900;
    RTC_WriteReg(YEAR, RTC_BinToBcd(year));
}

void sRTC_get_Date(date_t* date)
{
    date->day   = RTC_BcdToBin( RTC_ReadReg(DAY));
    date->month = RTC_BcdToBin( RTC_ReadReg(MONTH_CENTURY));
    date->year  = RTC_BcdToBin( RTC_ReadReg(YEAR))+
            (RTC_ReadReg(MONTH_CENTURY) & CENTURY_MASK) ? 1900 : 2000;
}

void sRTC_get_AlarmTime(time_t* time)
{
    time->hour   = RTC_BcdToBin( RTC_ReadReg(ALARM_HOUR)&ALARM_MASK);
    time->minute = RTC_BcdToBin( RTC_ReadReg(ALARM_MINUTE)&ALARM_MASK);
    time->second = 0;

}

void sRTC_set_AlarmTime(time_t* time)
{
    /*
    ��ÿ�ιر�����֮�󣬻�������һ�����ӣ�������ǵ�ǰ�����ӣ���Ҫ�ȴ�24��Сʱ���ٴ�����,����ٴ����õ�ǰ���ӣ�
    �ᵼ���������֮�����������rtc�����ӣ����¸ոչرվ��ֱ�����
    */
    if( (time->hour == RTC_BcdToBin( (RTC_ReadReg(ALARM_HOUR))&ALARM_MASK)) &&
        (time->minute == RTC_BcdToBin( (RTC_ReadReg(ALARM_MINUTE))&ALARM_MASK)) )
    {
        libc_printf("[Ext RTC] existed a same alarm, ignore\n");
    }
    else
    {
        RTC_WriteReg(ALARM_HOUR, ((RTC_BinToBcd(time->hour))| PCF_ALARM_HOURSCLOSE));
        RTC_WriteReg(ALARM_MINUTE, ((RTC_BinToBcd(time->minute))| PCF_ALARM_MINUTESCLOSE));
        RTC_WriteReg(ALARM_DAY, (0x0| PCF_ALARM_DAYSCLOSE));
        RTC_WriteReg(ALARM_WEEK, (0x0| PCF_ALARM_WEEKDAYSCLOSE));
    }
}

void sRTC_clear_alarm_flag(void)
{
    RTC_SetRegBit(CTL_STA_2, ALARM_FLAG_MASK, 0<<ALARM_FLAG_OFFSET);
}

void sRTC_alram_isr(void)
{
    if (alarm_inner_isr != NULL)
    {
        sRTC_clear_alarm_flag();//��flag
        alarm_inner_isr();
    }
}

void sRTC_enable_Alarm(ext_rtc_alarm_isr alarm_isr)
{
    /*
    ����ػ�ʱ��rtc����wio���Ի���С������ʱkey_scan�����⵽��RTC�������Լ�flag��
    �����g_ext_rtc_alram_isr�����Ǵ�ʱenable_alarm������δ���ã�����ζ���޷�����alarm��Ϣ������ϵͳû�м�⵽�����ӻ��ѵģ�
    ���Ը�Ϊ��һֱ��⵽��ֱ�����ô˺��������ô�isr�����ܵ���isr��
    ��flag�ͷ���Ϣ��
    */
    if (g_ext_rtc_alram_isr == NULL)
    {
        g_ext_rtc_alram_isr = sRTC_alram_isr;
    }
    alarm_inner_isr = alarm_isr;

    //Ŀǰֻ֧�� Сʱ+����
    //uint8 Alarm_Interrupt;
    //Alarm_Interrupt = RTC_ReadReg(CTL_STA_2);
    //Alarm_Interrupt &= ALARM_INT_CLOSE;  //�ȹر��ж����
    //Alarm_Interrupt &= CLEAR_AF;;  //�����־
    //RTC_WriteReg(CTL_STA_2, Alarm_Interrupt);
    //Alarm_Interrupt |= ALARM_INT_OPEN;
    //Alarm_Interrupt &= CLEAR_AF;  //�����־
    //RTC_WriteReg(CTL_STA_2,Alarm_Interrupt);

    if (((RTC_ReadReg(ALARM_MINUTE)& PCF_ALARM_MINUTESCLOSE) == 0)
        && ((RTC_ReadReg(ALARM_HOUR) & PCF_ALARM_HOURSCLOSE) == 0))
    {
        libc_printf("[Ext RTC] alarm already enable 1\n");
    }
    else
    {
        RTC_WriteReg(ALARM_MINUTE,( RTC_ReadReg(ALARM_MINUTE)& PCF_ALARM_MINUTESOPEN));
        RTC_WriteReg(ALARM_HOUR  ,( RTC_ReadReg(ALARM_HOUR)  & PCF_ALARM_HOURSOPEN));
        RTC_WriteReg(CTL_STA_2,((RTC_ReadReg(CTL_STA_2))|ALARM_INT_OPEN));
    }
    if (((RTC_ReadReg(CTL_STA_2)) & ALARM_INT_OPEN) == 0)
    {
        RTC_WriteReg(CTL_STA_2,((RTC_ReadReg(CTL_STA_2))|ALARM_INT_OPEN));
    }
    else
    {
        libc_printf("[Ext RTC] alarm already enable 2\n");
    }
}

void sRTC_disable_Alarm(void)
{
    sRTC_clear_alarm_flag();//ÿ��С������enable����disable���ӣ������һ�����������ӣ���һ�μ�������������ӡ����û�����ã���Ҫ�ر����ӡ�������rtc�ϵ�����ʱ�����һֱ�ڱ���״̬����Ҫ��������flag��

    RTC_WriteReg(ALARM_MINUTE,( RTC_ReadReg(ALARM_MINUTE)  | PCF_ALARM_MINUTESCLOSE));
    RTC_WriteReg(ALARM_HOUR  ,( RTC_ReadReg(ALARM_HOUR)  | PCF_ALARM_HOURSCLOSE));
    RTC_WriteReg(ALARM_DAY,( RTC_ReadReg(ALARM_DAY)  | PCF_ALARM_DAYSCLOSE));
    RTC_WriteReg(ALARM_WEEK,( RTC_ReadReg(ALARM_WEEK)  | PCF_ALARM_WEEKDAYSCLOSE));
}

void sRTC_set_Week(date_t* date)
{
    RTC_WriteReg(WEEK, date->week);
}

void sRTC_get_Week(date_t* date)
{
    date->week = RTC_ReadReg(WEEK);
}

void sRTC_get_status(rtc_status_t *p_rtc_status)
{
    p_rtc_status->alarm_en = ((RTC_ReadReg(ALARM_MINUTE) &ALARM_ENABLE_MASK)&&(RTC_ReadReg(ALARM_HOUR) & ALARM_ENABLE_MASK )) ? 1 : 0;
    //p_rtc_status->calendar_en = !((RTC_ReadReg(CTL_STA_1) &STOP_MASK));
    p_rtc_status->calendar_en = !((RTC_ReadReg(SECOND)) & (1<<7));
}

bool sRTC_get_alarm_flag(void)
{
    return (((RTC_ReadReg(CTL_STA_2)) & (1<<ALARM_FLAG_OFFSET))? TRUE : FALSE);
}

uint8 sRTC_get_rtc_int_level(void)
{
    uint8 gpio_level = 0;

    if (rtc_inited_flag == 0)//��δ��ʼ��
    {
        return 0;
    }

    ioctl_pad(EXT_RTC_INT_PIN, PAD_FUNC_SIOIN, &gpio_level);

    return gpio_level;
}

void sRTC_Init(void)
{
    uint8 temp = 0 ;
    temp = RTC_ReadReg(CTL_STA_1);
    if ((temp & (1<<5)) != 0)
    {
        temp &= ~(1<<5);//����оƬ
    }
    if ((temp & (1<<7)) == 0)//��ͨģʽ
    {
        temp &= ~(1<<3);//��Դ��λģʽʧЧ
    }
    RTC_WriteReg(CTL_STA_1, temp);//��д����ֵ

    RTC_WriteReg(CLKOUT, 0x0b);

    sRTC_clear_alarm_flag();//�״����������flag

    /*if( (RTC_ReadReg(SECOND)) & ((uint8)(1<<7)) == 1)//����ǲ���֤׼ȷ��ʱ��--��������
    {
        time_t time;
        date_t date;
        time.hour = 12;
        time.minute = 0;
        time.second = 0;

        date.day = 26;
        date.month = 6;
        date.year = 2018;
        date.week = 0;
        sRTC_set_Time(&time);
        sRTC_set_Date(&date);
        sRTC_set_Week(&date);

        RTC_WriteReg(CTL_STA_1, 0);
        RTC_WriteReg(CTL_STA_2, 0);

        RTC_WriteReg(ALARM_HOUR, 0x80);
        RTC_WriteReg(ALARM_DAY,  0x80);
        RTC_WriteReg(ALARM_WEEK,  0x80);
        RTC_WriteReg(CLKOUT, 0x0b);

    }*/
}

#endif
