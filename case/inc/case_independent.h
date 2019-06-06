/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：CASE 最基本的定制选项头文件，每个具体的案子根据自己的功能规格仔细配置好
 *       这些宏开关；该头文件可以让 CASE 层所有驱动和应用包含。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _CASE_INDEPENDENT_H
#define _CASE_INDEPENDENT_H

#include "psp_includes.h"

//#define ENABLE_TRUE_WIRELESS_STEREO //方案是否支持TWS真立体声

#define SUPPORT_FM_FUNC         1 //是否支持FM收音机功能
#define SUPPORT_TIME_ALARM      0 //是否支持时钟闹钟模式
#define SUPPORT_BT_SIRI         1 //是否支持按键激活Siri语音助手

#define SUPPORT_BTPLAY_RECORD_FUNC  0 //是否支持蓝牙播歌录音功能
#define SUPPORT_BTCALL_RECORD_FUNC  0 //是否支持蓝牙通话录音功能
#define SUPPORT_LINEIN_RECORD_FUNC  1 //是否支持AUX录音功能
#define SUPPORT_RADIO_RECORD_FUNC   1 //是否支持收音机录音功能
#define SUPPORT_USOUND_RECORD_FUNC  0 //是否支持USB音箱录音功能

#define SUPPORT_CARD_UPGRADE    1 //是否支持卡固件升级
#define SUPPORT_UHOST_UPGRADE   1 //是否支持U盘固件升级

#define SUPPORT_PHONE_CHARGING  0 //是否支持USB口给IPhone等手机充电

#define SUPPORT_WIO_WAKEUP      1 //是否支持WIO口唤醒，比如WIO0作为DC5V检测唤醒

#define SUPPORT_MFP_EJTAG       0
#define DEBUG_LRADC_PIN         PAD_GPIO3
#define DEBUG_LRADC_IDX         9
#define MPAD_EJTAG_DEF          MPAD_EJTAG4
#define SUPPORT_MFP_BTDEBUG     0
#define SUPPORT_MFP_DUTDEBUG    0

//调试模式下，可能会关闭以下几个用户交互相关的模块
/*********************************************************************************/
#define SUPPORT_FAST_PREVIEW    0 //是否支持快速预览模式  0:不支持  1:支持
#define SUPPORT_IR_KEY          1 //是否支持IR 按键  0:不支持  1:支持
#define SUPPORT_ROTARY_ENCODER  0 //是否支持回转型编码器  0:不支持  1:支持
#define SUPPORT_VR_ADCKEY       0 //是否支持可变电阻器旋钮  0:不支持  1:支持
#define SUPPORT_LED_LIGHT       0 //是否支持LED灯  0:不支持  1:支持
#define SUPPORT_LED_DISPLAY     1 //是否支持LED数码管  0:不支持  1:支持  2:支持，因不支持LED数码管硬件，使用打印模拟
#define SUPPORT_SPI_LCD_DISPLAY 0 //是否支持LCD点阵屏  0:不支持  1:支持
#define SUPPORT_SPI1NOR_STORAGE 0 //是否支持外置SPI1NOR存储  0:不支持  1:支持
#define SUPPORT_FONT_STORAGE_TYPE 0 //点阵屏字库存放磁盘  0:ExtLIB(SPI1NOR) 1:SD File
#define SUPPORT_EXTERN_PA       1 //是否使用外部PA，某些调试模式下不使用
#define EXTERN_PA_AB_D_ENABLE   1 //是否支持AB/D类切换，如果 SUPPORT_EXTERN_PA 为1，会强制不支持
#define USE_DCDC_CHIP           1 //是否支持DCDC给PA供电，如果 SUPPORT_EXTERN_PA 为1，会强制不支持
#define PA_CHIP_TYPE_NS4258     0
#define PA_CHIP_TYPE_CS8586     1
#define EXTERN_PA_CHIP_TYPE     PA_CHIP_TYPE_CS8586 //旧DVB板使用PA_CHIP_TYPE_NS4258，才能使用GPIO6进行DC升压给PA足够电压

