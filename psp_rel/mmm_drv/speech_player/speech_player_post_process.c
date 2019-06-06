/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件后处理，包括AEC和后置AGC处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

static int32 speech_post_process_peq(speech_player_t *speech_player, short *inout_buf)
{
    uint8 k;
    short *dest;
    short *src;
    int32 ret;

    libc_memset(PEQ_TEMP_BUFFER_ADDR, 0, PEQ_TEMP_BUFFER_LEN);

    update_PP_enable(speech_player->audiopp_handle, FALSE, TRUE);

    //kt替换左声道输出
    speech_player->sp_aout.pcm[0] = PEQ_TEMP_BUFFER_ADDR;
    speech_player->sp_aout.pcm[1] = inout_buf;
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

//固定处理大小为256字节，即单声道8KHz采样率的16ms时长
void speech_post_process_frame(speech_player_t *speech_player)
{
    sv_info_t tmp_sv_info;
    uint8 *post_deal_buf = (uint8 *) SPEECH_TEMP_BUF_ADDR;
    uint8 *post_deal_buf_spk;
    agc_inout tmp_agc_inout;
    aec_inout tmp_aec_inout;
    uint32 t1 = 0, t2;
    bool drop_dump_flag = FALSE;
    bool donot_post_process_flag = FALSE;

    if (speech_player->mic_input_buf.length >= MIC_INPUT_BUF_LEN_THRES)
    {
        donot_post_process_flag = TRUE;
        libc_printf_warning("donot post!\n");
    }

    if (speech_player->p_asqt_dump_buf != NULL)
    {
        uint32 post_total_len, dump_buf_remain;

        post_total_len = speech_player->aec_deal_block * 3 + sizeof(tmp_sv_info) + 4 * 4;
        dump_buf_remain = speech_player->p_asqt_dump_buf->dump_capacity - speech_player->p_asqt_dump_buf->dump_length;
        if (dump_buf_remain < post_total_len)
        {
            drop_dump_flag = TRUE;
        }
    }

    //MIC IN
    if (buffer_rw_read(&(speech_player->mic_input_buf), post_deal_buf, speech_player->aec_deal_block) == FALSE)
    {
        libc_printf_warning("mic_in empty!\n");
    }
    //SPK IN
    post_deal_buf_spk = speech_player->post_process_buf.raw_data + speech_player->post_process_buf.read_ptr;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1))
    if ((speech_player->record_enable == TRUE) && (speech_player->mmm_record_sta == MMM_RECORD_RECORDING))
    {
        player_encode_source_deal(speech_player, post_deal_buf_spk, post_deal_buf, speech_player->aec_deal_block/sizeof(short));
    }
