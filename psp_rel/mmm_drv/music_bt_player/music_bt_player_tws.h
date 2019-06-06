/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������������м���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "music_bt_player_tws_cmd.h"

#ifndef __MUSIC_BT_PLAYER_TWS_H__
#define __MUSIC_BT_PLAYER_TWS_H__

//#define BT_TWS_DEBUG_INFO //TWS Ƶ����ӡ��Debug��Ϣ
//#define BT_TWS_DEBUG_CMD_ID //TWS ��ӡ���ͺͽ�������ID

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

#define TWS_ADJUST_AUDIO_APS_TIMER           (50)  //TWS �����븱�� 50ms ����һ�ζ���΢��
#define TWS_SLAVE_UPDATE_PLAY_INFO_TIMER     (500) //TWS ����500ms����һ��״̬������

#define TWS_SAMPLES_DIFF_MAX                 (10)  //TWS�����䲥�Ų������ֵ���ֵ��������ֵ����Ϊ����״̬���˳����䲥��

#define TWS_IP_FRAME_MAX_COUNT               (300) //֡У��ͼ���ÿ300֡���һ��

#define TWS_START_PLAY_DELAY_CLOCK           (380) //�����븱��Լ��ͬ����ʼ������ʱ��Ҫ��ȷ���� 380*312.5 = 120ms
#define TWS_ADJUST_APS_DELAY_CLOCK           (380) //�����븱��Լ��ͬ������Audio APS��ʱ��Ҫ��ȷ���� 380*312.5 = 120ms

#define TWS_DIFF_MAX_RECENT_COUNT            (1000)//TWS���������30~60��������ֵ���ֵ

typedef struct
{
    /*! TWS��ɫ�����仹�Ǹ��䣻���䲥������Normal */
    dev_role_e dev_role;
    /*! TWS����λ�ã����仹�����䣻���䲥������˫���� */
    dev_spk_pos_e dev_spk;
    /*! ��ȡ����ʱ���������FALSE��ʾTWS��·�ѶϿ� */
    f_get_bt_clk tws_get_bt_clk;
    /*! ����ͬ������������ʱ���������FALSE��ʾTWS��·�ѶϿ� */
    f_set_bt_clk tws_set_bt_clk;
    /*! ����ͬ�������ص����� */
    f_register_bt_clk_sync_cbk tws_reg_bt_clk_sync_cbk;
    /*! �Ƿ���AVDTP���ݰ���ʧ */
    f_has_avdtp_pkg_lost tws_has_avdtp_pkg_lost;

    /* TWS ���� Aduio APS ��ر���*/
    uint32 adjust_audio_aps_timer; //50ms���ڼ�ⶨʱ��
    uint32 slave_update_play_info_timer; //����500ms�ϱ�������Ϣ��ʱ��
    uint32 send_to_dac_anolog_count; //�Ѿ�����DAC anolog���ŵĲ�������Ŀ
    uint32 samples_diff_too_large_timer; //���Ƹ����ʱ���׳���diff�������������ֵ��������Ҫ����1S�����ָܻ�����Ϊ�Ǵ���״̬
    uint16 samples_diff_max_recent_count1; //�Ǵ�����ÿ��50ms��1000��һ��ѭ��
    uint8  samples_diff_max; //������֮����������ƫ��
    uint8  samples_diff_max_recent1; //���30~60����������֮����������ƫ�� 1
    uint8  samples_diff_max_recent2; //���30~60����������֮����������ƫ�� 2����recent1���ʹ�ã�����̶�Ϊ30��
    //�������Audio APS
    bool   need_master_set_audio_aps;
    uint8 master_set_audio_aps;
    bool   slave_is_setting_audio_aps_m; //����Ӧ�����ڵ���Audio APS��������ʱ����Ҫ�жϵ���Clock
    uint32 master_set_audio_aps_bt_clock;
    uint32 slave_must_set_audio_aps_ok_bt_clock_m; //����Ӧ�������ʱ������OK

    //�������Audio APS
    tws_m2s_set_audio_aps_para_t slave_set_audio_aps_para; //�����յ��������Audio APS����
    bool  need_slave_set_audio_aps; //�����յ��������Audio APS������ڵ��ڹ�����
    uint8 slave_set_audio_aps_state; //0��ʾ��ʼ״̬����ɵ��������1��ʾ���ڵ��߻����һ��ʱ��
    uint8 slave_set_audio_aps_restore; //������߻����һ��ʱ���ָ���Audio APS
    uint32 slave_set_audio_aps_start_bt_clk; //���俪ʼ���߻����ʱ��㣬Ȼ���ʱһ��ʱ��ָ�Audio APS

    /* ͬ��������� */
    buffer_rw_t *p_tws_mmm_m2s_pipe;
    buffer_rw_t *p_tws_mmm_s2m_pipe;
    tws_sync_cmd_t *p_tws_mmm_m2s_play_sync_cmd;

    /* ͬ��������ر��� */
    os_event_t *start_dec_sem;
    volatile bool tws_need_start_sync_play;
    volatile bool tws_need_set_sync_stop;
    bt_clock_t start_con_clock; //����������ʼ��DAC��ʱ���
    bt_clock_t should_start_con_clock; //����ָ�����俪ʼ����ʱ���

    tws_play_info_t tws_slave_play_info; //master �����յ� slave �� play_info
    tws_ip_frame_info_t tws_slave_ip_frame_info; //master �����յ� slave �� ip_frame_info

    tws_play_info_t tws_play_info; //SLAVE USED TO REPORT MASTER
    tws_ip_frame_info_t tws_ip_frame_info; //SLAVE USED TO REPORT MASTER

    /* TWS ��ֹ��ͷ��֡������֡У��� */
    uint16 ip_frame_id_count;
    uint16 ip_frame_cks;
    uint16 ip_frame_id_cks;
    bool   need_update_ip_frame_cks;

    bool   tws_link_connect_flag; //TWS��·�Ƿ������ţ�ͨ�� tws_get_bt_clk ����ֵ����

    bool   tws_slave_frame_lost_flag; //TWS���䶪����־���ᷴ�������䣬�������������˳�����
    uint8  tws_master_lost_pkg_count; //TWS��������ֻ�������������������������������࣬Ӧ�ñ���
    uint32 tws_start_play_timer; //TWS��ʼ����ʱ���
    uint32 tws_slave_frame_lost_timer; //TWS���俪ʼ����ʱ��������ڿ���1S�����Լ��˳�����
} music_bt_player_tws_info_t;

#endif

#endif//__MUSIC_BT_PLAYER_TWS_H__
