/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������������湫��ͷ�ļ�����غ궨��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _CASE_TYPE_H_
#define _CASE_TYPE_H_

#include "psp_includes.h"
#include "case_independent.h"
#include "mmm_mp.h"

/*!
 * \brief
 *  app_type_e Ӧ������ö������
 */
typedef enum
{
    /*! ���̹����� */
    APP_TYPE_PMNG = 0,
    /*! ǰ̨Ӧ�ã�gui���̣� */
    APP_TYPE_GUI = 1,
    /*! ��̨Ӧ�ã�console���̣� */
    APP_TYPE_CONSOLE = 2,
    /*! ��̨Ӧ�ã�BTENGINE���̣� */
    APP_TYPE_BTENGINE = 3,
} app_type_e;

/*!
 * \brief
 *  ����Ӧ��ID�����֧��32��Ӧ�ã�ID��Ӧ app_name_ram �е����֡�
 */
#define APP_ID_MUSIC                (0x00)
#define APP_ID_LINEIN               (0x01)
#define APP_ID_RADIO                (0x02)
#define APP_ID_BTPLAY               (0x03)
#define APP_ID_BTCALL               (0x04)
#define APP_ID_USOUND               (0x05)
#define APP_ID_MANAGER              (0x06)
#define APP_ID_CONFIG               (0x07)
#define APP_ID_UDISK                (0x08)
#define APP_ID_MENGINE              (0x09)
#define APP_ID_LINEIN_EG            (0x0a)
#define APP_ID_FMENGINE             (0x0b)
#define APP_ID_BTPLAY_EG            (0x0c)
#define APP_ID_BTCALL_EG            (0x0d)
#define APP_ID_USOUND_EG            (0x0e)
#define APP_ID_BTENGINE             (0x0f)
#define APP_ID_UPGRADE              (0x10)
#define APP_ID_TEST                 (0x11)
#define APP_ID_BTHID                (0x12)
#define APP_ID_RECORD               (0x13)
#define APP_ID_ALARM                (0x14)
#define APP_ID_MAX                  (0x15)

#define APP_ID_THEENGINE            (0x80) //��ָ����

/*!
 * \brief
 *  ��������ID�����֧��32������
 */
//�����Ƹ�
#define APP_FUNC_BTPLAY             (0)
//���ſ�
#define APP_FUNC_PLAYCARD           (1)
//����linein
#define APP_FUNC_PLAYLINEIN         (2)
//fm radio
#define APP_FUNC_RADIO              (3)
//usb����
#define APP_FUNC_USOUND             (4)
//����UHOST
#define APP_FUNC_PLAYUHOST          (5)
//u��
#define APP_FUNC_UDISK              (6)
//��������
#define APP_FUNC_BTCALL             (7)
//confige
#define APP_FUNC_CONFIG             (8)
//����
#define APP_FUNC_UPGRADE            (9)
//�Զ�������
#define APP_FUNC_TEST               (10)
//����Ӧ��
#define APP_FUNC_PHOTO              (11)
//��¼��
#define APP_FUNC_CARD_REC           (12)
//U��¼��
#define APP_FUNC_UHOST_REC          (13)
//���ط�
#define APP_FUNC_CARD_REPLAY        (14)
//U�̻ط�
#define APP_FUNC_UHOST_REPLAY       (15)
//��������
#define APP_FUNC_PLAYMDISK          (16)
//���岥��Ӧ��
#define APP_FUNC_ALARM              (18)
#define APP_FUNC_ALARM_SDCARD       (19)
#define APP_FUNC_ALARM_UHOST        (20)
//SPDIF����
#define APP_FUNC_SPDIF_PLAY         (21)
//I2SRX����
#define APP_FUNC_I2SRX_PLAY         (22)
//HDMI����
#define APP_FUNC_HDMI_HDMI          (23)
#define APP_FUNC_HDMI_HDMI1         (24)
#define APP_FUNC_HDMI_HDMI2         (25)
#define APP_FUNC_HDMI_ARC           (26)

#define APP_FUNC_MAX                (32)

