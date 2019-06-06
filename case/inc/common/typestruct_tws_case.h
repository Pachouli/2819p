/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 真立体声-方案相关数据和函数声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __TYPESTRUCT_TWS_CASE_H__
#define __TYPESTRUCT_TWS_CASE_H__

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

/*【单箱】表示在单箱应用模式下允许进行的命令或事件；其他命令或事件则不允许在单箱应用模式下进行 */

/* TWS UI SYNC CMD */
#define TWS_SYNC_S2M_UI_SYNC_REPLY         0x01 //回复主箱的同步命令
#define TWS_SYNC_S2M_UI_UPDATE_BAT_EV      0x02 //上报电池电量
#define TWS_SYNC_S2M_UI_UPDATE_KEY_EV      0x03 //上报按键动作
#define TWS_SYNC_S2M_UI_PLAY_TTS_REQ       0x04 //请求播报TTS

#define TWS_SYNC_S2M_UI_UPDATE_BTPLAY_EV   0x05 //【单箱】副箱上报进入或退出BTPLAY应用，刚组队需要上报，后面切换时要上报，
                                                //如果500ms内没有收到主箱确定需要再次上传，直到收到为止

#define TWS_SYNC_M2S_UI_SET_VOL_VAL_CMD    0x81 //设置音量
#define TWS_SYNC_M2S_UI_SET_VOL_LIMIT_CMD  0x82 //设置音量限制
#define TWS_SYNC_M2S_UI_SWITCH_POS_CMD     0x83 //左右声道切换命令
#define TWS_SYNC_M2S_UI_POWEROFF_CMD       0x84 //关机
#define TWS_SYNC_M2S_UI_TTS_PLAY_REQ       0x85 //请求副箱TTS播报
#define TWS_SYNC_M2S_UI_TTS_STA_REQ        0x86 //查询副箱TTS播报状态
#define TWS_SYNC_M2S_UI_TTS_STOP_REQ       0x87 //请求副箱TTS停止播报

#define TWS_SYNC_M2S_SWITCH_NEXT_APP_CMD   0x88 //通知副箱切换到下一个应用，一般是通过Mode键切换

#define TWS_SYNC_M2S_UI_SWITCH_TWS_MODE_CMD   0x90 //【单箱】切换TWS模式，对箱应用模式和单箱应用模式
#define TWS_SYNC_M2S_UI_ACK_UPDATE_BTPLAY     0x91 //【单箱】主箱确定收到副箱上报进入或退出BTPLAY应用的事件

typedef enum
{
    TWS_SYNC_REPLY_ACK = 0,  //同步消息通用应答 ACK
    TWS_SYNC_REPLY_NACK = 1, //同步消息通用应答 NACK
} tws_sync_reply_e;

typedef enum
{
    TWS_SLAVE_STA_STOP,
    TWS_SLAVE_STA_PLAYING,
    TWS_SLAVE_STA_UNKNOWN,
} tws_slave_tts_state_e;


//公共接口声明
#if 0
extern dev_role_e get_tws_role_inner(void);
#endif
#define get_tws_role()                        ((dev_role_e(*)(void))COM_GET_TWS_ROLE)()

#if 0
extern void change_tws_role_inner(dev_role_e tws_role);
#endif
#define change_tws_role(a)                    ((void(*)(dev_role_e))COM_CHANGE_TWS_ROLE)((a))

#if 0
extern dev_spk_pos_e get_tws_spk_pos_inner(void);
#endif
#define get_tws_spk_pos()                     ((dev_spk_pos_e(*)(void))COM_GET_TWS_SPK_POS)()

#if 0
extern void change_tws_spk_pos_inner(dev_spk_pos_e tws_spk);
#endif
#define change_tws_spk_pos(a)                 ((void(*)(dev_spk_pos_e))COM_CHANGE_TWS_SPK_POS)((a))

#if 0
extern bool write_tws_cmd_to_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *w_buf, uint16 w_len);
#endif
#define write_tws_cmd_to_buffer_rw(a,b,c)     ((bool(*)(buffer_rw_t *, void *, uint16))COM_WRITE_TWS_CMD_TO_BUFFER_RW)((a),(b),(c))

#if 0
extern bool read_tws_cmd_from_buffer_rw_inner(buffer_rw_t *p_buf_rw, void *r_buf, uint16 r_len);
#endif
#define read_tws_cmd_from_buffer_rw(a,b,c)    ((bool(*)(buffer_rw_t *, void *, uint16))COM_READ_TWS_CMD_FROM_BUFFER_RW)((a),(b),(c))

#if 0
extern bool com_tws_m2s_switch_pos_cmd_inner(dev_spk_pos_e pos);
#endif
#define com_tws_m2s_switch_pos_cmd(a)         ((bool(*)(dev_spk_pos_e))COM_COM_TWS_M2S_SWITCH_POS_CMD)((a))

#if 0
extern bool com_tws_m2s_switch_tws_mode_cmd_inner(tws_mode_e tws_mode);
#endif
#define com_tws_m2s_switch_tws_mode_cmd(a)    ((bool(*)(tws_mode_e))COM_COM_TWS_M2S_SWITCH_TWS_MODE_CMD)((a))

#if 0
extern bool com_tws_s2m_tts_play_req_inner(uint8 tts_id);
#endif
#define com_tws_s2m_tts_play_req(a)           ((bool(*)(uint8))COM_COM_TWS_S2M_TTS_PLAY_REQ)((a))

#if 0
extern bool com_tws_slave_update_btplay_inner(bool is_btplay);
#endif
#define com_tws_slave_update_btplay(a)        ((bool(*)(bool))COM_COM_TWS_SLAVE_UPDATE_BTPLAY)((a))

#if 0
extern void com_tts_state_play_tws_slave_inner(uint8 tts_id, uint16 mode);
#endif
#define com_tts_state_play_tws_slave(a,b)     ((void(*)(uint8, uint16))COM_COM_TTS_STATE_PLAY_TWS_SLAVE)((a),(b))

#endif //__TYPESTRUCT_TWS_CASE_H__
