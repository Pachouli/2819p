/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������APPLIB ģ���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _APPLIB_INNER_H_
#define _APPLIB_INNER_H_

#include "psp_includes.h"
#include "applib.h"
#include "mmm_ktp.h"

//#define COMMON_TEST_EN

/*! �������֧�� 4 ��Ӧ��ͬʱ���� */
#define MAX_APP_COUNT   4

/*!
 * \brief
 *  app_info_state_all_t ϵͳ��ǰ״̬�ṹ��
 */
typedef struct
{
    /*! ��ǰ���е�ǰ̨Ӧ��ID */
    uint8 app_id;
    /*! ��ǰ���е��������ͣ��� engine_type_e ���� */
    uint8 engine_type;
    /*! ���浱ǰ״̬�����ڲ��Ż�����ͣ���� engine_state_e ���� */
    uint8 engine_state;
    /*! ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI���� app_state_e ���� */
    uint8 app_state;
    /*! BT ����״̬����δ���أ������������� bt_state_e ���� */
    uint8 bt_state;
    /*! TWSģʽ���� tws_mode_e ���� */
    uint8 tws_mode;
    /*! ǰ̨�û�����״̬ */
    ui_status_e ui_status;
    /*! ��ǰϵͳƵ��ģʽ�Ƿ����ڹ���ģʽ������̶�Ϊ������Ƶ�ʣ��������UI״̬�Զ����� */
    bool sys_clk_work;
    /*! ǰһ��APP ID */
    uint8 last_app_id;
    /*! �м��״̬ */
    uint8 mmm_state;
    /*! ������ͨ��״̬ */
    mmm_ktp_engine_t ktp_status;
    /*! ����4�ֽ��Ա���չ */
    uint8 reserves[2];
} app_info_state_all_t;

/*!
 * \brief
 *  conf_type_e ���ýű��ļ����
 */
typedef enum
{
    /*! �ַ������� */
    CONF_TYPE_STRING = 0,
    /*! ������ֵ���� */
    CONF_TYPE_NUM_LINEAR = 1,
    /*! ��������ֵ���� */
    CONF_TYPE_NUM_NONLINEAR = 2,
    /*! 32λ������ֵ���� */
    CONF_TYPE_32BIT_INTEGER = 3,
} conf_type_e;

/*!
 * \brief
 *  config_index_t �������������ݽṹ
 */
typedef struct
{
    /*! ��������������ļ�ͷ��ƫ�� */
    uint32 offset;
    /*! ���������ݳ��� */
    uint32 length;
} config_index_t;


extern bool com_open_config_file(void);
extern bool com_close_config_file(void);

typedef struct
{
    /* vector Ӧ�ü���ʱ������ָ�룬ָ��Ӧ�ÿռ�Ķ�ʱ������ */
    app_timer_t *vector;
    /* count Ӧ�ü���ʱ����Ŀ */
    uint8 count;
    /* tag ��ʱ��������־��AP����Ϊ0 */
    uint8 tag;
    /* ������ʱ������������ */
    bool in_handle;
} app_timer_group_t;


extern void app_timer_var_init(app_timer_group_id_e group_id);
extern void app_timer_common_group_init(void);
extern void handle_timers(app_timer_group_id_e group_id);
//�޸�ǰ̨��COMMON��ʱ��������־
extern void change_app_timer_tag_inner(uint8 tag);
extern bool get_app_timer_in_handle(app_timer_group_id_e group_id);

//inner function

//Ӧ�ã����̣�����ģ��ӿ���������ϸ˵�����ӿڶ��壩
extern bool applib_init_inner(uint8 app_id, app_type_e type);
extern bool applib_quit_inner(app_type_e type);
extern bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info);

extern engine_type_e get_engine_type_by_appid_inner(uint8 app_id);
extern uint8 get_engine_appid_by_type_inner(engine_type_e engine_type);

//Ӧ�ü���ʱ������ģ��ӿ���������ϸ˵�����ӿڶ��壩
extern int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
extern int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
extern bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout);
extern bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id);
extern bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count);

//��Ϣͨ�Ź���ģ��ӿ���������ϸ˵�����ӿڶ��壩
//������Ϣ�ӿ�
bool get_app_msg_inner(app_type_e type, private_msg_t *private_msg);
bool get_app_msg_hotplug(app_type_e type, private_msg_t *private_msg);
bool get_hotplug_msg(private_msg_t *private_msg);
void message_redeal_app_msg_inner(private_msg_t *p_app_msg);

//������Ϣ�ӿ�
bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout);
bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg);
bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode);

//ͬ����Ϣ��ִ�ӿ�
bool reply_sync_msg_inner(private_msg_t *private_msg);

//������ӿ�����

extern bool com_get_config_struct_inner(uint16 config_id, uint8 *buf, uint16 buf_len);
extern uint16 com_get_config_default_inner(uint16 config_id); //��������ֵ�Ͳ���

//try vram merge
extern void com_vram_merge_inner(void);

//��ȡ��̨��������
extern engine_type_e get_engine_type_inner(void);
//�޸ĺ�̨��������
extern void change_engine_type(engine_type_e type);
//��ȡ����Э��ջ����״̬
extern bt_state_e get_bt_state_inner(void);

extern tws_mode_e get_tws_mode_inner(void);
extern void change_tws_mode_inner(tws_mode_e mode);

//��ȡ��̨����״̬
extern engine_state_e get_engine_state(void);
//�޸ĺ�̨����״̬
extern void change_engine_state_inner(engine_state_e state);
//��ȡǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
extern app_state_e get_app_state(void);
//�޸�ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
extern void change_app_state_inner(app_state_e state);
//��ȡ���ǰ̨AP
extern uint8 get_last_app_id_inner(void);
//��ȡ��ǰǰ̨AP
extern uint8 get_cur_app_id_inner(void);
//��ȡ ui_status
extern ui_status_e get_ui_status(void);
//�ı� ui_status
extern void change_ui_status(ui_status_e ui_sta);
//��ȡ ui_status
extern bool get_sys_clk_work(void);
//�ı� ui_status
extern void change_sys_clk_work(bool sys);
//��ȡ mmm_state
extern mmm_state_e get_mmm_state_inner(void);
//�ı� mmm_state
extern void change_mmm_state_inner(mmm_state_e mmm_sta);
//��ȡ ktp_status
extern mmm_ktp_engine_t get_ktp_status_inner(void);
//�ı� ktp_status
extern void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta);

extern app_info_t *applib_get_app_info_by_type(app_type_e type);
extern app_info_t *applib_get_app_info_by_app_id(uint8 app_id);

extern app_msg_dispatch_handle g_app_msg_dispatch_handle;
extern void set_app_msg_dispatch_handle_inner(app_msg_dispatch_handle dispatch_handle);

#endif
