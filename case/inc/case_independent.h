/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ������Ķ���ѡ��ͷ�ļ���ÿ������İ��Ӹ����Լ��Ĺ��ܹ����ϸ���ú�
 *       ��Щ�꿪�أ���ͷ�ļ������� CASE ������������Ӧ�ð�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _CASE_INDEPENDENT_H
#define _CASE_INDEPENDENT_H

#include "psp_includes.h"

//#define ENABLE_TRUE_WIRELESS_STEREO //�����Ƿ�֧��TWS��������

#define SUPPORT_FM_FUNC         1 //�Ƿ�֧��FM����������
#define SUPPORT_TIME_ALARM      0 //�Ƿ�֧��ʱ������ģʽ
#define SUPPORT_BT_SIRI         1 //�Ƿ�֧�ְ�������Siri��������

#define SUPPORT_BTPLAY_RECORD_FUNC  0 //�Ƿ�֧����������¼������
#define SUPPORT_BTCALL_RECORD_FUNC  0 //�Ƿ�֧������ͨ��¼������
#define SUPPORT_LINEIN_RECORD_FUNC  1 //�Ƿ�֧��AUX¼������
#define SUPPORT_RADIO_RECORD_FUNC   1 //�Ƿ�֧��������¼������
#define SUPPORT_USOUND_RECORD_FUNC  0 //�Ƿ�֧��USB����¼������

#define SUPPORT_CARD_UPGRADE    1 //�Ƿ�֧�ֿ��̼�����
#define SUPPORT_UHOST_UPGRADE   1 //�Ƿ�֧��U�̹̼�����

#define SUPPORT_PHONE_CHARGING  0 //�Ƿ�֧��USB�ڸ�IPhone���ֻ����

#define SUPPORT_WIO_WAKEUP      1 //�Ƿ�֧��WIO�ڻ��ѣ�����WIO0��ΪDC5V��⻽��

#define SUPPORT_MFP_EJTAG       0
#define DEBUG_LRADC_PIN         PAD_GPIO3
#define DEBUG_LRADC_IDX         9
#define MPAD_EJTAG_DEF          MPAD_EJTAG4
#define SUPPORT_MFP_BTDEBUG     0
#define SUPPORT_MFP_DUTDEBUG    0

//����ģʽ�£����ܻ�ر����¼����û�������ص�ģ��
/*********************************************************************************/
#define SUPPORT_FAST_PREVIEW    0 //�Ƿ�֧�ֿ���Ԥ��ģʽ  0:��֧��  1:֧��
#define SUPPORT_IR_KEY          1 //�Ƿ�֧��IR ����  0:��֧��  1:֧��
#define SUPPORT_ROTARY_ENCODER  0 //�Ƿ�֧�ֻ�ת�ͱ�����  0:��֧��  1:֧��
#define SUPPORT_VR_ADCKEY       0 //�Ƿ�֧�ֿɱ��������ť  0:��֧��  1:֧��
#define SUPPORT_LED_LIGHT       0 //�Ƿ�֧��LED��  0:��֧��  1:֧��
#define SUPPORT_LED_DISPLAY     1 //�Ƿ�֧��LED�����  0:��֧��  1:֧��  2:֧�֣���֧��LED�����Ӳ����ʹ�ô�ӡģ��
#define SUPPORT_SPI_LCD_DISPLAY 0 //�Ƿ�֧��LCD������  0:��֧��  1:֧��
#define SUPPORT_SPI1NOR_STORAGE 0 //�Ƿ�֧������SPI1NOR�洢  0:��֧��  1:֧��
#define SUPPORT_FONT_STORAGE_TYPE 0 //�������ֿ��Ŵ���  0:ExtLIB(SPI1NOR) 1:SD File
#define SUPPORT_EXTERN_PA       1 //�Ƿ�ʹ���ⲿPA��ĳЩ����ģʽ�²�ʹ��
#define EXTERN_PA_AB_D_ENABLE   1 //�Ƿ�֧��AB/D���л������ SUPPORT_EXTERN_PA Ϊ1����ǿ�Ʋ�֧��
#define USE_DCDC_CHIP           1 //�Ƿ�֧��DCDC��PA���磬��� SUPPORT_EXTERN_PA Ϊ1����ǿ�Ʋ�֧��
#define PA_CHIP_TYPE_NS4258     0
#define PA_CHIP_TYPE_CS8586     1
#define EXTERN_PA_CHIP_TYPE     PA_CHIP_TYPE_CS8586 //��DVB��ʹ��PA_CHIP_TYPE_NS4258������ʹ��GPIO6����DC��ѹ��PA�㹻��ѹ

