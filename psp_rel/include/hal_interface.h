#ifndef __HAL_INTERFACE_H__
#define __HAL_INTERFACE_H__

#include <attr.h>
#include "hal_audio_extern.h"
#include "hal_pad_extern.h"
#include "hal_twi_slave_extern.h"
#include "hal_twi_master_extern.h"
#include "hal_pmu_extern.h"
#include "hal_irc_extern.h"
#include "hal_led_extern.h"
#include "hal_lib_fs_extern.h"
#include "hal_hdmi_extern.h"

#define SUPPORT_EXT_RTC 0 //是否使用外置RTC，1表示使用外置RTC，0表示使用内置RTC
/* 注意：由于外挂RTC芯片有在中断里处理RTC INT信号进行TWI读写，所以必须修改以下宏为1，否则可能会出现重入*/
/*
case_independent.h
//NOTE：TWI Master读写接口是否“有多个线程或中断的调用者，并且有中断调用”，如果会那么就需要关中断，否则只需要锁调度
#define TWI_MASTER_MULTY_CALLERS_AND_INT_CALL  0
*/

typedef enum
{
    HAL_SET_CARD_PW_CTRL_CFG = 0,
    HAL_SET_PHONE_CHARGING_CURRENT = 1,
} hw_set_op_e;

typedef enum
{
    HAL_GET_CARD_PW_CTRL_CFG = 0,
    HAL_GET_CHARGING_CURRENT = 1,
} hw_get_op_e;

#define USB_DET_DISABLE      0
#define USB_DET_FOR_DEVICE   1
#define USB_DET_FOR_UHOST    2

#define USB_DETECT_DISABLE  0
#define USB_DETECT_ENABLE   1
#define USB_DETECT_EN_RESET 2

typedef enum
{
    DEFAULT_TYPE = 0,
    UDEVICE_TYPE = 1,
    PHONE_TYPE   = 2,
} usb_host_in_e;

typedef enum
{
    RAM_CLEAR_WATCH_DOG = 0,
    RAM_IS_IRQ_PENDING,
    RAM_ENTER_LOWP_MODE,
    RAM_EXIT_LOWP_MODE,
    RAM_READ_LRADC,
    RAM_READ_BATADC,
    RAM_READ_ONOFF_KEY,
    RAM_WRITE_GPIO,
    RAM_READ_GPIO,
    RAM_READ_SIO,
} hal_cmd_ram0_e;

typedef enum
{
    //pa
    ENABLE_PA = 0,
    DISABLE_PA = 1,
    MUTE_PA = 2,

    //audio volume & clock
    SET_AUDIO_VOLUME = 3,
    SET_AUDIO_APS = 4,
    GET_AUDIO_APS_INFO = 5,

    //dac
    ENABLE_DAC = 6,
    DISABLE_DAC = 7,
    SET_DAC_RATE = 8,
    GET_DAC_RATE = 9,
    WAIT_DACFIFO_EMPTY = 10,

    //pcm buf
    RESET_PCMBUF = 11,
    SET_PCMBUF = 12,
    GET_PCMBUF_LENGTH = 13,

    //ain
    ENABLE_AIN = 14,
    DISABLE_AIN = 15,

    //adc
    SET_ADC_GAIN = 16,
    ENABLE_ADC = 17,
    DISABLE_ADC = 18,
    IS_SIM_ADC_DUAL = 19,

    //keytone
    START_KEY_TONE = 20,
    GET_KEY_TONE_STATUS = 21,

    //I2SRx
    ENABLE_I2SRX = 22,
    DISABLE_I2SRX = 23,

    //audio device reserve api
    AUD_RESERVE_API_1 = 24,
    AUD_RESERVE_API_2 = 25,
    AUD_RESERVE_API_3 = 26,
    AUD_RESERVE_API_4 = 27,
    AUD_RESERVE_API_5 = 28,
    AUD_RESERVE_API_6 = 29,

    //pad&sirq&fmclk&pwm
    SET_PAD_FUNCTION = 30,
    IOCTL_PAD = 31,

    SET_SIRQ = 32,
    DISABLE_SIRQ = 33,

    SET_FM_CLOCK = 34,
    DISABLE_FM_CLOCK = 35,

    SET_PWM = 36,
    DISABLE_PWM = 37,

    //pad&sirq&fmclk&pwm reserve api
    PAD_RESERVE_API_1 = 38,
    PAD_RESERVE_API_2 = 39,
    PAD_RESERVE_API_3 = 40,
    PAD_RESERVE_API_4 = 41,
    PAD_RESERVE_API_5 = 42,
    PAD_RESERVE_API_6 = 43,

    //pmu
    PMU_SET_WAKE_UP = 44,
    PMU_POWER_OFF = 45,
    PMU_SET_LRADC = 46,
    PMU_SET_BATADC = 47,
    PMU_SET_LRADC1_RESISTANCE = 48,
    POWER_RESERVE_API_1 = 49,

    //TWI interface
    TWI_SET_HW_INFO = 50,
    TWI_WRITE_BYTES = 51,
    TWI_READ_BYTES = 52,
    TWI_MASTER_SET_HW_INFO = 53,
    TWI_SLAVE_SET_HW_INFO = 54,
    TWI_RESERVE_API_1 = 55,

    //LED 7seg interface
    LED_HARDWARE_INIT = 56,
    LED_HARDWARE_EXIT = 57,
    LED_HARDWARE_DISPLAY = 58,
    LED_HARDWARE_ADJUST_LIGHT = 59,
    LED_RESERVE_API_1 = 60,
    LED_RESERVE_API_2 = 61,

    //IRC interface
    IRC_HARDWARE_INIT = 62,
    IRC_HARDWARE_EXIT = 63,
    IRC_CORRECT_USERCODE = 64,
    IRC_RESERVE_API_1 = 65,

    //RTC interface
    RTC_PHY_INIT = 66,
    RTC_SET_TIME = 67,
    RTC_GET_TIME = 68,
    RTC_SET_DATE = 69,
    RTC_GET_DATE = 70,
    RTC_SET_ALARM_TIME = 71,
    RTC_GET_ALARM_TIME = 72,
    RTC_ENABLE_ALARM = 73,
    RTC_DISABLE_ALARM = 74,
    RTC_GET_STATUS = 75,
    RTC_BAKDATA_WRITE = 76,
    RTC_BAKDATA_READ = 77,
    RTC_RESERVE_API_1 = 78,

    //SPI interface
    SPI1_TRANS_OVER = 79,
    SPI1_PHY_INIT = 80,
    SPI1_READ_DATA = 81,
    SPI1_WRITE_DATA = 82,
    SPI1_SET_CS = 83,
    //SPINOR
    SPI1_NOR_INIT = 84,
    SPI1_NOR_READ_CHIPID = 85,
    SPI1_NOR_ERASE = 86,
    SPI1_NOR_READ = 87,
    SPI1_NOR_WRITE = 88,
    SPI_RESERVE_API_1 = 89,

    //card hardware interface
    CARD_DETECT_DISK = 90,
    CARD_INIT = 91,
    CARD_EXIT = 92,
    CARD_SET_PM_PARAM = 93,
    CARD_ADJUST_CLK = 94,
    CARD_SELECT = 95,
    CARD_DESELECT = 96,
    CARD_PHY_RW_SUB = 97,
    CARD_WAIT_DMA_OVER = 98,
    CARD_STOP_TRANSMISSION = 99,
    CARD_GET_CUR_STAT = 100,
    CARD_GET_CAP = 101,
    CARD_RESERVE_API_1 = 102,
    CARD_RESERVE_API_2 = 103,
    CARD_RESERVE_API_3 = 104,
    CARD_RESERVE_API_4 = 105,

    //usb device hardware interface
    USB_DEV_UHOST_DET_SWITCH = 106,
    UDEV_SET_DETECT = 107,
    UDEV_DETECT = 108,
    UDEV_INIT = 109,
    UDEV_EXIT = 110,
    UDEV_SET_IN_EP = 111,
    UDEV_SET_OUT_EP = 112,
    UDEV_REST_ENDP_TOGGLE = 113,
    UDEV_STALL_ENDPOINT = 114,
    UDEV_RECOVER_STALL_ENDPOINT = 115,
    UDEV_PARSE_ISR_SOURCE = 116,
    UDEV_USB_IS_CABLE_PLUG = 117,
    UDEV_GET_USB_SETUP_DATA = 118,
    UDEV_USB_TO_ADFU = 119,
    UDEV_USB_CONNECT_STATE = 120,
    UDEV_USB_HID_PRO = 121,
    UDEV_USB_GET_SHORT_PK = 122,
    UDEV_USB_NAK_CTRL = 123,
    UDEV_USB_RECV_CTRL_DATA = 124,
    UDEV_USB_RECV_DATA = 125,
    UDEV_USB_SEND_DATA = 126,
    UDEV_RESERVE_API_1 = 127,

    //uhost hardware interface
    UHOST_DETECT = 128,
    UHOST_BUS_RESET = 129,
    UHOST_EP_RESET = 130,
    UHOST_CONTROLLER_EN = 131,
    UHOST_CONTROLLER_DISABLE = 132,
    UHOST_CLEAR_EP_ERR_IRQ = 133,
    UHOST_CLEAR_FEATURE_EP_STALL = 134,
    UHOST_SETUP_STAGE_TRANSCATION = 135,
    UHOST_CONTROL_IN_DATA = 136,
    UHOST_STATUS_STAGE_TRANSACATION_OUT = 137,
    UHOST_STATUS_STAGE_TRANSACTION_IN = 138,
    UHOST_SET_ENDPOINT = 139,
    UHOST_BULK_DATA = 140,
    UHOST_TYPE = 141,
    UHOST_SET_FUNCTION_ADDRESS = 142,
    UHOST_SET_MAX_PACKET = 143,
    UHOST_RESERVE_API_1 = 144,
    UHOST_RESERVE_API_2 = 145,
    UHOST_RESERVE_API_3 = 146,

    //hardware set & get
    HARDWARE_SET = 147,
    HARDWARE_GET = 148,

    //uart interface, for print etc
    UART_TX_SET = 149,
    UART_SEND_BYTES = 150,
    UART_RX_SET = 151,
    UART_RECV_BYTES = 152,
    UART_RESERVE_API_1 = 153,

    LIB_EXT_READ_API = 154,
    LIB_FS_INIT_API = 155,
    LIB_FS_OPEN_API = 156,
    LIB_FS_FCLOSE_API = 157,
    LIB_FS_SEEK_API = 158,
    LIB_FS_TELL_API = 159,
    LIB_FS_READ_API = 160,

    //hdmi conmunication interface  NOTE!!:HDMI API FIX TO 161 & 162
    HDMI_IIC_INIT = 161,
    HDMI_IIC_RW = 162,

    //share code api
    SHARE_CODE_UPDATE_PP_PARA_INFO_API = 163,
    SHARE_CODE_BUFFER_RW_WRITE_API = 164,
    SHARE_CODE_BUFFER_RW_READ_API = 165,
    SHARE_CODE_BUFFER_RW_UPDATE_API = 166,
    SHARE_CODE_BUFFER_RW_FLUSH_API = 167,
    SHARE_CODE_ENERGY_CALC_API = 168,

    //OTHER API
    HAL_OTHER_RESERVE_API_1 = 169,
    HAL_OTHER_RESERVE_API_X = 199,

    //NOTE: HAL驱动API接口最多200个，如果超出需要修改PSP AFI.bin

} audio_device_cmd_e;