#endif

    //DUMP MIC IN
    if ((speech_player->p_asqt_dump_buf != NULL) && (drop_dump_flag == FALSE))
    {
        dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_MIC_IN, post_deal_buf,
                speech_player->aec_deal_block);
    }

    energy_calc(post_deal_buf, speech_player->aec_deal_block/sizeof(short), FALSE, &(speech_player->speech_status.mic_cur_energy),
        &(speech_player->speech_status.mic_cur_energy_rms));

    //DUMP SPK IN
    if ((speech_player->p_asqt_dump_buf != NULL) && (drop_dump_flag == FALSE))
    {
        dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_SPK_IN, post_deal_buf_spk,
                speech_player->aec_deal_block);
    }

    energy_calc(post_deal_buf_spk, speech_player->aec_deal_block/sizeof(short), FALSE, &(speech_player->speech_status.play_cur_energy),
        &(speech_player->speech_status.play_cur_energy_rms));

    //AEC 处理
    if (speech_player->aec_handle != NULL)
    {
        tmp_aec_inout.Txinbuf_mic = (short *) post_deal_buf;
        tmp_aec_inout.RxinBuf_spk = (short *) post_deal_buf_spk;
        tmp_aec_inout.cnt = speech_player->aec_deal_block / 2;

        if (speech_player->need_print_time == 1)
        {
            t1 = sys_get_ab_timer_us();
        }

        if (donot_post_process_flag == FALSE)
        {
            hfp_speech_cmd(speech_player->aec_handle, HS_CMD_AEC_PROCESS, (uint32) &tmp_aec_inout);
        }

        //DUMP POST AEC
        if ((speech_player->p_asqt_dump_buf != NULL) && (drop_dump_flag == FALSE))
        {
            dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_POST_AEC, post_deal_buf,
                    speech_player->aec_deal_block);
        }

        if (speech_player->need_print_time == 1)
        {
            t2 = sys_get_ab_timer_us();

            speech_player->aec_deal_time += (t2 - t1);
        }
    }

    //POST AGC 处理
    if (speech_player->agc_handle_post != NULL)
    {
        tmp_agc_inout.inoutput = (short *) post_deal_buf;
        tmp_agc_inout.cnt = speech_player->aec_deal_block / 2;

        if (speech_player->need_print_time == 1)
        {
            t1 = sys_get_ab_timer_us();
        }

        if (donot_post_process_flag == FALSE)
        {
            hfp_speech_cmd(speech_player->agc_handle_post, HS_CMD_AGC_PROCESS, (uint32) &tmp_agc_inout);
        }

        if (speech_player->need_print_time == 1)
        {
            t2 = sys_get_ab_timer_us();

            speech_player->post_agc_deal_time += (t2 - t1);
        }
    }

    //后端PEQ处理
    if (speech_player->need_print_time == 1)
    {
        t1 = sys_get_ab_timer_us();
    }
    speech_post_process_peq(speech_player, (short *)post_deal_buf);
    speech_post_process_peq(speech_player, (short *)post_deal_buf + MY_POST_PROCESS_FRAME_SAMPLE);
    if (speech_player->need_print_time == 1)
    {
        t2 = sys_get_ab_timer_us();
        speech_player->post_peq_time += (t2 - t1);
    }

    //舒适噪声
    if ((speech_player->aec_handle != NULL) && (speech_player->g_p_speech_aec_prms->cng_enable == 1))
    {
        tmp_aec_inout.Txinbuf_mic = (short *) post_deal_buf;
        tmp_aec_inout.RxinBuf_spk = (short *) NULL;
        tmp_aec_inout.cnt = speech_player->aec_deal_block / 2;
        hfp_speech_cmd(speech_player->aec_handle, HS_CMD_CNG_PROCESS, (uint32) &tmp_aec_inout);
    }

    //DUMP POST RES
    if ((speech_player->p_asqt_dump_buf != NULL) && (drop_dump_flag == FALSE))
    {
        dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_POST_RES, post_deal_buf,
                speech_player->aec_deal_block);
    }

    //DUMP AEC INFO --> not support at new hfp.al
    if ((speech_player->p_asqt_dump_buf != NULL) && (drop_dump_flag == FALSE))
    {
        libc_memset(&tmp_sv_info, 0x00, sizeof(tmp_sv_info));
        dump_data_to_asqt_dump_buffer(speech_player->p_asqt_dump_buf, SP_ASQT_DUMP_AEC_INFO, (uint8 *) &tmp_sv_info,
                sizeof(tmp_sv_info));
    }

    //ASQT离线模式下不用填充SCO OUT BUFFER
    if (speech_player->speech_setting.fill_sco_out == TRUE)
    {
        if (speech_player->speech_setting.use_sco_data_postdeal == TRUE)
        {
            uint8 *p_EncodeDataIn = (uint8 *)(PEQ_TEMP_BUFFER_ADDR);
            uint16 *p_EncodeDataout = (uint16 *)(PEQ_TEMP_BUFFER_ADDR + SP_POST_IN_BUF_LEN);
            uint32 post_deal_buf_index = 0;
            uint32 post_deal_buf_remain = speech_player->aec_deal_block;

            while (post_deal_buf_remain + speech_player->sp_post_buffer_remain >= SP_POST_BLOCK_LENGTH*2)
            {
                post_inout_t temp_post_inout;
                uint32 load_length;
                uint32 t1, t2;

                if (speech_player->sp_post_buffer_remain > 0)
                {
                    libc_memcpy(p_EncodeDataIn, speech_player->sp_post_buffer, speech_player->sp_post_buffer_remain);

                    load_length = SP_POST_BLOCK_LENGTH*2 - speech_player->sp_post_buffer_remain;
                }
                else
                {
                    load_length = SP_POST_BLOCK_LENGTH*2;
                }

                libc_memcpy(p_EncodeDataIn + speech_player->sp_post_buffer_remain, post_deal_buf + post_deal_buf_index, load_length);

                if (speech_player->need_print_time == 1)
                {
                    t1 = sys_get_ab_timer_us();
                }

                temp_post_inout.post_in  = p_EncodeDataIn;
                temp_post_inout.post_out = p_EncodeDataout;
                temp_post_inout.in_len = SP_POST_BLOCK_LENGTH*2;
                sp_post_cmd(speech_player->sp_post_handle, SP_POST_CMD_FRAME_DEAL, &temp_post_inout);

                if (speech_player->need_print_time == 1)
                {
                    t2 = sys_get_ab_timer_us();

                    speech_player->post_deal_time += (t2 - t1);
                }

                if (buffer_rw_write(speech_player->sco_output_buf, p_EncodeDataout, SP_POST_BLOCK_LENGTH) == FALSE)
                {
                    if (speech_player->print_sco_out_full_flag == FALSE)
                    {
                        libc_printf_warning("sco_out full!\n");
                        speech_player->print_sco_out_full_flag = TRUE;
                    }
                }
                else
                {
                    speech_player->print_sco_out_full_flag = FALSE;
                }

                post_deal_buf_index += load_length;
                post_deal_buf_remain -= load_length;
                speech_player->sp_post_buffer_remain = 0;
            }

            if (post_deal_buf_remain > 0)
            {
                libc_memcpy(speech_player->sp_post_buffer, post_deal_buf + post_deal_buf_index, post_deal_buf_remain);
                speech_player->sp_post_buffer_remain = post_deal_buf_remain;
            }
        }
        else
        {
            //SCO硬编码
            if (buffer_rw_write(speech_player->sco_output_buf, post_deal_buf, speech_player->aec_deal_block) == FALSE)
            {
                if (speech_player->print_sco_out_full_flag == FALSE)
                {
                    libc_printf_warning("sco_out full!\n");
                    speech_player->print_sco_out_full_flag = TRUE;
                }
            }
            else
            {
                speech_player->print_sco_out_full_flag = FALSE;
            }
        }
    }

    //POST BUFFER 减去BLOCK
    speech_player->post_process_buf.read_ptr += speech_player->aec_deal_block;
    speech_player->post_process_buf.read_ptr %= speech_player->post_process_buf.capacity;
    speech_player->post_process_buf.length -= speech_player->aec_deal_block;
}