#define DETECT_NO_SUPPORT       0 //�����Ȱβ���
#define DETECT_BY_IO            1 //GPIO/SIO���
#define DETECT_BY_ADCKEY        2 //ADCKEY���
#define DETECT_BY_CMD           3 //SD��������

#define SUPPORT_CARD_DETECT     DETECT_BY_CMD //�Ƿ�֧�ֿ��Ȱβ���

#define SUPPORT_LINEIN_DETECT   DETECT_BY_ADCKEY //�Ƿ�֧��AUX�Ȱβ���

#define SUPPORT_DC5V_DETECT     DETECT_BY_IO //�Ƿ�֧��DC5V���
#define UDEV_DET_BASE_DC5V_DET  1 //USB Device����Ƿ�����DC5V��⣬ѡ������DC5V��������Ϊ��Uhost�����Ի��һЩ��
                                  //��Ȼ��ǰ�������Ӳ��֧��DC5V��⡣��������̼�����Ϊ��֧��Uhost���ţ���ô��ǿ��Ϊ������

#define BATTERY_VOL_DET_BATADC  0
#define BATTERY_VOL_DET_LRADC   1
#define BATTERY_VOL_DET_TYPE    BATTERY_VOL_DET_BATADC

#define BATTERY_CHARGE_BY_DC5V  1 //�Ƿ����DC5V���
/*********************************************************************************/

#define SUPPORT_ASET_TEST       1 //�Ƿ�֧��ASET��Ч���Թ���
#define SUPPORT_ASQT_TEST       1 //�Ƿ�֧��ASQTͨ�����Թ���
#define SUPPORT_ATT_TEST        1 //�Ƿ�֧��ATT�������Թ���

#define SUPPORT_UART_PRINT      1 //�Ƿ�ʹ��UART��ӡ

#define SUPPORT_CARD_HARDWARE   1 //�Ƿ�ʹ����������Ϊ����д�ã������ʹ������Ը�UART��ӡ��

/******************** PIN�ŷ��䶨�� *********************/

//UART1 TX PIN ������ : GPIO8/GPIO14/GPIO26/GPIO36/GPIO48
//UART0 TX PIN ������ : GPIO2/GPIO4/GPIO6/GPIO12/GPIO24/GPIO31/GPIO48
#define PRINT_UART_TX_IDX       1
#define PRINT_UART_TX_PIN       PAD_GPIO8 //Pattern : PAD_GPIOX
#define PRINT_UART_TX_FUNC      GPIO8_MFP_UART1_TX //Pattern : GPIOX_MFP_UART1_TX or GPIOX_MFP_UART0_TX
#define PRINT_UART_TX_DMA_CHAN  4 //UART�첽��ӡʹ�õ�DMA CHAN
//NOTE: LCD������Ҳʹ����DMA4ͨ�������Դ�LCD�������ķ������޷�֧���첽��ӡ

//UART1 RX PIN ������ ��GPIO9/GPIO15/GPIO27/GPIO37
#define SIMKEY_UART_RX_IDX      1
#define SIMKEY_UART_RX_PIN      PAD_GPIO9 //Pattern : PAD_GPIOX
#define SIMKEY_UART_RX_FUNC     GPIO9_MFP_UART1_RX //Pattern : GPIOX_MFP_UART1_RX

#if (EXTERN_PA_CHIP_TYPE == PA_CHIP_TYPE_NS4258)
//�ⲿPAʹ�ܽ�
#define EXTERN_PA_ENABLE_PIN    PAD_MIC1
#define EXTERN_PA_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define EXTERN_PA_ENABLE_LEVEL  1 //��Ч��ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
#define EXTERN_PA_ENABLE_DELAY  120 //�ⲿPA���ݳ��ʱ�䣬��λMS
//�ⲿPA AB/D�๦��ѡ���
#define EXTERN_PA_AB_D_PIN      PAD_GPIO5
#define EXTERN_PA_AB_D_FUNC     PAD_FUNC_GPIOOUT
#define EXTERN_PA_AB_D_AB_LEVEL 0 //AB��ѡ���ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
//DCDC��ѹоƬ
#define DCDC_CHIP_ENABLE_PIN    PAD_GPIO6
#define DCDC_CHIP_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define DCDC_CHIP_ENABLE_LEVEL  1 //��Ч��ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
#define DCDC_CHIP_ENABLE_DELAY  50 //��ѹ����ʱ�䣬��λMS
#else
/*CS8386������Ͽ������һ����ѹ���п���*/
/* EN[0]&AB_D[0] -> PA�ض� */
/* EN[0]&AB_D[1] -> ������״̬����Ӧ�ó��� */
/* EN[1]&AB_D[0] -> PA����AB�� */
/* EN[1]&AB_D[1] -> PA����D������� */
//�ⲿPAʹ�ܽ�
#define EXTERN_PA_ENABLE_PIN    PAD_MIC1
#define EXTERN_PA_ENABLE_FUNC   PAD_FUNC_GPIOOUT
#define EXTERN_PA_ENABLE_LEVEL  1 //��Ч��ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
#define EXTERN_PA_ENABLE_DELAY  130 //�ⲿPA���ݳ��ʱ�䣬��λMS
//�ⲿPA AB/D�๦��ѡ���
#define EXTERN_PA_AB_D_PIN      PAD_GPIO5
#define EXTERN_PA_AB_D_FUNC     PAD_FUNC_GPIOOUT
#define EXTERN_PA_AB_D_AB_LEVEL 0 //AB��ѡ���ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
#define EXTERN_PA_AB_D_DISABLE_LEVEL 0 //disable��ƽ
#endif

