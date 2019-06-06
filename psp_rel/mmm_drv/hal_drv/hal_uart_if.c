/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——UART TX/RX 相关接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

#define UART_RECV_SIZE  20

typedef struct
{
    buffer_rw_small_fifo_t uart_recv_cache[2];
    uint8 uart_recv_buffer[2][UART_RECV_SIZE];
} hal_uart_rx_info_t;

hal_uart_rx_info_t *p_hal_uart_rx_info;

void uart_recv_cache_init(void)
{
    p_hal_uart_rx_info = (hal_uart_rx_info_t *)sys_malloc(sizeof(hal_uart_rx_info_t));
    if (p_hal_uart_rx_info == NULL)
    {
        libc_printf_error("p_hal_uart_rx_info malloc fail!\n");
        while (1);
    }
    libc_memset(p_hal_uart_rx_info, 0, sizeof(hal_uart_rx_info_t));
    p_hal_uart_rx_info->uart_recv_cache[0].capacity = UART_RECV_SIZE;
    p_hal_uart_rx_info->uart_recv_cache[0].raw_data = p_hal_uart_rx_info->uart_recv_buffer[0];
    p_hal_uart_rx_info->uart_recv_cache[1].capacity = UART_RECV_SIZE;
    p_hal_uart_rx_info->uart_recv_cache[1].raw_data = p_hal_uart_rx_info->uart_recv_buffer[1];
}

void uart_recv_bytes_from_fifo(void)
{
    hal_uart_recv_bytes(0, &(p_hal_uart_rx_info->uart_recv_cache[0]));
    hal_uart_recv_bytes(1, &(p_hal_uart_rx_info->uart_recv_cache[1]));
}

uint32 uart_recv_bytes_inner(uint8 idx, uint8 *buf, uint32 bytes)
{
    uint32 temp_flag;
    uint32 ret = 0;
    buffer_rw_small_fifo_t *p_uart_recv_cache;

    temp_flag = sys_local_irq_save();

    p_uart_recv_cache = &(p_hal_uart_rx_info->uart_recv_cache[idx]);

    if (p_uart_recv_cache->length > 0)
    {
        if (bytes == (uint32)-1)
        {
            bytes = p_uart_recv_cache->length;
        }

        if (bytes <= p_uart_recv_cache->length)
        {
            uint16 read_len, rem_len;

            if ((p_uart_recv_cache->read_ptr + bytes) > p_uart_recv_cache->capacity)
            {
                read_len = p_uart_recv_cache->capacity - p_uart_recv_cache->read_ptr;
                rem_len = bytes - read_len;
            }
            else
            {
                read_len = bytes;
                rem_len = 0;
            }

            libc_memcpy(buf, p_uart_recv_cache->raw_data + p_uart_recv_cache->read_ptr, read_len);

            if (rem_len != 0)
            {
                libc_memcpy(buf + read_len, p_uart_recv_cache->raw_data, rem_len);
            }

            p_uart_recv_cache->read_ptr += bytes;
            p_uart_recv_cache->read_ptr %= p_uart_recv_cache->capacity;
            p_uart_recv_cache->length -= bytes;

            ret = bytes;
        }
    }

    sys_local_irq_restore(temp_flag);

    return ret;
}

