/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理器主循环，查询是否由创建或销毁应用的消息，并清看门狗。
 * 作者：cailizhen
 ********************************************************************************/

#include  "manager.h"

/******************************************************************************/
/*!
 * \par  Description:
 *  循环获取消息
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

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }
    return RESULT_NULL;
}

