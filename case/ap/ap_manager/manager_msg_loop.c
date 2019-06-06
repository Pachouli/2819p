/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù�������ѭ������ѯ�Ƿ��ɴ���������Ӧ�õ���Ϣ�����忴�Ź���
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "manager.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  ѭ����ȡ��Ϣ
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
app_result_e manager_message_loop(void)
{
    private_msg_t pri_msg;

    while (1)
    {
        clear_watch_dog();

        if ((int) sys_mq_receive(MQ_ID_MNG, &pri_msg) == 0)
        {
            manager_msg_callback(&pri_msg);
        }

        sys_mmu_access_error_check();

        //����10ms�����������
        sys_os_time_dly(1);
    }
    return RESULT_NULL;
}

