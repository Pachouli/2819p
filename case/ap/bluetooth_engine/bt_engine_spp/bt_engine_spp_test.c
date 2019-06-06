/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_spp_test.h"

#if (SPP_PASSTHROUGH_TEST_ENABLE == 1)

extern void spp_auto_send_data(void);

spp_passth_test_st spp_pass_test;

void spp_pass_test_loop(void)
{
    static uint32 last_time = 0;
    uint32 cur_time = sys_get_ab_timer();

    if (cur_time - last_time > 2000)
    {
        if (spp_pass_test.op_hdl == (uint32) NULL)
        {
            spp_passth_start();
        }
        else
        {
            if (spp_pass_test.status == SPP_PASSTH_CONNECTED)
            {
                spp_auto_send_data();
            }
        }
        last_time = cur_time;
    }
    else if (last_time > cur_time)
    {
        last_time = cur_time;
    }
}

static void print_receive(uint8 *data, uint16 data_len)
{
    libc_printf("spp recv: %d\n", data_len);
    libc_printhex(data, (data_len > 32) ? 32 : data_len, 0);
}

void spp_pass_test_event_cbk(uint8 ev, void *arg)
{
    switch (ev)
    {
        case SPP_RCV_RMT_DATA_IND:
        {
            if (spp_pass_test.op_hdl == (uint32) NULL)
            {
                BLE_DBG_PRINTF("spp passth not register\n");
                return;
            }

            uint8* rcv_data;
            receive_rcp_info_t * rcv_info = (receive_rcp_info_t *) arg;
            uint16 rcv_len = rcv_info->rcv_len;
            rcv_data = NEW(rcv_len);
            if (rcv_data == NULL)
            {
                BLE_DBG_PRINTF("rcv_data malloc fail\n");
                return;
            }
            spp_read(spp_pass_test.op_hdl, rcv_data, rcv_len);
            print_receive(rcv_data, rcv_len);
            FREE(rcv_data);
        }
        break;
        default:
        break;
    }
}

bool spp_pass_test_buffer_init(buffer_rw_t *pbuf)
{
    libc_memset(pbuf, 0, sizeof(buffer_rw_t));
    pbuf->capacity = SPP_PASS_TEST_BUFFER_LEN;
    pbuf->raw_data = NEW(SPP_PASS_TEST_BUFFER_LEN);
    if (pbuf->raw_data == NULL)
    {
        return FALSE;
    }
    libc_memset(pbuf->raw_data, 0, pbuf->capacity);
    return TRUE;
}

void spp_pass_test_buffer_deinit(buffer_rw_t *pbuf)
{
    if (pbuf->raw_data != NULL)
    {
        FREE(pbuf->raw_data);
        pbuf->raw_data = NULL;
    }
    libc_memset(pbuf, 0, sizeof(buffer_rw_t));
}

static const uint8 send_char_array[10] =
{ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

void spp_auto_send_data(void)
{
    static uint8 i = 0;

    uint16 data_len = 200;
    uint8 *data = NEW(data_len);
    if (data == NULL)
    {
        BLE_DBG_PRINTF("spp send data malloc fail\n");
        return;
    }
    libc_memset(data, send_char_array[i], data_len);

    if (spp_pass_test.op_hdl == (uint32) NULL)
    {
        BLE_DBG_PRINTF("op_hdl in null\n");
        return;
    }

    if (spp_send(spp_pass_test.op_hdl, data, data_len) == FALSE)
    {
        BLE_DBG_PRINTF("Spp send fail\n");
    }

    i++;
    if (i >= 10)
    {
        i = 0;
    }

    FREE(data);
}

bool spp_passth_start(void)
{
    if (spp_pass_test.status == SPP_PASSTH_INIT)
    {
        if (spp_pass_test_buffer_init(&spp_pass_test.rcv_buf) == FALSE)
        {
            return FALSE;
        }

        if (sys_shm_creat(SHARE_MEM_ID_BLESPP_INPIPE1, &spp_pass_test.rcv_buf, sizeof(buffer_rw_t)) == -1)
        {
            BLE_PRINTF_ERROR("spp_pass_test_buf shm create fail\n");
            while (1)
                ;
        }
        spp_pass_test.status = SPP_PASSTH_READY;
    }
    else if (spp_pass_test.status == SPP_PASSTH_READY)
    {
        spp_reg_param_st spp_reg_param;
        spp_reg_param.shm_id = SHARE_MEM_ID_BLESPP_INPIPE1;
        spp_reg_param.cbk = spp_pass_test_event_cbk;

        spp_pass_test.op_hdl = spp_register(&spp_reg_param);
        if (spp_pass_test.op_hdl == (uint32) NULL)
        {
            //SPP_DBG_PRINTF("register spp pass fail\n");
            return FALSE;
        }
        spp_pass_test.status = SPP_PASSTH_CONNECTED;
    }
    return TRUE;
}

void spp_passth_end(void)
{
    if (spp_pass_test.status == SPP_PASSTH_CONNECTED)
    {
        spp_pass_test.op_hdl = (uint32) NULL;
        spp_pass_test.status = SPP_PASSTH_READY;
    }
    else if (spp_pass_test.status == SPP_PASSTH_CONNECTED)
    {
        spp_pass_test_buffer_deinit(&spp_pass_test.rcv_buf);
        sys_shm_destroy(SHARE_MEM_ID_BLESPP_INPIPE1);
        spp_pass_test.status = SPP_PASSTH_INIT;
    }
}

#endif //#if (SPP_PASSTHROUGH_TEST_ENABLE == 1)