/*
 audio device 向上层提供统一的接口，即只提供一个API，并以ID的方式区分
 */

#define HAL_RAM_ENTRY_ADDR(x)       (HAL_DRV_RAM_OP_ENTRY + (4 * x))

#define AUD_CLEAR_WATCH_DOG         (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_CLEAR_WATCH_DOG))))
#define AUD_IS_IRQ_PENDING          (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_IS_IRQ_PENDING))))
#define AUD_ENTER_LOWP_MODE         (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_ENTER_LOWP_MODE))))
#define AUD_EXIT_LOWP_MODE          (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_EXIT_LOWP_MODE))))
#define AUD_READ_LRADC              (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_READ_LRADC))))
#define AUD_READ_BATADC             (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_READ_BATADC))))
#define AUD_READ_ONOFF_KEY          (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_READ_ONOFF_KEY))))
#define AUD_WRITE_GPIO              (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_WRITE_GPIO))))
#define AUD_READ_GPIO               (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_READ_GPIO))))
#define AUD_READ_SIO                (*((uint32*)(HAL_RAM_ENTRY_ADDR(RAM_READ_SIO))))

#if 0
extern void hal_clear_watch_dog(void);
#endif
#define clear_watch_dog()                         ((void(*)(void))AUD_CLEAR_WATCH_DOG)()

#if 0
extern bool hal_intc_is_irq_pending_inner(uint8 irq_idx);
#endif
#define s2_intc_is_irq_pending(a)                 ((bool(*)(uint8))AUD_IS_IRQ_PENDING)((a))

#if 0
extern int hal_pmu_enter_low_power_mode_inner(low_power_reg_bak_t *p_low_power_reg_bak);
#endif
#define s2_pmu_enter_low_power_mode(a)            ((int(*)(low_power_reg_bak_t *))AUD_ENTER_LOWP_MODE)((a))

#if 0
extern int hal_pmu_exit_low_power_mode_inner(low_power_reg_bak_t *p_low_power_reg_bak);
#endif
#define s2_pmu_exit_low_power_mode(a)             ((int(*)(low_power_reg_bak_t *))AUD_EXIT_LOWP_MODE)((a))

#if 0
extern uint32 hal_pmu_read_lradc_inner(uint8 lradc_id);
#endif
#define hal_pmu_read_lradc(a)                     ((uint32(*)(uint8))AUD_READ_LRADC)((a))

#if 0
extern uint32 hal_pmu_read_batadc_inner(void);
#endif
#define hal_pmu_read_batadc()                     ((uint32(*)(void))AUD_READ_BATADC)()

#if 0
extern uint32 hal_pmu_read_onoff_key_inner(void);
#endif
#define hal_pmu_read_onoff_key()                  ((uint32(*)(void))AUD_READ_ONOFF_KEY)()

#if 0
extern void hal_set_gpio_out_level(uint8 pad_gpio, uint8 level);//pad_gpio 0 ~ 51
#endif
#define hal_gpio_write_gpio(a,b)                  ((void(*)(uint8,uint8))AUD_WRITE_GPIO)((a),(b))

#if 0
extern uint8 hal_get_gpio_in_level(uint8 pad_gpio);//pad_gpio 0 ~ 51
#endif
#define hal_gpio_read_gpio(a)                     ((uint8(*)(uint8))AUD_READ_GPIO)((a))

#if 0
extern uint8 hal_get_sio_in_level(uint8 pad_sio);//pad_sio 0 ~ 1
#endif
#define hal_gpio_read_wio(a)                      ((uint8(*)(uint8))AUD_READ_SIO)((a))


#define AUDIO_ENTRY_ADDR(x)         (HAL_ENTRY_VA_START + (4 * x))

#define AUD_ENABLE_PA               (*((uint32*)(AUDIO_ENTRY_ADDR(ENABLE_PA))))
#define AUD_DISABLE_PA              (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_PA))))
#define AUD_MUTE_PA                 (*((uint32*)(AUDIO_ENTRY_ADDR(MUTE_PA))))

#define AUD_SET_AUDIO_VOLUME        (*((uint32*)(AUDIO_ENTRY_ADDR(SET_AUDIO_VOLUME))))
#define AUD_GET_AUDIO_VOLUME        (*((uint32*)(AUDIO_ENTRY_ADDR(GET_AUDIO_VOLUME))))
#define AUD_SET_AUDIO_APS           (*((uint32*)(AUDIO_ENTRY_ADDR(SET_AUDIO_APS))))
#define AUD_GET_AUDIO_APS_INFO      (*((uint32*)(AUDIO_ENTRY_ADDR(GET_AUDIO_APS_INFO))))

#define AUD_ENABLE_DAC              (*((uint32*)(AUDIO_ENTRY_ADDR(ENABLE_DAC))))
#define AUD_DISABLE_DAC             (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_DAC))))
#define AUD_SET_DAC_RATE            (*((uint32*)(AUDIO_ENTRY_ADDR(SET_DAC_RATE))))
#define AUD_GET_DAC_RATE            (*((uint32*)(AUDIO_ENTRY_ADDR(GET_DAC_RATE))))
#define AUD_WAIT_DACFIFO_EMPTY      (*((uint32*)(AUDIO_ENTRY_ADDR(WAIT_DACFIFO_EMPTY))))

