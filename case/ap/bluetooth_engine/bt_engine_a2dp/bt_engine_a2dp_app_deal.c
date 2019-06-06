/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "./../bluetooth_engine.h"

//一般都是一次刚好一帧
uint8 * get_aac_frame_data(void *data_addr)
{
    uint8 *p_data = (uint8 *) data_addr;

    return p_data;
}

uint8 * get_sbc_frame_data(void *data_addr)
{
    uint8 *p_data = (uint8 *) data_addr;

    /*
     ______________________   _____________   _______________________________________________
     | Media Packet Header |  | CP(1 byte) |  | Media Payload (Header + Frame + Frame + ...) |
     |_____________________|  |____________|  |______________________________________________|

     -------------------------|data ->  .......................................................
     */
    /* cut CP Header.
     The bit0 field of CP Headers is used for the L-bit and the bit 1 field of CP Header is used for the Cp-bit. */
    /*判断当前设备以及设备状态是否存在可以抽象为统一接口，存在多处调用。*/
    if ((btengine_global_variable.g_cur_rmt_dev != NULL)
            && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL))
    {
        if (btengine_global_variable.g_cur_rmt_dev->p_dev_status->a2dp_source.cp_active == BTA_AV_CP_SCMS_T_ID)
        {
            p_data += 1;
        }
    }

    return p_data;
}

