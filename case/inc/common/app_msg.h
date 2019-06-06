/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用间消息通信相关宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "app_msg_type.h"

/*!
 * \brief
 *  sys_msg_t 系统消息结构体
 */
typedef struct
{
    /*! 应用消息类型 */
    msg_apps_type_e type;
} sys_msg_t;

/*!
 * \brief
 *  msg_apps_t 应用消息结构体
 */
typedef struct
{
    /*! 应用消息类型 2 Bytes */
    msg_apps_type_e type;
    /*! 应用消息参数 2 Bytes */
    uint8 para[2];
    /*! 应用消息扩展参数 */
    union
    {
        /*! 消息内容真实数据 */
        uint8 data[4];
        /*! 消息内容缓冲区指针，指向消息发送方的地址空间 */
        void *addr;
    } content;
} msg_apps_t;

/*!
 * \brief
 *  msg_reply_type_e 同步消息回执枚举类型
 */
typedef enum
{
    /*! 同步消息无回复，用于应用退出时应答尚未处理的同步消息 */
    MSG_REPLY_NO_REPLY,
    /*! 回复同步消息发送者，同步消息要求功能成功完成 */
    MSG_REPLY_SUCCESS,
    /*! 回复同步消息发送者，同步消息要求功能完成失败 */
    MSG_REPLY_FAILED,
} msg_reply_type_e;

/*!
 * \brief
 *  msg_reply_t 同步消息回执结构体
 */
typedef struct
{
    /*! 同步消息回执枚举类型，参见 msg_reply_type_e 定义 */
    uint8 type;
    uint8 reserve[3];
    /*! 回执内容缓冲区指针，指向消息发送方的地址空间 */
    void *content;
} msg_reply_t;

/*!
 * \brief
 *  private_msg_t （应用间）私有消息结构体
 */
typedef struct
{
    /*! 私有消息消息内容 */
    msg_apps_t msg;
    /*! 同步信号量 */
    os_event_t *sem;
    /*! 同步消息回执指针 */
    msg_reply_t *reply;
} private_msg_t;


//接收消息接口
#if 0
extern bool get_app_msg_inner(app_type_e type, private_msg_t *private_msg);
#endif
#define get_app_msg(a,b)            ((bool(*)(app_type_e, private_msg_t *))COM_GET_APP_MSG)((a),(b))

#if 0
extern void message_redeal_app_msg_inner(private_msg_t *p_app_msg);
#endif
#define message_redeal_app_msg(a)   ((bool(*)(private_msg_t *))COM_MESSAGE_REDEAL_APP_MSG)((a))

//发送消息接口
#if 0
extern bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout);
#endif
#define send_sync_msg(a,b,c,d)      ((bool(*)(uint8 , msg_apps_t *, msg_reply_t *, uint32 ))COM_SEND_SYNC_MSG)((a),(b),(c),(d))

#if 0
extern bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg);
#endif
#define send_async_msg(a,b)         ((bool(*)(uint8 , msg_apps_t *))COM_SEND_ASYNC_MSG)((a),(b))

#define  BC_FRONT_APP  0 //只广播给前台应用
#define  BC_ALL_APP    1 //广播给所有应用
#if 0
extern bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode);
#endif
#define broadcast_msg(a,b)          ((bool(*)(msg_apps_t *, uint8))COM_BROADCAST_MSG)((a),(b))

//同步消息回执接口
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
