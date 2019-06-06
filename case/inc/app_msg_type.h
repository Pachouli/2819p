/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ü���Ϣ����ö�����͡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _APP_MSG_TYPE_H_
#define _APP_MSG_TYPE_H_

#include "psp_includes.h"

/*!
 * \brief
 *  msg_apps_type_e Ӧ����Ϣö������
 */
typedef enum
{
    /*! ����Ϣ */
    MSG_NULL = 0x0000,

    /****************************** shortcut�¼� ****************************/
    /*! ��ݼ�ռλ���������ռ� 0x0020 ~ 0x003f */
    SHORTCUT_RESERVE_BEGIN = 0x001f,
    /*! MUTE �����л� */
    EVENT_SWITCH_MUTE,
    /*! ���� PLAY ���ػ��¼� */
    EVENT_ENTER_SHUTOFF_PLAY,
    /*! ���� PLAY �������ӹػ��¼� */
    EVENT_ENTER_SHUTOFF_PLAY_SEC,
    /*! �̰� POWER ���ػ��¼� */
    EVENT_ENTER_SHUTOFF,

    /*! ������1�¼� */
    EVENT_VOLUME_ADD,
    EVENT_VOLUME_ADD_HOLD,
    EVENT_VOLUME_ADD_NEXT,
    /*! ������1�¼� */
    EVENT_VOLUME_SUB,
    EVENT_VOLUME_SUB_HOLD,
    EVENT_VOLUME_SUB_PREV,

    /*!TTS�����л��¼�*/
    EVENT_TTS_LANGUAGE_SWITCH,
    /*!TTS�����л�*/
    EVENT_TTS_ENABLE_SWITCH,

    /*!���������Ƹ�*/
    EVENT_ENTER_BTPLAY,
    /*!����MUSIC����*/
    EVENT_ENTER_MUSIC,
    /*!����LINEIN����*/
    EVENT_ENTER_LINEIN,
    /*!����fm����*/
    EVENT_ENTER_RADIO,
    /*!����忨����*/
    EVENT_ENTER_MUSIC_CARD,
    /*!����U�̲���*/
    EVENT_ENTER_MUSIC_UHOST,
    /*!���뱾�ش�������Ŀ¼*/
    EVENT_ENTER_FOLDER1,
    EVENT_ENTER_FOLDER2,

    EVENT_ENTER_CARD_REC,
    EVENT_ENTER_UHOST_REC,
    EVENT_ENTER_CARD_REPLAY,
    EVENT_ENTER_UHOST_REPLAY,
    EVENT_ENTER_REPLAY,

    EVENT_ENTER_RTC_TIME,
    EVENT_EXIT_RTC_TIME,
    EVENT_EXIT_RTC_TIME_HOLD,
    EVENT_SWITCH_RTC_SCENE,
    EVENT_RTC_VALUE_ADD,
    EVENT_RTC_VALUE_SUB,
    EVENT_RTC_VALUE_CONFORM,
    EVENT_ALARM_CLOSE,
    EVENT_ALARM_SNOOZE,
    EVENT_NUMBER_INPUT,

    EVENT_UI_VALUE_ADD,
    EVENT_UI_VALUE_SUB,
    EVENT_UI_CONFORM,
    EVENT_UI_CANCEL,

    EVENT_ENTER_MENU,

    EVENT_DUCKING_SWITCH,
    EVENT_REMOVAL_SWITCH,
    EVENT_RECORD_SWITCH,

    /******************************system message****************************/
    /*! system manager��Ϣռλ�������� 0x0080 ~ 0x00ff ������ϵͳ*/
    SYSTEM_RESERVE_BEGIN = 0x007f,
    /* for qac �ض���Ϊenum���ͣ����� qac ���� */
    MSG_RTC2HZ = MSG_SYS_RTC2HZ, /*0x80*/
    MSG_BAT_V_CHG = MSG_SYS_BAT_V_CHG, /*0x81*/
    MSG_LOW_POWER = MSG_SYS_LOW_POWER, /*0x82*/
    MSG_RTCALARM = MSG_SYS_RTCALARM, /*0x83*/
    MSG_POWER_OFF = MSG_SYS_POWER_OFF, /*0x84*/
    MSG_RECLAIM = MSG_SYS_RECLAIM, /*0x85*/

    MSG_FAST_PREVIEW_SWITCH_AP, /*0x86*/ /*����Ԥ���л�Ӧ��*/

    MSG_USB_STICK = MSG_SYS_USB_STICK, /*0x90*/
    MSG_USB_UNSTICK = MSG_SYS_USB_UNSTICK, /*0x91*/

    MSG_ADAPTOR_IN = MSG_SYS_ADAPTOR_IN, /*0xa0*/
    MSG_ADAPTOR_OUT = MSG_SYS_ADAPTOR_OUT, /*0xa1*/

    MSG_SD_IN = MSG_SYS_SD_IN, /*0xb0*/
    MSG_SD_OUT = MSG_SYS_SD_OUT, /*0xb1*/

    MSG_UH_IN = MSG_SYS_UH_IN, /*0xc0*/
    MSG_UH_OUT = MSG_SYS_UH_OUT, /*0xc1*/

    MSG_LINEIN_IN = MSG_SYS_LINEIN_IN, /*0xd0*/
    MSG_LINEIN_OUT = MSG_SYS_LINEIN_OUT, /*0xd1*/

    //HY HDMI MSG, RESERVE 0xd4 ~ 0xef
    MSG_HDMI_POWER_ON = MSG_SYS_POWER_ON, /*0xd4*/
    MSG_HDMI_POWER_HD1 = MSG_SYS_POWER_HD1, /*0xd5*/
    MSG_HDMI_POWER_HD2 = MSG_SYS_POWER_HD2, /*0xd6*/
    MSG_HDMI_POWER_HD3 = MSG_SYS_POWER_HD3, /*0xd7*/
    MSG_HDMI_POWER_ARC = MSG_SYS_POWER_ARC, /*0xd8*/
    MSG_HDMI_MUTE_ON = MSG_SYS_MUTE_ON, /*0xd9*/
    MSG_HDMI_MUTE_OFF = MSG_SYS_MUTE_OFF, /*0xda*/
    MSG_HDMI_VOL_ADD = MSG_SYS_VOL_ADD, /*0xdb*/
    MSG_HDMI_VOL_SUB = MSG_SYS_VOL_SUB, /*0xdc*/

    MSG_HDMI_HDMI0_IN = MSG_SYS_HDMI0_IN, /*0xe0*/
    MSG_HDMI_HDMI1_IN = MSG_SYS_HDMI1_IN, /*0xe1*/
    MSG_HDMI_HDMI2_IN = MSG_SYS_HDMI2_IN, /*0xe2*/
    MSG_HDMI_ARC_IN = MSG_SYS_ARC_IN, /*0xe3*/
    MSG_HDMI_HDMI_IN = MSG_SYS_HDMI_HDMI_IN,/*0xe4*/

    MSG_HDMI_MUTE_ON_S = MSG_SYS_MUTE_ON_S, /*0xe5*/
    MSG_HDMI_MUTE_OFF_S = MSG_SYS_MUTE_OFF_S, /*0xe6*/
    //HY HDMI MSG END

    /*! ���������ߣ����� */
    MSG_EARPHONE_IN = 0x00f0,
    /*! ���������ߣ��γ� */
    MSG_EARPHONE_OUT,
    /*! ���������Ϣ */
    MSG_FULL_CHARGE,
    /*! �����ϵͣ����� */
    MSG_LOW_BATTERY,
    /*! ������������״̬�����仯 */
    MSG_BT_CON_CHANGED,

    /*! ��Ϣ��ʾ֮��ˢ��LED����� */
    MSG_LED_REDRAW,

    /*! �л���Ӧ��ǰ̨���� */
    MSG_SWITCH_FRONT_APP,
    /*! ֪ͨӦ�ó����˳���Ϣ��Ӧ�ó�����ܵ�����Ϣ�󣬽����˳����̣�����Ϣֻ���� manager.app ���� */
    MSG_APP_QUIT,

    /*! ׼������TTS���� */
    MSG_TTS_START_SYNC,
    /*! TTS������� */
    MSG_TTS_STOP_SYNC,

    /*! ���󲥷������� */
    MSG_SOFT_KEYTONE_PLAY_SYNC,

    /*! ������������EQ���� */
    MSG_SET_EQ_SYNC,

    /*! ����LED�������ʾ */
    MSG_KEEP_LED_DISPLAY,

    /*! ¼������ */
    MSG_REC_START_SYNC,
    MSG_REC_STOP_SYNC,
    MSG_REC_SAVE_RESTART_NEXT_SYNC,

    /*! Ԥ��֪ͨӦ�ü����л�����̨Ӧ�ã�ͨ�����������֣�����Ӧ�ñ���״̬���Ա㷵��ʱ�ָ�״̬ */
    MSG_READY_TO_BG_APP_SYNC,

    /*! ϵͳ����S2�͹���ģʽ */
    MSG_SYSTEM_ENTER_S2,
    /*! ǰ̨���͸������BT ENGINE��֪����S2�͹���ģʽ */
    MSG_SYSTEM_ENTER_S2_SYNC,

    /******************************manager message****************************/
    /*! process manager��Ϣռλ��*/
    MANAGER_BASE = 0x0200,
    /*! �첽����Ӧ�ó�����Ϣ */
    MSG_CREAT_APP,
    /*! ͬ������Ӧ�ó�����Ϣ */
    MSG_CREAT_APP_SYNC,
    /*! �첽ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP,
    /*! ͬ��ɱ��Ӧ�ó�����Ϣ */
    MSG_KILL_APP_SYNC,

    /***************************Music engine message**************************/
    /*! Music engine��Ϣռλ�� */
    MENGINE_BASE = 0x0300,
    /*! ��ȡ������Ϣ�����ֲ��Ž��涨ʱ��ȡ������Ϣ */
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    /*! ��ȡ��ǰ�ļ�·�� */
    MSG_MENGINE_GET_FILEPATH_SYNC,
    /*! ���ò���ģʽ */
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    /*! ������ʾģʽ */
    MSG_MENGINE_SET_DISPMODE_SYNC,
    /*! ���õ��뵭�� */
    MSG_MENGINE_SET_FADE_SYNC,
    /*! ���ò����ٶ� */
    MSG_MENGINE_SET_PLAYSPEED_SYNC,
    /*! �������� */
    MSG_MENGINE_PLAY_SYNC,
    /*! ֹͣ���� */
    MSG_MENGINE_STOP_SYNC,
    /*! ��ͣ���� */
    MSG_MENGINE_PAUSE_SYNC,
    /*! �ָ����� */
    MSG_MENGINE_RESUME_SYNC,
    /*! ����ָ��ʱ�䲥������ */
    MSG_MENGINE_SEEK_SYNC,
    /*! �������ֲ����ļ�(�ļ�ѡ������ʽ) */
    MSG_MENGINE_SET_FILEPATH_SYNC,
    /*! �������ֲ����ļ�(playlist��ʽ)  */
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_MENGINE_PLAY_PREV_SYNC,
    /*! ɾ���ļ� */
    MSG_MENGINE_DELETE_FILE_SYNC,
    /*! ��� */
    MSG_MENGINE_FFWD_SYNC,
    /*! ���� */
    MSG_MENGINE_FBWD_SYNC,
    /*! ȡ���������� */
    MSG_MENGINE_CANCEL_FFB_SYNC,
    /*! ���ÿ������˲��� */
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    /*! ���ÿ������˲���ʱ�� */
    MSG_MENGINE_SET_FFBTIME_SYNC,
    /*! �����ϴβ����ļ� */
    MSG_MENGINE_SET_LAST_FILE_SYNC,
    /*! �������״̬: UIӦ�ô������� */
    MSG_MENGINE_CLEAR_ERR_SYNC,

    /*! ��ȡ��Ϣ */
    MSG_MENGINE_GET_ENGINE_CFG_SYNC,
    /*! ��ȡѭ��ģʽ+shuffle */
    MSG_MENGINE_GET_PLAYMODE_SYNC,
    /*! ��ȡ����ģʽ */
    MSG_MENGINE_GET_ENGINE_PLAYMODE_SYNC,
    /*! ��ȡ�ļ���ʽ */
    MSG_MENGINE_GET_FILEINFO_SYNC,
    /*! ��ȡ��ǰ����״̬ */
    MSG_MENGINE_GET_STATUS_SYNC,
    /*! ��ȡ��ǰ����ʱ��ͱ����� */
    MSG_MENGINE_GET_PLAYINFO_SYNC,
    /*! ��ȡ��ǰ����ʱ��ϵ� */
    MSG_MENGINE_GET_BKINFO_SYNC,
    /*! �л����̺���Ȳ� */
    MSG_MENGINE_SWITCH_DISK_SYNC,
    /*! ���ֵ�� */
    MSG_MENGINE_DIGITAL_SONG_SYNC,
    /*! folder�л���һ�׸��� */
    MSG_MENGINE_FOLFER_NEXT_SYNC,
    /*! folder�л���һ�׸��� */
    MSG_MENGINE_FOLFER_PREV_SYNC,

    /*! ��ȡid3��Ϣ */
    MSG_MENGINE_GET_ID3INFO_SYNC,
    /*! ��ȡĿ¼����Ϣ */
    MSG_MENGINE_GET_DIRNAME_SYNC,
    /*! ��ȡ�ļ����� */
    MSG_MENGINE_GET_FILENAME_SYNC,

    /*! ������������� */
    MSG_MENGINE_SET_SORTNUMBER_SYNC,

    /*! ��ȡ��ǰ��� */
    MSG_MENGINE_GET_DISKNUMBER_SYNC,

    /****************************fm engine message****************************/
    /*! fm engine��Ϣռλ��*/
    FMENGINE_BASE = 0x0400,

    /* ��ʼ����ж�����*/
    /* ��װFM drv ,  ִ��FM ������̺󼴰�װ*/
    MSG_FMENGINE_OPEN,
    /* FM ����IC  ��ʼ������*/
    MSG_FMENGINE_INIT,
    /* FM ����IC ����Standby ״̬*/
    MSG_FMENGINE_STANDBY,

    /* �������*/
    /* ����Ƶ�㲥��( �첽) */
    MSG_FMENGINE_SETFREQ,
    /* ����Ƶ�㲥��( ͬ��)  */
    MSG_FMENGINE_SETFREQ_SYNC,
    /* ���þ�����ȡ������*/
    MSG_FMENGINE_SETMUTE,
    /* ������̨����ֵ*/
    MSG_FMENGINE_SETTHRED,
    /* ���������̨*/
    MSG_FMENGINE_SET_SOFTSEEK,

    /* ��ȡ��Ϣ���*/
    /* ��ȡFM����״̬��Ϣ*/
    MSG_FMENGINE_GET_FM_STATUS,
    /* ��ȡ����״̬��Ϣ */
    MSG_FMENGINE_GET_PLAY_STATUS,
    /* ��ȡ��ǰƵ�� */
    MSG_FMENGINE_GET_FREQ,
    /* ��ͣ���� */
    MSG_FMENGINE_PAUSE_SYNC,
    /* �ָ����� */
    MSG_FMENGINE_RESUME_SYNC,

    /****************************linein engine message****************************/
    /*! linein engine��Ϣռλ��*/
    LINEINEG_BASE = 0x0480,
    /*! ��ȡ��ǰ����״̬ */
    MSG_LINEINEG_GET_STATUS_SYNC,
    /*! ��ͣ���� */
    MSG_LINEINEG_PAUSE_SYNC,
    /*! �ָ����� */
    MSG_LINEINEG_RESUME_SYNC,

    /****************************usond engine message****************************/
    /*! usound engine��Ϣռλ��*/
    USOUNDEG_BASE = 0x04a0,
    /*! ��ȡ��ǰ����״̬ */
    MSG_USOUNDEG_GET_STATUS_SYNC,
    /*! ��ͣ���� */
    MSG_USOUNDEG_PAUSE_SYNC,
    /*! �ָ����� */
    MSG_USOUNDEG_RESUME_SYNC,
    /*!�л���һ��*/
    MSG_USOUNDEG_NEXT_SONG_SYNC,
    /*!�л���һ��*/
    MSG_USOUNDEG_PREV_SONG_SYNC,
    /*!������*/
    MSG_USOUNDEG_VOL_ADD_SYNC,
    /*!������*/
    MSG_USOUNDEG_VOL_SUB_SYNC,

    /****************************bluetooth message****************************/
    /*! bluetooth engine��Ϣռλ�� */
    BTENGINE_BASE = 0x0500,

    /*!!!***** bt manager ���������Ϣ*****!!!*/

    /*! ��ȡ����״̬ ��Ϣ*/
    MSG_BTENGINE_GET_STATUS_SYNC,
    /*! ��ȡԶ���豸֧�ֵ�profile*/
    MSG_BTENGINE_GET_PROFILE_SYNC,
    /*! ��ȡ�������� */
    MSG_BTENGINE_GET_MEDIA_TYPE_SYNC,
    /*��ȡ����������б�*/
    MSG_BTENGINE_GET_PAIRED_LIST_SYNC,
    /*�������������б�*/
    MSG_BTENGINE_CLR_PAIRED_LIST_SYNC,
    /*��ȡ�����豸�Ŀɼ��ԣ��������Ե���Ϣ*/
    MSG_BTENGINE_GET_VISUALMODE_SYNC,
    /*����������ַ�����Զ�����(������������б��д��ڵĵ�ַ)*/
    MSG_BTENGINE_SET_CONNECT_ADDR_SYNC,
    /*�ظ����һ�����ӵ��豸*/
    MSG_BTENGINE_SET_CONNECT_LAST_SYNC,
    /*! ����mode (�������ÿɼ��ԣ���������)*/
    MSG_BTENGINE_MGR_SET_VISUALMODE_SYNC,
    /*! ǿ����������ȴ�����״̬*/
    MSG_BTENGINE_FORCE_WAITPAIRED_SYNC,
    /*! ����ֹͣ���� */
    MSG_BTENGINE_STOP_AUTO_CONNECT_SYNC,
    /*! TWS�豸ֹͣ�����ֻ� */
    MSG_BTENGINE_SET_TWS_DEV_RECONNECT_PHONE_POLICY_SYNC,
    /*!����ͬ�����*/
    MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC,
    /*!��Ҫͬ���ϱ���ص������ֻ���*/
    MSG_BTENGINE_REPORT_BAT_SYNC,
    /*!�����ȡ�ֻ�ʱ�䣬����ѯ��ǰʱ���Ƿ�ɶ�*/
    MSG_BTENGINE_READ_PHONE_CLOCK_REQ_SYNC,
    /*!��ȡ�ֻ�ʱ��*/
    MSG_BTENGINE_READ_PHONE_CLOCK_SYNC,
    /*! transmit a specific DTMF code */
    MSG_BTENGINE_TRANSMIT_DTMF_SYNC,
    /*BQB����*/
    MSG_BTENGINE_BQB_TEST_SYNC,
    /*enter sniff*/
    MSG_BTENGINE_ENTER_SNIFF_SYNC,
    /*exit sniff*/
    MSG_BTENGINE_EXIT_SNIFF_SYNC,
    /*����������������*/
    MSG_BTENGINE_RESTART_INCOMING_CALL_RING,

    /*! START/STOP SIRI*/
    MSG_BTENGINE_SET_SIRI_SYNC,
    /*hid take photo*/
    MSG_BTENGINE_HID_TAKE_PHOTO_SYNC,
    /*hid connect*/
    MSG_BTENGINE_HID_CONNECT_SYNC,
    /*hid disconnect*/
    MSG_BTENGINE_HID_DISCONNECT_SYNC,
    /*��ȡ�绰����Ϣ*/
    MSG_BTENGINE_GET_PB_CONTENT_SYNC,
    /* �������ɼ��� */
    MSG_BTENGINE_OPEN_BT_VISIBLE_SYNC,
    /* app�˽�ֹ����sniff */
    MSG_BTENGINE_DISABLE_ENTER_BT_SNIFF_SYNC,
    /* app���������sniff */
    MSG_BTENGINE_ENABLE_ENTER_BT_SNIFF_SYNC,
    /* ��ȡ��ǰ������������ֻ����� */
    MSG_BTENGINE_GET_CURRENT_ACTIVE_CONNECTED_PHONE_NAME_SYNC,

    // FOR BQB
    MSG_BTENGINE_SET_TO_LIMITED_DISCOVERABLE_MODE_SYNC = 0x5b0,
    MSG_BTENGINE_SET_TO_GENERAL_DISCOVERABLE_MODE_SYNC,

    /*!!!!*****������绰���������Ϣ*****!!!*/

    /*!��������Э��ջ���������ѿ�ʼ*/
    MSG_BTENGINE_ENTER_HFP_CALL_SYNC = 0x0600,
    /*!��������Э��ջ����������ֹͣ*/
    MSG_BTENGINE_EXIT_HFP_CALL_SYNC,
    /*! �����绰 */
    MSG_BTENGINE_ACCEPT_CALL_SYNC,
    /*! �ܽӵ绰 */
    MSG_BTENGINE_REJECT_CALL_SYNC,
    /*! �Ҷϵ绰 */
    MSG_BTENGINE_HANDUP_CALL_SYNC,
    /*! �����绰 */
    MSG_BTENGINE_OUTGOING_CALL_SYNC,
    /*! ����sco��·*/
    MSG_BTENGINE_MGR_SETSYN_SYNC,
    /*! �Ͽ�sco��·*/
    MSG_BTENGINE_MGR_RELEASESYN_SYNC,
    /*! �δ�ָ���ĵ绰����*/
    MSG_BTENGINE_DIAL_PHONE_NUMBER_SYNC,
    /*! �����������ݹ��˿��� */
    MSG_BTENGINE_SCO_PIPE_CTRL_SYNC,
    /*3��ͨ������*/
    MSG_BTENGINE_3WAY_HANDLE_SYNC,

    /*!!!!*****�����Ƹ����������Ϣ*****!!!!*/

    /*! ��������Э��ջ�����Ƹ��Ѳ��� */
    MSG_BTENGINE_ENTER_SBC_PLAY_SYNC = 0x0680,
    /*! ��������Э��ջ�����Ƹ���ֹͣ */
    MSG_BTENGINE_EXIT_SBC_PLAY_SYNC,
    /*! ����/��ͣ���� */
    MSG_BTENGINE_PLAY_PAUSE_SYNC,
    /*! �л�����һ�� */
    MSG_BTENGINE_PLAY_NEXT_SYNC,
    /*! �л�����һ�� */
    MSG_BTENGINE_PLAY_PREV_SYNC,
    /*! ͬ������˵��������ֻ��� */
    MSG_BTENGINE_SET_VOLUME_SYNC,
    /*! �����Ƹ����ݹ��˿��� */
    MSG_BTENGINE_PLAY_PIPE_CTRL_SYNC,
    /*���*/
    MSG_BTENGINE_FFWD_SYNC,
    /*����*/
    MSG_BTENGINE_FBWD_SYNC,
    /*ȡ�����*/
    MSG_BTENGINE_FFWD_CANCEL_SYNC,
    /*ȡ������*/
    MSG_BTENGINE_FBWD_CANCEL_SYNC,
    /*�л�AVDTP����Դ*/
    MSG_BTENGINE_SWITCH_AVDTP_SOURCE_SYNC,

    /*!!!***** spp/ble ����ģ�����������Ϣ*****!!!*/
    /*! ����SPP�������� */
    MSG_BTENGINE_SPP_CREATE_CONNECTION_SYNC,
    /*! �Ͽ�SPP�������� */
    MSG_BTENGINE_SPP_DISCON_CONNECTION_SYNC,

    /*! ����BLE͸���������� */
    MSG_BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC,
    /*! ɾ��BLE͸���������� */
    MSG_BTENGINE_DELETE_BLE_PASSTHROUGH_SYNC,
     /*! BLE͸���������ݷ������� */
    MSG_BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC,
     /*! BLE͸���������ݶ�ȡ���� */
    MSG_BTENGINE_BLE_PASSTHROUGH_DATA_READ_SYNC,
     /*! BLE͸������״̬��ѯ��ȡ���� */
    MSG_BTENGINE_BLE_PASSTHROUGH_GET_INFO_SYNC,

    /*! ����BLE͸���������� */
    MSG_BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC,
    /*! ɾ��BLE͸���������� */
    MSG_BTENGINE_DELETE_SPP_PASSTHROUGH_SYNC,
     /*! BLE͸���������ݷ������� */
    MSG_BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC,
     /*! BLE͸���������ݶ�ȡ���� */
    MSG_BTENGINE_SPP_PASSTHROUGH_DATA_READ_SYNC,
     /*! BLE͸������״̬��ѯ��ȡ���� */
    MSG_BTENGINE_SPP_PASSTHROUGH_GET_INFO_SYNC,

    /***************�Զ������Ը��ӵ�������Ϣ*******************/
    /*! auto test��Ϣռλ�� */
    AUTOTEST_BASE = 0x0700,
    /*! ��ȡ�������ƣ���ַ������ֵ */
    MSG_AUTOTEST_GET_BTINFO_SYNC,
    /*! �޸��������ƣ���ַ������ֵ */
    MSG_AUTOTEST_SET_BTINFO_SYNC,
    /*! ���û���������Ϣ*/
    MSG_AUTOTEST_SET_RECONNECT_SYNC,
    /*����ȡ�߼�����������Ϣ*/
    MSG_GET_LOGIC_KEY_VALUE_SYNC,
    /*����ʼ����������Ϣ*/
    MSG_AUTOTEST_START_BTTEST_SYNC,
    /*����ʼ����RF������Ϣ*/
    MSG_BTENGINE_RF_TEST_SYNC,
    /*���Ͽ�����������Ϣ*/
    MSG_AUTOTEST_RELEASE_CONNECT_SYNC,

    /*! ���ÿɼ��� */
    MSG_BTENGINE_MGR_SET_DISCOVERABLE_SYNC,
    /*! ��������AVDTP */
    MSG_BTENGINE_AVDTP_CONNECT_SYNC,
    /*! �����Ͽ�AVDTP */
    MSG_BTENGINE_AVDTP_DISCONNECT_SYNC,
    /*! ��������HFP */
    MSG_BTENGINE_HFP_CONNECT_SYNC,
    /*! �����Ͽ�HFP */
    MSG_BTENGINE_HFP_DISCONNECT_SYNC,
    /*! ��������*/
    MSG_BTENGINE_CLEAR_ERR_SYNC,
    /*! ����Ƶƫ����ʱ,��������host�˷���*/
    MSG_BTENGINE_SET_ATT_CMD_SYNC,
    /*! ����Ƶƫ����ʱ,��ȡhost�˵���Ӧ��Ϣ*/
    MSG_BTENGINE_GET_ATT_RSP_SYNC,
    /*! Ƶƫ���Բ��Խ���*/
    MSG_BTENGINE_ATT_TEST_END_SYNC,
    /*! ����豸������б���*/
    MSG_BTENGINE_ADD_DEV_TO_PAIRED_LIST_SYNC,
    /*! auto test��Ϣ������ */
    AUTOTEST_END,

    /***************����TWS���������Ϣ*******************/
    MSG_BTENGINE_TWS_BASE = 0x780,

    MSG_BTENGINE_TWS_LINK_SYNC,
    MSG_BTENGINE_TWS_FORCE_UNLINK_SYNC,

    MSG_BTENGINE_TWS_GET_BT_CBK_SYNC,
    MSG_BTENGINE_TWS_GET_SEND_FRAME_CBK_SYNC,
    MSG_BTENGINE_TWS_SET_SCAN_MODE_SYNC,

    /***************���������������Ϣ*******************/
    MSG_BTCALLEG_BASE = 0x800,
    MSG_BTCALLEG_GET_STATUS_SYNC,
    MSG_BTCALLEG_SPEECH_PLAY_SYNC,
    MSG_BTCALLEG_SPEECH_STOP_SYNC,

    /***************���������������Ϣ*******************/
    MSG_BTPLAYEG_BASE = 0x900,
    MSG_BTPLAYEG_GET_STATUS_SYNC,
    //TWS��ͣ�ͻָ���Ϣ
    MSG_BTPLAYEG_TWS_PAUSE_SYNC,
    MSG_BTPLAYEG_TWS_RESUME_SYNC,
    //TWSȷ������Ӧ��ģʽ�����Ӧ��ģʽ
    MSG_BTPLAYEG_TWS_CONFORM_MODE_SYNC,

    /****************************** play scene �¼� ****************************/
    /*! ���ų����¼�ռλ */
    EVENT_PLAYING_BASE = 0x2000,
    /*! ����¼� */
    EVENT_PLAYING_FFOREWORD,
    /*! �����¼� */
    EVENT_PLAYING_FBACKWORD,
    /*! ��һ�� or AB �¼� */
    EVENT_PLAYING_NEXTSONG,
    /*! ��һ���¼� */
    EVENT_PLAYING_PREVSONG,
    /*! option�¼� */
    EVENT_PLAYING_OPTION,
    /*! ����/��ͣ�¼� */
    EVENT_PLAYING_PAUSE,
    /*! EQ�����¼� */
    EVENT_EQ_SET,
    /*! ������EQ�¼� */
    EVENT_EQ_NEXT,
    /*! ������EQ�¼� */
    EVENT_EQ_PREV,
    /*! ����EQʱ�����¼� */
    EVENT_EQ_IGNORE,
    /*! ѭ��ģʽ�����¼� */
    EVENT_LOOPMODE_SET,
    /*! mute�����¼� */
    EVENT_MUTE_SWITCH,
    /*! ģʽ(mode short)�л��¼� */
    EVENT_MODE_SWITCH,
    /*! �л���ʱ����ʾ�¼� */
    EVENT_ENTER_TIMESHOW,
    /*! �л���ʱ�������¼� */
    EVENT_ENTER_TIMESET,
    /*! folder���¼� */
    EVENT_FOLDER_ADD,
    /*! folder���¼� */
    EVENT_FOLDER_SUB,
    /*! ���ֵ���¼� */
    EVENT_DIGITAL_SONG,
    /*! �����¼� */
    EVENT_PLAYING_RETURN,

    /*! ǿ����������ȴ������¼�*/
    EVENT_FORCE_WAITPAIRED,
    /*! ǿ�������л����Ͽ����ӻ�������һ���豸*/
    EVENT_SWITCH_CONNECT,
    /*! �������һ�ε绰 */
    EVENT_CALL_LASTPHONE,
    /*! ����HID���� */
    EVENT_HID_TAKE_PHOTO,
    /*! HID BQB KEY */
    EVENT_HID_BQB_PREV,
    EVENT_HID_BQB_NEXT,

    /*! ����TWS������� */
    EVENT_TWS_PAIR_LINK,
    /*! ����TWS��ӶϿ� */
    EVENT_TWS_PAIR_UNLINK,
    /*! �л�TWS�������� */
    EVENT_TWS_SWITCH_POS,
    /*! �������ɼ��� */
    EVENT_OPEN_BT_VISIBLE,

    /*! ��������SIRI */
    EVENT_SIRI_START,

    /****************************** linein �¼� ****************************/
    /*! linein�¼�ռλ */
    EVENT_LINEIN_BASE = 0x2100,
    /*!line in next*/
    EVENT_LINEIN_NEXT,
    /*!line in prev*/
    EVENT_LINEIN_PREV,
    /*!line in play*/
    EVENT_LINEIN_PLAY,
    /*! ¼��PLAY�¼� */
    EVENT_RECORD_PLAY_PAUSE,
    /*! ¼�������¼� */
    EVENT_RECORD_SAVE,

    /****************************** radio �¼� ****************************/
    /*! radio �¼�ռλ */
    EVENT_RADIO_BASE = 0x2200,
    /*! �л�����һ��Ƶ�ʵ��¼� */
    EVENT_RADIO_PREV_FREQ,
    /*! �л�����һ��Ƶ�ʵ��¼� */
    EVENT_RADIO_NEXT_FREQ,
    /*! �л�����һ��Ԥ���̨�¼� */
    EVENT_RADIO_PREV_PRESET,
    /*! �л�����һ��Ԥ���̨�¼� */
    EVENT_RADIO_NEXT_PRESET,
    /*! ��ǰ�Զ���̨�¼�*/
    EVENT_RADIO_SEEK_PREV,
    /*! ����Զ���̨�¼� */
    EVENT_RADIO_SEEK_NEXT,
    /*! �Զ���̨�¼� */
    EVENT_RADIO_AUTO_SEARCH,
    /*! ��ͣ����/�ָ������¼� */
    EVENT_RADIO_PLAY_PAUSE,
    /*! �˳���̨���� */
    EVENT_RADIO_SEARCH_EXIT,
    /*! ����ĳЩ������Ϣ */
    EVENT_RADIO_SEARCH_IGNORE,

    /****************************** record �����¼� ****************************/
    EVENT_RECPLAY_BASE = 0x2600,
    /*! MODE�¼� */
    EVENT_RECPLAY_MODE,
    /*! ��ͷ��ʼ�����¼� */
    EVENT_RECPLAY_PLAY,
    /*! �˳����ŷ���¼���¼� */
    EVENT_RECPLAY_REC,
    /*! ħ��ѡ��ģʽ(������ģʽ)*/
    EVENT_RECPLAY_SELMODE_ADD,
    /*! ħ��ѡ��ģʽ(������ģʽ)*/
    EVENT_RECPLAY_SELMODE_SUB,

} msg_apps_type_e;

#endif