#define AUD_RESET_PCMBUF            (*((uint32*)(AUDIO_ENTRY_ADDR(RESET_PCMBUF))))
#define AUD_SET_PCMBUF              (*((uint32*)(AUDIO_ENTRY_ADDR(SET_PCMBUF))))
#define AUD_GET_PCMBUF_LENGTH       (*((uint32*)(AUDIO_ENTRY_ADDR(GET_PCMBUF_LENGTH))))

#define AUD_ENABLE_AIN              (*((uint32*)(AUDIO_ENTRY_ADDR(ENABLE_AIN))))
#define AUD_DISABLE_AIN             (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_AIN))))
#define AUD_SET_ADC_GAIN            (*((uint32*)(AUDIO_ENTRY_ADDR(SET_ADC_GAIN))))
#define AUD_ENABLE_ADC              (*((uint32*)(AUDIO_ENTRY_ADDR(ENABLE_ADC))))
#define AUD_DISABLE_ADC             (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_ADC))))
#define AUD_IS_SIM_ADC_DUAL         (*((uint32*)(AUDIO_ENTRY_ADDR(IS_SIM_ADC_DUAL))))

#define AUD_START_KEY_TONE          (*((uint32*)(AUDIO_ENTRY_ADDR(START_KEY_TONE))))
#define AUD_GET_KEY_TONE_STATUS     (*((uint32*)(AUDIO_ENTRY_ADDR(GET_KEY_TONE_STATUS))))

#define AUD_ENABLE_I2SRX            (*((uint32*)(AUDIO_ENTRY_ADDR(ENABLE_I2SRX))))
#define AUD_DISABLE_I2SRX           (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_I2SRX))))

#define AUD_SET_PAD_FUNCTION        (*((uint32*)(AUDIO_ENTRY_ADDR(SET_PAD_FUNCTION))))
#define AUD_IOCTL_PAD               (*((uint32*)(AUDIO_ENTRY_ADDR(IOCTL_PAD))))
#define AUD_SET_SIRQ                (*((uint32*)(AUDIO_ENTRY_ADDR(SET_SIRQ))))
#define AUD_DISABLE_SIRQ            (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_SIRQ))))
#define AUD_SET_FM_CLOCK            (*((uint32*)(AUDIO_ENTRY_ADDR(SET_FM_CLOCK))))
#define AUD_DISABLE_FM_CLOCK        (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_FM_CLOCK))))
#define AUD_SET_PWM                 (*((uint32*)(AUDIO_ENTRY_ADDR(SET_PWM))))
#define AUD_DISABLE_PWM             (*((uint32*)(AUDIO_ENTRY_ADDR(DISABLE_PWM))))


#define AUD_PMU_SET_WAKE_UP         (*((uint32*)(AUDIO_ENTRY_ADDR(PMU_SET_WAKE_UP))))
#define AUD_PMU_POWER_OFF           (*((uint32*)(AUDIO_ENTRY_ADDR(PMU_POWER_OFF))))
#define AUD_PMU_SET_LRADC           (*((uint32*)(AUDIO_ENTRY_ADDR(PMU_SET_LRADC))))
#define AUD_PMU_SET_BATADC          (*((uint32*)(AUDIO_ENTRY_ADDR(PMU_SET_BATADC))))
#define AUD_PMU_SET_LRADC1_RESISTANCE  (*((uint32*)(AUDIO_ENTRY_ADDR(PMU_SET_LRADC1_RESISTANCE))))

#define AUD_HARDWARE_SET            (*((uint32*)(AUDIO_ENTRY_ADDR(HARDWARE_SET))))
#define AUD_HARDWARE_GET            (*((uint32*)(AUDIO_ENTRY_ADDR(HARDWARE_GET))))

#define AUD_TWI_SET_HW_INFO         (*((uint32*)(AUDIO_ENTRY_ADDR(TWI_SET_HW_INFO))))
#define AUD_TWI_WRITE_BYTES         (*((uint32*)(AUDIO_ENTRY_ADDR(TWI_WRITE_BYTES))))
#define AUD_TWI_READ_BYTES          (*((uint32*)(AUDIO_ENTRY_ADDR(TWI_READ_BYTES))))
#define AUD_TWI_MASTER_SET_HW_INFO  (*((uint32*)(AUDIO_ENTRY_ADDR(TWI_MASTER_SET_HW_INFO))))
#define AUD_TWI_SLAVE_SET_HW_INFO   (*((uint32*)(AUDIO_ENTRY_ADDR(TWI_SLAVE_SET_HW_INFO))))

#define AUD_LED_HARDWARE_INIT       (*((uint32*)(AUDIO_ENTRY_ADDR(LED_HARDWARE_INIT))))
#define AUD_LED_HARDWARE_EXIT       (*((uint32*)(AUDIO_ENTRY_ADDR(LED_HARDWARE_EXIT))))
#define AUD_LED_HARDWARE_DISPLAY    (*((uint32*)(AUDIO_ENTRY_ADDR(LED_HARDWARE_DISPLAY))))
#define AUD_LED_HARDWARE_ADJUST_LIGHT  (*((uint32*)(AUDIO_ENTRY_ADDR(LED_HARDWARE_ADJUST_LIGHT))))


#define AUD_UART_TX_SET             (*((uint32*)(AUDIO_ENTRY_ADDR(UART_TX_SET))))
#define AUD_UART_SEND_BYTES         (*((uint32*)(AUDIO_ENTRY_ADDR(UART_SEND_BYTES))))
#define AUD_UART_RX_SET             (*((uint32*)(AUDIO_ENTRY_ADDR(UART_RX_SET))))
#define AUD_UART_RECV_BYTES         (*((uint32*)(AUDIO_ENTRY_ADDR(UART_RECV_BYTES))))


#define AUD_LIB_INIT                (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_INIT_API))))
#define AUD_LIB_FS_OPEN             (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_OPEN_API))))
#define AUD_LIB_FS_CLOSE            (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_FCLOSE_API))))
#define AUD_LIB_FS_SEEK             (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_SEEK_API))))
#define AUD_LIB_FS_TELL             (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_TELL_API))))
#define AUD_LIB_FS_READ            (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_FS_READ_API))))
#define AUD_LIB_READ_DATA          (*((uint32*)(AUDIO_ENTRY_ADDR(LIB_EXT_READ_API))))

#define AUD_IRC_HARDWARE_INIT       (*((uint32*)(AUDIO_ENTRY_ADDR(IRC_HARDWARE_INIT))))
#define AUD_IRC_HARDWARE_EXIT       (*((uint32*)(AUDIO_ENTRY_ADDR(IRC_HARDWARE_EXIT))))
#define AUD_IRC_CORRECT_USERCODE    (*((uint32*)(AUDIO_ENTRY_ADDR(IRC_CORRECT_USERCODE))))

#if (SUPPORT_EXT_RTC == 0)
#define AUD_RTC_PHY_INIT            (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_PHY_INIT))))
#define AUD_RTC_SET_TIME            (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_SET_TIME))))
#define AUD_RTC_GET_TIME            (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_GET_TIME))))
#define AUD_RTC_SET_DATE            (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_SET_DATE))))
#define AUD_RTC_GET_DATE            (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_GET_DATE))))
#define AUD_RTC_SET_ALARM_TIME      (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_SET_ALARM_TIME))))
#define AUD_RTC_GET_ALARM_TIME      (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_GET_ALARM_TIME))))
#define AUD_RTC_ENABLE_ALARM        (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_ENABLE_ALARM))))
#define AUD_RTC_DISABLE_ALARM       (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_DISABLE_ALARM))))
#define AUD_RTC_GET_STATUS          (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_GET_STATUS))))
#endif
#define AUD_RTC_BAKDATA_WRITE       (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_BAKDATA_WRITE))))
#define AUD_RTC_BAKDATA_READ        (*((uint32*)(AUDIO_ENTRY_ADDR(RTC_BAKDATA_READ))))

