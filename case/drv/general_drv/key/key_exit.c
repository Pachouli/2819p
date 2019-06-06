/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动退出，由于KEY驱动常驻系统，该接口目前不会调用。
 * 作者：cailizhen
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
 * 负责中断向量卸载
 *******************************************************************************/
int key_exit(void)
{
    sys_del_irq_timer1(key_timer_id);
    return 0;
}

module_exit(key_exit)