#define DETECT_NO_SUPPORT       0 //不做热拔插检测
#define DETECT_BY_IO            1 //GPIO/SIO检测
#define DETECT_BY_ADCKEY        2 //ADCKEY检测
#define DETECT_BY_CMD           3 //SD卡命令检测

#define SUPPORT_CARD_DETECT     DETECT_BY_CMD //是否支持卡热拔插检测

#define SUPPORT_LINEIN_DETECT   DETECT_BY_ADCKEY //是否支持AUX热拔插检测

#define SUPPORT_DC5V_DETECT     DETECT_BY_IO //是否支持DC5V检测
#define UDEV_DET_BASE_DC5V_DET  1 //USB Device检测是否依赖DC5V检测，选择依赖DC5V检测仅仅是为了Uhost兼容性会好一些。
                                  //当然，前提必须是硬件支持DC5V检测。所以如果固件配置为不支持Uhost播放，那么会强制为不依赖

#define BATTERY_VOL_DET_BATADC  0
#define BATTERY_VOL_DET_LRADC   1
#define BATTERY_VOL_DET_TYPE    BATTERY_VOL_DET_BATADC

#define BATTERY_CHARGE_BY_DC5V  1 //是否采用DC5V充电
/*********************************************************************************/

#define SUPPORT_ASET_TEST       1 //是否支持ASET音效调试工具
#define SUPPORT_ASQT_TEST       1 //是否支持ASQT通话调试工具
#define SUPPORT_ATT_TEST        1 //是否支持ATT量产测试工具

#define SUPPORT_UART_PRINT      1 //是否使用UART打印

#define SUPPORT_CARD_HARDWARE   1 //是否使用物理卡槽作为卡读写用；如果不使用则可以给UART打印用

/******************** PIN脚分配定义 *********************/

//UART1 TX PIN 脚配置 : GPIO8/GPIO14/GPIO26/GPIO36/GPIO48
//UART0 TX PIN 脚配置 : GPIO2/GPIO4/GPIO6/GPIO12/GPIO24/GPIO31/GPIO48
#define PRINT_UART_TX_IDX       1
#define PRINT_UART_TX_PIN       PAD_GPIO8 //Pattern : PAD_GPIOX
#define PRINT_UART_TX_FUNC      GPIO8_MFP_UART1_TX //Pattern : GPIOX_MFP_UART1_TX or GPIOX_MFP_UART0_TX
#define PRINT_UART_TX_DMA_CHAN  4 //UART异步打印使用的DMA CHAN
//NOTE: LCD点阵屏也使用了DMA4通道，所以带LCD点阵屏的方案，无法支持异步打印

//UART1 RX PIN 脚配置 ：GPIO9/GPIO15/GPIO27/GPIO37
#define SIMKEY_UART_RX_IDX      1
#define SIMKEY_UART_RX_PIN      PAD_GPIO9 //Pattern : PAD_GPIOX
#define SIMKEY_UART_RX_FUNC     GPIO9_MFP_UART1_RX //Pattern : GPIOX_MFP_UART1_RX