#define AUD_SPI1_TRANS_OVER         (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_TRANS_OVER))))
#define AUD_SPI1_PHY_INIT           (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_PHY_INIT))))
#define AUD_SPI1_READ_DATA          (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_READ_DATA))))
#define AUD_SPI1_WRITE_DATA         (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_WRITE_DATA))))
#define AUD_SPI1_SET_CS             (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_SET_CS))))

#define AUD_SPI1_NOR_INIT           (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_NOR_INIT))))
#define AUD_SPI1_NOR_READ_CHIPID    (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_NOR_READ_CHIPID))))
#define AUD_SPI1_NOR_NOR_ERASE      (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_NOR_ERASE))))
#define AUD_SPI1_NOR_READ           (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_NOR_READ))))
#define AUD_SPI1_NOR_WRITE          (*((uint32*)(AUDIO_ENTRY_ADDR(SPI1_NOR_WRITE))))

#define AUD_CARD_DETECT_DISK        (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_DETECT_DISK))))
#define AUD_CARD_INIT               (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_INIT))))
#define AUD_CARD_EXIT               (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_EXIT))))
#define AUD_CARD_SET_PM_PARAM       (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_SET_PM_PARAM))))
#define AUD_CARD_ADJUST_CLK         (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_ADJUST_CLK))))
#define AUD_CARD_SELECT             (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_SELECT))))
#define AUD_CARD_DESELECT           (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_DESELECT))))
#define AUD_CARD_PHY_RW_SUB         (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_PHY_RW_SUB))))
#define AUD_CARD_WAIT_DMA_OVER      (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_WAIT_DMA_OVER))))
#define AUD_CARD_STOP_TRANSMISSION  (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_STOP_TRANSMISSION))))
#define AUD_CARD_GET_CUR_STAT       (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_GET_CUR_STAT))))
#define AUD_CARD_GET_CAP            (*((uint32*)(AUDIO_ENTRY_ADDR(CARD_GET_CAP))))


#define AUD_USB_DEV_UHOST_DET_SWITCH     (*((uint32*)(AUDIO_ENTRY_ADDR(USB_DEV_UHOST_DET_SWITCH))))
#define AUD_UDEV_SET_DETECT              (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_SET_DETECT))))
#define AUD_UDEV_DETECT                  (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_DETECT))))
#define AUD_UDEV_INIT                    (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_INIT))))
#define AUD_UDEV_EXIT                    (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_EXIT))))
#define AUD_UDEV_SET_BULK_IN_EP          (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_SET_IN_EP))))
#define AUD_UDEV_SET_BULK_OUT_EP         (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_SET_OUT_EP))))
#define AUD_UDEV_REST_ENDP_TOGGLE        (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_REST_ENDP_TOGGLE))))
#define AUD_UDEV_STALL_ENDPOINT          (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_STALL_ENDPOINT))))
#define AUD_UDEV_RECOVER_STALL_ENDPOINT  (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_RECOVER_STALL_ENDPOINT))))
#define AUD_UDEV_PARSE_ISR_SOURCE        (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_PARSE_ISR_SOURCE))))
#define AUD_UDEV_USB_IS_CABLE_PLUG       (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_IS_CABLE_PLUG))))
#define AUD_UDEV_GET_SETUP_DATA          (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_GET_USB_SETUP_DATA))))
#define AUD_UDEV_SWITCH_TO_ADFU          (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_TO_ADFU))))
#define AUD_UDEV_CONNECT_STATE           (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_CONNECT_STATE))))
#define AUD_UDEV_USB_HID_PRO             (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_HID_PRO))))
#define AUD_UDEV_GET_SHORT_PK            (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_GET_SHORT_PK))))
#define AUD_UDEV_NAK_CTRL                (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_NAK_CTRL))))
#define AUD_UDEV_RECV_CTRL_DATA          (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_RECV_CTRL_DATA))))
#define AUD_UDEV_RECEIVE_DATA            (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_RECV_DATA))))
#define AUD_UDEV_SEND_DATA               (*((uint32*)(AUDIO_ENTRY_ADDR(UDEV_USB_SEND_DATA))))


#define AUD_UHOST_DETECT                        (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_DETECT))))
#define AUD_UHOST_BUS_RESET                     (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_BUS_RESET))))
#define AUD_UHOST_EP_RESET                      (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_EP_RESET))))
#define AUD_UHOST_CONTROLLER_EN                 (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_CONTROLLER_EN))))
#define AUD_UHOST_CONTROLLER_DISABLE            (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_CONTROLLER_DISABLE))))
#define AUD_UHOST_CLEAR_EP_ERR_IRQ              (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_CLEAR_EP_ERR_IRQ))))
#define AUD_UHOST_CLEAR_FEATURE_EP_STALL        (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_CLEAR_FEATURE_EP_STALL))))
#define AUD_UHOST_SETUP_STAGE_TRANSCATION       (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_SETUP_STAGE_TRANSCATION))))
#define AUD_UHOST_CONTROL_IN_DATA               (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_CONTROL_IN_DATA))))
#define AUD_UHOST_STATUS_STAGE_TRANSACATION_OUT (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_STATUS_STAGE_TRANSACATION_OUT))))
#define AUD_UHOST_STATUS_STAGE_TRANSACTION_IN   (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_STATUS_STAGE_TRANSACTION_IN))))
#define AUD_UHOST_SET_ENDPOINT                  (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_SET_ENDPOINT))))
#define AUD_UHOST_BULK_DATA                     (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_BULK_DATA))))
#define AUD_UHOST_TYPE                          (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_TYPE))))
#define AUD_UHOST_SET_FUNCTION_ADDR             (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_SET_FUNCTION_ADDRESS))))
#define AUD_UHOST_SET_MAX_PACKET                (*((uint32*)(AUDIO_ENTRY_ADDR(UHOST_SET_MAX_PACKET))))

#define AUD_HDMI_IIC_INIT                       (*((uint32*)(AUDIO_ENTRY_ADDR(HDMI_IIC_INIT))))
#define AUD_HDMI_IIC_RW                         (*((uint32*)(AUDIO_ENTRY_ADDR(HDMI_IIC_RW))))

//share code api
#define AUD_UPDATE_PP_PARA_INFO                 (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_UPDATE_PP_PARA_INFO_API))))
#define AUD_BUFFER_RW_WRITE                     (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_BUFFER_RW_WRITE_API))))
#define AUD_BUFFER_RW_READ                      (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_BUFFER_RW_READ_API))))
#define AUD_BUFFER_RW_UPDATE                    (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_BUFFER_RW_UPDATE_API))))
#define AUD_BUFFER_RW_FLUSH                     (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_BUFFER_RW_FLUSH_API))))
#define AUD_ENERGY_CALC                         (*((uint32*)(AUDIO_ENTRY_ADDR(SHARE_CODE_ENERGY_CALC_API))))


#if 0
extern int32 hal_audio_enable_pa(pa_setting_t *pa_sett);
#endif
#define enable_pa(a)                ((int32(*)(pa_setting_t *))AUD_ENABLE_PA)((a))

#if 0
extern int32 hal_audio_disable_pa(void);
#endif
#define disable_pa()                ((int32(*)(void))AUD_DISABLE_PA)()

#if 0
extern int32 hal_audio_mute_pa(bool mute);
#endif
#define mute_pa(a)                  ((int32(*)(bool))AUD_MUTE_PA)((a))

#if 0
extern int32 audio_set_audio_volume(int32 vol_p1_db);
#endif
#define set_audio_volume(a)         ((int32(*)(int32))AUD_SET_AUDIO_VOLUME)((a))

#if 0
extern int32 hal_audio_set_audio_aps(audio_aps_e aps);
#endif
#define set_audio_aps(a)            ((int32(*)(uint8))AUD_SET_AUDIO_APS)((a))

#if 0
extern int32 hal_audio_get_audio_aps_info(audio_aps_info_t *p_audio_aps_info);
#endif
#define get_audio_aps_info(a)       ((int32(*)(audio_aps_info_t *))AUD_GET_AUDIO_APS_INFO)((a))

#if 0
extern void hal_audio_enable_dac(dac_setting_t *dac_sett);
#endif
#define enable_dac(a)               ((void(*)(dac_setting_t *))AUD_ENABLE_DAC)((a))

