/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TTS ������صĺ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_TTS_H_
#define _COMMON_TTS_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "mmm_mp.h"
#include "common_tts_id.h"

#define TTS_MODE_NORECV_ALL_EV_MASK (1 << 0)  //�����պʹ��������¼�ѡ�����ػ�ʱTTS
#define TTS_MODE_NORECV_ALL_EV_NO   (0 << 0)  //���պʹ��������¼�
#define TTS_MODE_NORECV_ALL_EV_YES  (1 << 0)  //�����պʹ��������¼�������TTS

#define TTS_MODE_SYS_OPTION_MASK    (3 << 1)  //TTS��ϵͳ��Ϣѡ��
#define TTS_MODE_SYS_BREAK_COND     (0 << 1)  //ֻ���ڻᷢ��Ӧ���л�ʱ����ֹTTS����Ӧ
#define TTS_MODE_SYS_BREAK          (1 << 1)  //��ֹTTS����Ӧ
#define TTS_MODE_SYS_NORECV         (2 << 1)  //����ȡϵͳ��Ϣ

#define TTS_MODE_KEY_OPTION_MASK    (3 << 3)  //TTS�а���ѡ��
#define TTS_MODE_KEY_BREAK_REDEAL   (0 << 3)  //��ֹTTS����Ӧ
#define TTS_MODE_KEY_BREAK_FILTER   (1 << 3)  //��ֹTTS�ҹ���
#define TTS_MODE_KEY_FILTER         (2 << 3)  //ֱ�ӹ���
#define TTS_MODE_KEY_NORECV         (3 << 3)  //����ȡ������Ϣ

#define TTS_MODE_CONTROL_TTS_MASK   (1 << 8)  //������TTS��־
#define TTS_MODE_CONTROL_TTS_NO     (0 << 8)
#define TTS_MODE_CONTROL_TTS_YES    (1 << 8)

typedef enum
{
    TTS_PLAY_RET_NORMAL = 0,
    TTS_PLAY_RET_BY_KEY = 1,
    TTS_PLAY_RET_BY_SYS = 2,
    TTS_PLAY_RET_NO_PLAY = 3, //��������򱻹��˵��˻�����������
} tts_play_ret_e;

//״̬��������
#if 0
extern tts_play_ret_e com_tts_state_play_inner(uint8 tts_id, uint16 mode);
#endif
#define com_tts_state_play(a,b)          ((tts_play_ret_e(*)(uint8 , uint16))COM_COM_TTS_STATE_PLAY)((a),(b))

#if 0
extern tts_play_ret_e com_tts_state_play_list_inner(uint8 *tts_list, uint8 section_cnt, uint16 mode);
#endif
#define com_tts_state_play_list(a,b,c)   ((tts_play_ret_e(*)(uint8 *, uint8, uint16))COM_COM_TTS_STATE_PLAY_LIST)((a),(b),(c))

typedef void (*tts_play_background_hook)(void);
#if 0
extern tts_play_ret_e com_tts_state_play_ext_inner(uint8 tts_id, uint16 mode, tts_play_background_hook bg_hook);
#endif
#define com_tts_state_play_ext(a,b,c)    ((tts_play_ret_e(*)(uint8 , uint16, tts_play_background_hook))COM_COM_TTS_STATE_PLAY_EXT)((a),(b),(c))

typedef enum
{
    TTS_CTRL_IGNORE = 0,
    TTS_CTRL_CANCEL_IGNORE = 1,
    TTS_CTRL_FORCE_STOP = 2,
    TTS_CTRL_CANCEL_FORCE_STOP = 3,
    TTS_CTRL_ENABLE_SWITCH = 4,
    TTS_CTRL_LANGUAGE_SWITCH = 5,
} tts_control_opr_e;

#if 0
extern void com_tts_state_play_control_inner(tts_control_opr_e opr);
#endif
#define com_tts_state_play_control(a)     ((void(*)(tts_control_opr_e))COM_COM_TTS_STATE_PLAY_CONTROL)((a))

#if 0
extern void * com_sd_mp3_player_play_tts_inner(uint8 * sec_ids, uint8 sec_cnt);
#endif
#define com_sd_mp3_player_play_tts(a,b)   ((void *(*)(uint8 * , uint8))COM_COM_SD_MP3_PLAYER_PLAY_TTS)((a),(b))

#if 0
extern void * com_sd_mp3_player_play_mp3_inner(uint8 * mp3_name);
#endif
#define com_sd_mp3_player_play_mp3(a)     ((void *(*)(uint8 * ))COM_COM_SD_MP3_PLAYER_PLAY_MP3)((a))

#if 0
extern bool com_sd_mp3_player_stop_inner(void *player_handle);
#endif
#define com_sd_mp3_player_stop(a)         ((bool(*)(void * ))COM_COM_SD_MP3_PLAYER_STOP)((a))

#if 0
extern bool com_sd_mp3_player_is_stop_inner(void *player_handle);
#endif
#define com_sd_mp3_player_is_stop(a)      ((bool(*)(void * ))COM_COM_SD_MP3_PLAYER_IS_STOP)((a))

#endif
