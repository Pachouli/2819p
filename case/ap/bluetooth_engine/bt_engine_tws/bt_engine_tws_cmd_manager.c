/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：btstack tws cmd。
 * 作者：kehaitao
 ********************************************************************************/

#include "./../bluetooth_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

bool has_TWS_cmd_to_send(void);

#define TWS_SRC_CMD_SEND_OVERTIME  80

void TWS_Get_Cmd_Data_ToSend(uint8 packetnum)
{
    uint8 i;
    if (tws_status_info.acl_handle != 0)
    {
        //AVDTP Cache 10ms查询并发送一次
        if (tws_status_info.is_src_flag == TRUE)
        {
            bool cur_has_cmd_to_send = has_TWS_cmd_to_send();

            if ((g_tws_src_last_has_cmd_to_send == FALSE) && (cur_has_cmd_to_send == TRUE))
            {
                g_tws_src_last_cmd_send_timer = sys_get_ab_timer();
            }
            g_tws_src_last_has_cmd_to_send = cur_has_cmd_to_send;

            for (i = 0; i < packetnum; i++)
            {
                if ((sys_get_ab_timer() - g_tws_src_last_cmd_send_timer) >= TWS_SRC_CMD_SEND_OVERTIME)
                {
                    if (TWS_GetCmdToSend() == TRUE)
                    {
                        g_tws_src_last_has_cmd_to_send = has_TWS_cmd_to_send();
                        g_tws_src_last_cmd_send_timer = sys_get_ab_timer();
                        i++;
                        continue;
                    }
                }
                if (send_list.count > 0)
                {
                    TWS_SourceSendData(NULL, 0);
                }
                if (send_list.count >= 5)
                {
                    continue;
                }
                if (TWS_GetCmdToSend() == TRUE)
                {
                    g_tws_src_last_has_cmd_to_send = has_TWS_cmd_to_send();
                    g_tws_src_last_cmd_send_timer = sys_get_ab_timer();
                    i++;
                }
                else
                {
                    if (send_list.count == 0)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            TWS_GetCmdToSend();
        }
    }
}

void TWS_Send_10ms_Handle(void)
{
    if (tws_status_info.acl_handle != 0)
    {
        if (tws_status_info.is_src_flag == TRUE)
        {
            bool cur_has_cmd_to_send = has_TWS_cmd_to_send();

            if ((g_tws_src_last_has_cmd_to_send == FALSE) && (cur_has_cmd_to_send == TRUE))
            {
                g_tws_src_last_cmd_send_timer = sys_get_ab_timer();
            }
            g_tws_src_last_has_cmd_to_send = cur_has_cmd_to_send;

            if (TWS_GetCmdToSend() == TRUE)
            {
                g_tws_src_last_has_cmd_to_send = has_TWS_cmd_to_send();
                g_tws_src_last_cmd_send_timer = sys_get_ab_timer();
                return;
            }

            if (send_list.count > 0)
            {
                TWS_SourceSendData(NULL, 0);
            }
        }
        else
        {
            TWS_GetCmdToSend();
        }
    }
}

void tws_deal_acl_handle(uint16 aclpackets, uint8 mode)
{
    if (mode == 0)
    {
        tws_status_info.acl_handle = 0;
        senddata_cnt = 0;
    }
    else if (mode == 1)
    {
        if (senddata_cnt >= aclpackets)
        {
            senddata_cnt = senddata_cnt - aclpackets;
        }
        else
        {
            TWS_PRINTF_WARNNING("senddanta_cnt manage error\n", 0, 0);
            senddata_cnt = 0;
        }
    }
    else
    {
        ;
    }
}

uint32 tws_get_aclhandle_sendcount(void)
{
    return (tws_status_info.acl_handle << 16) | senddata_cnt;
}

bool has_TWS_cmd_to_send(void)
{
    bool has_cmdtosend_flag = FALSE;

    if (tws_status_info.acl_handle != 0)
    {
        return FALSE;
    }

    if (tws_status_info.is_src_flag == 1)
    {
        if ((g_tws_mmm_m2s_pipe.length > 0) || (g_tws_engine_m2s_pipe.length > 0) || (g_tws_ui_m2s_pipe.length > 0))
        {
            has_cmdtosend_flag = TRUE;
        }
    }
    else
    {
        if ((g_tws_mmm_s2m_pipe.length > 0) || (g_tws_engine_s2m_pipe.length > 0) || (g_tws_ui_s2m_pipe.length > 0))
        {
            has_cmdtosend_flag = TRUE;
        }
    }

    return has_cmdtosend_flag;
}

//从list中取数发送
bool TWS_GetCmdToSend(void)
{
    uint8 cmd_pkg[30];
    uint32 pkg_num;
    bool has_cmdtosend_flag = FALSE;
    tws_sync_cmd_t *p_tws_sync_cmd;
    uint8 cmd_length;
    uint16 tws_sendnum;
    buffer_rw_t *buffer_manager_p1 = NULL;
    buffer_rw_t *buffer_manager_p2 = NULL;
    buffer_rw_t *buffer_manager_p3 = NULL;
    buffer_rw_t *cur_buffer_manager_p = NULL;

    if (tws_sync_group_all_count > 2)
    {
        tws_sync_group_all_count = 0;
    }

    if (tws_status_info.is_src_flag == 1)
    {
        if (tws_sync_group_all_count == 0)
        {
            buffer_manager_p1 = &g_tws_mmm_m2s_pipe;
            buffer_manager_p2 = &g_tws_engine_m2s_pipe;
            buffer_manager_p3 = &g_tws_ui_m2s_pipe;
        }
        else if (tws_sync_group_all_count == 1)
        {
            buffer_manager_p1 = &g_tws_engine_m2s_pipe;
            buffer_manager_p2 = &g_tws_mmm_m2s_pipe;
            buffer_manager_p3 = &g_tws_ui_m2s_pipe;
        }
        else
        {
            buffer_manager_p1 = &g_tws_ui_m2s_pipe;
            buffer_manager_p2 = &g_tws_mmm_m2s_pipe;
            buffer_manager_p3 = &g_tws_engine_m2s_pipe;
        }
    }
    else
    {
        if (tws_sync_group_all_count == 0)
        {
            buffer_manager_p1 = &g_tws_mmm_s2m_pipe;
            buffer_manager_p2 = &g_tws_engine_s2m_pipe;
            buffer_manager_p3 = &g_tws_ui_s2m_pipe;
        }
        else if (tws_sync_group_all_count == 1)
        {
            buffer_manager_p1 = &g_tws_engine_s2m_pipe;
            buffer_manager_p2 = &g_tws_mmm_s2m_pipe;
            buffer_manager_p3 = &g_tws_ui_s2m_pipe;
        }
        else
        {
            buffer_manager_p1 = &g_tws_ui_s2m_pipe;
            buffer_manager_p2 = &g_tws_mmm_s2m_pipe;
            buffer_manager_p3 = &g_tws_engine_s2m_pipe;
        }
    }

    if (tws_status_info.acl_handle != 0)
    {
        //从list中取数构造cmd_pkg准备发送
        if ((buffer_manager_p1 != NULL) && (buffer_manager_p1->length > 0))
        {
            has_cmdtosend_flag = TRUE;
            cur_buffer_manager_p = buffer_manager_p1;
        }
        else if ((buffer_manager_p2 != NULL) && (buffer_manager_p2->length > 0))
        {
            has_cmdtosend_flag = TRUE;
            cur_buffer_manager_p = buffer_manager_p2;
        }
        else if ((buffer_manager_p3 != NULL) && (buffer_manager_p3->length > 0))
        {
            has_cmdtosend_flag = TRUE;
            cur_buffer_manager_p = buffer_manager_p3;
        }
    }

    if (has_cmdtosend_flag == TRUE)
    {
        pkg_num = HCI_Get_Acl_packet_num(tws_status_info.acl_handle);
        tws_sendnum = pkg_num & 0xffff; //待发送的tws_pkg_num
        pkg_num = (pkg_num >> 16); //远端可以接收的pkg_num

        if ((pkg_num + tws_sendnum) < 4)
        {
            TWS_PRINTF("send cmd:%d,%d,%d\n", pkg_num, tws_sendnum, tws_max_acl_pktnums);
        }

        if ((pkg_num == 0) || (tws_sendnum >= tws_max_acl_pktnums))
        {
            return FALSE;
        }

        tws_sync_group_all_count++;
        if (tws_sync_group_all_count > 2)
        {
            tws_sync_group_all_count = 0;
        }

        read_tws_cmd_from_buffer_rw(cur_buffer_manager_p, &cmd_pkg[10], sizeof(tws_sync_cmd_t));

        p_tws_sync_cmd = (tws_sync_cmd_t *) (&cmd_pkg[10]);
        cmd_length = p_tws_sync_cmd->cmd_len + CMD_TYPE_HEAD_LEN;

        cmd_pkg[0] = 0x02;
        cmd_pkg[1] = (uint8) (tws_status_info.acl_handle);
        cmd_pkg[2] = (uint8) ((tws_status_info.acl_handle >> 8));
        cmd_pkg[3] = (uint8) (4 + cmd_length + 1);
        cmd_pkg[4] = 0x00;
        cmd_pkg[5] = (uint8) (cmd_length + 1);
        cmd_pkg[6] = 0x00;
        cmd_pkg[7] = (uint8) tws_status_info.src_stream_cid;
        cmd_pkg[8] = (uint8) (tws_status_info.src_stream_cid >> 8);

        cmd_pkg[9] = 0xee;

        Send(0xff, cmd_pkg, cmd_length + 10); //10表示ACL SYNC CMD 头部长度，到0xee为止

        HCI_Set_Acl_packet_num(tws_status_info.acl_handle, 1, 0);
        senddata_cnt++;
        return TRUE;
    }
    return FALSE;
}

uint8 TWS_RxCmdDeal(uint8* pHciInfo, uint8 length)
{
    uint8 ret_val = FALSE;

    if ((*pHciInfo) != TWS_SYNC_BASE)
    {
        return ret_val;
    }
    length--;
    if (length > CMD_TYPE_DATA_LEN)
    {
        length = CMD_TYPE_DATA_LEN;
    }

    if (pHciInfo[1] == TWS_SYNC_GROUP_PLAY)
    {
        if (tws_status_info.is_src_flag == 1)
        { //从到主
            write_tws_cmd_to_buffer_rw(&g_tws_mmm_s2m_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
        else
        { //主到从
            write_tws_cmd_to_buffer_rw(&g_tws_mmm_m2s_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
    }
    else if (pHciInfo[1] == TWS_SYNC_GROUP_EG)
    {
        if (tws_status_info.is_src_flag == 1)
        { //从到主
            write_tws_cmd_to_buffer_rw(&g_tws_engine_s2m_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
        else
        { //主到从
            write_tws_cmd_to_buffer_rw(&g_tws_engine_m2s_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
    }
    else if (pHciInfo[1] == TWS_SYNC_GROUP_UI)
    {
        if (tws_status_info.is_src_flag == 1)
        { //从到主
            write_tws_cmd_to_buffer_rw(&g_tws_ui_s2m_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
        else
        { //主到从
            write_tws_cmd_to_buffer_rw(&g_tws_ui_m2s_pipe, &pHciInfo[1], sizeof(tws_sync_cmd_t));
        }
    }
    else
    {
        TWS_PRINTF_WARNNING("invalid cmd group\n");
    }

    return TRUE;

}
#endif
