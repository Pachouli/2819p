/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：按键消息逻辑按键枚举类型。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _KEY_MSG_VALUE_H
#define _KEY_MSG_VALUE_H

#include "psp_includes.h"
#include "vr_adckey_msg_value.h"

/*!
 * \brief
 *  key_value_e 逻辑按键枚举类型
 */
typedef enum
{
    /*! 空按键 */
    KEY_NULL = 0x00,
    /*! 播放/暂停按键 */
    KEY_PLAY = 0x01,
    /*! 上一项（左）按键 */
    KEY_PREV = 0x02,
    /*! 下一项（右）按键 */
    KEY_NEXT = 0x03,
    /*! 音量调节快捷按键 */
    KEY_VOL = 0x04,
    /*! 音量加快捷按键 */
    KEY_VADD = 0x05,
    /*! 音量减快捷按键 */
    KEY_VSUB = 0x06,
    /*! 菜单/选项按键 */
    KEY_MODE = 0x07,
    /*! 连接/断开连接按键 */
    KEY_CONNECT = 0x08,
    /*! 功能按键 */
    KEY_F1 = 0x09,
    /*! mute按键 */
    KEY_MUTE = 0x0a,
    /*!开关机按键*/
    KEY_POWER = 0xb,
    /*! folder+按键 */
    KEY_FOLDER_ADD = 0x0c,
    /*! folder-按键 */
    KEY_FOLDER_SUB = 0x0d,
    /*!next_vol_add复用按键*/
    KEY_NEXT_VOLADD = 0x0e,
    /*!prev_vol_sub复用按键*/
    KEY_PREV_VOLSUB = 0x0f,
    /*! 数字0按键 */
    KEY_NUM0 = 0x10,
    /*! 数字1按键 */
    KEY_NUM1 = 0x11,
    /*! 数字2按键 */
    KEY_NUM2 = 0x12,
    /*! 数字3按键 */
    KEY_NUM3 = 0x13,
    /*! 数字4按键 */
    KEY_NUM4 = 0x14,
    /*! 数字5按键 */
    KEY_NUM5 = 0x15,
    /*! 数字6按键 */
    KEY_NUM6 = 0x16,
    /*! 数字7按键 */
    KEY_NUM7 = 0x17,
    /*! 数字8按键 */
    KEY_NUM8 = 0x18,
    /*! 数字9按键 */
    KEY_NUM9 = 0x19,
    /*! FM频点微调加 */
    KEY_CHADD = 0x1a,
    /*! FM频点微调减 */
    KEY_CHSUB = 0x1b,

    /*! K歌宝专用：短按混响+，长按下一曲 */
    KEY_DEPTH_ADD_NEXT = 0x1c,
    /*! K歌宝专用：短按混响-，长按上一曲 */
    KEY_DEPTH_SUB_PREV = 0x1d,
    /*! K歌宝专用：短按MIC音量+，长按伴唱音量+ */
    KEY_MIC_ADD_VADD = 0x1e,
    /*! K歌宝专用：短按MIC音量-，长按伴唱音量- */
    KEY_MIC_SUB_VSUB = 0x1f,

    /*! 旋转按钮顺时针加 */
    KEY_ROTARY_ADD = 0x20,
    /*! 旋转按钮逆时针减 */
    KEY_ROTARY_SUB = 0x21,

    /*! 话筒优先开关 */
    KEY_DUCKING_SWITCH = 0x22,
    /*! 原音消除开关 */
    KEY_REMOVAL_SWITCH = 0x23,
    /*! 录音开关 */
    KEY_RECORD_SWITCH = 0x24,
    /*! 录音回放 */
    KEY_REPLAY = 0x25,

    KEY_VALUE_MAX,

    /*! 开发板多了一个ADFU按键，需要过滤掉 */
    KEY_ADFU_NO_USE = 0x40,
    /*! 使能快速预览模式时，切换应用按键会被转换为DUMMY键，实现一个按键应该做的事情，而切换应用的事情则由 fast preview 通过系统消息来达成 */
    KEY_DUMMY = 0x41,

    /*! 泛指数字按键 */
    KEY_NUMBER = 0x7e,
    /*! 虚拟按键，用在用户按键映射表中取代 KEY_NULL 放在最后一项，表示不再使用快捷键映射表，
     这种用法可以在嵌套子场景中使用，避免嵌套层次加深 */
    KEY_NULL_NO_SHORTCUT = 0x7f,
} key_value_e;


#ifdef USE_KEY_NAME
const uint8 key_name_list[KEY_VALUE_MAX][8] =
{
    "NULL",
    "PLAY",
    "PREV",
    "NEXT",
    "VOL",
    "VOL+",
    "VOL-",
    "MODE",
    "LINK",
    "F1",
    "MUTE",
    "POWER",
    "FADD",
    "FSUB",
    "NEXT/V+",
    "PREV/V-",
    "NUM0",
    "NUM1",
    "NUM2",
    "NUM3",
    "NUM4",
    "NUM5",
    "NUM6",
    "NUM7",
    "NUM8",
    "NUM9",
    "CHADD",
    "CHSUB",
    "D+/NEXT",
    "D-/PREV",
    "M+/VADD",
    "M-/VSUB",
    "RADD",
    "RSUB",
    "DUCKING",
    "REMOVAL",
    "RECORD",
    "REPLAY",
};
#endif

#endif

