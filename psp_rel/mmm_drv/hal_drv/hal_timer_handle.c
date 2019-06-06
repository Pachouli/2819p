/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——硬件定时处理，比如定时查询接收UART RX数据。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

int8 hal_timer_id = -1;

sys_hal_callback_t * g_p_sys_hal_cbks;
sys_global_info_t* p_sys_global_info;

void hal_timer_handle(void)
{
    uart_recv_bytes_from_fifo();
}

void hal_timer_init(void)
{
    uart_recv_cache_init();
    hal_timer_id = sys_set_irq_timer1(hal_timer_handle, 0x01);
}

void hal_timer_deinit(void)
{
    sys_del_irq_timer1 (hal_timer_id);
    hal_timer_id = -1;
}