//LINEIN����
#define LINEIN_DETECT_PIN       0
#define LINEIN_DETECT_FUNC      0
#define LINEIN_DETECT_LEVEL     0 //��Ч��ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
//LINEIN ADCKEY���
#define LINEIN_ADCKEY_DET_ADC_VAL_MAX             LRADC_2_70_V
#define LINEIN_ADCKEY_DET_ADC_VAL_MIN             LRADC_2_40_V
#define LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MAX  LRADC_2_83_V
#define LINEIN_ADCKEY_DET_ADC_VAL_AVCC_3_1_V_MIN  LRADC_2_52_V
//DV5V����
#define DC5V_DETECT_PIN         PAD_WIO0
#define DC5V_DETECT_FUNC        PAD_FUNC_SIOIN
#define DC5V_DETECT_LEVEL       1 //��Ч��ƽ��1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ

#define SUPPORT_FM_TX           0 //�Ƿ�֧��FM����

//FM TWI Master����Ӳ���ӿڻ������ģ��
#define FM_TWI_MASTER_MODE      TWI_MASTER_SIM

//NOTE��TWI Master��д�ӿ��Ƿ��ж���̻߳��жϵĵ����ߣ��������жϵ��á����������ô����Ҫ���жϣ�����ֻ��Ҫ������
#if (SUPPORT_EXT_RTC == 1)
#define TWI_MASTER_MULTY_CALLERS_AND_INT_CALL  1
#else
#define TWI_MASTER_MULTY_CALLERS_AND_INT_CALL  0
#endif

//FM TWI SCL/SDA PIN�Ŷ���
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

//7SEG LED����� PIN�Ŷ���
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

//IR����ͷPIN�Ŷ���
#define IR_RX_PIN               PAD_GPIO19
#define IR_RX_FUNC              GPIO19_MFP_IR_RX

//��ת�ͱ�����PIN�Ŷ���
#define RC_TERMINAL_A_PIN       PAD_GPIO5
#define RC_TERMINAL_B_PIN       PAD_GPIO6

//SPI1���LCD��������Norflash PIN�Ŷ���
#define SPI1_1ST_SS_PIN         PAD_GPIO20 //LCD
#define SPI1_1ST_SS_FUNC        GPIO20_MFP_SPI1_SS
#define SPI1_CLK_PIN            PAD_GPIO21
#define SPI1_CLK_FUNC           GPIO21_MFP_SPI1_CLK
#define SPI1_MOSI_PIN           PAD_GPIO22
#define SPI1_MOSI_FUNC          GPIO22_MFP_SPI1_MOSI
#define SPI1_MISO_PIN           PAD_GPIO23
#define SPI1_MISO_FUNC          GPIO23_MFP_SPI1_MISO
//֧��2·SPI�ڸ��ã�Ĭ��ʹ��2ND����Ҫʹ��1STʱ���л�SS��1ST��ʹ����ָ�Ϊ2ND
#define SUPPORT_2ND_SS_PIN      1
#define SPI1_2ND_SS_PIN         PAD_GPIO12 //NORFLASH
#define SPI1_2ND_SS_FUNC        GPIO12_MFP_SPI1_SS

//LCD����������PIN�Ŷ���
#define SPI_LCD_MOD_RST_PIN       PAD_GPIO24
#define SPI_LCD_MOD_RST_FUNC      PAD_FUNC_GPIOOUT
#define SPI_LCD_MOD_RS_SEL_PIN    PAD_GPIO25
#define SPI_LCD_MOD_RS_SEL_FUNC   PAD_FUNC_GPIOOUT
#define SPI_LCD_MOD_BKLIGHT_PIN   PAD_GPIO26
#define SPI_LCD_MOD_BKLIGHT_FUNC  PAD_FUNC_GPIOOUT