#define LINEIN_MODE_AUX        0 //ģ������

#define LINEIN_MODE_MIC_C      1 //MIC¼������
#define LINEIN_MODE_MIC_U      2 //MIC¼����U��

#define LINEIN_MODE_SPDIF      3 //��������
#define LINEIN_MODE_I2SRX      4 //I2SRX����
#define LINEIN_MODE_HDMI       5 //HDMI����
#define LINEIN_MODE_HDMI1      6 //HDMI1����
#define LINEIN_MODE_HDMI2      7 //HDMI2����
#define LINEIN_MODE_ARC        8 //HDMI ARC����

/*!
 * \brief
 *  Ӧ�ú�׺�� bitmap��bit<31> = 0h�����׺�������Ӧ��ϵ��bit[30] ��Ӧ��׺������ rom_ext_name ��Ԫ�أ��� MP3��
 */
#if (SUPPORT_APE_TYPE == 1)
#define MUSIC_APE_BITMAP            EXT_NAME_APE
#else
#define MUSIC_APE_BITMAP            0
#endif

#if (SUPPORT_FLAC_TYPE == 1)
#define MUSIC_FLAC_BITMAP           EXT_NAME_FLAC
#else
#define MUSIC_FLAC_BITMAP           0
#endif

#define MUSIC_BITMAP                (EXT_NAME_MP3|EXT_NAME_WMA|EXT_NAME_WAV|EXT_NAME_MP1|EXT_NAME_MP2|MUSIC_APE_BITMAP|MUSIC_FLAC_BITMAP)
#define RECORD_BITMAP               (EXT_NAME_MP3|EXT_NAME_WAV)
#define ALLALL_BITMAP               (EXT_NAME_ALL)
#define ALLFILE_BITMAP              (EXT_NAME_ALL_FILE)
#define ALLDIR_BITMAP               (EXT_NAME_ALL_DIR)
#define RECORD_WAV_BITMAP           (EXT_NAME_WAV)
#define RECORD_MP3_BITMAP           (EXT_NAME_MP3)


/*!
 * \brief
 *  app_result_e Ӧ�ò�ӿڷ��ؽ��ö������
 */