#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
//外部PA使能脚
#define EXTERN_PA_ENABLE_PIN    PAD_MIC1
#define EXTERN_PA_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define EXTERN_PA_ENABLE_LEVEL  1 //有效电平，1表示高电平，0表示低电平
#define EXTERN_PA_ENABLE_DELAY  120 //外部PA电容充电时间，单位MS
//外部PA AB/D类功放选择脚
#define EXTERN_PA_AB_D_PIN      PAD_GPIO5
#define EXTERN_PA_AB_D_FUNC     PAD_FUNC_GPIOOUT
#define EXTERN_PA_AB_D_AB_LEVEL 0 //AB类选择电平，1表示高电平，0表示低电平
//DCDC升压芯片
#define DCDC_CHIP_ENABLE_PIN    PAD_GPIO6
#define DCDC_CHIP_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define DCDC_CHIP_ENABLE_LEVEL  1 //有效电平，1表示高电平，0表示低电平
#define DCDC_CHIP_ENABLE_DELAY  50 //升压工作时间，单位MS
#else
/*CS8386采用组合控制输出一个电压进行控制*/
/* EN[0]&AB_D[0] -> PA关断 */
/* EN[0]&AB_D[1] -> 不可用状态，不应该出现 */
/* EN[1]&AB_D[0] -> PA工作AB类 */
/* EN[1]&AB_D[1] -> PA工作D类防破音 */
//外部PA使能脚
#define EXTERN_PA_ENABLE_PIN    PAD_MIC1
#define EXTERN_PA_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define EXTERN_PA_ENABLE_LEVEL  1 //有效电平，1表示高电平，0表示低电平
#define EXTERN_PA_ENABLE_DELAY  130 //外部PA电容充电时间，单位MS
//外部PA AB/D类功放选择脚
#define EXTERN_PA_AB_D_PIN      PAD_GPIO5
#define EXTERN_PA_AB_D_FUNC     PAD_FUNC_GPIOOUT
#define EXTERN_PA_AB_D_AB_LEVEL 0 //AB类选择电平，1表示高电平，0表示低电平
#define EXTERN_PA_AB_D_DISABLE_LEVEL 0 //disable电平
#endif

//LINEIN检测口
#define LINEIN_DETECT_PIN       0
#define LINEIN_DETECT_FUNC      0
#define LINEIN_DETECT_LEVEL     0 //有效电平，1表示高电平，0表示低电平
//LINEIN ADCKEY检测
#define LINEIN_ADCKEY_DET_ADC_VAL_MAX             LRADC_2_70_V
#define LINEIN_ADCKEY_DET_ADC_VAL_MIN             LRADC_2_40_V
#define LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MAX  LRADC_2_83_V
#define LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MIN  LRADC_2_52_V
//DV5V检测口
#define DC5V_DETECT_PIN         PAD_WIO0
#define DC5V_DETECT_FUNC        PAD_FUNC_SIOIN
#define DC5V_DETECT_LEVEL       1 //有效电平，1表示高电平，0表示低电平

#define SUPPORT_FM_TX           0 //是否支持FM发射

//FM TWI Master采用硬件接口还是软件模拟
#define FM_TWI_MASTER_MODE      TWI_MASTER_SIM

//NOTE：TWI Master读写接口是否“有多个线程或中断的调用者，并且有中断调用”，如果会那么就需要关中断，否则只需要锁调度
#if (SUPPORT_EXT_RTC == 1)
#define TWI_MASTER_MULTY_CALLERS_AND_INT_CALL  1
#else
#define TWI_MASTER_MULTY_CALLERS_AND_INT_CALL  0
#endif

//FM TWI SCL/SDA PIN脚定义
#if (FM_TWI_MASTER_MODE == TWI_MASTER_SIM)
#define FM_TWI_SCL_PIN          PAD_GPIO13
#define FM_TWI_SCL_FUNC         PAD_FUNC_GPIOOUT
#define FM_TWI_SDA_PIN          PAD_GPIO12
#define FM_TWI_SDA_FUNC         PAD_FUNC_GPIOOUT
#define FM_TWI_SDA_IN_FUNC      PAD_FUNC_GPIOIN
#else
#define FM_TWI_SCL_PIN          PAD_GPIO13
#define FM_TWI_SCL_FUNC         GPIO13_MFP_I2C1_SCL
#define FM_TWI_SDA_PIN          PAD_GPIO12
#define FM_TWI_SDA_FUNC         GPIO12_MFP_I2C1_SDA
#endif
#define FM_FMCLKOUT_PIN         PAD_GPIO4
#define FM_FMCLKOUT_FUNC        GPIO4_MFP_FMCLKOUT

