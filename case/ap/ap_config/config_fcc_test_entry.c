
#include  "ap_config.h"

#ifdef FCC_TEST_MODE
void config_fcc_test_entry(void)
{
    libc_printf("FCC TEST ENTER...\n");
    sys_os_time_dly(5);

    //配置PIN脚 GPIO5 -> Rx, GPIO6 -> Tx
    set_pad_function(PAD_GPIO5, GPIO5_MFP_UART0_RX, NULL, 0);
    uart_rx_set(0, 115200, UART_CLK_AHB);
    set_pad_function(PAD_GPIO6, GPIO6_MFP_UART0_TX, NULL, 0);
    uart_tx_set(0, 115200, UART_CLK_AHB);

    //装载FCC测试驱动，Init接口即进行FCC测试
    sys_adjust_clk(FREQ_54M, SET_SYS_CLK);
    sys_drv_install(DRV_GROUP_BT_CON, 0, "fcc.drv"); //0表示Uart0，如果配置为Uart1，那么需要传递1给fcc驱动
}
#endif