typedef enum
{
    /*! û���κ���Ҫ֪ͨ�����ߵ�������أ������߲���Ҫ���κ����⴦�� */
    RESULT_NULL = 0x00,
    /*! ���Է��أ�������RESULT_NULL�������߲���Ҫ���κ����⴦��*/
    RESULT_IGNORE,
    /*! �ػ�UI���أ�ͨ�����غ���Ҫ���»��Ƶ�ǰUI��������Ҫ�˳���ǰ��Ϣѭ��*/
    RESULT_REDRAW,
    /*! Ӧ���˳���Ϣ */
    RESULT_APP_QUIT,
    /*! �������󷵻�*/
    RESULT_ERROR,

    _RESERVE_FOR_COMMON_RESULT_ = 0x8000,

    /*! �����ļ����ſ��ļ���Ϣ*/
    RESULT_MUSIC_CPLAY,
    /*! �����ļ�����u���ļ���Ϣ*/
    RESULT_MUSIC_UPLAY,
    /*! �����ļ����������ļ���Ϣ*/
    RESULT_MUSIC_MDISKPLAY,
    /*! ����line in */
    RESULT_ENTER_LINEIN,
    /*! ����SPDIF���� */
    RESULT_ENTER_SPDIF,
    /*! ����HDMI���� */
    RESULT_ENTER_HDMI,
    RESULT_ENTER_HDMI1,
    RESULT_ENTER_HDMI2,
    RESULT_ENTER_HDMI_ARC,
    /*! ����I2SRX���� */
    RESULT_ENTER_I2SRX,
    /*!RADIO������Ϣ*/
    RESULT_RADIO,
    /*! ������������ */
    RESULT_BLUETEETH_SOUND,
    /*! ��������HFP��绰 */
    RESULT_BLUETEETH_HFP,
    /*! ����ͨ������ģʽ */
    RESULT_ENTER_ASQT_TEST,
    /*! ����usb���� */
    RESULT_USB_SOUND,
    /*! ���ػ����ػ�*/
    RESULT_POWER_OFF,
    /*! �͵�ػ�*/
    RESULT_LOW_POWER,
    /*! ������Ӧ */
    RESULT_ENTER_ALARM,
    /*! ������Ӧ���Զ��ػ� */
    RESULT_ALARM_POWER_OFF,

    /*! ͨ��ATDģʽ�������AP */
    RESULT_TESTAP_ATD_ENTER,
    /*! ͨ���忨ģʽ�������AP */
    RESULT_TESTAP_CARD_ENTER,

    /*! ESD RESET �ϵ�ָ�*/
    RESULT_ESD_RESET,

    /*! ����Ԥ���л�Ӧ�ã�func id����� g_p_global_var->fp_switch_func_index */
    RESULT_FAST_PREVIEW_SWITCH_AP,

    /*! ����RTCʱ����ʾ */
    RESULT_ENTER_RTC_TIME,
    RESULT_BACK_RTC_TIME,
    RESULT_SWITCH_RTC_SCENE,

    //������Ϊ��ap�л���أ��벻Ҫ�������λ�ã���ֹ
    /*!��һ�β��Ž�����Ϣ*/
    RESULT_LASTPLAY,
    /*!��һ������ѡ��*/
    RESULT_NEXT_FUNCTION,
    /*! ����upgrade*/
    RESULT_ENTER_CARD_UPGRADE,
    RESULT_ENTER_UHOST_UPGRADE,
    /*! ���뱾�ش���Ŀ¼1*/
    RESULT_ENTER_FOLDER1,
    /*! ���뱾�ش���Ŀ¼2*/
    RESULT_ENTER_FOLDER2,

    /*! USBѡ��ѡ�����ݴ���ģʽ */
    RESULT_USB_TRANS,
    /*! ������г��ģʽ */
    RESULT_ENTER_CHARGE_LOOP,
    /*! ���ػ���������TTS */
    RESULT_JUST_POWER_OFF,

    /*! ��������Ӧ�� */
    RESULT_ENTER_PHOTO,

    /* ��ʼFM ���ţ�ֱ�ӽ��벥�ų���*/
    RESULT_RADIO_START_PLAY,
    /* �����Զ���̨����*/
    RESULT_RADIO_AUTO_SEARCH,
    /* �����ֶ���̨����-�������*/
    RESULT_RADIO_MANUAL_UP_SEARCH,
    /* �����ֶ���̨����-��ǰ����*/
    RESULT_RADIO_MANUAL_DOWN_SEARCH,

    RESULT_ENTER_CARD_REC,
    RESULT_ENTER_UHOST_REC,
    RESULT_ENTER_CARD_REPLAY,
    RESULT_ENTER_UHOST_REPLAY,
    RESULT_ENTER_REC_REWRITE_FILE,
    RESULT_MAIN_APP,
    /*! ���������˳� */
    RESULT_SCENE_EXIT,

    RESULT_UI_CONFIRM,
    RESULT_UI_CANCEL,
} app_result_e;

/*! ��̨Ӧ�ã�ͨ�����������֣�״̬*/
typedef enum
{
    BG_STATUS_NULL = 0, //�Ǻ�̨Ӧ��״̬
    BG_STATUS_IN = 1,   //�л�����̨Ӧ��
    BG_STATUS_BACK = 2, //�Ӻ�̨Ӧ�÷���
} background_app_status_e;

