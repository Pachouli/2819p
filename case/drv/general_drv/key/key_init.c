/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������KEY������ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "key_inner.h"

int32 key_timer_id;

/******************************************************************************/
/*!
 * \par  Description:
 *    key initialize
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
 * ������ʼ���������ж�������װ��
 *******************************************************************************/
int key_init(uint32 para)
{
    bool need_usb_detect = ((keymap_para_t *) para)->need_usb_detect;
    key_inner_scan_init((keymap_para_t *) para);

    key_inner_bat_det_init();

    key_inner_device_detect_init(need_usb_detect);

    key_timer_id = sys_set_irq_timer1(key_inner_timer_handle, 0x02);

    return 0;
}

module_init (key_init)