void read_avdtp_sound_hook(void)
{
    uint16 pause_limit = 300;
    uint16 start_limit = 300;
    bt_dev_t *dev;
    dev = btengine_global_variable.g_cur_rmt_dev;
    btengine_global_variable.last_avdtp_data_timer = sys_get_ab_timer();

#if (SUPPORT_ONLY_BT_TEST == 0)
    g_p_info->avdtp_data_flag = TRUE;
#endif

    switch (get_cur_device_status(dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        case STATUS_A2DP_PAUSE:
        case STATUS_A2DP_PLAY:
        /* sangsung S8538 , only remove SCO, but not CIEV status, when hangup call.  */
        case STATUS_CALL_PHONE:
        if (g_p_info->enter_hfp_flag == TRUE)
        {
            break;
        }
        //BTEG_DBG_PRINTF("\nA2DP PCM Stream\n");

        //当前尚未创建解码线程并且处理全功能模式，请求创建解码线程
        if (btengine_global_variable.g_sbc_thread_exist == 0)
        {
            pause_cnt = 0;
            if (get_cur_device_status(dev) == STATUS_A2DP_PAUSE)
            {
                if ((g_p_avrcp->EventMask_CT & (1 << AVRCP_EVENT_PLAYBACK_STATUS_CHANGED)) != 0)
                {
                    start_limit = APPLE_A2DP_PAUSE_TIME;
                }
                start_cnt++;
                if (start_cnt >= start_limit)
                {
                    set_exit_schedule_flag(TRUE);
                    start_cnt = 0;
                }
            }
            else
            {
                //SBC解码线程只在btplay中已打开A2DP服务情况下支持，其他状态下不需要做处理，所以提前结束本次任务
                set_exit_schedule_flag(TRUE);
                start_cnt = 0;
            }
        }
        else
        {
            if (get_cur_device_status(dev) == STATUS_A2DP_PAUSE)
            {
                pause_cnt++;
                if (pause_cnt >= pause_limit)
                {
                    pause_cnt = 0;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                    if (dev->dev_type == TWS_DEV)
                    {
                        A2DP_TWS_PRINTF("pause -> start\n");
                    }
                    else
#endif
                    {
                        A2DP_PRINTF("pause -> start\n");
                    }
                    set_cur_device_status(dev, STATUS_A2DP_PLAY);

                }
            }
        }

        break;
        case STATUS_CALL_INCOMING:
        break;
        case STATUS_CALL_OUTGOING:
        break;
        case STATUS_CALL_HFP:
        break;
        //    case STATUS_CALL_PHONE:
        //        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
}

//建立a2dp 连接通道
bool app_msg_deal_a2dp_connect(bt_dev_t *dev)
{
    bool ret = FALSE;
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        if (dev == NULL)
        {
            return ret;
        }

        if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
        {
            return ret;
        }

        if ((dev->p_dev_status == NULL) || (dev->p_dev_status->a2dp_source.stream_handle == NULL))
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (dev->dev_type == TWS_DEV)
            {
                A2DP_TWS_DBG_PRINTF("before avdtp setup\n");
            }
            else
#endif
            {
                A2DP_DBG_PRINTF("before avdtp setup\n");
            }

            bt_set_visual_mode(GAP_VM_PAIR);

            //0: snk 1:src
            ret = A2dpConnect(0, (uint8 *) &dev->dev_info.bd_addr);
        }
        break;
        case STATUS_LINKED:
        break;
        case STATUS_A2DP_PLAY:
        break;
        case STATUS_A2DP_PAUSE:
        break;
        case STATUS_CALL_INCOMING:
        break;
        case STATUS_CALL_OUTGOING:
        break;
        case STATUS_CALL_HFP:
        break;
        case STATUS_CALL_PHONE:
        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
    return ret;
}

uint8 app_message_deal_a2dp(private_msg_t *pri_msg)
{
    uint8 rst;
    //uint8 play_type;
    rst = 0;
    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_AVDTP_CONNECT_SYNC: //连接a2dp
        //支持a2dp的案子才支持连接
        if (g_btengine_cfg_data.enable_a2dp_flag == 1)
        {
            app_msg_deal_a2dp_connect(btengine_global_variable.g_cur_rmt_dev);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        /*! 蓝牙推歌数据过滤控制 */
        case MSG_BTENGINE_PLAY_PIPE_CTRL_SYNC:
        {
            uint8 pipe_op = (uint8) pri_msg->msg.content.data[0];
            uint32 temp_flag;

            temp_flag = sys_local_irq_save();

            if ((pipe_op & PIPE_OP_FILTER) != 0)
            {
                btengine_global_variable.filter_avdtp_sound_flag = 1;
                g_p_info->avdtp_data_flag = FALSE;
                A2DP_PRINTF("FILTER at %d\n", sys_get_ab_timer());
            }
            if ((pipe_op & PIPE_OP_UNFILTER) != 0)
            {
                btengine_global_variable.filter_avdtp_sound_flag = 0;
                A2DP_PRINTF("UNFILTER at %d\n", sys_get_ab_timer());
            }
            if ((pipe_op & PIPE_OP_FLUSH) != 0)
            {
                //将 play pipe 清空，以免有杂音
                g_a2dp_input_buf.length = 0;
                g_a2dp_input_buf.read_ptr = 0;
                g_a2dp_input_buf.write_ptr = 0;
                g_a2dp_input_buf.full_error = FALSE;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                if (bt_manager_cfg_info.sp_tws_link == 1)
                {
                    //对箱需要把缓冲起来的数据包也丢弃
                    TWS_MsgListInit();
                    tws_status_info.sequence_number = 1;
                    tws_status_info.tws_acl_link_time_timer = 0;
                }
                if (btengine_global_variable.avdtp_data_source == AVDTP_SOURCE_LINEIN_ADC)
                {
                    linein_tws_buffer_init();
                }
#endif
            }

            sys_local_irq_restore(temp_flag);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_ENTER_SBC_PLAY_SYNC:
        btengine_global_variable.g_sbc_thread_exist = 1;
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_EXIT_SBC_PLAY_SYNC:
        btengine_global_variable.g_sbc_thread_exist = 0;
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_SWITCH_AVDTP_SOURCE_SYNC:
        {
            avdtp_data_source_e source = (avdtp_data_source_e) pri_msg->msg.content.data[0];
            btengine_global_variable.avdtp_data_source = source;
            if (btengine_global_variable.avdtp_data_source == AVDTP_SOURCE_BT_A2DP)
            {
                g_a2dp_input_buf.raw_data = (uint8 *) (SBC_INPUT_BUFFER_ADDRESS);
                g_a2dp_input_buf.capacity = A2DP_SBC_INPUT_BUFFER_LEN;
                g_a2dp_input_buf.length = 0;
                g_a2dp_input_buf.write_ptr = 0;
                g_a2dp_input_buf.read_ptr = 0;
                g_a2dp_input_buf.full_error = FALSE;
            }
            else
            {
                linein_tws_buffer_init();
                g_a2dp_input_buf.raw_data = (uint8 *) (LINEIN_INPUT_BUFFER_ADDRESS);
                g_a2dp_input_buf.capacity = LINEIN_SBC_INPUT_BUFFER_LEN;
                g_a2dp_input_buf.length = 0;
                g_a2dp_input_buf.write_ptr = 0;
                g_a2dp_input_buf.read_ptr = 0;
                g_a2dp_input_buf.full_error = FALSE;
            }
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}

