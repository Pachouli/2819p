/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������KEY�����˳�������KEY������פϵͳ���ýӿ�Ŀǰ������á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "key_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    key exit
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * �����ж�����ж��
 *******************************************************************************/
int key_exit(void)
{
    sys_del_irq_timer1(key_timer_id);
    return 0;
}

module_exit(key_exit)
