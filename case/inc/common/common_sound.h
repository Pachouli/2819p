/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：声音功能接口相关的宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_SOUND_H_
#define _COMMON_SOUND_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"


//声音输出管理接口声明
#define SET_VOL_BY_REMOTE    0
#define SET_VOL_BY_KEYSHORT  1
#define SET_VOL_BY_KEYHOLD   2
#define SET_VOL_BT_TOOLS     3 //ASET/ASQT
#define SET_VOL_BY_SWITCH    4 //主音量与电话音量切换自动设置，或者重置当前音量，需同步到手机
#define SET_VOL_BY_SYSTEM    5 //系统自动设置，无需同步到手机
#define SET_VOL_BY_PC_VOL    6 //USB音箱PC音量同步到小机，按照100级调到50级衰减6dB进行调节

#define VOLUME_MODE_MAIN     0 //主音量模式
#define VOLUME_MODE_CALL     1 //打电话音量模式

#define VOLUME_GAIN_MAIN     0 //主音量增益
#define VOLUME_GAIN_CALL     1 //打电话音量增益
#define VOLUME_GAIN_LINEIN   2 //line-in音量增益
#define VOLUME_GAIN_RADIO    3 //fm 收音机音量增益

#if 0
extern uint8 com_get_sound_volume_inner(void);
#endif
#define com_get_sound_volume()          ((uint8(*)(void))COM_COM_GET_SOUND_VOLUME)()

#if 0
extern uint8 com_get_sound_volume_pc_inner(void);
#endif
#define com_get_sound_volume_pc()       ((uint8(*)(void))COM_COM_GET_SOUND_VOLUME_PC)()

#if 0
extern bool com_reset_sound_volume_inner(uint8 mode);
#endif
#define com_reset_sound_volume(a)       ((bool(*)(uint8))COM_COM_RESET_SOUND_VOLUME)((a))

#if 0
extern bool com_set_sound_volume_inner(uint8 mode, uint8 volume);
#endif
#define com_set_sound_volume(a,b)       ((bool(*)(uint8, uint8))COM_COM_SET_SOUND_VOLUME)((a),(b))

#if 0
extern void com_set_sound_volume_mode_inner(uint8 volume_mode, uint8 gain_mode);
#endif
#define com_set_sound_volume_mode(a,b)  ((void(*)(uint8, uint8))COM_COM_SET_SOUND_VOLUME_MODE)((a),(b))

#if 0
extern void com_set_sound_volume_gain_inner(int16 gain);
#endif
#define com_set_sound_volume_gain(a)    ((void(*)(int16))COM_COM_SET_SOUND_VOLUME_GAIN)((a))

#if 0
extern void com_volume_add_inner(uint8 mode);
#endif
#define com_volume_add(a)       ((void(*)(uint8))COM_COM_VOLUME_ADD)((a))

#if 0
extern void com_volume_sub_inner(uint8 mode);
#endif
#define com_volume_sub(a)       ((void(*)(uint8))COM_COM_VOLUME_SUB)((a))

#if 0
extern void com_volume_set_inner(uint8 mode, uint8 set_vol);
#endif
#define com_volume_set(a,b)     ((void(*)(uint8, uint8))COM_COM_VOLUME_SET)((a),(b))

#if 0
extern bool com_set_mute_inner(bool mute);
#endif
#define com_set_mute(a)         ((bool(*)(bool))COM_COM_SET_MUTE)((a))

#if 0
extern bool com_set_mute_toggle_inner(void);
#endif
#define com_set_mute_toggle()   ((bool(*)(void))COM_COM_SET_MUTE_TOGGLE)()

#define USBBOX_VOL_CTRL_ENTER   0
#define USBBOX_VOL_CTRL_RESET   1
#define USBBOX_VOL_CTRL_EXIT    2
#if 0
extern void com_set_usbbox_volume_ctrl_inner(uint8 ctrl_mode);
#endif
#define com_set_usbbox_volume_ctrl(a)   ((void(*)(uint8))COM_COM_SET_USBBOX_VOLUME_CTRL)((a))

