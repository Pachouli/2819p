/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������������ܽӿ���صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_SOUND_H_
#define _COMMON_SOUND_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"


//�����������ӿ�����
#define SET_VOL_BY_REMOTE    0
#define SET_VOL_BY_KEYSHORT  1
#define SET_VOL_BY_KEYHOLD   2
#define SET_VOL_BT_TOOLS     3 //ASET/ASQT
#define SET_VOL_BY_SWITCH    4 //��������绰�����л��Զ����ã��������õ�ǰ��������ͬ�����ֻ�
#define SET_VOL_BY_SYSTEM    5 //ϵͳ�Զ����ã�����ͬ�����ֻ�
#define SET_VOL_BY_PC_VOL    6 //USB����PC����ͬ����С��������100������50��˥��6dB���е���

#define VOLUME_MODE_MAIN     0 //������ģʽ
#define VOLUME_MODE_CALL     1 //��绰����ģʽ

#define VOLUME_GAIN_MAIN     0 //����������
#define VOLUME_GAIN_CALL     1 //��绰��������
#define VOLUME_GAIN_LINEIN   2 //line-in��������
#define VOLUME_GAIN_RADIO    3 //fm ��������������

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

#define SETEQ_TYPE  0 //ָ��EQ���ͣ����õ�DAE MODE
#define SETEQ_NEXT  1 //������EQ�����õ�DAE MODE
#define SETEQ_PREV  2 //������EQ�����õ�DAE MODE
#define USED_EQ_SEG 7 //�Ѿ�ʹ��EQƵ����Ŀ

typedef enum
{
    /*! ��ͨ */
    EQ_NOR = 0,
    /*!��ʿ */
    EQ_JAZZ,
    /*! ����*/
    EQ_POP,
    /*!���� */
    EQ_CLASSIC,
    /*!��� */
    EQ_SOFT,
    /*! �ص���*/
    EQ_DBB,
    /*!ҡ�� */
    EQ_ROCK,
    /*!�û��Զ���EQ*/
    EQ_USR_DEF,
} com_eqtype_e;

#if 0
extern bool com_switch_geq_inner(uint8 mode, com_eqtype_e eq_type, uint8 *eq_para);
#endif
#define com_switch_geq(a,b,c)   ((bool(*)(uint8 , com_eqtype_e , uint8 *))COM_COM_SWITCH_GEQ)((a),(b),(c))

//���ȹ��ܽӿ�
#define SPK_PLAY  0 //���Ź������Ⱥ㶨���
#define SPK_SYS   1 //ϵͳ��ʾ��������ʱ���
#define SPK_MODE_OPTION (0x01) //BIT0��ʾPLAY/SYSѡ��
#define SPK_EN_DCDC   0x40 //��ʹ��DC��ѹ
#define SPK_POWER_OFF 0x80 //�ػ�ѡ��
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

//DAE���ܽӿ�
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
