/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动相关宏定义，枚举类型，结构体定义等，为KEY驱动和前台应用共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _KEY_INTERFACE_H
#define _KEY_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"
#include "key_msg_value.h"

/*!
 * \brief
 *  input_msg_type_e 输入型消息类型，也叫 gui 消息类型
 */
typedef enum
{
    /*! 普通按键 */
    INPUT_MSG_KEY = 0x00,
    /*! 主箱收到的副箱按键 */
    INPUT_MSG_KEY_SLAVE,
} input_msg_type_e;

/*!
 * \brief
 *  key_type_e 按键类型
 */
typedef enum
{
    /*! 没有按键*/
    KEY_TYPE_NULL = 0x0000,
    /*! 双击按键 */
    KEY_TYPE_DOUBLE_CLICK = 0x4000,
    /*! 按键按下*/
    KEY_TYPE_DOWN = 0x2000,
    /*! 按键长按*/
    KEY_TYPE_LONG = 0x1000,
    /*! 按键保持*/
    KEY_TYPE_HOLD = 0x0800,
    /*! 按键短按弹起*/
    KEY_TYPE_SHORT_UP = 0x0400,
    /*! 按键长按弹起*/
    KEY_TYPE_LONG_UP = 0x0200,
    /*! 按键保持弹起*/
    KEY_TYPE_HOLD_UP = 0x0100,
    /*! 任意按键类型，不包含双击按键 */
    KEY_TYPE_ALL = 0x3f00,
} key_type_e;

/*!
 * \brief
 *  key_event_t 按键事件结构体
 */
typedef struct
{
    /*! 按键（逻辑）值*/
    key_value_e val;
    uint8 reserve;
    /*! 按键类型*/
    key_type_e type;
} key_event_t;


/*!
 * \brief
 *  input_gui_msg_t 输入型消息，又称 gui 消息结构体
 */
typedef struct
{
    /*! 消息类型*/
    input_msg_type_e type;
    /*! 消息数据*/
    union
    {
        /*! 按键事件*/
        key_event_t kmsg;
    } data;
} input_gui_msg_t;


#define KEY_NUM_MAX    10 //最多支持10个线控按键
typedef struct
{
    uint8 key_num; //线控按键数
    uint8 key_onoff; //ONOFF KEY 逻辑按键值
    uint8 lradc_pin; //ADC按键PIN选择，PAD_GPIO8=LRADC1 或 PAD_GPIO4=LRADC2
    uint8 uart_sim_key_support : 1; //是否支持UART模拟按键
    uint8 lradc_res_enable : 1; //是否使能 LRADCX 内置100K上拉电阻
    uint8 need_usb_detect : 1; //是否需要检测USB Device或Host设备
    uint8 vcc_voltage_select : 1; //VCC电压选择，0:3.1V,对应AVCC 2.95V；1:3.4V,对应AVCC 3.1V
    uint8 key_val[KEY_NUM_MAX]; //逻辑线控按键序列
    uint16 key_adc_val[KEY_NUM_MAX]; //物理线控按键ADC值序列
} keymap_para_t;

#define KEY_IR_NUM_MAX 20 //最多支持20个红外按键
typedef struct
{
    uint8 ir_protocol;
    uint8 ir_key_num; //红外按键数
    uint16 ir_usercode;
    uint8 ir_key_val[KEY_IR_NUM_MAX]; //红外遥控按键序列
    uint8 ir_key_code[KEY_IR_NUM_MAX]; //红外遥控按键key code 序列
} irmap_para_t;

/*1.定义提供的具体接口命令，这里是按键驱动统一提供的接口*/

typedef enum
{
    GET_BATTERY = 0,
    AUX_HOT_DETECT = 1,
    USBHOST_HOT_DETECT = 2,
    USBDEV_HOT_DETECT = 3,
    ADAPTOR_HOT_DETECT = 4,
    CARD_HOT_DETECT = 5,
    USB_DETECT_SWITCH = 6,

    ADCKEY_SWITCH = 7,
    SET_FAST_PREVIEW_CBK = 8,
    GET_VR_ADC_VALUE = 9,

    IR_MODULE_INSTALL = 10,
    IR_MODULE_UNINSTALL = 11,
} key_cmd_e;

#define KEY_ENTRY_ADDR(x)               (KEY_ENTRY_VA_START + (4 * x))