#define SETEQ_TYPE  0 //指定EQ类型，沿用到DAE MODE
#define SETEQ_NEXT  1 //向下切EQ，沿用到DAE MODE
#define SETEQ_PREV  2 //向上切EQ，沿用到DAE MODE
#define USED_EQ_SEG 7 //已经使用EQ频段数目

typedef enum
{
    /*! 普通 */
    EQ_NOR = 0,
    /*!爵士 */
    EQ_JAZZ,
    /*! 流行*/
    EQ_POP,
    /*!经典 */
    EQ_CLASSIC,
    /*!柔和 */
    EQ_SOFT,
    /*! 重低音*/
    EQ_DBB,
    /*!摇滚 */
    EQ_ROCK,
    /*!用户自定义EQ*/
    EQ_USR_DEF,
} com_eqtype_e;

#if 0
extern bool com_switch_geq_inner(uint8 mode, com_eqtype_e eq_type, uint8 *eq_para);
#endif
#define com_switch_geq(a,b,c)   ((bool(*)(uint8 , com_eqtype_e , uint8 *))COM_COM_SWITCH_GEQ)((a),(b),(c))

//喇叭功能接口
#define SPK_PLAY  0 //播放功能喇叭恒定输出
#define SPK_SYS   1 //系统提示音喇叭临时输出
#define SPK_MODE_OPTION (0x01) //BIT0表示PLAY/SYS选择
#define SPK_EN_DCDC   0x40 //仅使能DC升压
#define SPK_POWER_OFF 0x80 //关机选项
#if 0
extern bool is_speaker_on_inner(void);
#endif
#define is_speaker_on()     ((bool(*)(void))COM_IS_SPEAKER_ON)()

#if 0
extern void com_speaker_on_inner(uint8 mode);
#endif
#define com_speaker_on(a)   ((void(*)(uint8))COM_COM_SPEAKER_ON)((a))

#if 0
extern void com_speaker_off_inner(uint8 mode);
#endif
#define com_speaker_off(a)  ((void(*)(uint8))COM_COM_SPEAKER_OFF)((a))

#define PA_SWITCH_D   0
#define PA_SWITCH_AB  1
#if 0
extern void com_speaker_ab_d_switch_inner(uint8 ab_d);
#endif
#define com_speaker_ab_d_switch(a)  ((void(*)(uint8))COM_COM_SPEAKER_AB_D_SWITCH)((a))

//DAE功能接口
#if 0
extern bool com_set_dae_onoff_inner(bool onoff);
#endif
#define com_set_dae_onoff(a)        ((bool(*)(bool))COM_COM_SET_DAE_ONOFF)((a))

#if 0
extern bool com_set_drc_onoff_inner(bool onoff);
#endif
#define com_set_drc_onoff(a)        ((bool(*)(bool))COM_COM_SET_DRC_ONOFF)((a))

#if 0
extern uint8 com_switch_drc_mode_inner(uint8 mode);
#endif
#define DRC_MODE_NORMAL  0
#define DRC_MODE_AUX     1
#define com_switch_drc_mode(a)      ((uint8(*)(uint8))COM_COM_SWITCH_DRC_MODE)((a))

#if 0
extern bool com_set_speech_peq_para_inner(bool in);
#endif
#define com_set_speech_peq_para(a)  ((bool(*)(bool))COM_COM_SET_SPEECH_PEQ_PARA)((a))

#if 0
extern bool com_update_speech_peq_para_inner(bool peq_enable, peq_band_t * peq_array);
#endif
#define com_update_speech_peq_para(a,b) ((bool(*)(bool, peq_band_t *))COM_COM_UPDATE_SPEECH_PEQ_PARA)((a),(b))

#endif