#if 0
extern void hal_audio_disable_dac(void);
#endif
#define disable_dac()               ((void(*)(void))AUD_DISABLE_DAC)()

#if 0
extern void hal_audio_set_dac_rate(uint8 dac_rate);
#endif
#define set_dac_rate(a)             ((void(*)(uint8))AUD_SET_DAC_RATE)((a))

#if 0
extern uint8 hal_audio_get_dac_rate(void);
#endif
#define get_dac_rate()              ((uint8(*)(void))AUD_GET_DAC_RATE)()

#if 0
extern void hal_audio_wait_dacfifo_empty(void);
#endif
#define wait_dacfifo_empty()        ((void(*)(void))AUD_WAIT_DACFIFO_EMPTY)()

#if 0
extern void hal_audio_reset_pcmbuf(void);
#endif
#define reset_pcmbuf()              ((void(*)(void))AUD_RESET_PCMBUF)()

#if 0
extern void hal_audio_set_pcmbuf(pcmbuf_setting_t *pcmbuf_sett);
#endif
#define set_pcmbuf(a)               ((void(*)(pcmbuf_setting_t *))AUD_SET_PCMBUF)((a))

#if 0
extern uint32 hal_audio_get_pcmbuf_length(void);
#endif
#define get_pcmbuf_length()         ((uint32(*)(void))AUD_GET_PCMBUF_LENGTH)()

#if 0
extern void hal_audio_enable_ain(ain_source_type_e ain_src, ain_setting_t *ain_sett);
#endif
#define enable_ain(a,b)             ((void(*)(ain_source_type_e, ain_setting_t *))AUD_ENABLE_AIN)((a), (b))

#if 0
extern void hal_audio_disable_ain(ain_source_type_e ain_src);
#endif
#define disable_ain(a)              ((void(*)(ain_source_type_e))AUD_DISABLE_AIN)((a))

#if 0
extern void hal_audio_set_adc_gain(uint8 adc_gain);
#endif
#define set_adc_gain(a)             ((void(*)(adc_gain_e))AUD_SET_ADC_GAIN)((a))

#if 0
extern void hal_audio_enable_adc(ain_source_type_e ain_src, adc_setting_t *adc_sett);
#endif
#define enable_adc(a,b)             ((void(*)(ain_source_type_e, adc_setting_t *))AUD_ENABLE_ADC)((a), (b))

#if 0
extern void hal_audio_disable_adc(ain_source_type_e ain_src);
#endif
#define disable_adc(a)              ((void(*)(ain_source_type_e))AUD_DISABLE_ADC)((a))

#if 0
extern bool hal_audio_is_sim_adc_dual(void);
#endif
#define is_sim_adc_dual()           ((bool(*)(void))AUD_IS_SIM_ADC_DUAL)()

#if 0
extern void hal_audio_start_key_tone(kt_setting_t *kt_sett);
#endif
#define start_key_tone(a)           ((void(*)(kt_setting_t *))AUD_START_KEY_TONE)((a))

#if 0
extern uint8 hal_audio_get_key_tone_status(void);
#endif
#define get_key_tone_status()       ((uint8(*)(void))AUD_GET_KEY_TONE_STATUS)()

#if 0
extern void hal_audio_enable_i2srx(i2srx_setting_t *i2srx_sett);
#endif
#define enable_i2srx(a)             ((void(*)(i2srx_setting_t *))AUD_ENABLE_I2SRX)((a))

#if 0
extern void hal_audio_disable_i2srx(void);
#endif
#define disable_i2srx()             ((void(*)(void))AUD_DISABLE_I2SRX)()


#if 0
extern int8 hal_set_pad_function(ic_pad_e pad_id, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);
#endif
#define set_pad_function(a,b,c,d)   ((int8(*)(ic_pad_e, pad_function_e, pad_phy_para_t *, void *))AUD_SET_PAD_FUNCTION)((a), (b), (c), (d))

#if 0
extern void hal_ioctl_pad(ic_pad_e pad_id, pad_function_e pad_func, void *para);
#endif
#define ioctl_pad(a,b,c)            ((void(*)(ic_pad_e, pad_function_e, void *))AUD_IOCTL_PAD)((a), (b), (c))

#if 0
//sirq_num : 指IO序号，从0开始，比如GPIO3，要传递为：PAD_GPIO3-PAD_GPIO0 = 3
extern void hal_set_sirq_inner(uint8 sirq_num, sirq_set_para_t *sirq_set);
#endif
#define hal_set_sirq(a,b)                               ((void(*)(uint8,sirq_set_para_t *))AUD_SET_SIRQ)((a),(b))

#if 0
//sirq_num : 指IO序号，从0开始，比如GPIO3，要传递为：PAD_GPIO3-PAD_GPIO0 = 3
extern void hal_disable_sirq_inner(uint8 sirq_num);
#endif
#define hal_disable_sirq(a)                             ((void(*)(uint8))AUD_DISABLE_SIRQ)((a))

#if 0
extern void hal_set_fm_clock_inner(uint8 clk_select);
#endif
#define hal_set_fm_clock(a)                             ((void(*)(uint8))AUD_SET_FM_CLOCK)((a))

#if 0
extern void hal_disable_fm_clock_inner(void);
#endif
#define hal_disable_fm_clock()                          ((void(*)(void))AUD_DISABLE_FM_CLOCK)()

#if 0
extern void hal_set_pwm_init(uint8 idx, pwm_para_t *pwm_para);
#endif
#define hal_set_pwm(a,b)                                ((void(*)(uint8,pwm_para_t*))AUD_SET_PWM)((a),(b))

#if 0
extern void hal_reset_pwm(uint8 idx);
#endif
#define hal_disable_pwm(a)                              ((void(*)(uint8))AUD_DISABLE_PWM)((a))


#if 0
extern void hal_pmu_en_wake_up(wakeup_en_para_t *p_wakeup_en_para);
#endif
#define pmu_set_wake_up(a)          ((void(*)(wakeup_en_para_t *))AUD_PMU_SET_WAKE_UP)((a))

#if 0
extern void hal_pmu_power_off(void);
#endif
#define pmu_power_off()             ((void(*)(void))AUD_PMU_POWER_OFF)()

#if 0
extern void hal_pmu_enable_lradc(uint8 lradc_id, bool enable);
#endif
#define pmu_enable_lradc(a,b)       ((void(*)(uint8,bool))AUD_PMU_SET_LRADC)((a),(b))

#if 0
extern void hal_pmu_enable_batadc(bool enable);
#endif
#define pmu_enable_batadc(a)        ((void(*)(bool))AUD_PMU_SET_BATADC)((a))

#if 0
extern void hal_pmu_enable_lradc1_resistance(bool enable);
#endif
#define pmu_enable_lradc1_resistance(a)  ((void(*)(bool))AUD_PMU_SET_LRADC1_RESISTANCE)((a))



#if 0
extern int32 hal_hardware_set(hw_set_op_e set_op, void *para);
#endif
#define hardware_set(a,b)           ((int32(*)(hw_set_op_e, void *))AUD_HARDWARE_SET)((a), (b))

#if 0
extern int32 hal_hardware_get(hw_get_op_e get_op, void *para);
#endif
#define hardware_get(a,b)           ((int32(*)(hw_get_op_e, void *))AUD_HARDWARE_GET)((a), (b))


#if 0
extern void hal_TWI_set_hw_info_sim(uint8 idx, twi_hw_info_t *p_twi_hw_info);
#endif
#define twi_set_hw_info_sim(a,b)      ((void(*)(uint8, twi_hw_info_t *))AUD_TWI_SET_HW_INFO)((a),(b))

#if 0
extern uint8 hal_TWI_write_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode);
#endif
#define twi_write_bytes(a,b,c,d,e,f)  ((uint8(*)(uint8, uint8, uint8 *, uint8, uint8, uint8))AUD_TWI_WRITE_BYTES)((a), (b), (c), (d), (e), (f))

#if 0
extern uint8 hal_TWI_read_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length);
#endif
#define twi_read_bytes(a,b,c,d,e)     ((uint8(*)(uint8, uint8, uint8 *, uint8, uint8))AUD_TWI_READ_BYTES)((a), (b), (c), (d), (e))

