/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù�����غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _APPLIB_H_
#define _APPLIB_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "app_msg.h"
#include "app_timer.h"
#include "fw_config.h"
#include "typestruct_tws_case.h"

/*!
 * \brief
 *  app_info_t Ӧ����Ϣ�ṹ��
 */
typedef struct
{
    /*! �ṹ��ʹ�ñ�־��1��ʾ�ѱ�ʹ�ã�0��ʾδ��ʹ�� */
    uint8 used;
    /*! ����ID�ţ��� case_type.h �ж�����б� */
    uint8 app_id;
    /*! Ӧ�����ͣ���app_type_e ���� */
    uint8 app_type;
    /*! ����˽����Ϣ����ID����mq_id_e ���� */
    uint8 mq_id;
} app_info_t;

/*!
 * \brief
 *  bt_state_e ������������ö������
 */
typedef enum
{
    BT_STATE_NONE = 0,
    BT_STATE_WORK = 1,
} bt_state_e;

/*TWSģʽ*/
typedef enum
{
    TWS_MODE_SINGLE  = 0, //����Ӧ��ģʽ
    TWS_MODE_BT_TWS  = 1, //����TWS�Ƹ�
    TWS_MODE_AUX_TWS = 2, //AUX TWS�Ƹ�
} tws_mode_e;

/*!
 * \brief
 *  engine_type_e ��������ö�����ͣ�����Ӧ���û������̨��������
 */
typedef enum
{
    /*! û������ */
    ENGINE_NULL = 0,
    /*! music���� */
    ENGINE_MUSIC = 1,
    /*! fm���� */
    ENGINE_RADIO = 2,
    /*! LINEIN���� */
    ENGINE_LINEIN = 3,
    /*! �����Ƹ����� */
    ENGINE_BTPLAY   = 4,
    /*! ������绰���� */
    ENGINE_BTCALL   = 5,
    /*! usb�������� */
    ENGINE_USOUND   = 6,
} engine_type_e;

/*!
 * \brief
 *  engine_state_e ����״̬ö������
 */
typedef enum
{
    /*! û������ */
    ENGINE_STATE_NULL = 0,
    /*! �������ڲ��� */
    ENGINE_STATE_PLAYING = 1,
    /*! ���洦����ͣ״̬ */
    ENGINE_STATE_PAUSE = 2,
} engine_state_e;

/*!
 * \brief
 *  app_state_e ǰ̨Ӧ�� UI ״̬ö������
 */
typedef enum
{
    /*! ǰ̨AP���ڷǲ��� UI ״̬ */
    APP_STATE_NO_PLAY = 0,
    /*! ǰ̨AP���ڲ��� UI ״̬ */
    APP_STATE_PLAYING = 1,
    /*! ǰ̨AP���ڲ��� UI ״̬����ϣ�����ⳣ�� */
    APP_STATE_PLAYING_ALWAYS = 2,
} app_state_e;

#define UI_IDLE_TIMER    14 //����״̬����7���Ӿ͵���Ƶ��
/*! ǰ̨�û�����״̬ */
typedef enum
{
    UI_STATUS_IDLE = 0, //��ʾû��UI��Ҫ����
    UI_STATUS_BUSY = 1, //��ʾ���ڽ���UI����
} ui_status_e;

//mmm state
typedef enum
{
    MMM_STATE_NO_PLAY = 0, //û���м��
    MMM_STATE_PLAYING = 1, //�����м��
    MMM_STATE_TTS_PLAYING = 2, //TTS�����м��
    MMM_STATE_KT_PLAYING = 3, //������ͨ���м��
} mmm_state_e;


//Ӧ�ù���ģ��ӿ�����
#if 0
extern bool applib_init_inner(uint8 app_id, app_type_e type);
#endif
#define applib_init(a,b)             ((bool(*)(uint8, app_type_e))COM_APPLIB_INIT)((a),(b))

#if 0
extern bool applib_quit_inner(app_type_e type);
#endif
#define applib_quit(a)               ((bool(*)(app_type_e))COM_APPLIB_QUIT)((a))

#if 0
extern bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info);
#endif
#define applib_get_app_info(a,b)     ((bool(*)(app_type_e, app_info_t *))COM_APPLIB_GET_APP_INFO)((a),(b))

#if 0
extern engine_type_e get_engine_type_by_appid_inner(uint8 app_id);
#endif
#define get_engine_type_by_appid(a)  ((engine_type_e(*)(uint8))COM_GET_ENGINE_TYPE_BY_APPID)((a))

#if 0
extern uint8 get_engine_appid_by_type_inner(engine_type_e engine_type);
#endif
#define get_engine_appid_by_type(a)  ((uint8(*)(engine_type_e))COM_GET_ENGINE_APPID_BY_TYPE)((a))

#if 0
extern engine_type_e get_engine_type_inner(void);
#endif
#define get_engine_type()           ((engine_type_e(*)(void))COM_GET_ENGINE_TYPE)() //��ȡ��̨��������

#if 0
extern bt_state_e get_bt_state_inner(void);
#endif
#define get_bt_state()              ((bt_state_e(*)(void))COM_GET_BT_STATE)() //��ȡ��������״̬

#if 0
extern tws_mode_e get_tws_mode_inner(void);
#endif
#define get_tws_mode()              ((tws_mode_e(*)(void))COM_GET_TWS_MODE)()

#if 0
extern void change_tws_mode_inner(tws_mode_e mode);
#endif
#define change_tws_mode(a)          ((void(*)(tws_mode_e))COM_CHANGE_TWS_MODE)((a))

#if 0
extern void change_engine_state_inner(engine_state_e state);
#endif
#define change_engine_state(a)      ((void(*)(engine_state_e))COM_CHANGE_ENGINE_STATE)((a)) //�޸ĺ�̨����״̬

#if 0
extern void change_app_state_inner(app_state_e state);
#endif
#define change_app_state(a)         ((void(*)(app_state_e))COM_CHANGE_APP_STATE)((a)) //�޸�ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI

#if 0
extern uint8 get_last_app_id_inner(void);
#endif
#define get_last_app_id()           ((uint8(*)(void))COM_GET_LAST_APP_ID)() //��ȡ���һ��ǰ̨Ӧ��

#if 0
extern uint8 get_cur_app_id_inner(void);
#endif
#define get_cur_app_id()            ((uint8(*)(void))COM_GET_CUR_APP_ID)() //��ȡ��ǰǰ̨Ӧ��

#if 0
extern mmm_state_e get_mmm_state_inner(void);
#endif
#define get_mmm_state()             ((mmm_state_e(*)(void))COM_GET_MMM_STATE)()

#if 0
extern void change_mmm_state_inner(mmm_state_e mmm_sta);
#endif
#define change_mmm_state(a)         ((void(*)(mmm_state_e))COM_CHANGE_MMM_STATE)((a))

#if 0
extern mmm_ktp_engine_t get_ktp_status_inner(void);
#endif
#define get_ktp_status()            ((mmm_ktp_engine_t(*)(void))COM_GET_KTP_STATUS)()

#if 0
extern void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta);
#endif
#define change_ktp_status(a)        ((void(*)(mmm_ktp_engine_t))COM_CHANGE_KTP_STATUS)((a))

//try vram merge
#if 0
extern void com_vram_merge_inner(void);
#endif
#define com_vram_merge()            ((void(*)(void))COM_COM_VRAM_MERGE)()

#endif

