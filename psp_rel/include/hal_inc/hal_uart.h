
#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private defines

extern void hal_uart_send_bytes(uint8 idx, uint8 *buf, uint32 bytes);
extern uint32 hal_uart_recv_bytes(uint8 idx, buffer_rw_small_fifo_t *p_uart_fifo);
extern void hal_set_uart_tx_init(uint8 idx, uint32 baudrate, uint8 clk_sel);
extern void hal_set_uart_rx_init(uint8 idx, uint32 baudrate, uint8 clk_sel);
extern void hal_set_uart_baudrate(uint8 idx, uint32 baudrate);
extern void hal_uart_ioctl(uint8 idx, uint8 mode);
extern void boot_print(UINT8 *string, UINT32 data);

#endif