#if 0
extern void hal_TWI_master_set_hw_info(uint8 idx, twi_master_hw_info_t *p_twi_master_hw_info);
#endif
#define twi_master_set_hw_info(a,b)   ((void(*)(uint8, twi_master_hw_info_t *))AUD_TWI_MASTER_SET_HW_INFO)((a),(b))

#if 0
extern void hal_TWI_slave_set_hw_info(twi_slave_hw_info_t *p_hw_info);
#endif
#define twi_slave_set_hw_info(a)    ((void(*)(twi_slave_hw_info_t *))AUD_TWI_SLAVE_SET_HW_INFO)((a))


#if 0
extern void hal_led_phy_init(void *p_led_phy_para);
#endif
#define hal_led_hardware_init(a)    ((void(*)(void*))AUD_LED_HARDWARE_INIT)((a))

#if 0
extern void hal_led_phy_exit(void);
#endif
#define hal_led_hardware_exit()     ((void(*)(void))AUD_LED_HARDWARE_EXIT)()

#if 0
extern void hal_led_display(uint32 *p_seg_disp_data);
#endif
#define hal_led_hardware_display(a) ((void(*)(uint32 *))AUD_LED_HARDWARE_DISPLAY)((a))

#if 0
extern void hal_led_adjust_light(uint8 light_level);
#endif
#define hal_led_hardware_adjust_light(a) ((void(*)(uint8))AUD_LED_HARDWARE_ADJUST_LIGHT)((a))


#if 0
extern void hal_set_uart_tx_init(uint8 idx, uint32 baudrate, uint8 clk_sel);
#endif
#define uart_tx_set(a,b,c)          ((void(*)(uint8, uint32, uint8))AUD_UART_TX_SET)((a), (b), (c))

#if 0
extern void uart_send_bytes_inner(uint8 idx, uint8 *buf, uint32 bytes);
#endif
#define uart_send_bytes(a,b,c)      ((void(*)(uint8, uint8 *, uint32))AUD_UART_SEND_BYTES)((a), (b), (c))

#if 0
extern void hal_set_uart_rx_init(uint8 idx, uint32 baudrate, uint8 clk_sel);
#endif
#define uart_rx_set(a,b,c)          ((void(*)(uint8, uint32, uint8))AUD_UART_RX_SET)((a), (b), (c))

#if 0
extern uint32 uart_recv_bytes_inner(uint8 idx, uint8 *buf, uint32 bytes);
#endif
#define uart_recv_bytes(a,b,c)      ((uint32(*)(uint8, uint8 *, uint32))AUD_UART_RECV_BYTES)((a), (b), (c))

#if 0
extern void hal_lib_fs_init_inner(void);
#endif
#define lib_fs_init()               ((void(*)(void))AUD_LIB_INIT)()

#if 0
extern lib_file_t *hal_lib_fopen_inner(const char *filename);
#endif
#define lib_fopen(a)                ((lib_file_t*(*)(const char *))AUD_LIB_FS_OPEN)((a))

#if 0
extern int hal_lib_fclose_inner(lib_file_t *fp);
#endif
#define lib_fclose(a)               ((int32(*)(lib_file_t *))AUD_LIB_FS_CLOSE)((a))

#if 0
extern int hal_lib_fseek_inner(lib_file_t *fp, uint8 whence, int32 offset);
#endif
#define lib_fseek(a,b,c)            ((int32(*)(lib_file_t *,uint8 ,int32))AUD_LIB_FS_SEEK)((a),(b),(c))

#if 0
extern uint32 hal_lib_ftell_inner(lib_file_t *fp);
#endif
#define lib_ftell(a)                ((uint32(*)(lib_file_t *))AUD_LIB_FS_TELL)((a))

#if 0
extern int32 hal_lib_fread_inner(lib_file_t *fp, void *buffer, uint32 len);
#endif
#define lib_fread(a,b,c)            ((int32(*)(lib_file_t *,void*,uint32))AUD_LIB_FS_READ)((a),(b),(c))

#if 0
extern int32 hal_lib_read_data_inner(uint32 byte_addr, uint8 *buf, uint32 byte_num);
#endif
#define lib_read_data(a,b,c)        ((int32(*)(uint32, uint8 *, uint32))AUD_LIB_READ_DATA)((a),(b),(c))

#if 0
extern void hal_irc_phy_init(uint8 irc_mode, uint32 usercode, __irc_detect_key_cbk detect_key_cbk, __irc_repeat_key_cbk repeat_key_cbk);
#endif
#define irc_hardware_init(a,b,c,d)  ((void(*)(uint8 , uint32 , __irc_detect_key_cbk, __irc_repeat_key_cbk ))AUD_IRC_HARDWARE_INIT)((a), (b), (c), (d))

#if 0
extern void hal_irc_phy_exit(void);
#endif
#define irc_hardware_exit()         ((void(*)(void))AUD_IRC_HARDWARE_EXIT)()

#if 0
extern int32 hal_irc_correct_usercode(uint32 *usercode);
#endif
#define irc_correct_usercode(a)     ((int32(*)(uint32 *))AUD_IRC_CORRECT_USERCODE)((a))


#if (SUPPORT_EXT_RTC == 0)

#if 0
extern void hal_rtc_phy_init(void);
#endif
#define rtc_phy_init()              ((void(*)(void))AUD_RTC_PHY_INIT)()

#if 0
extern void hal_rtc_set_time(time_t *time);
#endif
#define rtc_set_time(a)             ((void(*)(time_t*))AUD_RTC_SET_TIME)((a))

#if 0
extern void hal_rtc_get_time(time_t *time);
#endif
#define rtc_get_time(a)             ((void(*)(time_t*))AUD_RTC_GET_TIME)((a))

#if 0
extern void hal_rtc_set_date(date_t *date);
#endif
#define rtc_set_date(a)             ((void(*)(date_t*))AUD_RTC_SET_DATE)((a))

#if 0
extern void hal_rtc_get_date(date_t *date);
#endif
#define rtc_get_date(a)             ((void(*)(date_t*))AUD_RTC_GET_DATE)((a))

#if 0
extern void hal_rtc_set_alarm_time(time_t *time);
#endif
#define rtc_set_alarm_time(a)       ((void(*)(time_t*))AUD_RTC_SET_ALARM_TIME)((a))

#if 0
extern void hal_rtc_get_alarm_time(time_t *time);
#endif
#define rtc_get_alarm_time(a)       ((void(*)(time_t*))AUD_RTC_GET_ALARM_TIME)((a))

#if 0
extern void hal_rtc_enable_alarm(rtc_alarm_isr alarm_isr);
#endif
#define rtc_enable_alarm(a)         ((void(*)(void (*alarm_isr)(void)))AUD_RTC_ENABLE_ALARM)((a))

#if 0
extern void hal_rtc_disable_alarm(void);
#endif
#define rtc_disable_alarm()         ((void(*)(void))AUD_RTC_DISABLE_ALARM)()

#if 0
extern void hal_rtc_get_status(rtc_status_t *p_rtc_status);
#endif
#define rtc_get_status(a)           ((void(*)(rtc_status_t*))AUD_RTC_GET_STATUS)((a))

#endif

#if 0
extern void hal_rtc_bakdata_write(uint8 bak_idx, uint32 bak_val);
#endif
#define rtc_bakdata_write(a,b)      ((void(*)(uint8,uint32))AUD_RTC_BAKDATA_WRITE)((a),(b))

#if 0
extern uint32 hal_rtc_bakdata_read(uint8 bak_idx);
#endif
#define rtc_bakdata_read(a)         ((uint32(*)(uint8))AUD_RTC_BAKDATA_READ)((a))

#if 0
bool hal_spi1_trans_voer(struct spi_info *si, int dir)
#endif
#define spi1_trans_over(a,b)        ((bool(*)(void*,int))AUD_SPI1_TRANS_OVER)((a),(b))

#if 0
extern int hal_spi1_init(struct spi_info *si, unsigned int clk_khz);
#endif
#define spi1_phy_init(a,b)          ((int(*)(void*,uint32))AUD_SPI1_PHY_INIT)((a),(b))

#if 0
extern void hal_spi1_read_data(struct spi_info *si, unsigned char *buf, int len, int mode);
#endif
#define spi1_read_data(a,b,c,d)     ((void(*)(void*,void*,int,int))AUD_SPI1_READ_DATA)((a),(b),(c),(d))

