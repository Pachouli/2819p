/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：引擎处理，包括创建引擎，杀死引擎。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"

bool create_app_engine(uint8 engine_id, uint8 para)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    bool bret;

    //先检查是否已装载其他后台引擎,如有先杀掉
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    //无消息内容
    msg.para[0] = engine_id;
    msg.para[1] = para;
    msg.content.addr = NULL;

    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}




 bool kill_current_app_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    bool bret = TRUE;

    //先检查是否已装载其他后台引擎,如有先杀掉
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //发送同步消息
        bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return bret;
}