#define KEY_GET_BATTERY                 (*((uint32*)(KEY_ENTRY_ADDR(GET_BATTERY))))
#define KEY_AUX_HOT_DETECT              (*((uint32*)(KEY_ENTRY_ADDR(AUX_HOT_DETECT))))
#define KEY_UBSHOST_HOT_DETECT          (*((uint32*)(KEY_ENTRY_ADDR(USBHOST_HOT_DETECT))))
#define KEY_USBDEV_HOT_DETECT           (*((uint32*)(KEY_ENTRY_ADDR(USBDEV_HOT_DETECT))))
#define KEY_ADAPTOR_HOT_DETECT          (*((uint32*)(KEY_ENTRY_ADDR(ADAPTOR_HOT_DETECT))))
#define KEY_CARD_HOT_DETECT             (*((uint32*)(KEY_ENTRY_ADDR(CARD_HOT_DETECT))))
#define KEY_USB_DETECT_SWITCH           (*((uint32*)(KEY_ENTRY_ADDR(USB_DETECT_SWITCH))))

#define KEY_ADCKEY_SWITCH               (*((uint32*)(KEY_ENTRY_ADDR(ADCKEY_SWITCH))))

#define KEY_SET_FAST_PREVIEW_CBK        (*((uint32*)(KEY_ENTRY_ADDR(SET_FAST_PREVIEW_CBK))))

#define KEY_GET_VR_ADC_VALUE            (*((uint32*)(KEY_ENTRY_ADDR(GET_VR_ADC_VALUE))))

#define IR_MODULE_INSTALL_ENTRY         (*((uint32*)(KEY_ENTRY_ADDR(IR_MODULE_INSTALL))))
#define IR_MODULE_UNINSTALL_ENTRY       (*((uint32*)(KEY_ENTRY_ADDR(IR_MODULE_UNINSTALL))))

#if 0
extern uint8 key_inner_get_battery(void);
#endif
#define key_get_battery()               ((uint8(*)(void))KEY_GET_BATTERY)()


//设备热拔插检测接口，返回是否物理插入，物理插入是相对系统的逻辑插入状态而言的
#if 0
extern bool key_inner_aux_hot_detect(bool chk_in);
#endif
#define key_aux_hot_detect(a)           ((bool(*)(bool))KEY_AUX_HOT_DETECT)((a))

#if 0
extern bool key_inner_usbhost_hot_detect(bool chk_in);
#endif
#define key_usbhost_hot_detect(a)       ((bool(*)(bool))KEY_UBSHOST_HOT_DETECT)((a))

#if 0
extern bool key_inner_usbdev_hot_detect(bool chk_in);
#endif
#define key_usbdev_hot_detect(a)        ((bool(*)(bool))KEY_USBDEV_HOT_DETECT)((a))

#if 0
extern bool key_inner_adaptor_hot_detect(bool chk_in);
#endif
#define key_adaptor_hot_detect(a)       ((bool(*)(bool))KEY_ADAPTOR_HOT_DETECT)((a))

#if 0
extern bool key_inner_card_hot_detect(bool chk_in);
#endif
#define key_card_hot_detect(a)          ((bool(*)(bool))KEY_CARD_HOT_DETECT)((a))

#if 0
extern void key_inner_usb_detect_switch(uint8 det_mode);
#endif
#define key_usb_detect_switch(a)        ((void(*)(uint8))KEY_USB_DETECT_SWITCH)((a))

#if 0
extern void key_inner_adckey_switch(void);
#endif
#define key_adckey_switch()             ((void(*)(void))KEY_ADCKEY_SWITCH)()

typedef void (*fast_preview_cbk)(input_gui_msg_t *input_msg);
#if 0
extern void key_inner_set_fast_preview_cbk(fast_preview_cbk cbk);
#endif
#define key_set_fast_preview_cbk(a)     ((void(*)(fast_preview_cbk))KEY_SET_FAST_PREVIEW_CBK)((a))

#if 0
extern void key_inner_get_vr_adc_value(uint16 *p_adc_value);
#endif
#define key_get_vr_adc_value(a)         ((void(*)(uint16 *))KEY_GET_VR_ADC_VALUE)((a))

#if 0
extern void key_ir_init(irmap_para_t *p_irmap_para);
#endif
#define ir_module_install(a)            ((void(*)(irmap_para_t*))IR_MODULE_INSTALL_ENTRY)((a))

#if 0
extern void key_ir_deinit(void);
#endif
#define ir_module_uninstall()           ((void(*)(void))IR_MODULE_UNINSTALL_ENTRY)()

#endif