#if 0
extern void hal_spi1_write_data(struct spi_info *si, const unsigned char *buf, int len, int mode);
#endif
#define spi1_write_data(a,b,c,d)    ((void(*)(void*,void*,int,int))AUD_SPI1_WRITE_DATA)((a),(b),(c),(d))

#if 0
extern void hal_spi1_set_cs(struct spi_info *si, int value);
#endif
#define spi1_set_cs(a,b)            ((void(*)(void*,int))AUD_SPI1_SET_CS)((a),(b))


#if 0
extern int hal_spi1nor_init(void);
#endif
#define spi1nor_init()              ((int(*)(void))AUD_SPI1_NOR_INIT)()

#if 0
extern unsigned int hal_spi1nor_read_chipid(void)
#endif
#define spi1nor_read_chipid()       ((uint32(*)(void))AUD_SPI1_NOR_READ_CHIPID)()

#if 0
extern int hal_spi1nor_erase(unsigned int addr, int size);
#endif
#define spi1nor_erase(a,b)          ((int(*)(unsigned int,int))AUD_SPI1_NOR_NOR_ERASE)((a),(b))

#if 0
extern int hal_spi1nor_read(unsigned int addr, void *data, int len);
#endif
#define spi1nor_read_data(a,b,c)    ((int(*)(unsigned int,void*,int))AUD_SPI1_NOR_READ)((a),(b),(c))

#if 0
extern int hal_spi1nor_write(unsigned int addr, const void *data, int len);
#endif
#define spi1nor_write_data(a,b,c)   ((int(*)(unsigned int,void*,int))AUD_SPI1_NOR_WRITE)((a),(b),(c))


#if 0
extern int hal_sdmmc_app_detect(void);
#endif
#define hal_card_detect()           ((int32(*)(void))AUD_CARD_DETECT_DISK)()

#if 0
extern int32 hal_sdmmc_app_phy_configure(void *card_param, uint8 *configure_buffer);
#endif
#define card_phy_configure(a,b)     ((int32(*)(void*, uint8*))AUD_CARD_INIT)((a),(b))

#if 0
extern void hal_sdmmc_app_reset_pad(void *para);
#endif
#define card_reset_pad(a)           ((int32(*)(void*))AUD_CARD_EXIT)((a))

#if 0
extern int audio_device_set_card_pm_param(card_pm_cfg_t *card_pm_cfg);
#endif
#define hal_set_card_pm_param(a)    ((int32(*)(card_pm_cfg_t *))AUD_CARD_SET_PM_PARAM)((a))

#if 0
extern void hal_sdmmc_set_clk(uint8 sdclk_div);
#endif
#define adjust_card_clk(a)          ((void(*)(uint32))AUD_CARD_ADJUST_CLK)((a))

#if 0
extern int32 hal_sdmmc_app_select(void);
#endif
#define card_select()               ((int32(*)(void))AUD_CARD_SELECT)()

#if 0
extern int32 hal_sdmmc_app_deselect(void);
#endif
#define card_deselect()             ((int32(*)(void))AUD_CARD_DESELECT)()

#if 0
extern int32 hal_sdmmc_app_start_phy_rw(uint32 opr, uint32 cmd, uint32 arg, uint32 blk_num, uint32 blk_size, uint8 *ram);
#endif
#define phy_card_rw_sub(a,b,c,d,e,f)((int32(*)(uint32,uint32,uint32,uint32,uint32,uint8*))AUD_CARD_PHY_RW_SUB)((a),(b),(c),(d),(e),(f))

#if 0
extern int32 hal_sdmmc_app_wait_phy_rw_cmp(uint32 timeout_ms);
#endif
#define card_wait_dma_complete(a)   ((int32(*)(uint32))AUD_CARD_WAIT_DMA_OVER)((a))

#if 0
extern int32 hal_sdmmc_app_stop_transmission(void);
#endif
#define card_stop_transmission()    ((int32(*)(void))AUD_CARD_STOP_TRANSMISSION)()

#if 0
extern int32 hal_sdmmc_app_get_status(uint8 *status);
#endif
#define get_card_cur_status(a)      ((int32(*)(uint8*))AUD_CARD_GET_CUR_STAT)((a))

#if 0
extern int32 hal_sdmmc_get_cap(uint32 *ret_cap);
#endif
#define card_get_cap(a)             ((int32(*)(uint32*))AUD_CARD_GET_CAP)((a))



#if 0
extern int32 hal_usb_device_uhost_detect_switch_inner(uint8 det_mode);
#endif
#define hal_usb_dev_uhost_detect_switch(a)      ((int32(*)(uint8))AUD_USB_DEV_UHOST_DET_SWITCH)((a))

#if 0
extern int32 hal_set_usb_detect_inner(uint8 enable);
#endif
#define hal_set_usb_detect(a)                   ((int32(*)(uint8))AUD_UDEV_SET_DETECT)((a))

#if 0
extern int32 hal_usb_detect_inner(void);
#endif
#define hal_usb_detect()                        ((int32(*)(void))AUD_UDEV_DETECT)()

#if 0
extern void audio_usb_hardware_init(usb_isr_setting_t *p_usb_isr_sett);
#endif
#define usb_hardware_init(a)                    ((void(*)(usb_isr_setting_t*))AUD_UDEV_INIT)((a))

#if 0
extern void audio_usb_hardware_exit(void);
#endif
#define usb_hardware_exit()                     ((void(*)(void))AUD_UDEV_EXIT)()

#if 0
extern void hal_usb_set_in_ep(uint8 transfertype,uint8 enpnum,uint16 maxpck,uint32 staddr,uint8 buftype);
#endif
#define usb_hardware_set_in_ep(a,b,c,d,e)       ((void(*)(uint8,uint8,uint16 ,uint32,uint8 ))AUD_UDEV_SET_BULK_IN_EP)(a,b,c,d,e)

#if 0
extern void hal_usb_set_out_ep(uint8 transfertype,uint16 maxpck,uint32 staddr,uint8 buftype);
#endif
#define usb_hardware_set_out_ep(a,b,c,d)        ((void(*)(uint8,uint16 ,uint32,uint8 ))AUD_UDEV_SET_BULK_OUT_EP)(a,b,c,d)

#if 0
extern void hal_usb_rest_ep_toggle(uint8 enpnum);
#endif
#define usb_hardware_rest_endp_toggle(a)        ((void(*)(uint8 ))AUD_UDEV_REST_ENDP_TOGGLE)(a)

#if 0
extern void hal_usb_stall_ep(uint8 ep_num);
#endif
#define usb_hardware_stall_endpoint(a)          ((void(*)(uint8 ))AUD_UDEV_STALL_ENDPOINT)(a)

#if 0
extern void hal_usb_clear_stall_ep(uint8 ep_num);
#endif
#define usb_hardware_recover_stall_endpoint(a)  ((void(*)(uint8 ))AUD_UDEV_RECOVER_STALL_ENDPOINT)(a)

#if 0
extern int32 hal_usb_send_ctrl_data(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen);
#endif
#define usb_hardware_send_ctrl_data(a,b,c)      ((int32(*)(uint8 *, uint8 , uint8 ))AUD_UDEV_PARSE_ISR_SOURCE)((a),(b),(c))

#if 0
extern bool hal_usb_is_cable_plug(void);
#endif
#define usb_is_cable_plug()                     ((bool(*)(void))AUD_UDEV_USB_IS_CABLE_PLUG)()

#if 0
extern int32 hal_usb_receive_data(uint32 address, uint32 length, bool need_clear_busy, uint32 timeout_ms);
#endif
#define USB_Receive_Data(a,b,c,d)               ((int32(*)(uint32 , uint32 , bool , uint32))AUD_UDEV_RECEIVE_DATA)((a),(b),(c),(d))

#if 0
extern int32 hal_usb_send_data(uint32 address, uint32 length, uint32 pre_fill_bytes, uint32 timeout_ms);
#endif
#define USB_Send_Data(a,b,c,d)                  ((int32(*)(uint32 , uint32 , uint32 , uint32))AUD_UDEV_SEND_DATA)((a),(b),(c),(d))

