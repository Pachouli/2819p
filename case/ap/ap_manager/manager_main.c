/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù�������ڡ���ʼ�����̣���ʼ��Common������װ�ؿ���ǰ̨Ӧ�á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "manager.h"

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_PROCESS_MANAGER_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ�õ���ں���
 * \param[in]    δʹ��
 * \param[out]   none
 * \return       ��Զ�����˳�
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
    //STEP 1: ����ϵͳ����Ƶ��Ϊ SYS_CLK_ON_BUSY
    sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);

    //STEP 2: COMMON ��ʼ����ֻ�г�ʼ������ܵ���COMMON�������ӿ�
    common_init();

    libc_printf_info("Enter Manager AP... @%d\n", sys_get_ab_timer());

    //STEP 3: ע��Ϊ MANAGER Ӧ������
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);

    //STEP 4: ��� MANAGER ˽����Ϣ����
    sys_mq_flush (MQ_ID_MNG);

    //STEP 5: ����config����Ӧ��
    sys_exece_ap("config.ap", 0, (void *) CONFIG_MODE_POWER_ON);

    //STEP 6: ������ѭ��
    manager_message_loop();

    return 0;
}