//7SEG LED数码管 PIN脚定义
#define LED_7SEG_SEG0_PIN       PAD_GPIO20
#define LED_7SEG_SEG0_FUNC      GPIO20_MFP_LED_SEG0
#define LED_7SEG_SEG1_PIN       PAD_GPIO21
#define LED_7SEG_SEG1_FUNC      GPIO21_MFP_LED_SEG1
#define LED_7SEG_SEG2_PIN       PAD_GPIO22
#define LED_7SEG_SEG2_FUNC      GPIO22_MFP_LED_SEG2
#define LED_7SEG_SEG3_PIN       PAD_GPIO23
#define LED_7SEG_SEG3_FUNC      GPIO23_MFP_LED_SEG3
#define LED_7SEG_SEG4_PIN       PAD_GPIO24
#define LED_7SEG_SEG4_FUNC      GPIO24_MFP_LED_SEG4
#define LED_7SEG_SEG5_PIN       PAD_GPIO25
#define LED_7SEG_SEG5_FUNC      GPIO25_MFP_LED_SEG5
#define LED_7SEG_SEG6_PIN       PAD_GPIO26
#define LED_7SEG_SEG6_FUNC      GPIO26_MFP_LED_SEG6

//IR接收头PIN脚定义
#define IR_RX_PIN               PAD_GPIO19
#define IR_RX_FUNC              GPIO19_MFP_IR_RX

//回转型编码器PIN脚定义
#define RC_TERMINAL_A_PIN       PAD_GPIO5
#define RC_TERMINAL_B_PIN       PAD_GPIO6

//SPI1外挂LCD串口屏和Norflash PIN脚定义
#define SPI1_1ST_SS_PIN         PAD_GPIO20 //LCD
#define SPI1_1ST_SS_FUNC        GPIO20_MFP_SPI1_SS
#define SPI1_CLK_PIN            PAD_GPIO21
#define SPI1_CLK_FUNC           GPIO21_MFP_SPI1_CLK
#define SPI1_MOSI_PIN           PAD_GPIO22
#define SPI1_MOSI_FUNC          GPIO22_MFP_SPI1_MOSI
#define SPI1_MISO_PIN           PAD_GPIO23
#define SPI1_MISO_FUNC          GPIO23_MFP_SPI1_MISO
//支持2路SPI口复用，默认使用2ND，需要使用1ST时先切换SS到1ST，使用完恢复为2ND
#define SUPPORT_2ND_SS_PIN      1
#define SPI1_2ND_SS_PIN         PAD_GPIO12 //NORFLASH
#define SPI1_2ND_SS_FUNC        GPIO12_MFP_SPI1_SS

//LCD串口屏控制PIN脚定义
#define SPI_LCD_MOD_RST_PIN       PAD_GPIO24
#define SPI_LCD_MOD_RST_FUNC      PAD_FUNC_GPIOOUT
#define SPI_LCD_MOD_RS_SEL_PIN    PAD_GPIO25
#define SPI_LCD_MOD_RS_SEL_FUNC   PAD_FUNC_GPIOOUT
#define SPI_LCD_MOD_BKLIGHT_PIN   PAD_GPIO26
#define SPI_LCD_MOD_BKLIGHT_FUNC  PAD_FUNC_GPIOOUT

//SPDIF
#define SUPPORT_SPDIF_PLAY      0 //是否支持SPDIF播歌  0:不支持  1:支持
#define SPDIF_RX_PIN            PAD_GPIO6
#define SPDIF_RX_FUNC           GPIO6_MFP_SPDIF_RX

//Extern RTC
#define EXT_RTC_INT_PIN         PAD_WIO0

/******************** 其他公共参数 *********************/

/*! 电池电压等级数，0 ~ 9，0表示显示低电关机，1表示低电需提示 */
#define BATTERY_GRADE_MAX     9

/*! 音量最大值 31，总共 32 级 */
#define VOLUME_VALUE_MAX      31
#define VOLUME_VALUE_CALL_MAX 15

//电池电量等级对应 ADC 参考值  BATADC 直接测试
#define BAT_ADC_LOW_POWER  BAT_ADC_3_30_V //低电关机电压
#define BAT_ADC_LOW_BAT    BAT_ADC_3_50_V //提示低电电压
#define BAT_ADC_FULL       BAT_ADC_4_15_V //满电电压
#ifdef USE_BATTERY_GRADE_VEL
const uint16 battery_grade_vel[BATTERY_GRADE_MAX + 1] =
{
    0, //返回 0 -- 低电关机
    BAT_ADC_LOW_POWER, //返回 1
    BAT_ADC_LOW_BAT, //返回 2
    BAT_ADC_3_65_V, //返回 3  NOTE：上述低电电压必须比这个值小
    BAT_ADC_3_70_V, //返回 4
    BAT_ADC_3_75_V, //返回 5
    BAT_ADC_3_80_V, //返回 6
    BAT_ADC_3_90_V, //返回 7
    BAT_ADC_4_00_V, //返回 8
    BAT_ADC_4_10_V //返回 9
};
#endif

