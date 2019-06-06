/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：btstack tws avdtp数据管理。
 * 作者：kehaitao
 ********************************************************************************/

#include "./../bluetooth_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

bool TWS_SourceSendData(uint8* send_data, uint32 in_len)
{
    uint8 *packet = g_tws_avdtp_packet;
    uint8 i = 0;
    uint16 packet_len = 0;
    uint32 pkg_num;
    bool packet_flag = TRUE;
    uint16 tws_sendnum;
    bool calling_flag = TRUE;

    if (send_data == NULL)
    {
        packet_flag = FALSE;
    }
    else
    {
        tws_status_info.tws_acl_link_time_timer = sys_get_ab_timer();
    }

    if (tws_status_info.is_src_flag == TRUE)
    {
        if (packet_flag == TRUE)
        {
            packet_len = in_len + 22;

            libc_memset(packet, 0x0, 22);
            packet[i++] = 0x02;
            packet[i++] = (uint8) (tws_status_info.acl_handle);
            packet[i++] = (uint8) ((tws_status_info.acl_handle >> 8));
            packet[i++] = (uint8) (4 + 13 + in_len);
            packet[i++] = (uint8) ((4 + 13 + in_len) >> 8);
            packet[i++] = (uint8) (13 + in_len);
            packet[i++] = (uint8) ((13 + in_len) >> 8);
            packet[i++] = (uint8) tws_status_info.src_stream_cid;
            packet[i++] = (uint8) (tws_status_info.src_stream_cid >> 8);

            packet[i++] = 0x80;
            packet[i++] = 0x60;
            packet[i++] = (tws_status_info.sequence_number >> 8) & 0xff;
            packet[i++] = tws_status_info.sequence_number & 0xff;

            tws_status_info.sequence_number++;

            //将MMM 命令长度小于等于6个字节 命令通过AVDTP命令包发送给副箱，减轻主箱的带宽压力
            if (g_tws_mmm_m2s_play_sync_cmd.cmd_group != 0)
            {
                packet[i++] = 0xee;
                packet[i++] = 0xee;
                libc_memcpy(packet + i, &g_tws_mmm_m2s_play_sync_cmd, 6);
                g_tws_mmm_m2s_play_sync_cmd.cmd_group = 0;
            }

            packet[21] = 0x05/*SBC_BLOCK_NUM*/;
            libc_memcpy(&packet[22], send_data, in_len);

            pkg_num = HCI_Get_Acl_packet_num(tws_status_info.acl_handle);
            tws_sendnum = pkg_num & 0xffff;
            pkg_num = (pkg_num >> 16);
            if ((pkg_num == 0) || (tws_sendnum >= tws_max_acl_pktnums) || (send_list.count > 0))
            {
                packet_flag = FALSE;

                if (send_list.count >= SEND_LIST_MAX)
                {
                    TWS_PRINTF_WARNNING("cache full! drop packet\n");
                    g_a2dp_input_buf.full_error = TRUE; //主箱到副箱的Cache满了，标记主箱的Input Buffer full error，主箱报错退出解码
                    return FALSE;
                }
                else
                {
                    if (buffer_rw_write(&(send_list.avdtp_cache), packet, packet_len) == FALSE)
                    {
                        TWS_PRINTF_WARNNING("cache full! drop packet\n");
                        g_a2dp_input_buf.full_error = TRUE; //主箱到副箱的Cache满了，标记主箱的Input Buffer full error，主箱报错退出解码
                        return FALSE;
                    }
                    else
                    {
                        send_list.count++;
                        send_list.data_len[send_list.recv_prt] = packet_len;
                        send_list.recv_prt++;
                        send_list.recv_prt %= SEND_LIST_MAX;

                        if (send_list.count >= 10)
                        {
                            TWS_PRINTF_WARNNING("packet full! temp save to cache! += %d\n", send_list.count);
                        }
                    }
                }
            }
        }
        else
        {
            pkg_num = HCI_Get_Acl_packet_num(tws_status_info.acl_handle);
            tws_sendnum = pkg_num & 0xffff;
            pkg_num = (pkg_num >> 16);
        }

        if ((g_p_info->sco_created_flag == FALSE)
                && ((sco_connect_request_flag == 0)
                        || ((sco_connect_request_flag == 1) && (sys_get_ab_timer() - sco_connect_request_time > 2000))))
        {
            calling_flag = FALSE;
        }

        if (calling_flag == TRUE)
        {
            if (tws_status_info.tws_acl_link_time != -16)
            {
                tws_status_info.tws_acl_link_time = -16; //多让手机传输
                con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt-1\n");
            }
        }
        else
        {
            //优先照顾手机传输到主箱，只有手机传输没有问题了才会照顾主箱传输到副箱
            if (send_list.count >= 9)
            {
                if (tws_status_info.tws_acl_link_time != 64)
                {
                    tws_status_info.tws_acl_link_time = 64; //很大阻塞调高到70%
                    con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt+3\n");
                }
            }
            else if (send_list.count >= 7)
            {
                if (tws_status_info.tws_acl_link_time != 34)
                {
                    tws_status_info.tws_acl_link_time = 34; //较大阻塞调高到63%
                    con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt+2\n");
                }
            }
            else if (send_list.count >= 5)
            {
                if (tws_status_info.tws_acl_link_time != 16)
                {
                    tws_status_info.tws_acl_link_time = 16; //较小阻塞调高到57%
                    con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt+1\n");
                }
            }
            else if (send_list.count >= 3)
            {
                if (tws_status_info.tws_acl_link_time > 0) //如果当前多让手机传输，先不要调整link_time
                {
                    tws_status_info.tws_acl_link_time = 0; //几乎没有阻塞调到50%
                    con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt+0\n");
                }
            }
            else
            {
                if (tws_status_info.tws_acl_link_time != -16)
                {
                    tws_status_info.tws_acl_link_time = -16; //多让手机传输
                    con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);TWS_DBG_PRINTF("lt-1\n");
                }
            }
        }

        if ((pkg_num == 0) || (tws_sendnum >= tws_max_acl_pktnums))
        {
            return FALSE;
        }

        //流控还允许发送数据包
        if ((packet_flag == FALSE) && (send_list.count > 0))
        {
            packet = g_tws_avdtp_packet;
            packet_len = send_list.data_len[send_list.send_ptr];
            buffer_rw_read(&(send_list.avdtp_cache), packet, packet_len);

            send_list.count--;
            send_list.send_ptr++;
            send_list.send_ptr %= SEND_LIST_MAX;

            //libc_printf("read cache and send again! -= %d",send_list.count);
        }

        //SCO链路建立起来后主箱不再往副箱发送数据
        if (calling_flag == FALSE)
        {
            Send(0xff, packet, packet_len);

            HCI_Set_Acl_packet_num(tws_status_info.acl_handle, 1, 0);
            senddata_cnt++;
            return TRUE;
        }
    }
    return FALSE;
}
#endif
