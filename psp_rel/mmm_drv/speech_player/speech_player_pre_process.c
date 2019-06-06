/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件预处理，包括PLC和前置AGC处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

static int32 speech_pre_process_peq(speech_player_t *speech_player, short *inout_buf, short *ktd_input_buf)
{
    uint8 k;
    short *dest;
    short *src;
    int32 ret;

    dest = PEQ_TEMP_BUFFER_ADDR;
    src = inout_buf;
    for (k = 0; k < MY_POST_PROCESS_FRAME_SAMPLE; k++)
    {
        dest[2*k] = src[k];
        dest[2*k + 1] = src[k];
    }

    update_PP_enable(speech_player->audiopp_handle, TRUE, FALSE);

    //kt替换左声道输出
    speech_player->sp_aout.pcm[0] = PEQ_TEMP_BUFFER_ADDR;
    speech_player->sp_aout.samples = MY_POST_PROCESS_FRAME_SAMPLE;
    ret = post_process_ops(speech_player->audiopp_handle, PP_CMD_FRAME_PROCESS, (uint32) &speech_player->sp_aout);
    if (ret != 0)
    {
        libc_printf_warning("post_process_ops frame error:%d\n", ret);
    }
    else
    {
        dest = inout_buf;
        src = PEQ_TEMP_BUFFER_ADDR;
        for (k = 0; k < MY_POST_PROCESS_FRAME_SAMPLE; k++)
        {
            dest[k] = src[2*k];
        }
    }

    return ret;
}

