/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ����洦�������������棬ɱ�����档
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"

bool create_app_engine(uint8 engine_id, uint8 para)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    bool bret;

    //�ȼ���Ƿ���װ��������̨����,������ɱ��
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    //����Ϣ����
    msg.para[0] = engine_id;
    msg.para[1] = para;
    msg.content.addr = NULL;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}




 bool kill_current_app_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    bool bret = TRUE;

    //�ȼ���Ƿ���װ��������̨����,������ɱ��
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return bret;
}

