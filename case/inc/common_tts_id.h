/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TTS 资源ID号定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_TTS_ID_H_
#define _COMMON_TTS_ID_H_

#include "psp_includes.h"

typedef enum
{
    TTS_ENTER_BT,          //进入蓝牙
    TTS_WAIT_BT_CON,       //等待连接
    TTS_BT_CON_OK,         //蓝牙连接成功
    TTS_BT_DISCON,         //蓝牙断开
    TTS_PLAY_SONG,         //播放
    TTS_PAUSE_SONG,        //暂停
    TTS_LAST_SONG,         //上一曲
    TTS_NEXT_SONG,         //下一曲
    TTS_PLAY_SDCARD,       //卡播放
    TTS_PLAY_UHOST,        //uhost播放
    TTS_PLAY_MDISK,        //主盘播放
    TTS_ENTER_RADIO,       //收音机
    TTS_PLAY_LINEIN,       //音频输入
    TTS_PLAY_SPDIF,        //光纤输入
    TTS_ENTER_USBBOX,      //USB音箱
    TTS_ENTER_UDISK,       //进入U盘
    TTS_ENTER_PHOTO,       //进入拍照
    TTS_BAT_LOW_CHARGE,    //电量低请充电
    TTS_BATTERY_LOW,       //电量不足
    TTS_POWERON,           //开机
    TTS_POWEROFF,          //关机
    TTS_SECTION_NO,        //第
    TTS_SECTION_NUM_0,     //零
    TTS_SECTION_NUM_1,     //一
    TTS_SECTION_NUM_2,     //二
    TTS_SECTION_NUM_3,     //三
    TTS_SECTION_NUM_4,     //四
    TTS_SECTION_NUM_5,     //五
    TTS_SECTION_NUM_6,     //六
    TTS_SECTION_NUM_7,     //七
    TTS_SECTION_NUM_8,     //八
    TTS_SECTION_NUM_9,     //九
    TTS_SECTION_TEN,       //十
    TTS_SECTION_HUNDRED,   //百
    TTS_SECTION_THOUSAND,  //千
    TTS_SECTION_DOT,       //点
    TTS_SECTION_SONG,      //首
    TTS_SECTION_CHAR_FM,   //FM
    TTS_WAIT_BT_PAIR,      //等待组对
    TTS_PAIR_SUCCEED,      //组对成功
    TTS_PAIR_DISCON,       //组队断开
    TTS_RECORD,            //录音
    TTS_REPLAY,            //回放
    TTS_PLAY_HDMI,         //高清输入
    TTS_TWS_LEFT_BOX,      //左箱
    TTS_TWS_RIGHT_BOX,     //右箱
    TTS_CARD_RECORD,       //卡录音
    TTS_UHOST_RECORD,      //U盘录音
    TTS_CARD_REPLAY,       //卡回放
    TTS_UHOST_REPLAY,      //U盘回放
    TTS_SENTENCE_MAX,
} tts_id_e;

#endif
