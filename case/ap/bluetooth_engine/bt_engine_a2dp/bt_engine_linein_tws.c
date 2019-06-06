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
 * \file     bt_engine_linein_tws.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

static buffer_rw_t linein_tws_rw_buffer;
static uint8 frame_count;

void linein_tws_buffer_init(void)
{
    buffer_rw_t *buffer_rw;

    buffer_rw = &linein_tws_rw_buffer;
    buffer_rw->raw_data = SBC_DIRECT_INPUT_BUFFER;
    buffer_rw->capacity = A2DP_SBC_DIRECT_INPUT_BUFFER_LEN;
    buffer_rw->write_ptr = 0;
    buffer_rw->read_ptr = 0;
    buffer_rw->length = 0;
    buffer_rw->full_error = FALSE;
    frame_count = 0;
}

extern uint16 get_frame_data(void *data_addr, uint16 data_len);

//必须严格按照一帧一帧的发
void linein_tws_send_one_frame(uint8 *frame_data, uint16 data_len)
{
    uint32 temp_flag;

    temp_flag = sys_local_intc_save(0x01);

    //普通蓝牙推歌中，一般一次发送5帧，所以为保持一致性，先缓冲5帧，满5帧后立即发送出去
    if (frame_count < 5)
    {
        if (buffer_rw_write(&linein_tws_rw_buffer, frame_data, data_len) == FALSE)
        {
            A2DP_PRINTF_WARNNING("linein tws input buf full\n");
            linein_tws_buffer_init();
        }
        else
        {
            frame_count++;
        }
    }

    //满5帧后立即发送出去
    if (frame_count == 5)
    {
        get_frame_data(linein_tws_rw_buffer.raw_data, linein_tws_rw_buffer.length);
        linein_tws_buffer_init();
    }

    sys_local_intc_restore(temp_flag);
}