#if 0
extern void hal_get_usb_setup_data(device_requests_t* dat);
#endif
#define USB_get_setup_data(a)                   ((int32(*)(device_requests_t *))AUD_UDEV_GET_SETUP_DATA)((a))

#if 0
extern void hal_usb_to_adfu(void);
#endif
#define USB_switch_to_adfu()                    ((int32(*)(void))AUD_UDEV_SWITCH_TO_ADFU)()

#if 0
extern uint8 hal_usb_connect_state(uint8 mode);
#endif
#define USB_connect_state(a)                    ((uint8(*)(uint8))AUD_UDEV_CONNECT_STATE)((a))

#if 0
extern void hal_usb_hid_pro(uint8* p_hid_change,uint16* p_idle_time,uint8* p_hid_value);
#endif
#define USB_hid_pro(a,b,c)                      ((void(*)(uint8*,uint16*,uint8*))AUD_UDEV_USB_HID_PRO)((a),(b),(c))

#if 0
extern void hal_usb_get_short_pk_data(bulkcbw* pdata);
#endif
#define USB_get_short_pk_data(a)                ((void(*)(bulkcbw* ))AUD_UDEV_GET_SHORT_PK)((a))

#if 0
extern void hal_usb_nak_ctrl(void);
#endif
#define usb_hardware_nak_ctrl()                 ((void(*)(void))AUD_UDEV_NAK_CTRL)()

#if 0
extern int32 hal_usb_recv_ctrl_data(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen);
#endif
#define usb_hardware_recv_ctrl_data(a,b,c)      ((int32(*)(uint8 *, uint8 , uint8))AUD_UDEV_RECV_CTRL_DATA)((a),(b),(c))


#if 0
extern int hal_uhost_detect_inner(void);
#endif
#define hal_uhost_detect()                              ((int32(*)(void))AUD_UHOST_DETECT)()

#if 0
extern uint8 audio_device_host_bus_reset(void);
#endif
#define hal_host_bus_reset()                            ((uint8(*)(void))AUD_UHOST_BUS_RESET)()

#if 0
extern void audio_device_host_ep_reset(uint8 mode);
#endif
#define hal_host_ep_reset(a)                             ((void(*)(uint8))AUD_UHOST_EP_RESET)(a)

#if 0
extern uint8 audio_device_host_controller_enable(uint8 speed_mode, uint8 * init_already, uint8 *uhost_state);
#endif
#define hal_host_controller_enable(a,b,c)               ((uint8(*)(uint8,uint8*,uint8*))AUD_UHOST_CONTROLLER_EN)((a),(b),(c))

#if 0
extern uint8 audio_device_host_controller_disable(void);
#endif
#define hal_host_controller_disable()                   ((uint8(*)(void))AUD_UHOST_CONTROLLER_DISABLE)()

#if 0
extern void audio_device_host_clear_eperrorirq(void);
#endif
#define hal_host_clear_eperrorirq()                     ((void(*)(void))AUD_UHOST_CLEAR_EP_ERR_IRQ)()

#if 0
extern uint8 audio_device_host_clear_feature_ep_stall(uint8 stall_stage, usb_host_information_t* host_info);
#endif
#define hal_host_clear_feature_ep_stall(a,b)            ((uint8(*)(uint8,void*))AUD_UHOST_CLEAR_FEATURE_EP_STALL)((a),(b))

#if 0
extern uint8 audio_device_host_setup_stage_transaction(uint8 *data_address, uint8 data_length);
#endif
#define hal_host_setup_stage_transaction(a,b)           ((uint8(*)(uint8*,uint8))AUD_UHOST_SETUP_STAGE_TRANSCATION)((a),(b))

#if 0
extern uint8 audio_device_host_control_in_data(uint8 *data_address, uint8 data_length);
#endif
#define hal_host_control_in_data(a,b)                   ((uint8(*)(uint8*,uint8))AUD_UHOST_CONTROL_IN_DATA)((a),(b))

#if 0
extern uint8 audio_device_host_status_stage_transaction_out(void);
#endif
#define hal_host_status_stage_transaction_out()         ((uint8(*)(void))AUD_UHOST_STATUS_STAGE_TRANSACATION_OUT)()

#if 0
extern uint8 audio_device_host_status_stage_transaction_in(void);
#endif
#define hal_host_status_stage_transaction_in()          ((uint8(*)(void))AUD_UHOST_STATUS_STAGE_TRANSACTION_IN)()

#if 0
extern uint8 audio_device_host_set_endpoint(device_configuration_t* device_config, usb_host_information_t* host_info);
#endif
#define hal_host_set_endpoint(a,b)                      ((uint8(*)(void*,void*))AUD_UHOST_SET_ENDPOINT)((a),(b))

#if 0
extern uint8 audio_device_host_bulk_data(uint8 *buf, uint32 len, usb_host_information_t* host_info, uint8 rw_mode);
#endif
#define hal_host_bulk_data(a,b,c,d)                     ((uint8(*)(uint8*,uint32,void*,uint8))AUD_UHOST_BULK_DATA)((a),(b),(c),(d))

#if 0
extern uint8 hal_get_uhost_in_type(void);
#endif
#define hal_get_uhost_type()                            ((usb_host_in_e(*)(void))AUD_UHOST_TYPE)()

#if 0
extern void hal_host_set_function_address(uint8 address);
#endif
#define hal_uhost_set_func_addr(a)              ((void(*)(uint8 ))AUD_UHOST_SET_FUNCTION_ADDR)((a))

#if 0
extern void hal_host_set_max_packet(uint8 max_packet);
#endif
#define hal_uhost_set_max_packet(a)             ((void(*)(uint8 ))AUD_UHOST_SET_MAX_PACKET)((a))

//hdmi communication interface
#if 0
extern void hal_iic_init(SMBUS_ID bus, SMBUS_SPEED speed, hdmi_hw_info_t *p_hdmi_hw_info);
#endif
#define hal_i2c_init(a,b,c)         ((void(*)(SMBUS_ID, SMBUS_SPEED, hdmi_hw_info_t *))AUD_HDMI_IIC_INIT)((a),(b),(c))

#if 0
extern SMBUS_STATUS hal_iic_sync(SMBUS_ID bus, uint8 addr, uint8 *buf, uint16 length, SMBUS_MODE mode);
#endif
#define hal_i2c_sync(a,b,c,d,e)     ((SMBUS_STATUS(*)(SMBUS_ID, uint8, uint8*, uint16, SMBUS_MODE))AUD_HDMI_IIC_RW)((a),(b),(c),(d),(e))

//share code api
#if 0
extern void update_PP_para_info_inner(post_process_detail_t *p_pp_detail);
#endif
#define update_pp_para_info(a)                  ((int32(*)(void *))AUD_UPDATE_PP_PARA_INFO)((a))

#if 0
extern bool buffer_rw_write_inner(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len);
#endif
#define buffer_rw_write(a,b,c)                  ((bool(*)(buffer_rw_t *, uint8 *, uint16))AUD_BUFFER_RW_WRITE)((a),(b),(c))

#if 0
extern bool buffer_rw_read_inner(buffer_rw_t *p_buf_rw, uint8 *r_buf, uint16 r_len);
#endif
#define buffer_rw_read(a,b,c)                   ((bool(*)(buffer_rw_t *, uint8 *, uint16))AUD_BUFFER_RW_READ)((a),(b),(c))

#if 0
extern bool buffer_rw_update_inner(buffer_rw_t *p_buf_rw, uint16 r_len);
#endif
#define buffer_rw_update(a,b)                   ((bool(*)(buffer_rw_t *, uint16))AUD_BUFFER_RW_UPDATE)((a),(b))

#if 0
extern bool buffer_rw_flush_inner(buffer_rw_t *p_buf_rw);
#endif
#define buffer_rw_flush(a)                      ((bool(*)(buffer_rw_t *))AUD_BUFFER_RW_FLUSH)((a))

#if 0
extern void energy_calc_inner(short *frame_buf, uint16 samples_cnt, bool is_stereo, short *p_cur_power_max, short *p_cur_power_rms);
#endif
#define energy_calc(a,b,c,d,e)                  ((void(*)(short *, uint16 , bool , short *, short *))AUD_ENERGY_CALC)((a),(b),(c),(d),(e))


#endif

