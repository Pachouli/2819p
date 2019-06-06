/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：STUB读写接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_aset_inner.h"

/*! stub读写错误统计 */
static uint8 g_stub_rw_errcnt;

static int32 com_stub_rw_fail_deal(void);

int32 com_stub_ack_ext_cmd_inner(uint16 cmd)
{
    int32 ret_val;
    stub_ext_param_t stub_ext_param;
    uint8 send_cmd_data[8];

    stub_ext_param.opcode = cmd;
    stub_ext_param.payload_len = 0;
    stub_ext_param.rw_buffer = send_cmd_data;

    while (1)
    {
        ret_val = stub_ext_write(&stub_ext_param);

        if (ret_val == 0)
        {
            break;
        }
        else
        {
            if (com_stub_rw_fail_deal() < 0)
            {
                ret_val = -1;
                break;
            }
        }
    }

    if (ret_val == 0)
    {
        g_stub_rw_errcnt = 0;
    }

    return ret_val;
}

int32 com_stub_read_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len)
{
    int32 ret_val;
    stub_ext_param_t ext_param;
    uint8 send_cmd_data[8];

    ext_param.opcode = cmd;
    ext_param.payload_len = 0;
    ext_param.rw_buffer = send_cmd_data;

    ret_val = stub_ext_write(&ext_param);

    if (ret_val == 0)
    {
        ext_param.payload_len = data_len;
        ext_param.rw_buffer = (uint8 *) data_buffer_rev_8bhead_2btail + 2;
        ret_val = stub_ext_read(&ext_param);
        if (ret_val < 0)
        {
            com_stub_rw_fail_deal();
            libc_printf_warning("stub read err.L&C=0x%x\n", (data_len << 16) + cmd);
        }
        else
        {
            //回复ACK
            ret_val = com_stub_ack_ext_cmd_inner((cmd & 0xff00) | STUB_CMD_ACK);
        }
    }
    else
    {
        com_stub_rw_fail_deal();
    }

    if (ret_val == 0)
    {
        g_stub_rw_errcnt = 0;
    }

    return ret_val;
}

int32 com_stub_write_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len)
{
    int32 ret_val;
    int32 timeout;

    stub_ext_param_t ext_param;

    try_again:

    timeout = 0;

    ext_param.opcode = cmd;
    ext_param.payload_len = data_len;
    //预留8字节头部，但是跳过2个字节再传递到驱动，驱动填充接下来的6个字节头部
    ext_param.rw_buffer = (uint8 *) data_buffer_rev_8bhead_2btail + 2;

    ret_val = stub_ext_write(&ext_param);
    if (ret_val == 0)
    {
        //等待ACK 100ms
        while (1)
        {
            uint8 ack_cmd_buf[8]; //应答数据包

            ext_param.payload_len = 0;
            ext_param.rw_buffer = ack_cmd_buf;
            ret_val = stub_ext_read(&ext_param);
            if (ret_val == 0)
            {
                if ((ack_cmd_buf[1] == (uint8)(cmd >> 8)) && (ack_cmd_buf[2] == STUB_CMD_ACK))
                {
                    break;
                }
                else
                {
                    stub_ioctrl_set(RESET_FIFO, 0 , 0);
                    stub_ioctrl_set(RESET_FIFO, 1 , 0);
                }
            }
            else
            {
                if (ret_val < 0)
                {
                    if (com_stub_rw_fail_deal() < 0)
                    {
                        return -1;
                    }
                }
                sys_mdelay(10);
                timeout++;
                if (timeout == 10)
                {
                    //如果数据接收失败，会重新发送数据
                    goto try_again;
                }
            }
        }
    }
    else
    {
        if (com_stub_rw_fail_deal() < 0)
        {
            return -1;
        }
    }

    if (ret_val == 0)
    {
        g_stub_rw_errcnt = 0;
    }

    return ret_val;
}

int32 com_stub_read_cmd_inner(uint16 cmd, void *data_buffer, uint32 data_len)
{
    int32 ret_val;

    ret_val = stub_get_data((uint32) cmd, data_buffer, data_len);
    if (ret_val < 0)
    {
        com_stub_rw_fail_deal();
    }

    if (ret_val == 0)
    {
        g_stub_rw_errcnt = 0;
    }

    return ret_val;
}

int32 com_stub_write_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead, uint32 data_len)
{
    int32 ret_val;

    ret_val = stub_set_data((uint32) cmd, data_buffer_rev_8bhead, data_len);
    if (ret_val < 0)
    {
        com_stub_rw_fail_deal();
    }

    if (ret_val == 0)
    {
        g_stub_rw_errcnt = 0;
    }

    return ret_val;
}

static int32 com_stub_rw_fail_deal(void)
{
    int32 ret_val = 0;

    g_stub_rw_errcnt++;
    if ((get_cable_state_inner() == CABLE_STATE_CABLE_NULL) || (g_stub_rw_errcnt >= 10))
    {
        com_stub_tools_disconnect();

        ret_val = -1;
    }

    return ret_val;
}
