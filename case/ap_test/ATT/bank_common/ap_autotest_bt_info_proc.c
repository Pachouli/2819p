#include "ap_test_head.h"

 bool act_test_get_btinfo(btengine_setinfo_t *p_btinfo)
{
    bool ret_val;
    msg_apps_t msg;

    //创建蓝牙后台
    com_creat_bt_engine(1);

   //消息类型(即消息名称)
    msg.type = MSG_AUTOTEST_GET_BTINFO_SYNC;
    msg.content.addr = p_btinfo;
    //发送同步消息
    ret_val = send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return ret_val;
}


 bool act_test_modify_btinfo(btengine_setinfo_t *p_btinfo)
{
    bool ret_val;
    msg_apps_t msg;

    //创建蓝牙后台
    com_creat_bt_engine(1);

    //消息类型(即消息名称)
    msg.type = MSG_AUTOTEST_SET_BTINFO_SYNC;
    msg.content.addr = p_btinfo;
    //发送同步消息
    ret_val = send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    return ret_val;
}