//SPDIF
#define SUPPORT_SPDIF_PLAY      0 //�Ƿ�֧��SPDIF����  0:��֧��  1:֧��
#define SPDIF_RX_PIN            PAD_GPIO6
#define SPDIF_RX_FUNC           GPIO6_MFP_SPDIF_RX

//Extern RTC
#define EXT_RTC_INT_PIN         PAD_WIO0

/******************** ������������ *********************/

/*! ��ص�ѹ�ȼ�����0 ~ 9��0��ʾ��ʾ�͵�ػ���1��ʾ�͵�����ʾ */
#define BATTERY_GRADE_MAX     9

/*! �������ֵ 31���ܹ� 32 �� */
#define VOLUME_VALUE_MAX      31
#define VOLUME_VALUE_CALL_MAX 15

//��ص����ȼ���Ӧ ADC �ο�ֵ  BATADC ֱ�Ӳ���
#define BAT_ADC_LOW_POWER  BAT_ADC_3_30_V //�͵�ػ���ѹ
#define BAT_ADC_LOW_BAT    BAT_ADC_3_50_V //��ʾ�͵��ѹ
#define BAT_ADC_FULL       BAT_ADC_4_15_V //�����ѹ
#ifdef USE_BATTERY_GRADE_VEL
const uint16 battery_grade_vel[BATTERY_GRADE_MAX + 1] =
{
    0, //���� 0 -- �͵�ػ�
    BAT_ADC_LOW_POWER, //���� 1
    BAT_ADC_LOW_BAT, //���� 2
    BAT_ADC_3_65_V, //���� 3  NOTE�������͵��ѹ��������ֵС
    BAT_ADC_3_70_V, //���� 4
    BAT_ADC_3_75_V, //���� 5
    BAT_ADC_3_80_V, //���� 6
    BAT_ADC_3_90_V, //���� 7
    BAT_ADC_4_00_V, //���� 8
    BAT_ADC_4_10_V //���� 9
};
#endif

//��ص����ȼ���Ӧ ADC �ο�ֵ  LRADC2 3/5��ѹ����
#define LRADC_LOW_POWER  LRADC_1_98_V //�͵�ػ���ѹ
#define LRADC_LOW_BAT    LRADC_2_10_V //��ʾ�͵��ѹ
#define LRADC_FULL       LRADC_2_49_V //�����ѹ
#ifdef USE_BATTERY_GRADE_VEL
const uint16 battery_grade_vel_lradc[BATTERY_GRADE_MAX + 1] =
{
    0, //���� 0 -- �͵�ػ�
    LRADC_LOW_POWER, //���� 1
    LRADC_LOW_BAT, //���� 2
    LRADC_2_19_V, //���� 3  NOTE�������͵��ѹ��������ֵС
    LRADC_2_22_V, //���� 4
    LRADC_2_25_V, //���� 5
    LRADC_2_28_V, //���� 6
    LRADC_2_34_V, //���� 7
    LRADC_2_40_V, //���� 8
    LRADC_2_46_V //���� 9
};
#endif

//��ȡ��Ƶ����������
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
CARD_POWER_CFG ����:

    bit0~bit4��ʾGPIO��index;bit5~bit6��ʾ��POWER ON�ĵ�ƽ;bit7��ʾ�Ƿ�֧�ֿ���Դ����;
    bit0~bit4 ��0~14�ֱ��ʾGPIO0~GPIO14;15~18�ֱ��ʾSIO0~SIO3;bit5~bit6Ϊ1��ʾPOWER_ON
                  Ϊ�ߵ�ƽ;bit7Ϊ1��ʾ֧�ֿ���Դ����;
    ���磺0x84��ʾ֧�ֿ���Դ���ƣ�GPIO4Ϊ��Դ����IO,GPIO4�͵�ƽ��ʾ ��POWER ON
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

//I2S TX�ӿڶ���
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

//I2S RX�ӿڶ���
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
#define I2SRX_SLAVE_MCLK_USE_INNER  1 //I2SRX��ΪSlaveʹ��ʱ��֧�ֲ�������96KHz�����£����Բ��ý�MCLK����ʡһ��PIN
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


/*ϵͳƵ������*/
#define SYS_CLK_ON_IDLE   FREQ_4M  //����״̬ϵͳ����Ƶ���Խ�ʡ����
#define SYS_CLK_ON_BUSY   FREQ_26M //�а����Ȳ���ʱ��Ƶ�Լӿ���Ӧ��7�����޲����󽵵�ΪSYS_CLK_ON_IDLE

#endif
