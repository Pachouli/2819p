/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动UART模拟按键实现，与 PC 按键录制工具通信。
 * 作者：cailizhen
 ********************************************************************************/

#define USE_KEY_NAME

#include "key_inner.h"

#if (SUPPORT_UART_SIM_KEY == 1)

sim_key_infor_t g_sim_key_infor;
uint8 g_rx_buf[17];

uint32 atoitok(char *digit_str, uint8 digit_len);

uint8 key_get_uart_sim_key(void)
{
    uint8 tmp_key_val = KEY_NULL;
    uint8 rx_bytes,i;

    read_char:

    switch (g_sim_key_infor.status)
    {
        case SIM_KEY_NULL:
        case SIM_KEY_ERROR:
        for(i=0;i<17;i++)
        {
            g_rx_buf[i] = 0;
        }
        rx_bytes = uart_recv_bytes(SIMKEY_UART_RX_IDX, g_rx_buf, 1);
        if (rx_bytes == 1)
        {
            if (g_rx_buf[0] == 'k')
            {
                g_sim_key_infor.status = SIM_KEY_CHECK_TYPE;
            }
            else
            {
                g_sim_key_infor.status = SIM_KEY_ERROR;
                g_sim_key_infor.sendkey = FALSE;
            }
            goto read_char;
        }
        break;

        case SIM_KEY_CHECK_TYPE:
        rx_bytes = uart_recv_bytes(SIMKEY_UART_RX_IDX,g_rx_buf+1,3);
        if(rx_bytes == 3)
        {
            if(g_rx_buf[3] == 'd')
            {
                g_sim_key_infor.status = SIM_KEY_RECE_KEY_DOWN;
            }
            else if(g_rx_buf[3] == 'u')
            {
                g_sim_key_infor.status = SIM_KEY_RECE_KEY_UP;
            }
            else
            {
                g_sim_key_infor.status = SIM_KEY_ERROR;
                g_sim_key_infor.sendkey = FALSE;
            }
        }
        break;

        case SIM_KEY_RECE_KEY_DOWN:
        rx_bytes = uart_recv_bytes(SIMKEY_UART_RX_IDX,g_rx_buf+4,7);
        if(rx_bytes == 7)
        {
            if (libc_memcmp(g_rx_buf, "keydown:", 8) == 0)
            {
                g_sim_key_infor.sim_key.key_id = atoitok(g_rx_buf + 8, 2);
                libc_printf("<SIM KEY,DOWN> %s\n", key_name_list[g_sim_key_infor.sim_key.key_id]);
            }

            g_sim_key_infor.status = SIM_KEY_NULL;
            g_sim_key_infor.sendkey = TRUE;
        }
        break;

        case SIM_KEY_RECE_KEY_UP:
        rx_bytes = uart_recv_bytes(SIMKEY_UART_RX_IDX,g_rx_buf+4,5);
        if(rx_bytes == 5)
        {
            if (libc_memcmp(g_rx_buf, "keyup:", 6) == 0)
            {
                g_sim_key_infor.sim_key.key_id = atoitok(g_rx_buf + 6, 2);
                libc_printf("<SIM KEY,UP> %s\n", key_name_list[g_sim_key_infor.sim_key.key_id]);
            }

            g_sim_key_infor.status = SIM_KEY_NULL;
            g_sim_key_infor.sendkey = FALSE;
        }
        break;
        default:
        break;
    }

    if (TRUE == g_sim_key_infor.sendkey)
    {
        tmp_key_val = g_sim_key_infor.sim_key.key_id;
    }

    return tmp_key_val;
}

//16进制字符串转换为数字
uint32 atoitok(char *digit_str, uint8 digit_len)
{
    uint32 number = 0;
    uint32 temp_value;

    while (digit_len != 0)
    {
        if ((*digit_str >= 'a') && (*digit_str <= 'f'))
        {
            temp_value = 10 + *digit_str - 'a';
        }
        else if ((*digit_str >= 'A') && (*digit_str <= 'F'))
        {
            temp_value = 10 + *digit_str - 'A';
        }
        else
        {
            temp_value = *digit_str - '0';
        }
        number = number * 10 + temp_value;
        digit_str++;
        digit_len--;
    }

    return number;
}

void print_key_name(uint8 key_val)
{
    if (key_val < KEY_VALUE_MAX)
    {
        libc_printf("key:%d:%s\n", key_val, key_name_list[key_val]);
    }
    else
    {
        libc_printf("key:%d:UNKNOWN\n", key_val);
    }
}

uint8 key_init_uart_sim_key(void)
{
    uint8 i;

    set_pad_function(SIMKEY_UART_RX_PIN, SIMKEY_UART_RX_FUNC, NULL, 0);
    uart_rx_set(SIMKEY_UART_RX_IDX, 115200, UART_CLK_AHB);

    libc_printf("SIM KEY STUDY START\n");
    for (i = 0; i < g_keymap_para.key_num; i++)
    {
        print_key_name(g_keymap_para.key_val[i]);
    }
    print_key_name(g_keymap_para.key_onoff);
    print_key_name(KEY_CHADD);
    print_key_name(KEY_CHSUB);
    libc_printf("SIM KEY STUDY END\n");

    return 0;
}

#endif
