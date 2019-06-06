
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

uint8 uart_en[2];

void hal_uart_send_bytes(uint8 idx, uint8 *buf, uint32 bytes)
{
    uint32 uart_base = (idx == 0)? UART0_BASE: UART1_BASE;
    uint32 i;

    if (uart_en[idx] == 0)
    {
        return;
    }

    for (i = 0; i < bytes; i++)
    {
        while (0 != (act_readl(uart_base + (UART0_STA - UART0_BASE)) & (1 << UART0_STA_TFFU)))
        {
            //loop until tx fifo not full
        }

        act_writel(buf[i], uart_base + (UART0_TXDAT - UART0_BASE));
    }
}

uint32 hal_uart_recv_bytes(uint8 idx, buffer_rw_small_fifo_t *p_uart_fifo)
{
    uint8 uart_rx_dat;
    uint32 uart_rx_bytes = 0;
    uint32 uart_base = (idx == 0)? UART0_BASE: UART1_BASE;

    if (uart_en[idx] == 0)
    {
        return 0;
    }

    while ((act_readl(uart_base + (UART0_STA - UART0_BASE)) & (1<<UART0_STA_RFEM)) == 0)//RX FIFO NOT EMPTY
    {
        uart_rx_dat = (uint8) act_readl(uart_base + (UART0_RXDAT - UART0_BASE));

        if ((p_uart_fifo->length + 1) <= p_uart_fifo->capacity)
        {
            p_uart_fifo->raw_data[p_uart_fifo->write_ptr] = uart_rx_dat;
            p_uart_fifo->write_ptr++;
            p_uart_fifo->write_ptr %= p_uart_fifo->capacity;
            p_uart_fifo->length++;
            uart_rx_bytes++;
        }
        else
        {
            p_uart_fifo->full_error = TRUE;
        }
    }

    return uart_rx_bytes;
}

void hal_uart_open(uint8 idx)
{
    if (uart_en[idx] == 1)
    {
        return;
    }

    uart_en[idx] = 1;

    //UART clock enable
    if (idx == 0)
    {
        act_writel(act_readl(MRCR) & ~(1 << MRCR_UART0_RESET), MRCR);
        act_writel((act_readl(CMU_UARTCLK) & ~(1 << CMU_UARTCLK_SEL0)), CMU_UARTCLK);
        act_writel(act_readl(CMU_DEVCLKEN) | (1 << CMU_DEVCLKEN_UART0CLKEN), CMU_DEVCLKEN);
        act_writel(act_readl(MRCR) | (1 << MRCR_UART0_RESET), MRCR);
    }
    else
    {
        act_writel(act_readl(MRCR) & ~(1 << MRCR_UART1_RESET), MRCR);
        act_writel((act_readl(CMU_UARTCLK) & ~(1 << CMU_UARTCLK_SEL1)), CMU_UARTCLK);
        act_writel(act_readl(CMU_DEVCLKEN) | (1 << CMU_DEVCLKEN_UART1CLKEN), CMU_DEVCLKEN);
        act_writel(act_readl(MRCR) | (1 << MRCR_UART1_RESET), MRCR);
    }
}

void hal_set_uart_tx_init(uint8 idx, uint32 baudrate, uint8 clk_sel)
{
    uint32 cpu_sr, temp;
    uint32 uart_base = (idx == 0)? UART0_BASE: UART1_BASE;

    if (clk_sel == 0xff) //0xff表示不需要进行Uart控制器配置
    {
        return ;
    }

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    hal_uart_open(idx);

    temp = act_readl(uart_base + (UART0_CTL - UART0_BASE));
    temp |= (1<<UART0_CTL_TX_FIFO_EN);//enable TX FIFO
    temp |= (1<<UART0_CTL_EN);//enable UART
    temp &= ~(UART0_CTL_PRS_MASK);//no parity
    temp &= ~(1<<UART0_CTL_STPS);//1 stop bit
    temp &= ~(UART0_CTL_DWLS_MASK);
    temp |= (0x03<<UART0_CTL_DWLS_SHIFT);//8bits

    //AHB bus access
    if (clk_sel == 0)
    {
        temp &= ~(1<<UART0_CTL_TXAHB_DMA_SEL);
        temp &= ~(1<<UART0_CTL_TXDE);
        temp &= ~(UART0_CTL_TDIC_MASK);
    }
    //DMA bus access
    else
    {
        temp |= (1<<UART0_CTL_TXAHB_DMA_SEL);
        temp |= (1<<UART0_CTL_TXDE);
        temp &= ~(UART0_CTL_TDIC_MASK);
        temp |= (0x02<<UART0_CTL_TDIC_SHIFT);
    }

    act_writel(temp | (0x01 << 30), uart_base + (UART0_CTL - UART0_BASE));

    //UART baudrate = Colck_source/BaudRate diveder
    act_writel(0x00E200E2, uart_base + (UART0_BR - UART0_BASE));         //26M/115200

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
}

void hal_set_uart_rx_init(uint8 idx, uint32 baudrate, uint8 clk_sel)
{
    uint32 cpu_sr, temp;
    uint32 uart_base = (idx == 0)? UART0_BASE: UART1_BASE;

    if (clk_sel == 0xff) //0xff表示不需要进行Uart控制器配置
    {
        return ;
    }

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    hal_uart_open(idx);

    temp = act_readl(uart_base + (UART0_CTL - UART0_BASE));
    temp |= (1<<UART0_CTL_RX_FIFO_EN);//enable RX FIFO
    temp |= (1<<UART0_CTL_EN);//enable UART
    temp &= ~(UART0_CTL_PRS_MASK);//no parity
    temp &= ~(1<<UART0_CTL_STPS);//1 stop bit
    temp &= ~(UART0_CTL_DWLS_MASK);
    temp |= (0x03<<UART0_CTL_DWLS_SHIFT);//8bits

    //AHB bus access
    if (clk_sel == 0)
    {
        temp &= ~(1<<UART0_CTL_RXAHB_DMA_SEL);
        temp &= ~(1<<UART0_CTL_RXDE);
        temp &= ~(UART0_CTL_RDIC_MASK);
    }
    //DMA bus access
    else
    {
        temp |= (1<<UART0_CTL_RXAHB_DMA_SEL);
        temp |= (1<<UART0_CTL_RXDE);
        temp &= ~(UART0_CTL_RDIC_MASK);
        temp |= (0x02<<UART0_CTL_RDIC_SHIFT);
    }

    act_writel(temp | (0x01 << 31), uart_base + (UART0_CTL - UART0_BASE));

    //UART baudrate = Colck_source/BaudRate diveder
    act_writel(0x00E200E2, uart_base + (UART0_BR - UART0_BASE));         //26M/115200

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);
}

void hal_set_uart_baudrate(uint8 idx, uint32 baudrate)
{

}

void hal_uart_ioctl(uint8 idx, uint8 mode)
{

}
