/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "bluetooth_engine.h"

//发送sco数据包 —— 准备数据发送
bool send_one_sco_package(void)
{
    bt_dev_t *dev = btengine_global_variable.g_cur_rmt_dev;
    uint8 *p_buf;
    uint8 hci_header[2];
    uint8 sco_out_payload_size = SCO_OUT_PAYLOAD_SIZE;
    if (btengine_param.sco_postdeal_enable == TRUE)
    {
        sco_out_payload_size = SCO_OUT_PAYLOAD_SIZE / 2;
    }

    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
    {
        hci_header[0] = (uint8) ((dev->p_dev_status->sco_handle_setup) & 0xff);
        hci_header[1] = (uint8) ((dev->p_dev_status->sco_handle_setup) >> 8);
        p_buf = con_buf_req(HCIT_SCODATA, hci_header, sco_out_payload_size);
        if (p_buf != NULL)
        {
            if (buffer_rw_read(&g_sco_output_buf, p_buf, sco_out_payload_size) == FALSE)
            {
                libc_memset(p_buf, 0x00, sco_out_payload_size);
                BTEG_PRINTF_WARNNING("host empty\n");
            }

            con_buf_commit(HCIT_SCODATA, p_buf, sco_out_payload_size);
            return TRUE;
        }
        else
        {
            //TODO 临时处理，避免死机
            g_sco_output_buf.read_ptr += sco_out_payload_size;
            if (g_sco_output_buf.capacity != 0)
            {
                g_sco_output_buf.read_ptr %= g_sco_output_buf.capacity;
            }
            g_sco_output_buf.length -= sco_out_payload_size;

            BTEG_PRINTF_WARNNING("con_buf_req null\n");
            return FALSE;
        }
    }
    return FALSE;
}

void deal_micout_sco_data(void)
{
    uint32 temp_flag;
    uint8 sco_out_payload_size = SCO_OUT_PAYLOAD_SIZE;
    if (btengine_param.sco_postdeal_enable == TRUE)
    {
        sco_out_payload_size = SCO_OUT_PAYLOAD_SIZE / 2;
    }

    temp_flag = sys_local_intc_save(0x01);
    //发送 micout sco 数据包
    while (g_sco_output_buf.length >= sco_out_payload_size)
    {
        if (send_one_sco_package() == FALSE)
        {
            break;
        }
    }
    sys_local_intc_restore(temp_flag);
}
