/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：RTC AiP8563 模组驱动头文件。
 * 作者：wangli
 ********************************************************************************/

#ifndef _RTC_AiP8563_H
#define _RTC_AiP8563_H

#include "ext_rtc_inner.h"

//fm module I2C address
#define  I2C_READ_ADDR                   (0xA3)
#define  I2C_WRITE_ADDR                  (0xA2)



/*******************************************************************************
 definition register   寄存器配置
 *******************************************************************************/
#define CTL_STA_1                       0x00
#define CTL_STA_2                       0x01
#define SECOND                          0x02
#define MINUTE                          0x03
#define HOUR                            0x04
#define DAY                             0x05
#define WEEK                            0x06
#define MONTH_CENTURY                   0x07
#define YEAR                            0x08
#define ALARM_MINUTE                    0x09
#define ALARM_HOUR                      0x0a
#define ALARM_DAY                       0x0b
#define ALARM_WEEK                      0x0c
#define CLKOUT                          0x0d
#define TIMER_CTL                       0x0e
#define TIMER_VAL                       0x0f


/***************************************End************************************/

/*******************************************************************************
 definition operation bit of register   寄存器 bit
 *******************************************************************************/
//BIT
#define TESTC_OFFSET                    0x03
#define STOP_OFFSET                     0x05
#define TEST1_OFFSET                    0x07

#define TESTC_MASK                      0x08
#define STOP_MASK                       0x20
#define TEST1_MASK                      0x80

#define ALARM_FLAG_OFFSET         0x03
#define ALARM_FLAG_MASK           0x08



#define TIE                       0x00
#define AIE                       0x01
#define TIMER_FLAG                0x02

#define TI_TF                     0x04
#define CENTURY_OFFSET            0x07


//MASK
#define SECOND_MASK               0x7F
#define MINUTE_MASK               0x7F
#define HOUR_MASK                 0x3F
#define DAY_MASK                  0x3F
#define WEEK_MASK                 0x07//星期几
#define MONTH_MASK                0x1F
#define CENTURY_MASK              0x80//=0表示20XX，=1表示19XX
#define YEAR_MASK                 0xFF

#define ALARM_ENABLE_MASK         0x80
#define ALARM_ENABLE_OFFSET       0x07


#define ALARM_MASK                0x7f

#define ENABLE                       1
#define DISABLE                      0



#define BCD_MASK                0x0F


#define ALARM_INT_OPEN                     (uint8)(1<<1)     //报警中断有效
#define ALARM_INT_CLOSE                    (uint8)(~(1<<1))  //报警中断无效


#define CLEAR_AF                      (uint8)(~(1<<3))  //清除报警
#define CLEAR_TF                      (uint8)(~(1<<2))


#define ALARM_OPEN                    1
#define ALARM_OPEN_INT                2
#define ALARM_CLOSE                   3


//定义闹铃类型
#define RTC_ALARMTYPE_MINUTES                    (uint8)0X01
#define RTC_ALARMTYPE_HOURS                      (uint8)0X02
#define RTC_ALARMTYPE_DAYS                       (uint8)0X04
#define RTC_ALARMTYPE_WEEKDAYS                   (uint8)0X08

//分钟闹铃寄存器 --> 0X09
#define PCF_ALARM_MINUTESOPEN                    (uint8)(~(1<<7))  //分钟报警有效
#define PCF_ALARM_MINUTESCLOSE                   (uint8)(1<<7)     //分钟报警无效

//小时闹铃寄存器 --> 0X0A
#define PCF_ALARM_HOURSOPEN                      (uint8)(~(1<<7))  //小时报警有效
#define PCF_ALARM_HOURSCLOSE                     (uint8)(1<<7)     //小时报警无效

//日期闹铃寄存器 --> 0X0B
#define PCF_ALARM_DAYSOPEN                       (uint8)(~(1<<7))  //日报警有效
#define PCF_ALARM_DAYSCLOSE                      (uint8)(1<<7)     //日报警无效

//星期闹铃寄存器 --> 0X0C
#define PCF_ALARM_WEEKDAYSOPEN                   (uint8)(~(1<<7))  //星期报警有效
#define PCF_ALARM_WEEKDAYSCLOSE                  (uint8)(1<<7)     //星期报警无效

/***************************************End************************************/

extern int RTC_WriteReg(uint8 RegAddr, uint8 Data);
extern uint8 RTC_ReadReg(uint8 RegAddr);
extern int RTC_SetRegBit(uint8 RegAddr, uint8 bitMask, uint8 data_val);

#endif /*_RTC_AiP8563_H */

