/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ����ṹ�嶨�塣
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _COMMON_SYS_INNER_H_
#define _COMMON_SYS_INNER_H_

#include "common_func.h"

/*!
 * \brief
 *  backlight_state_e ����״̬ö������
 */
typedef enum
{
    /*! ��������״̬������ */
    BACKLIGHT_STATE_NORMAL = 0,
    /*! ����䰵״̬������ */
    BACKLIGHT_STATE_DARK = 1,
    /*! ����ر�״̬������ */
    BACKLIGHT_STATE_OFF = 2,
} backlight_state_e;

/*!
 * \brief
 *  app_info_state_t ϵͳ��ǰ״̬�ṹ��
 */
typedef struct
{
    /*! ��״̬��û�����п����� card_state_e ���� */
    uint8 card_state;
    /*! linein״̬��ûlinein����linein���� linein_state_e ���� */
    uint8 linein_state;
    /*! uhost״̬��û��u�̲��룬�в� u�̲��룬�� cable_state_e ���� */
    uint8 uhost_state;
    /*! ����״̬��û�� USB �ߣ��в� USB �ߣ��� cable_state_e ���� */
    uint8 cable_state;
    /*! �����״̬��û�� USB����ߣ��в� USB ����ߣ��� adaptor_state_e ���� */
    uint8 adaptor_state;
    /*! ���������ߣ�״̬ */
    bool antenna_state;
    /*! OK MIC ״̬ */
    bool ok_mic_state;
    /*! UHOST�豸���ͣ�U�̻��߽�����ֻ����� usb_host_in_e ���� */
    uint8 uhost_type;
    /*! LED������Ƿ���� */
    backlight_state_e backlight_state;
    /*! ��ص�����0 ~ 10 */
    uint8 bat_value;
    /*! ��ǰӦ�ù���ӳ������*/
    uint8 cur_func_index;
    /*! ��ǰӦ�ù���ʵ�����*/
    uint8 cur_func_id;
    /*! stub�������� */
    uint8 stub_tools_type;
    /*! ����BT DEBUG (1) ��EJTAG 3��ģʽ (2) */
    uint8 btdbg_ejtag_flag;
    /*! TWS ��������ɫ */
    dev_role_e tws_role;
    /*! TWS ��������λ�� */
    dev_spk_pos_e tws_spk;
    /*! �����ص��� */
    uint8 slave_bat_value;
    /*! UI��ʾ��LED����ܺ�LED�ƣ���ֹ���� */
    uint8 ui_display_forbidden;
} app_info_state_t;

/*!
 * \brief
 *  app_last_state_t ϵͳ���״̬�ṹ�壬�����Զ����״̬�仯������
 */
typedef struct
{
    /*! ��һ��Ӧ�ù������*/
    uint8 last_func_index;
    /*! ���ϵͳ�Ƿ��ڿ���״̬ */
    uint8 last_idle_flag;
} app_last_state_t;

/*!
 *  \brief
 *  g_app_info_state ȫ��ϵͳ��ǰ״̬�ṹ�壬�����ϵͳȫ�ֿռ�
 */
extern app_info_state_t g_app_info_state;

/*!
 *  \brief
 *  g_app_last_state ȫ��ϵͳ���״̬�ṹ��
 */
extern app_last_state_t g_app_last_state;

///�޸�ϵͳLINEIN״̬
static inline void change_linein_state(linein_state_e linein_state)
{
    g_app_info_state.linein_state = linein_state;
}

///�޸�ϵͳ uhost ״̬
static inline void change_uhost_state(uhost_state_e uhost_state)
{
    g_app_info_state.uhost_state = uhost_state;
}

///�޸�ϵͳ USB ����״̬
static inline void change_cable_state(cable_state_e cable_state)
{
    g_app_info_state.cable_state = cable_state;
}

///�޸�ϵͳ USB �����״̬
static inline void change_adaptor_state(adaptor_state_e adaptor_state)
{
    g_app_info_state.adaptor_state = adaptor_state;
}

///��ȡ���������ߣ�״̬
static inline bool get_antenna_state(void)
{
    return g_app_info_state.antenna_state;
}

///�޸Ķ��������ߣ�״̬
static inline void change_antenna_state(bool in_out)
{
    g_app_info_state.antenna_state = in_out;
}

///��ȡMIC״̬
static inline bool get_ok_mic_state(void)
{
    return g_app_info_state.ok_mic_state;
}

///�޸�MIC״̬
static inline void change_ok_mic_state(bool in_out)
{
    g_app_info_state.ok_mic_state = in_out;
}

///��ȡ����״̬��TRUE��ʾ��������FALSE��ʾ������
static inline backlight_state_e get_backlight_state(void)
{
    return g_app_info_state.backlight_state;
}

///�޸ı���״̬
static inline void change_backlight_state(backlight_state_e state)
{
    g_app_info_state.backlight_state = state;
}

//��ȡ��һ��ǰ̨AP fuction index
static inline uint8 get_last_func_index(void)
{
    return g_app_last_state.last_func_index;
}

///������һ��ǰ̨AP fuction index
static inline void set_last_func_index(uint8 index_num)
{
    g_app_last_state.last_func_index = index_num;
}

///��ȡ��ǰǰ̨AP fuction index
static inline uint8 get_cur_func_index(void)
{
    return g_app_info_state.cur_func_index;
}

///���õ�ǰǰ̨AP fuction index
static inline void set_cur_func_index(uint8 index_num)
{
    g_app_info_state.cur_func_index = index_num;
}

///��ȡ��ǰǰ̨AP funciton id
static inline uint8 get_cur_func_id(void)
{
    return g_app_info_state.cur_func_id;
}

#endif
