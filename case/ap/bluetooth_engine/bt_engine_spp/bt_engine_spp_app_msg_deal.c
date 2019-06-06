/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bluetooth_engine.h"
#include "bt_engine_spp_app_msg_deal.h"
#include "bt_engine_spp.h"
#include "bt_engine_passthrough.h"

#if (BT_ENGINE_SPP_APP_MSG_DEAL == 1)

spp_passthrough_st spp_pass;

int32 spp_register(spp_reg_param_st *param)
{
    bt_dev_t *dev;
    libc_memset(&spp_pass, 0, sizeof(spp_passthrough_st));
    spp_pass.hdl = 0;
    spp_pass.cbk = param->cbk;
    spp_pass.rcv_buffer = (buffer_rw_t *) sys_shm_mount(param->shm_id);

    dev = btengine_global_variable.g_cur_rmt_dev;
    if ((dev == NULL) || (dev->p_dev_status == NULL))
    {
        return (int32) NULL;
    }

    if ((dev->p_dev_status->service_connect & SPP_CONNECTED) == SPP_CONNECTED)
    {
        spp_pass.hdl = (uint32) dev;
    }

    return spp_pass.hdl;
}

bool spp_unregister(int32 hdl)
{
    libc_memset(&spp_pass, 0, sizeof(spp_passthrough_st));
    return TRUE;
}

void spp_app_cbk(uint8 ev, void *arg)
{
    if (spp_pass.cbk == NULL)
    {
        return;
    }

    spp_pass.cbk(ev, arg);
}

bool spp_send(uint32 hdl, uint8 *wdata, uint16 len)
{
    if (hdl != spp_pass.hdl)
    {
        return FALSE;
    }

    return SppAppWriteData(wdata, len);
}

bool spp_read(uint32 hdl, uint8 *rdata, uint16 len)
{
    if (hdl != spp_pass.hdl)
    {
        return FALSE;
    }

    if (spp_pass.rcv_buffer == NULL)
    {
        return FALSE;
    }

    if (buffer_rw_read(spp_pass.rcv_buffer, rdata, len) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

bool spp_get_info(uint32 p_hdl,uint8 info_type,void *param)
{
    return TRUE;
}

bool spp_rcv_rmt_data(uint8 *data, uint16 data_len)
{
    receive_rcp_info_t receive_rcp_info;

    if (spp_pass.rcv_buffer == NULL)
    {
        return FALSE;
    }

    //print_receive(data, data_len);

    spp_pass.rcv_info.rcv_len = data_len;
    if (buffer_rw_write(spp_pass.rcv_buffer, data, data_len) == FALSE)
    {
        spp_pass.rcv_info.t_loss += data_len;
        spp_pass.rcv_info.c_loss += data_len;
        BTEG_PRINTF("passth buffer full,closs %d,tloss\n", spp_pass.rcv_info.c_loss, spp_pass.rcv_info.t_loss);
        return FALSE;
    }
    else
    {
        if (spp_pass.rcv_info.c_loss != 0)
        {
            spp_pass.rcv_info.c_loss = 0;
        }
    }

    receive_rcp_info.rcv_len = spp_pass.rcv_info.rcv_len;
    receive_rcp_info.c_loss = spp_pass.rcv_info.c_loss;
    receive_rcp_info.t_loss = spp_pass.rcv_info.t_loss;
    spp_app_cbk(SPP_RCV_RMT_DATA_IND, &receive_rcp_info);
    BTEG_DBG_PRINTF("send SPP_RCV_RMT_DATA_IND\n");

    return TRUE;
}

#endif //end of #if (BT_ENGINE_SPP_APP_MSG_DEAL == 1)