#define ESD_BK_FLAG  0x3456
//ESD�ϵ�ʵʱ���ݽṹ��
typedef struct
{
    uint16 flag;           //0x3456 ��ʾ��Ч����ʼ֮���������Ӧ�þͻ���Ϊ��Ч��־��
    /*NOTE! ���±���λ�ò��ܵ������ñ�������welcome��ֵ��Ȼ����Ӧ�ò��ȡ */
    uint8  force_enable_udisk; //ǿ��ʹ�ܶ�����ģʽ����ʹ����������Ϊ��֧��Ҳ���Խ��룬1��ʾǿ��ʹ��
    uint8  reset_flag;     //ESD�����ָ��ϵ��־��0��ʾ�������룬1��ʾESD��������
    uint8  app_id;         //ESD����ʱǰ̨Ӧ��ID
    uint8  func_id;        //ESD����ʱǰ̨����ID
    uint8  func_index;     //ESD����ʱǰ̨������ap_switch_info���±꣬�� com_ap_switch_deal ��ʹ��
    uint8  app_para;       //ESD����ʱǰ̨Ӧ�ô�������
    uint8  app_state;      //ESD����ʱǰ̨Ӧ��״̬
    uint8  app_scene;      //ESD����ʱǰ̨����
    uint8  engine_id;      //ESD����ʱ����Ӧ��ID
    uint8  engine_para;    //ESD����ʱ����Ӧ�ô�������
    uint8  engine_state;   //ESD����ʱ����Ӧ��״̬
    uint8  play_status;    //ESD����ʱ���沥��״̬
    uint8  bt_state;       //ESD����ʱ�����Ƿ��ڹ���
    uint8  bt_info_state;  //ESD����ʱ����״̬��״̬
    uint8  volume_mode;    //ESD����ʱ����ģʽ
    uint8  volume_value;   //ESD����ʱ����ֵ
    uint8  volume_value_call; //ESD����ʱ��绰����ֵ
    uint8  eq_type;        //ESD����ʱEQ����


    /*NOTE! �����Ҫ���ӱ�����ֻ���� bk_info ֮ǰ�ӣ���Ϊ bk_info ��һ�����������飬����Ŀռ���ܻᱻʹ�õ� */
    uint32 bk_info[3];     //ESD����ʱʵʱ�ϵ���Ϣ����uint32��Ϊ��ȷ��word���룬��Ӧ��ʵ��ʹ��ʱ����ǿ������ת��Ϊ���ʵĶϵ�ṹ��
    /*NOTE! ������ bk_info ֮�����ӱ�������Ϊ bk_info ��һ�����������飬����Ŀռ�ᱻʹ�õ� */
} esd_bk_infor_t; //max length is 0x50 bytes

//ָ��ESD�ϵ�ʵʱ���ݻ�������ָ��
#define g_p_esd_bk_info ((esd_bk_infor_t *)(ESD_BK_INFOR_ADDR))

//Ӧ�ò�ȫ�ֱ���������Ƭ�ڴ���� common_init ���� memset
typedef struct
{
    uint8  bg_app_sta;         //��̨Ӧ��״̬
    uint8  play_status_before; //��¼�л���̨Ӧ�ã�ͨ�����������֣�֮ǰ�Ĳ���״̬���Ա㷵��ʱ�ָ�״̬
    uint8  only_hid_mode;      //����ΪHID�豸����������Ӧ��ģʽ
    uint8  att_flag;           //�Զ������Ա�־
    uint8  slave_btplay_flag;  //�����Ƿ��������Ƹ�ģʽ
    uint8  update_btplay_flag; //�����Ƿ��յ������ϱ�����BTPLAY״̬�¼�
    uint8  alarm_poweron_flag; //���ӻ��ѿ������ر����Ӻ��Զ���ػ�
    uint8  fp_switch_func_index;//����Ԥ���л�Ӧ��func index
    uint8  fp_switch_jump_welcome;//����Ԥ���л�Ӧ�ã��Ѿ���ʾwelcome���棬��������ʾ
    uint16 record_result;      //¼�����غ���תӦ��
    uint32 display_time;       //�Ӻ�̨Ӧ�÷���ʱ��ʾʱ���

    uint8 *pointer_1; //���������ӣ��갸û��ʹ�ã���ɾ������Ҫ����ڴ棬����ԴӶ����룬�����ַ����ʹ�á�pointer_1 = sys_malloc(0x100);
} global_var_t; //max length is 0x30 bytes

//ָ��ȫ�ֱ�������ָ��
#define g_p_global_var ((global_var_t *)(GLOBAL_VAR_ADDR))

#endif