//电池电量等级对应 ADC 参考值  LRADC2 3/5分压测试
#define LRADC_LOW_POWER  LRADC_1_98_V //低电关机电压
#define LRADC_LOW_BAT    LRADC_2_10_V //提示低电电压
#define LRADC_FULL       LRADC_2_49_V //满电电压
#ifdef USE_BATTERY_GRADE_VEL
const uint16 battery_grade_vel_lradc[BATTERY_GRADE_MAX + 1] =
{
    0, //返回 0 -- 低电关机
    LRADC_LOW_POWER, //返回 1
    LRADC_LOW_BAT, //返回 2
    LRADC_2_19_V, //返回 3  NOTE：上述低电电压必须比这个值小
    LRADC_2_22_V, //返回 4
    LRADC_2_25_V, //返回 5
    LRADC_2_28_V, //返回 6
    LRADC_2_34_V, //返回 7
    LRADC_2_40_V, //返回 8
    LRADC_2_46_V //返回 9
};
#endif

//获取多频点能量参数
#define DEF_FREQ_POINT_ENABLE  FALSE
#define DEF_FREQ_POINT_NUM     9
#define DEF_FREQ_POINT_0       64
#define DEF_FREQ_POINT_1       125
#define DEF_FREQ_POINT_2       250
#define DEF_FREQ_POINT_3       500
#define DEF_FREQ_POINT_4       1000
#define DEF_FREQ_POINT_5       2000
#define DEF_FREQ_POINT_6       4000
#define DEF_FREQ_POINT_7       8000
#define DEF_FREQ_POINT_8       16000
#define DEF_FREQ_POINT_9       0

/*
CARD_POWER_CFG 定义:

    bit0~bit4表示GPIO的index;bit5~bit6表示卡POWER ON的电平;bit7表示是否支持卡电源控制;
    bit0~bit4 中0~14分别表示GPIO0~GPIO14;15~18分别表示SIO0~SIO3;bit5~bit6为1表示POWER_ON
                  为高电平;bit7为1表示支持卡电源控制;
    例如：0x84表示支持卡电源控制，GPIO4为电源控制IO,GPIO4低电平表示 卡POWER ON
*/

#define CARD_POWER_CFG    0x00
#define SDCARD_CMD_PIN    PAD_GPIO0
#define SDCARD_CMD_FUNC   GPIO0_MFP_SD0_CMD
#define SDCARD_CLK_PIN    PAD_GPIO1
#define SDCARD_CLK_FUNC   GPIO1_MFP_SD0_CLK
#define SDCARD_DAT0_PIN   PAD_GPIO2
#define SDCARD_DAT0_FUNC  GPIO2_MFP_SD0_DAT

#define USB_DM_PIN        PAD_GPIO10
#define USB_DM_FUNC       GPIO10_MFP_DM
#define USB_DP_PIN        PAD_GPIO11
#define USB_DP_FUNC       GPIO11_MFP_DP

//I2S TX接口定义
//BCLK  : PAD_GPIO0/PAD_GPIO5/PAD_GPIO14/PAD_GPIO20/PAD_GPIO30/PAD_GPIO36
#define I2STX_BCLK_PIN    PAD_GPIO5
#define I2STX_BCLK_FUNC   GPIO5_MFP_I2STX_BCLK
//LRCLK : PAD_GPIO1/PAD_GPIO6/PAD_GPIO15/PAD_GPIO21/PAD_GPIO31/PAD_GPIO37
#define I2STX_LRCLK_PIN   PAD_GPIO6
#define I2STX_LRCLK_FUNC  GPIO6_MFP_I2STX_LRCLK
//MCLK :  PAD_GPIO2/PAD_GPIO8/PAD_GPIO16/PAD_GPIO22/PAD_GPIO32
#define I2STX_MCLK_PIN    PAD_GPIO8
#define I2STX_MCLK_FUNC   GPIO8_MFP_I2STX_MCLK
//DOUT :  PAD_GPIO3/PAD_GPIO9/PAD_GPIO17/PAD_GPIO23/PAD_GPIO33
#define I2STX_DOUT_PIN    PAD_GPIO9
#define I2STX_DOUT_FUNC   GPIO9_MFP_I2STX_DOUT

