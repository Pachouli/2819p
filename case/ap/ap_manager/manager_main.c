/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理器入口、初始化流程；初始化Common驱动后装载开机前台应用。
 * 作者：cailizhen
 ********************************************************************************/

#include  "manager.h"

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_PROCESS_MANAGER_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *    应用的入口函数
 * \param[in]    未使用
 * \param[out]   none
 * \return       永远不会退出
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const void *argv[])
{
    //STEP 1: 设置系统运行频率为 SYS_CLK_ON_BUSY
    sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);

    //STEP 2: COMMON 初始化，只有初始化后才能调用COMMON的其他接口
    common_init();

    libc_printf_info("Enter Manager AP... @%d\n", sys_get_ab_timer());

    //STEP 3: 注册为 MANAGER 应用类型
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);

    //STEP 4: 清空 MANAGER 私有消息队列
    sys_mq_flush (MQ_ID_MNG);

    //STEP 5: 创建config开机应用
    sys_exece_ap("config.ap", 0, (void *) CONFIG_MODE_POWER_ON);

    //STEP 6: 进行主循环
    manager_message_loop();

    return 0;
}

