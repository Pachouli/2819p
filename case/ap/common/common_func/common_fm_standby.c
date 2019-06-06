/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������װ����ʼ��֮��ʹ֮����Standby״̬����ȷ��FM�������̬������Ӱ��
 *       LINEIN����Ƶָ�ꡣ
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "common_func_inner.h"
#include  "fm_common.h"

#if (SUPPORT_FM_FUNC == 1)
static void _kill_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //�鿴��ǰ�����Ƿ�Ϊmusic
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //ע��radio��̨
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

static void _create_engine(uint8 mode)
{
    msg_apps_t msg;

    //����radio ��̨�������
    msg.type = MSG_CREAT_APP_SYNC;
    msg.para[0] = APP_ID_FMENGINE;
    if (mode == FM_STANDBY_MODE)
    {
        msg.para[1] = FM_EG_ENTER_STANDBY;
    }
    else
    {
        msg.para[1] = FM_EG_ENTER_IDLE;
    }
    msg.content.addr = NULL;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}
#endif

void common_fmdrv_standby_inner(uint8 mode)
{
#if (SUPPORT_FM_FUNC == 1)
    if (com_ap_switch_check_inner(APP_FUNC_RADIO) == TRUE)
    {
        _create_engine(mode);
        _kill_engine();
    }
#endif
}
