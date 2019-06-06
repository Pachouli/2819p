/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������ʱ����غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _APP_TIMER_H_
#define _APP_TIMER_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

/*!
 * \brief
 *  Ӧ�ü���ʱ���������� tag id�����ڰѶ�ʱ��������ĳ�����������õع���Ӧ�������ж�ʱ����
 */
/*! ������ tag */
#define APP_TIMER_TAG_MAIN          0x00
/*! �ӳ��� tag ��ʼID */
#define APP_TIMER_TAG_SUB           0x80
/*! ϵͳ���� tag�����ⳡ�����κγ����¶���ִ�� */
#define APP_TIMER_TAG_SYS           0xff

/*!
 * \brief
 *  timer_type_e ��ʱ������ö�ٽṹ
 */
typedef enum
{
    /*! ��ͨģʽ�Ķ�ʱ�����Զ����ڷ��� */
    TIMER_TYPE_NORMAL,
    /*! ������ʱ����ֻ��ʱһ�κ����TIMER_STATE_STOPED ״̬ */
    TIMER_TYPE_SINGLE_SHOT,
} timer_type_e;

/*!
 * \brief
 *  timer_state_e ��ʱ��״̬ö�ٽṹ
 */
typedef enum
{
    /*! ��־����״̬��������������ʱ�� */
    TIMER_STATE_NOUSED = 0,
    /*! ������ʱ״̬ */
    TIMER_STATE_RUNNING = 1,
    /*! ֹͣ��ʱ״̬��ֻ��ͨ��restart���¿�ʼ��ʱ */
    TIMER_STATE_STOPED = 2,
} timer_state_e;

/*!
 * \brief
 *  timer_attrb_e ��ʱ������ö�ٽṹ����������ʱ����Դ���г�ʼ����
 */
typedef enum
{/*! NOTE: �����Է����� */
    /*! UI ����ʱ�����ڱ���Ϩ�����ִ�� ISR */
    APP_TIMER_ATTRB_UI,
    /*! control ����ʱ�����κ�ʱ�򶼻�ִ�� ISR */
    APP_TIMER_ATTRB_CONTROL,
} timer_attrb_e;

/*!
 * \brief
 *  timer_proc ��ʱ����������
 */
typedef void (*timer_proc)(void);

/*!
 * \brief
 *  app_timer_t ��ʱ������ṹ��
 */
typedef struct
{
    /*! ��ʱ��״̬����timer_state_e ���壬TIMER_STATE_NOUSED��ʾ���� */
    uint8 state;
    /*! ��ʱ�����ԣ���timer_attrb_e ���� */
    uint8 attrb;
    /*! ��ʱ�� ��ʱ���ڣ���λΪ 1ms */
    uint16 timeout;
    /*! ��ʱ�����ͣ���timer_type_e ���� */
    uint8 type;
    /*! ��ʱ��������־ tag */
    uint8 tag;
    uint8 reserve[2];
    /*! ��ʱ�� ��һ�γ�ʱ����ʱ��㣬��λΪ 1ms */
    uint32 timeout_expires;
    /*! ��ʱ�� �������� */
    timer_proc func_proc;
} app_timer_t;

/*! ��ʱ������ID */
typedef enum
{
    /*! ���̹����� */
    APP_TIMER_GROUP_PMNG = 0,
    /*! ǰ̨Ӧ�ã�gui���̣� */
    APP_TIMER_GROUP_GUI = 1,
    /*! ��̨Ӧ�ã�console���̣� */
    APP_TIMER_GROUP_CONSOLE = 2,
    /*! ��̨Ӧ�ã�BTENGINE���̣� */
    APP_TIMER_GROUP_BTENGINE = 3,
    /*! COMMON ģ��*/
    APP_TIMER_GROUP_COMMON = 4,
    /*! �������� */
    APP_TIMER_GROUP_MAX,
} app_timer_group_id_e;


//�ӿ�����
#if 0
extern int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
#endif
#define set_app_timer(a, b, c, d, e)              ((int8(*)(app_timer_group_id_e , uint8 , timer_attrb_e , uint16 , timer_proc ))COM_SET_APP_TIMER)((a),(b),(c),(d),(e))

#if 0
extern int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
#endif
#define set_single_shot_app_timer(a, b, c, d, e)  ((int8(*)(app_timer_group_id_e , uint8 , timer_attrb_e , uint16 , timer_proc ))COM_SET_SINGLE_SHOT_APP_TIMER)((a),(b),(c),(d),(e))

#if 0
extern bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout);
#endif
#define modify_app_timer(a, b, c)                 ((bool(*)(app_timer_group_id_e , int8 , uint16 ))COM_MODIFY_APP_TIMER)((a),(b),(c))

#if 0
extern bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define stop_app_timer(a, b)                      ((bool(*)(app_timer_group_id_e , int8 ))COM_STOP_APP_TIMER)((a),(b))

#if 0
extern bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define restart_app_timer(a, b)                   ((bool(*)(app_timer_group_id_e , int8 ))COM_RESTART_APP_TIMER)((a),(b))

#if 0
extern bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id);
#endif
#define standby_restart_all_app_timer(a)          ((bool(*)(app_timer_group_id_e ))COM_STANDBY_RESTART_ALL_APP_TIMER)((a))

#if 0
extern bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define kill_app_timer(a,b)                       ((bool(*)(app_timer_group_id_e , int8 ))COM_KILL_APP_TIMER)((a),(b))

#if 0
extern bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count);
#endif
#define init_app_timers(a,b)                      ((bool(*)(app_timer_group_id_e , uint8 ))COM_INIT_APP_TIMERS)((a),(b))

#if 0
extern void change_app_timer_tag_inner(uint8 tag);
#endif
#define change_app_timer_tag(a)                   ((void(*)(uint8))COM_CHANGE_APP_TIMER_TAG)((a))

#endif

