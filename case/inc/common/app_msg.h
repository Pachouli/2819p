/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ü���Ϣͨ����غ궨�壬ö�����ͣ��ṹ�嶨�壬�ⲿ�ӿ������ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "app_msg_type.h"

/*!
 * \brief
 *  sys_msg_t ϵͳ��Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���� */
    msg_apps_type_e type;
} sys_msg_t;

/*!
 * \brief
 *  msg_apps_t Ӧ����Ϣ�ṹ��
 */
typedef struct
{
    /*! Ӧ����Ϣ���� 2 Bytes */
    msg_apps_type_e type;
    /*! Ӧ����Ϣ���� 2 Bytes */
    uint8 para[2];
    /*! Ӧ����Ϣ��չ���� */
    union
    {
        /*! ��Ϣ������ʵ���� */
        uint8 data[4];
        /*! ��Ϣ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
        void *addr;
    } content;
} msg_apps_t;

/*!
 * \brief
 *  msg_reply_type_e ͬ����Ϣ��ִö������
 */
typedef enum
{
    /*! ͬ����Ϣ�޻ظ�������Ӧ���˳�ʱӦ����δ�����ͬ����Ϣ */
    MSG_REPLY_NO_REPLY,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ���ܳɹ���� */
    MSG_REPLY_SUCCESS,
    /*! �ظ�ͬ����Ϣ�����ߣ�ͬ����ϢҪ�������ʧ�� */
    MSG_REPLY_FAILED,
} msg_reply_type_e;

/*!
 * \brief
 *  msg_reply_t ͬ����Ϣ��ִ�ṹ��
 */
typedef struct
{
    /*! ͬ����Ϣ��ִö�����ͣ��μ� msg_reply_type_e ���� */
    uint8 type;
    uint8 reserve[3];
    /*! ��ִ���ݻ�����ָ�룬ָ����Ϣ���ͷ��ĵ�ַ�ռ� */
    void *content;
} msg_reply_t;

/*!
 * \brief
 *  private_msg_t ��Ӧ�ü䣩˽����Ϣ�ṹ��
 */
typedef struct
{
    /*! ˽����Ϣ��Ϣ���� */
    msg_apps_t msg;
    /*! ͬ���ź��� */
    os_event_t *sem;
    /*! ͬ����Ϣ��ִָ�� */
    msg_reply_t *reply;
} private_msg_t;


//������Ϣ�ӿ�
#if 0
extern bool get_app_msg_inner(app_type_e type, private_msg_t *private_msg);
#endif
#define get_app_msg(a,b)            ((bool(*)(app_type_e, private_msg_t *))COM_GET_APP_MSG)((a),(b))

#if 0
extern void message_redeal_app_msg_inner(private_msg_t *p_app_msg);
#endif
#define message_redeal_app_msg(a)   ((bool(*)(private_msg_t *))COM_MESSAGE_REDEAL_APP_MSG)((a))

//������Ϣ�ӿ�
#if 0
extern bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout);
#endif
#define send_sync_msg(a,b,c,d)      ((bool(*)(uint8 , msg_apps_t *, msg_reply_t *, uint32 ))COM_SEND_SYNC_MSG)((a),(b),(c),(d))

#if 0
extern bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg);
#endif
#define send_async_msg(a,b)         ((bool(*)(uint8 , msg_apps_t *))COM_SEND_ASYNC_MSG)((a),(b))

#define  BC_FRONT_APP  0 //ֻ�㲥��ǰ̨Ӧ��
#define  BC_ALL_APP    1 //�㲥������Ӧ��
#if 0
extern bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode);
#endif
#define broadcast_msg(a,b)          ((bool(*)(msg_apps_t *, uint8))COM_BROADCAST_MSG)((a),(b))

//ͬ����Ϣ��ִ�ӿ�
#if 0
extern bool reply_sync_msg_inner(private_msg_t *private_msg);
#endif
#define reply_sync_msg(a)           ((bool(*)(private_msg_t *))COM_REPLY_SYNC_MSG)((a))

typedef app_result_e (*app_msg_dispatch_handle)(private_msg_t *msg);
#if 0
extern void set_app_msg_dispatch_handle_inner(app_msg_dispatch_handle dispatch_handle);
#endif
#define set_app_msg_dispatch_handle(a)  ((void(*)(app_msg_dispatch_handle))COM_SET_APP_MSG_DISPATCH_HANDLE)((a))

#endif