//预处理：包括PLC和前端AGC
void speech_pre_process_frame(speech_player_t *speech_player, bool fill_empty_block)
{
    uint8 *plc_deal_buf = (uint8 *) SPEECH_TEMP_BUF_ADDR;
    sco_err_report_info_t *p_sco_err_report_info = (sco_err_report_info_t *) (speech_player->sco_input_buf->ext_data);
    sco_err_report_t *p_cur_err_report;
    uint16 plc_buf_index = 0;
    uint8 plc_pkg_status[4], plc_pkg_status_cnt = 0;
    plc_inout tmp_plc_inout;
    agc_inout tmp_agc_inout;
    int32 agc_in_len, agc_in_index;
    uint32 t1 = 0, t2;
    uint32 temp_flag;
    uint32 pre_process_timer = sys_get_ab_timer();
    uint16 pre_right_length;
    int32 ret = 0;
    bool pre_process_buf_filled = FALSE;

    fill_empty_block_cont:
    if (fill_empty_block == TRUE)
    {
        plc_pkg_status[0] = SCO_PKG_STATE_NO_DATA_RECEIVED;
        plc_pkg_status[1] = SCO_PKG_STATE_NO_DATA_RECEIVED;
        plc_pkg_status_cnt = 2;

        libc_memset(plc_deal_buf, 0x00, speech_player->plc_deal_block);
        tmp_plc_inout.inoutput = (short *) plc_deal_buf;
        tmp_plc_inout.cnt = speech_player->plc_deal_block/sizeof(short);

        goto __plc_deal__;
    }

    while ((sys_get_ab_timer() - pre_process_timer) < 12)//不能运行太长时间，否则会导致PCM BUFFER填充不及时而出错
    {
        bool has_enough_data = FALSE;
        uint16 one_pkg_length;
        uint16 read_len, remain_len;

        //统计合适的时长进行PLC处理
        if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
        {
            if (speech_player->sco_input_buf->length >= speech_player->codec_deal_block)
            {
                has_enough_data = TRUE;
            }
        }
        else
        {
            if (speech_player->sco_input_buf->length >= speech_player->plc_deal_block)
            {
                has_enough_data = TRUE;
            }
        }

        if (has_enough_data == TRUE)
        {
            read_again:

            temp_flag = sys_local_intc_save(0x01);

            //pre_porcess_buf 合法性检测
            pre_right_length = speech_player->pre_process_buf.write_ptr + speech_player->pre_process_buf.capacity;
            pre_right_length -= speech_player->pre_process_buf.read_ptr;
            pre_right_length %= speech_player->pre_process_buf.capacity;
            if (pre_right_length != (speech_player->pre_process_buf.length % speech_player->pre_process_buf.capacity))
            {
                libc_printf_warning("pre length error!\n");
                speech_player->error_need_reset_flag = TRUE;
            }

            p_cur_err_report = &(p_sco_err_report_info->err_report_data[p_sco_err_report_info->read_ptr]);
            p_sco_err_report_info->read_ptr++;
            p_sco_err_report_info->read_ptr %= p_sco_err_report_info->count;
            p_sco_err_report_info->length--;
            sys_local_intc_restore(temp_flag);

            if (speech_player->speech_setting.use_sco_data_predeal == TRUE)
            {
                bool predeal_one_package_flag = FALSE;

                if (speech_player->sp_pre_buffer_remain + p_cur_err_report->pkg_length >= SP_RRE_DEAL_BLOCK_LEN)
                {
                    libc_memcpy(plc_deal_buf, speech_player->sp_pre_buffer, speech_player->sp_pre_buffer_remain);
                    read_len = SP_RRE_DEAL_BLOCK_LEN - speech_player->sp_pre_buffer_remain;
                    remain_len = p_cur_err_report->pkg_length - read_len;
                    if (buffer_rw_read(speech_player->sco_input_buf, plc_deal_buf + speech_player->sp_pre_buffer_remain,
                        read_len) == FALSE)
                    {
                        libc_printf_warning("sco_in empty!\n");
                    }

                    if ((speech_player->sco_pkg_flag == TRUE)
                        && (p_cur_err_report->pkg_status == SCO_PKG_STATE_CORRECTLY_RECEIVED))
                    {
                        pre_inout_t temp_pre_inout;

                        if (speech_player->need_print_time == 1)
                        {
                            t1 = sys_get_ab_timer_us();
                        }

                        temp_pre_inout.pre_in  = plc_deal_buf;
                        temp_pre_inout.pre_out = speech_player->sco_data_predeal_buf;
                        temp_pre_inout.in_len  = SP_RRE_DEAL_BLOCK_LEN;
                        sp_pre_cmd(speech_player->sp_pre_handle, SP_PRE_CMD_FRAME_DEAL, &temp_pre_inout);

                        if (speech_player->need_print_time == 1)
                        {
                            t2 = sys_get_ab_timer_us();

                            speech_player->pre_deal_time += (t2 - t1);
                        }

                        plc_pkg_status[plc_pkg_status_cnt] = SCO_PKG_STATE_CORRECTLY_RECEIVED;
                        plc_pkg_status_cnt++;
                        plc_pkg_status[plc_pkg_status_cnt] = SCO_PKG_STATE_CORRECTLY_RECEIVED;
                        plc_pkg_status_cnt++;
                    }
                    else
                    {
                        sp_pre_cmd(speech_player->sp_pre_handle, SP_PRE_CMD_RESET, 0);

                        plc_pkg_status[plc_pkg_status_cnt] = SCO_PKG_STATE_NO_DATA_RECEIVED;
                        plc_pkg_status_cnt++;
                        plc_pkg_status[plc_pkg_status_cnt] = SCO_PKG_STATE_NO_DATA_RECEIVED;
                        plc_pkg_status_cnt++;
                    }
                    one_pkg_length = SP_RRE_DEAL_BLOCK_LEN * 2;
                    libc_memcpy(plc_deal_buf, speech_player->sco_data_predeal_buf, one_pkg_length);

                    speech_player->sp_pre_buffer_remain = 0;
                    speech_player->sco_pkg_flag = TRUE;
                    predeal_one_package_flag = TRUE;
                }
                else
                {
                    remain_len = p_cur_err_report->pkg_length;
                    if (p_cur_err_report->pkg_status != SCO_PKG_STATE_CORRECTLY_RECEIVED)
                    {
                        speech_player->sco_pkg_flag = FALSE;
                    }
                }

                if (remain_len > 0)
                {
                    if (buffer_rw_read(speech_player->sco_input_buf, speech_player->sp_pre_buffer + speech_player->sp_pre_buffer_remain,
                        remain_len) == FALSE)
                    {
                        libc_printf_warning("sco_in empty!\n");
                    }
                    speech_player->sp_pre_buffer_remain += remain_len;
                    if (p_cur_err_report->pkg_status != SCO_PKG_STATE_CORRECTLY_RECEIVED)
                    {
                        speech_player->sco_pkg_flag = FALSE;
                    }
                }

                if (predeal_one_package_flag == FALSE)
                {
                    continue;
                }
            }
            else
            {
                if (buffer_rw_read(speech_player->sco_input_buf, plc_deal_buf + plc_buf_index,
                        p_cur_err_report->pkg_length) == FALSE)
                {
                    libc_printf_warning("sco_in empty!\n");
                }
                one_pkg_length = p_cur_err_report->pkg_length;
                plc_buf_index += one_pkg_length;
            }

            if (one_pkg_length == speech_player->plc_deal_block/2)
            {
                plc_pkg_status[plc_pkg_status_cnt] = p_cur_err_report->pkg_status;
                plc_pkg_status_cnt++;
                if (plc_buf_index < speech_player->plc_deal_block)
                {
                    goto read_again;
                }
            }
            else if (one_pkg_length == speech_player->plc_deal_block)
            {
                if (speech_player->speech_setting.use_sco_data_predeal == FALSE)
                {
                    plc_pkg_status[plc_pkg_status_cnt] = p_cur_err_report->pkg_status;
                    plc_pkg_status_cnt++;
                    plc_pkg_status[plc_pkg_status_cnt] = p_cur_err_report->pkg_status;
                    plc_pkg_status_cnt++;
                }
            }
            else
            {
                libc_printf_warning("sco pkg length unexpect!%d\n", one_pkg_length);
                temp_flag = sys_local_intc_save(0x01);
                p_sco_err_report_info->read_ptr = 0;
                p_sco_err_report_info->write_ptr = 0;
                p_sco_err_report_info->length = 0;
                buffer_rw_flush(speech_player->sco_input_buf);
                sys_local_intc_restore(temp_flag);
                break;
            }

            //DUMP SCO IN
            if (speech_player->p_asqt_dump_buf != NULL)
            {
                dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_SCO_IN, plc_deal_buf,
                        speech_player->plc_deal_block);
            }

            //PLC 处理
            tmp_plc_inout.inoutput = (short *) plc_deal_buf;
            tmp_plc_inout.cnt = speech_player->plc_deal_block/sizeof(short);

            if (plc_pkg_status[0] != SCO_PKG_STATE_CORRECTLY_RECEIVED)
            {
                speech_player->sco_pkg_err_count++;
            }
            if (plc_pkg_status[1] != SCO_PKG_STATE_CORRECTLY_RECEIVED)
            {
                speech_player->sco_pkg_err_count++;
            }

            if ((plc_pkg_status[0] != SCO_PKG_STATE_CORRECTLY_RECEIVED)
                    || (plc_pkg_status[1] != SCO_PKG_STATE_CORRECTLY_RECEIVED))
            {
                speech_player->sco_pkg_err2_count += 2;
                speech_player->sco_pkg_err_continuous += 2;
            }
            else
            {
                if (speech_player->sco_pkg_err_continuous > speech_player->sco_pkg_err_continuous_max)
                {
                    speech_player->sco_pkg_err_continuous_max = speech_player->sco_pkg_err_continuous;
                }
                speech_player->sco_pkg_err_continuous = 0;
            }
            speech_player->sco_pkg_all_count += 2;

            if (speech_player->sco_pkg_all_count >= 150)
            {
                if (speech_player->sco_pkg_err_continuous > speech_player->sco_pkg_err_continuous_max)
                {
                    speech_player->sco_pkg_err_continuous_max = speech_player->sco_pkg_err_continuous;
                }
                speech_player->sco_pkg_err_continuous = 0;

                if (speech_player->sco_pkg_err2_count != 0)
                {
                    libc_printf_info("sco err:%d,%d,%d\n", speech_player->sco_pkg_err2_count,
                            speech_player->sco_pkg_err_count, speech_player->sco_pkg_err_continuous_max);
                }

                speech_player->sco_pkg_err_count = 0;
                speech_player->sco_pkg_err2_count = 0;
                speech_player->sco_pkg_all_count = 0;
                speech_player->sco_pkg_err_continuous_max = 0;
            }

            __plc_deal__:
            if (speech_player->plc_handle != NULL)
            {
                if (speech_player->need_print_time == 1)
                {
                    t1 = sys_get_ab_timer_us();
                }

                if ((plc_pkg_status[0] == SCO_PKG_STATE_CORRECTLY_RECEIVED)
                        && (plc_pkg_status[1] == SCO_PKG_STATE_CORRECTLY_RECEIVED))
                {
                    hfp_plc_cmd(speech_player->plc_handle, HS_CMD_PLC_PROCESS_NORMAL, (uint32) &tmp_plc_inout);
                }
                else
                {
                    hfp_plc_cmd(speech_player->plc_handle, HS_CMD_PLC_PROCESS_LOSS, (uint32) &tmp_plc_inout);
                }

                if (speech_player->need_print_time == 1)
                {
                    t2 = sys_get_ab_timer_us();

                    speech_player->plc_deal_time += (t2 - t1);
                }

                //DUMP PRE PLC
                if (speech_player->p_asqt_dump_buf != NULL)
                {
                    dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_PRE_PLC, plc_deal_buf,
                            speech_player->plc_deal_block);
                }
            }

            agc_in_len = speech_player->plc_deal_block;
            agc_in_index = 0;
            while (agc_in_len > 0)
            {
                if ((speech_player->agc_temp_buf_len + agc_in_len) > speech_player->pre_agc_deal_block)
                {
                    libc_memcpy(speech_player->agc_predeal_buf + speech_player->agc_temp_buf_len, plc_deal_buf + agc_in_index,
                            speech_player->pre_agc_deal_block - speech_player->agc_temp_buf_len);

                    if (speech_player->g_filter_start_sound_count < FILTER_START_SOUND_COUNT)
                    {
                        speech_player->g_filter_start_sound_count++;
                        libc_memset(speech_player->agc_predeal_buf, 0x00, speech_player->pre_agc_deal_block);
                    }

                    //PRE AGC 处理
                    if (speech_player->agc_handle_pre != NULL)
                    {
                        tmp_agc_inout.inoutput = (short *) speech_player->agc_predeal_buf;
                        tmp_agc_inout.cnt = speech_player->pre_agc_deal_block / 2;

                        if (speech_player->need_print_time == 1)
                        {
                            t1 = sys_get_ab_timer_us();
                        }

                        hfp_speech_cmd(speech_player->agc_handle_pre, HS_CMD_AGC_PROCESS, (uint32) &tmp_agc_inout);

                        if (speech_player->need_print_time == 1)
                        {
                            t2 = sys_get_ab_timer_us();

                            speech_player->pre_agc_deal_time += (t2 - t1);
                        }
                    }

                    //前端PEQ处理
                    if (speech_player->need_print_time == 1)
                    {
                        t1 = sys_get_ab_timer_us();
                    }

#if (SUPPORT_OK_MIC == 1)
                    if (speech_player->work_mode == SP_WORK_MODE_MIC)
                    {
                        //OK MIC在播放时再做PEQ处理
                    }
                    else
#endif
                    {
                        //前端PEQ处理
                        uint8 p;
                        for (p = 0; p < speech_player->pre_agc_deal_block/(MY_POST_PROCESS_FRAME_SAMPLE*2); p++)
                        {
                            speech_pre_process_peq(speech_player, (short *)(speech_player->agc_predeal_buf) + \
                                MY_POST_PROCESS_FRAME_SAMPLE*p, NULL);

                            //软按键音叠加处理
#if (SUPPORT_SOFT_KEYTONE == 1)
                            if ((speech_player->kt_play_enable == TRUE)
                                && (speech_player->speech_status.ktp_status == MMM_KTP_ENGINE_PLAYING))
                            {
                                keytone_frame_deal(speech_player, (short *)(speech_player->agc_predeal_buf) + \
                                    MY_POST_PROCESS_FRAME_SAMPLE*p, MY_POST_PROCESS_FRAME_SAMPLE);
                            }
#endif
                        }
                    }

                    if (speech_player->need_print_time == 1)
                    {
                        t2 = sys_get_ab_timer_us();
                        speech_player->pre_peq_time += (t2 - t1);
                    }

                    //PRE AGC 处理OK后才进行播放
                    if (buffer_rw_write(&(speech_player->pre_process_buf), speech_player->agc_predeal_buf,
                            speech_player->pre_agc_deal_block) == FALSE)
                    {
                        libc_printf_warning("pre full!\n");
                    }
                    else
                    {
                        speech_player->post_process_buf.length += speech_player->pre_agc_deal_block;
                    }
                    pre_process_buf_filled = TRUE;

                    agc_in_index += speech_player->pre_agc_deal_block - speech_player->agc_temp_buf_len;
                    agc_in_len -= speech_player->pre_agc_deal_block - speech_player->agc_temp_buf_len;
                    speech_player->agc_temp_buf_len = 0;
                }
                else
                {
                    libc_memcpy(speech_player->agc_predeal_buf + speech_player->agc_temp_buf_len, plc_deal_buf + agc_in_index,
                            agc_in_len);
                    speech_player->agc_temp_buf_len += agc_in_len;
                    break;
                }
            }

            plc_buf_index = 0;
            plc_pkg_status_cnt = 0;

            //只填充1个128样本点Block
            if (fill_empty_block == TRUE)
            {
                if (pre_process_buf_filled == TRUE)
                {
                    break;
                }
                else
                {
                    goto fill_empty_block_cont;
                }
            }
        }
        else
        {
            break;
        }
    }
}
