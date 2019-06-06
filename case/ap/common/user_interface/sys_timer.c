/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������COMMON ������ʱ������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*!
 *  \brief
 *  sys_counter_timer_id ϵͳ��ʱ���ܣ��������⣬������ʡ��ػ�����ʱ�ػ����ȣ���ʱ�� timer id
 */
static int8 sys_counter_timer_id = -1;

/*! �豸�Ȱβ��ⶨʱ�� */
static int8 sys_hot_detect_timer_id = -1;

/*! Ӳ����鶨ʱ����-1��ʾΪ���� */
static int8 sys_hw_timer_id = -1;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ����ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ�ý���Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ���� config.app��ǰ̨Ӧ���ڽ���ʱ������øýӿڣ����򱳹⣬������ʡ��ػ���
 *      ��ʱ�ػ���״̬�����£��ȵȹ��ܽ��޷�ʹ�á�
 *******************************************************************************/
void sys_timer_init(void)
{
    sys_hw_timer_id = sys_set_irq_timer1(hardware_timer_500ms_isr, 50);
    hardware_timer_500ms_isr(); //������ִ��һ��

    sys_counter_var_init();
    sys_counter_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SYS, APP_TIMER_ATTRB_CONTROL, 500, sys_counter_handle);

    sys_hot_detect_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SYS, APP_TIMER_ATTRB_CONTROL, 200, sys_hot_detect_handle_inner);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    ɾ��ϵͳ��ʱ��ʱ���� headbar ���ڸ�����ʱ������ǰ̨Ӧ���˳�Ӧ��ʱ���á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  �˳�Ӧ��ʱ����Ҫ���� sys_counter_exit����ɾ�� sys_counter_init ��������ʱ����
 *******************************************************************************/
void sys_timer_exit(void)
{
    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, sys_counter_timer_id);
    sys_counter_timer_id = -1;

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, sys_hot_detect_timer_id);
    sys_hot_detect_timer_id = -1;

    sys_del_irq_timer1(sys_hw_timer_id);
}

/*! \endcond */
