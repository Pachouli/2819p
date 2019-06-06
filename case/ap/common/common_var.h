/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：系统环境变量结构体定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_VAR_H
#define _COMMON_VAR_H

#include "psp_includes.h"

/*!
 * \brief
 *  comval_t 系统公共变量组合结构体
 */
typedef struct
{
    /*! 支持卡选择  0:不支持  1:支持 */
    uint8 support_cardplay;
    /*!支持linein选择 0:不支持 1:支持*/
    uint8 support_linein;
    /*! 支持FM选择  0:不支持  1:支持 */
    uint8 support_fmradio;
    /*! 支持后台蓝牙 0:不支持  1:支持 */
    uint8 support_bt_inback;
    /*! 支持蓝牙推歌 0:不支持  1:支持 */
    uint8 support_btplay;
    /*! 支持蓝牙免提 0:不支持  1:支持 */
    uint8 support_btcall;
    /*! 支持USB音箱 0:不支持  1:支持 */
    uint8 support_usbbox;
    /*! 支持大容量磁盘 0:不支持  1:支持 */
    uint8 support_usbmsc;
    /*!支持USB播放 UHOST 0:不支持 1:支持*/
    uint8 support_uhostplay;
    /*! 硬件层配置项 */
    uint8 hard_support_card;
    /*! 是否支持USB口给IPhone充电 */
    uint8 support_iphonecharge;

    /*! 是否支持LED灯显示 */
    uint8 support_led_light;
    /*! 是否支持LED数码管显示 */
    uint8 support_led_segment;
    /*! 是否支持红外遥控输入 */
    uint8 support_ir_key;
    /*! 是否支持外部功放输出，含DCDC升压控制 */
    uint8 support_extern_pa;
    /*! 是否支持LINEIN热拔插检测 */
    uint8 support_aux_detect;
    /*! 是否支持DC5V 热拔插检测 */
    uint8 support_dc5v_detect;
    /*! USB Device检测是否依赖DC5V检测 */
    uint8 is_udev_det_base_dc5v_det;
    /*! 是否支持电池电量检测 */
    uint8 support_bat_lradc;
} comval_t;

typedef struct
{
    /*! magic，值为 0x55AA 时有效 */
    uint16 magic;

    /*! 当前主音量 */
    uint8 main_volume;
    /*! 当前电话音量 */
    uint8 call_volume;
    /*! 是否支持 Graphics EQ 功能 */
    uint8 eq_enable;
    /*! Graphics EQ 类型 */
    uint8 eq_type;

    /*! 对箱角色记忆 */
    dev_role_e dev_role;
    /*! 对箱箱体左右声道记忆 */
    dev_spk_pos_e spk_pos;

#if (SUPPORT_FM_TX == 1)
    /*! FM发射频率 */
    uint16 fm_tx_freq;
#endif

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    /*! LCD对比度 */
    uint8 contrast;
#endif
} env_var_t;

#endif //_COMMON_VAR_H
