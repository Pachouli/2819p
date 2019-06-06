/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TWS ��������-����������ݺͺ��������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __TYPESTRUCT_TWS_CASE_H__
#define __TYPESTRUCT_TWS_CASE_H__

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

/*�����䡿��ʾ�ڵ���Ӧ��ģʽ��������е�������¼�������������¼��������ڵ���Ӧ��ģʽ�½��� */

/* TWS UI SYNC CMD */
#define TWS_SYNC_S2M_UI_SYNC_REPLY         0x01 //�ظ������ͬ������
#define TWS_SYNC_S2M_UI_UPDATE_BAT_EV      0x02 //�ϱ���ص���
#define TWS_SYNC_S2M_UI_UPDATE_KEY_EV      0x03 //�ϱ���������
#define TWS_SYNC_S2M_UI_PLAY_TTS_REQ       0x04 //���󲥱�TTS

#define TWS_SYNC_S2M_UI_UPDATE_BTPLAY_EV   0x05 //�����䡿�����ϱ�������˳�BTPLAYӦ�ã��������Ҫ�ϱ��������л�ʱҪ�ϱ���
                                                //���500ms��û���յ�����ȷ����Ҫ�ٴ��ϴ���ֱ���յ�Ϊֹ

#define TWS_SYNC_M2S_UI_SET_VOL_VAL_CMD    0x81 //��������
#define TWS_SYNC_M2S_UI_SET_VOL_LIMIT_CMD  0x82 //������������
#define TWS_SYNC_M2S_UI_SWITCH_POS_CMD     0x83 //���������л�����
#define TWS_SYNC_M2S_UI_POWEROFF_CMD       0x84 //�ػ�
#define TWS_SYNC_M2S_UI_TTS_PLAY_REQ       0x85 //������TTS����
#define TWS_SYNC_M2S_UI_TTS_STA_REQ        0x86 //��ѯ����TTS����״̬
#define TWS_SYNC_M2S_UI_TTS_STOP_REQ       0x87 //������TTSֹͣ����

#define TWS_SYNC_M2S_SWITCH_NEXT_APP_CMD   0x88 //֪ͨ�����л�����һ��Ӧ�ã�һ����ͨ��Mode���л�

#define TWS_SYNC_M2S_UI_SWITCH_TWS_MODE_CMD   0x90 //�����䡿�л�TWSģʽ������Ӧ��ģʽ�͵���Ӧ��ģʽ
#define TWS_SYNC_M2S_UI_ACK_UPDATE_BTPLAY     0x91 //�����䡿����ȷ���յ������ϱ�������˳�BTPLAYӦ�õ��¼�

typedef enum
{
    TWS_SYNC_REPLY_ACK = 0,  //ͬ����Ϣͨ��Ӧ�� ACK
    TWS_SYNC_REPLY_NACK = 1, //ͬ����Ϣͨ��Ӧ�� NACK
} tws_sync_reply_e;

typedef enum
{
    TWS_SLAVE_STA_STOP,
    TWS_SLAVE_STA_PLAYING,
    TWS_SLAVE_STA_UNKNOWN,
} tws_slave_tts_state_e;


//�����ӿ�����
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
