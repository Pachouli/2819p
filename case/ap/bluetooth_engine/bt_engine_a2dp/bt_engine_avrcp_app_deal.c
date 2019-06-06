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
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "./../bluetooth_engine.h"

//�������貥��
void app_msg_deal_play_pause(uint8 play_type)
{
    bt_dev_t *dev;
    dev = btengine_global_variable.g_cur_rmt_dev;
    switch (get_cur_device_status(dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        case STATUS_A2DP_PAUSE:
        //������ͣ���Ų��������첽���������ڷ�����ͣ�������Ȼ����������Ƶ���ݴ���
        //���Ƕ���ĳЩ�ֻ���˵���˵������ǲ������Ӷ����·��Ͳ�������ʱ�������Ѿ�����play״̬
        case STATUS_A2DP_PLAY:
        if ((dev != NULL) && (dev->p_dev_status != NULL) && ((dev->p_dev_status->avrcp_source.connected_flag) != 0))
        {
            if ((get_cur_device_status(dev) != STATUS_A2DP_PLAY))
            {
                APP_AVRCP_PassThrough_Req(dev, AVRCP_OPID_PLAY, 0);
                set_cur_device_status(dev, STATUS_A2DP_PLAY);
            }
            else
            {
                APP_AVRCP_PassThrough_Req(dev, AVRCP_OPID_PAUSE, 0);
                set_cur_device_status(dev, STATUS_A2DP_PAUSE);

            }
            /* whether force start play.  */
        }
        else
        {
            ; //nothing for QAC
        }
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
}
//���������л���һ��
void app_msg_deal_play_next(void)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        //break;
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        if ((btengine_global_variable.g_cur_rmt_dev != NULL)
                && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL)
                && ((btengine_global_variable.g_cur_rmt_dev->p_dev_status->avrcp_source.connected_flag) != 0))
        {
            AVRCP_PRINTF("opid_forward\n");
            APP_AVRCP_PassThrough_Req(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_FORWARD, 0);
        }
        else
        {
            ; //nothing for QAC
        }
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
}
//���������л���һ��

void app_msg_deal_play_prev(void)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        //break;  //ֻҪ������AVRCP��linked״̬��Ҳ���Է�
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        if ((btengine_global_variable.g_cur_rmt_dev != NULL)
                && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL)
                && ((btengine_global_variable.g_cur_rmt_dev->p_dev_status->avrcp_source.connected_flag) != 0))
        {
            AVRCP_PRINTF("opid_backward\n");
            APP_AVRCP_PassThrough_Req(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_BACKWARD, 0);
        }
        else
        {
            ; //nothing for QAC
        }
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
}

void app_avrcp_volume_update(bt_dev_t *dev, uint8 g_volume)
{
    if (dev != NULL)
    {
        g_p_info->remote_volume = g_volume;
        APP_AVRCP_VolumeChangedNTF(dev->dev_info.bd_addr.bytes, AVRCP_RSP_CHANGED, g_volume);
    }
}

//�����->�ֻ�������ͬ��
//�ֻ�ͨ��Ҳ������ģʽ:��SCOʱ����HFP����SCOʱ���������ķ���
void app_msg_deal_volume_sync(uint8 g_volume)
{
#ifdef ENABLE_HFP_VOLUME_SYNC
    if (bt_manager_cfg_info.sp_hfp_vol_syn == 1)
    {
        if ((g_p_info->enter_hfp_flag == 1) && ((g_p_info->service_connect & HFP_CONNECTED) != 0))
        {
            app_hfp_volume_update(g_volume);
        }
    }
#endif
    if (bt_manager_cfg_info.sp_avrcp_vol_syn != 1)
    {
        return;
    }
    if ((g_p_info->enter_hfp_flag != 1) && ((g_p_info->service_connect & AVRCP_CONNECTED_MY) != 0)
            && (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_A2DP_PLAY))
    {
        /* for BQB, not care avrcp_play_status. */
        if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) != 0)
        {
            app_avrcp_volume_update(btengine_global_variable.g_cur_rmt_dev, g_volume);
        }
        else if (g_p_avrcp->avrcp_play_status == AVRCP_PLAYSTATUS_PLAYING)
        {
            app_avrcp_volume_update(btengine_global_variable.g_cur_rmt_dev, g_volume);
        }
    }
}

uint8 app_message_deal_avrcp(private_msg_t *pri_msg)
{
    uint8 rst;
    uint8 play_type;
    rst = 0;
    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_PLAY_PAUSE_SYNC: //����
        play_type = (uint8) pri_msg->msg.content.data[0];
        app_msg_deal_play_pause(play_type);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_PLAY_NEXT_SYNC: //��һ��
        app_msg_deal_play_next();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_PLAY_PREV_SYNC: //��һ��
        app_msg_deal_play_prev();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_FFWD_SYNC:
        if (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_A2DP_PLAY)
        {
            APP_AVRCP_PassThrough_Press(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_FAST_FORWARD, 0);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_FBWD_SYNC:
        if (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_A2DP_PLAY)
        {
            APP_AVRCP_PassThrough_Press(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_REWIND, 0);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_FFWD_CANCEL_SYNC:

        APP_AVRCP_PassThrough_Release(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_FAST_FORWARD, 0);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_FBWD_CANCEL_SYNC:
        APP_AVRCP_PassThrough_Release(btengine_global_variable.g_cur_rmt_dev, AVRCP_OPID_REWIND, 0);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        default:
        rst = 1;
        break;
    }

    return rst;
}