#define I2STX_EFF_WIDTH   I2STX_WIDTH_16BIT
#define I2STX_DATA_MODE   I2STX_DATA_I2S_MODEL

//I2S RX接口定义
//BCLK  : PAD_GPIO0/PAD_GPIO12/PAD_GPIO14/PAD_GPIO36
#define I2SRX_BCLK_PIN    PAD_GPIO0
#define I2SRX_BCLK_FUNC   GPIO0_MFP_I2SRX_BCLK
//LRCLK : PAD_GPIO1/PAD_GPIO13/PAD_GPIO15/PAD_GPIO37
#define I2SRX_LRCLK_PIN   PAD_GPIO1
#define I2SRX_LRCLK_FUNC  GPIO1_MFP_I2SRX_LRCLK
//MCLK :  PAD_GPIO2/PAD_GPIO11/PAD_GPIO17/PAD_GPIO25/PAD_GPIO29
#define I2SRX_MCLK_PIN    PAD_GPIO2
#define I2SRX_MCLK_FUNC   GPIO2_MFP_I2SRX_MCLK
//DI :    PAD_GPIO3/PAD_GPIO10/PAD_GPIO16/PAD_GPIO24/PAD_GPIO28
#define I2SRX_DI_PIN      PAD_GPIO3
#define I2SRX_DI_FUNC     GPIO3_MFP_I2SRX_DI

#define I2SRX_WORK_MODE   I2SRX_WORK_SLAVE
#define I2SRX_SLAVE_MCLK_USE_INNER  1 //I2SRX作为Slave使用时，支持采样率在96KHz及以下，可以不用接MCLK，节省一个PIN
#define I2SRX_EFF_WIDTH   I2SRX_WIDTH_16BIT
#define I2SRX_DATA_MODE   I2SRX_DATA_I2S_MODEL


/******************************** HDMI Module ********************************/
#if (CUSTOMER_BOARD_TYPE == CUSTOMER_EVB_ATS2819P)
#define HDMI_SCL_PIN      PAD_GPIO29
#define HDMI_SDA_PIN      PAD_GPIO28
#define HDMI_INT_PIN      PAD_GPIO4
#define HDMI_RST_PIN      PAD_GPIO9
#endif

#define HDMI_SCL_FUNC     PAD_FUNC_GPIOOUT
#define HDMI_SCL_IN_FUNC  PAD_FUNC_GPIOIN

#define HDMI_SDA_FUNC     PAD_FUNC_GPIOOUT
#define HDMI_SDA_IN_FUNC  PAD_FUNC_GPIOIN

#define HDMI_INT_FUNC     PAD_FUNC_GPIOIN
#define HDMI_RST_FUNC     PAD_FUNC_GPIOOUT

//HDMI
#define SPDIF_RX_HDMI_PIN    PAD_GPIO3
#define SPDIF_RX_HDMI_FUNC   GPIO3_MFP_SPDIF_RX
//ARC
#define SPDIF_RX_ARC_PIN     PAD_GPIO3
#define SPDIF_RX_ARC_FUNC    GPIO3_MFP_SPDIF_RX
/******************************** HDMI Module End ********************************/


/*系统频率配置*/
#define SYS_CLK_ON_IDLE   FREQ_4M  //空闲状态系统降低频率以节省功耗
#define SYS_CLK_ON_BUSY   FREQ_26M //有按键等操作时提频以加快响应，7秒钟无操作后降低为SYS_CLK_ON_IDLE

#endif
