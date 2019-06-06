/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASQT调试处理程序，与PC ASQT工具通信。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

#if (SUPPORT_ASQT_TEST == 1)

uint8 __section__(".bss.asqt_dump_buffer") ASQT_DUMP_BUFFER_ADDR[ASQT_DUMP_BUFFER_SIZE];
uint8 __section__(".bss.asqt_sco_in_cache") ASQT_SCO_IN_CACHE_ADDR[ASQT_SCO_IN_CACHE_SIZE];
uint8 __section__(".bss.asqt_stub_rw") STUB_RW_BUFFER_ADDR[STUB_RW_DATA_SIZE];

bool write_sco_in_cache(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len)
{
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if ((p_buf_rw->length + w_len) > p_buf_rw->capacity)
    {
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    if ((p_buf_rw->write_ptr + w_len) > p_buf_rw->capacity)
    {
        read_len = p_buf_rw->capacity - p_buf_rw->write_ptr;
        rem_len = w_len - read_len;
    }
    else
    {
        read_len = w_len;
        rem_len = 0;
    }
    libc_memcpy(p_buf_rw->raw_data + p_buf_rw->write_ptr, w_buf, read_len);

    if (rem_len != 0)
    {
        libc_memcpy(p_buf_rw->raw_data, w_buf + read_len, rem_len);
    }

    p_buf_rw->write_ptr += w_len;
    p_buf_rw->write_ptr %= p_buf_rw->capacity;

    p_buf_rw->length += w_len;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//ASQT DUMP TAG
bool read_dump_tag_from_asqt_dump_buffer(asqt_dump_buf_t *p_asqt_dump_buf, asqt_dump_tag_t *p_asqt_dump_tag)
{
    uint32 temp_flag = sys_local_intc_save(0x01);

    if (p_asqt_dump_buf->dump_length == 0)
    {
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    libc_memcpy(p_asqt_dump_tag, p_asqt_dump_buf->dump_buf + p_asqt_dump_buf->dump_read_ptr, 4);

    p_asqt_dump_buf->dump_read_ptr += 4;
    p_asqt_dump_buf->dump_read_ptr %= p_asqt_dump_buf->dump_capacity;
    p_asqt_dump_buf->dump_length -= 4;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//ASQT DUMP DATA
bool read_dump_data_from_asqt_dump_buffer(asqt_dump_buf_t *p_asqt_dump_buf, asqt_dump_tag_t *p_asqt_dump_tag, uint8 *dump_buf)
{
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if (p_asqt_dump_buf->dump_length == 0)
    {
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    if ((p_asqt_dump_buf->dump_read_ptr + p_asqt_dump_tag->length) > p_asqt_dump_buf->dump_capacity)
    {
        read_len = p_asqt_dump_buf->dump_capacity - p_asqt_dump_buf->dump_read_ptr;
        rem_len = p_asqt_dump_tag->length - read_len;
    }
    else
    {
        read_len = p_asqt_dump_tag->length;
        rem_len = 0;
    }

    libc_memcpy(dump_buf, p_asqt_dump_buf->dump_buf + p_asqt_dump_buf->dump_read_ptr, read_len);

    if ( rem_len != 0 )
    {
        libc_memcpy(dump_buf + read_len, p_asqt_dump_buf->dump_buf, rem_len);
    }

    p_asqt_dump_buf->dump_read_ptr += p_asqt_dump_tag->length;
    p_asqt_dump_buf->dump_read_ptr %= p_asqt_dump_buf->dump_capacity;
    p_asqt_dump_buf->dump_length -= p_asqt_dump_tag->length;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

bool asqt_init_dump_buffer(void)
{
    libc_memset(&g_asqt_dump_buf, 0x00, sizeof(g_asqt_dump_buf));
    g_asqt_dump_buf.dump_buf = (uint8 *) ASQT_DUMP_BUFFER_ADDR;

    if (g_asqt_configs.analysis_mode == 0)
    {
        g_asqt_dump_buf.dump_capacity = ASQT_DUMP_BUFFER_SIZE;
        //DUMP MIC IN/SPK IN/POST RES/AEC INFO
        g_asqt_dump_buf.dump_bitmap = SP_ASQT_DUMP_MIC_IN|SP_ASQT_DUMP_SPK_IN|SP_ASQT_DUMP_POST_RES|SP_ASQT_DUMP_AEC_INFO;
        g_asqt_dump_buf.sco_in_flag = TRUE;
    }
    else
    {
        g_asqt_dump_buf.dump_capacity = ASQT_DUMP_BUFFER_SIZE + ASQT_SCO_IN_CACHE_SIZE;
        //DUMP SCO IN/MIC IN/SPK IN/POST RES/AEC INFO
        g_asqt_dump_buf.dump_bitmap = SP_ASQT_DUMP_SCO_IN|SP_ASQT_DUMP_MIC_IN|SP_ASQT_DUMP_SPK_IN|SP_ASQT_DUMP_POST_RES|SP_ASQT_DUMP_AEC_INFO;
        g_asqt_dump_buf.sco_in_flag = FALSE;
    }

    g_sco_in_cache.raw_data = ASQT_SCO_IN_CACHE_ADDR;
    g_sco_in_cache.capacity = ASQT_SCO_IN_CACHE_SIZE;
    g_sco_in_cache.length = 0;
    g_sco_in_cache.read_ptr = 0;
    g_sco_in_cache.write_ptr = 0;
    need_load_full = TRUE;

    return TRUE;
}

void asqt_set_config(void)
{
    libc_memset(&g_asqt_config, 0, sizeof(g_asqt_config));
    libc_memcpy(g_asqt_config.fw_version, "281B100", 8);
    g_asqt_config.eq_point_nums = 14;
    g_asqt_config.eq_speaker_nums = 14 - com_get_config_default(BTCALL_PEQ_BAND_NUM_POSTREAT);
    g_asqt_config.eq_version = 1;
    g_asqt_config.sample_rate = SR_8KHZ;
    libc_memset(STUB_RW_BUFFER_ADDR,0,sizeof(g_asqt_config)+8);
    libc_memcpy(STUB_RW_BUFFER_ADDR+8,&g_asqt_config, sizeof(g_asqt_config));
    com_stub_write_cmd(STUB_CMD_ASQT_WRITE_CONFIG, STUB_RW_BUFFER_ADDR, sizeof(g_asqt_config));
}

void asqt_update_volume(int pa_volume)
{
    if ((uint8) pa_volume > VOLUME_VALUE_CALL_MAX)
    {
        pa_volume = VOLUME_VALUE_CALL_MAX;
    }
    if ((uint8) pa_volume != g_pc_pa_volume)
    {
        g_pc_pa_volume = (uint8) pa_volume;
        com_volume_set(SET_VOL_BT_TOOLS, g_pc_pa_volume);
    }
}

void asqt_update_peq(asqt_peq_status_t * p_asqt_peq_status)
{
    asqt_peq_data_t * p_asqt_peq_data;

    if (p_asqt_peq_status->peq_changed == 1)
    {
        peq_band_t * p_peq_array;
        uint8 i;

        if (com_stub_read_cmd(STUB_CMD_ASQT_READ_EQ1_PARA, STUB_RW_BUFFER_ADDR, sizeof(asqt_peq_data_t)) < 0)
        {
            return;
        }
        p_asqt_peq_data = (asqt_peq_data_t *)STUB_RW_BUFFER_ADDR;

        p_peq_array = (peq_band_t *) sys_malloc(sizeof(peq_band_t)*14);

        for (i = 0; i < 14; i++)
        {
            p_peq_array[i].cutoff = (uint16) p_asqt_peq_data->peq_array[i].freq;
            p_peq_array[i].q = (uint16) p_asqt_peq_data->peq_array[i].Qval;
            p_peq_array[i].type = (uint16) p_asqt_peq_data->peq_array[i].type;

            if (p_asqt_peq_data->peq_array[i].enable != 0)
            {
                p_peq_array[i].gain = p_asqt_peq_data->peq_array[i].gain;
            }
            else
            {
                p_peq_array[i].gain = 0;
            }
        }

        com_update_speech_peq_para(p_asqt_peq_data->peq_enable, p_peq_array);

        sys_free(p_peq_array);
    }
}

//ASQT LOOP
void asqt_loop_deal(void)
{
    int pc_state, pa_volume, volume_call_gain;
    asqt_peq_status_t asqt_peq_status;
    asqt_configs_ext_t * p_asqt_configs_ext;

    //只在刚连接ASQT时发送该0x10号配置命令
    if (g_asqt_config_count < 3)
    {
        asqt_set_config();
        g_asqt_config_count++;
        sys_os_time_dly(1);
        return;
    }

    //STUB READ STATUS
    if ((sys_get_ab_timer() - g_pc_state_timer) >= 250)
    {
        com_get_btengine_status(&g_btcall_bt_info);

        g_pc_state_count++;
        if (g_pc_state_count >= 2)
        {
            g_pc_state_count = 0;

            if (com_stub_read_cmd(STUB_CMD_ASQT_READ_STATUS, &pc_state, 4) < 0)
            {
                return;
            }
            g_pc_status = (PC_curStatus_e) pc_state;

            if (g_asqt_state == ASQT_STATUS_RUN)
            {
                bool need_update_speech = FALSE;
                com_stub_read_cmd(STUB_CMD_ASQT_READ_CONFIG_EXT, STUB_RW_BUFFER_ADDR, sizeof(asqt_configs_ext_t));
                p_asqt_configs_ext = (asqt_configs_ext_t *)STUB_RW_BUFFER_ADDR;
                pa_volume = p_asqt_configs_ext->volume_data;
                volume_call_gain = p_asqt_configs_ext->cfg_data.volume_call_gain;
                libc_memcpy(&asqt_peq_status, &(p_asqt_configs_ext->peq_status_data), sizeof(asqt_peq_status));
                if (volume_call_gain != g_asqt_configs.volume_call_gain)
                {
                    g_asqt_configs.volume_call_gain = volume_call_gain;
                    //update volume call gain
                    com_set_sound_volume_gain(0 - volume_call_gain*10);
                }
                if (libc_memcmp(&g_asqt_configs, &(p_asqt_configs_ext->cfg_data), sizeof(g_asqt_configs)) != 0)
                {
                    libc_memcpy(&g_asqt_configs, &(p_asqt_configs_ext->cfg_data), sizeof(g_asqt_configs));
                    need_update_speech = TRUE;
                }
                //实时更新音量
                asqt_update_volume(pa_volume);
                //实时更新PEQ参数
                asqt_update_peq(&asqt_peq_status);
                //实时更新AEC参数
                if (need_update_speech == TRUE)
                {
                    //强制退出中间件，然后再重新装载中间件
                    if (g_speech_play_flag == TRUE)
                    {
                        if (g_asqt_configs.analysis_mode == 0)
                        {
                            _speech_stop(FALSE);
                        }
                        else
                        {
                            _speech_stop(TRUE);
                        }
                    }
                    g_asqt_state = ASQT_STATUS_CONFIG;
                }
            }
            else
            {
                g_pc_pa_volume = 0xff;
            }
        }

        g_pc_state_timer = sys_get_ab_timer();
    }

    if ((g_asqt_state == ASQT_STATUS_CONFIG) && ((g_pc_status != sUserStart) && (g_pc_status != sUserUpdate)))
    {
        return;
    }

    if (g_asqt_state == ASQT_STATUS_CONFIG)
    {
        //sUserUpdate读参数，PC工具状态就会转换为sUserStart
        if ((g_pc_status == sUserStart) || (g_pc_status == sUserUpdate))
        {
            if (com_stub_read_cmd(STUB_CMD_ASQT_READ_CONFIG_EXT, STUB_RW_BUFFER_ADDR, sizeof(asqt_configs_ext_t)) < 0)
            {
                return;
            }

            p_asqt_configs_ext = (asqt_configs_ext_t *)STUB_RW_BUFFER_ADDR;
            pa_volume = p_asqt_configs_ext->volume_data;
            libc_memcpy(&asqt_peq_status, &(p_asqt_configs_ext->peq_status_data), sizeof(asqt_peq_status));
            libc_memcpy(&g_asqt_configs, &(p_asqt_configs_ext->cfg_data), sizeof(g_asqt_configs));
            volume_call_gain = g_asqt_configs.volume_call_gain;

            //update volume call gain
            com_set_sound_volume_gain(0 - volume_call_gain*10);
            //实时更新音量
            asqt_update_volume(pa_volume);

            //切换为通话场景PEQ参数
            if (g_speech_peq_flag == FALSE)
            {
                com_set_speech_peq_para(TRUE);
                g_speech_peq_flag = TRUE;
            }

            //实时更新PEQ参数
            asqt_update_peq(&asqt_peq_status);

            //INIT DUMP BUFFER
            asqt_init_dump_buffer();

            if (g_pc_status == sUserStart)
            {
                if (g_speech_play_flag == FALSE)
                {
                    if (g_asqt_configs.analysis_mode == 0)
                    {
                        p_sco_input_buf = (buffer_rw_t *) sys_shm_mount(SHARE_MEM_ID_SCO_INPIPE);
                        if (p_sco_input_buf == NULL)
                        {
                            libc_printf_error("SHARE_MEM_ID_SCO_INPIPE mount fail!\n");
                            while (1);
                        }
                        p_sco_input_buf->length = 0;
                        p_sco_input_buf->write_ptr = 0;
                        p_sco_input_buf->read_ptr = 0;

                        _speech_play(FALSE);
                    }
                    else
                    {
                        _speech_play(TRUE);
                    }
                }
                g_asqt_state = ASQT_STATUS_RUN;
            }

            if (g_pc_status == sUserUpdate)
            {
                //sUserUpdate状态下可以频繁读
                com_stub_read_cmd(STUB_CMD_ASQT_READ_STATUS, &pc_state, 4);
                g_pc_status = (PC_curStatus_e) pc_state;
            }
        }
    }
    else if (g_asqt_state == ASQT_STATUS_RUN)
    {
        if (g_pc_status == sUserStop)
        {
            stop_asqt_do:

            //DUMP RESULT COMPLETE
            com_stub_write_cmd(STUB_CMD_ASQT_DUMP_RESULT_CP, NULL, 0);

            //DUMP SCO IN/PRE PLC COMPLETE
            if (g_asqt_configs.analysis_mode == 1)
            {
                com_stub_write_cmd(STUB_CMD_ASQT_DUMP_SCOIN_CP, NULL, 0);
            }

            if (g_speech_play_flag == TRUE)
            {
                if (g_asqt_configs.analysis_mode == 0)
                {
                    _speech_stop(FALSE);
                }
                else
                {
                    _speech_stop(TRUE);
                }
            }

            //必须等待状态为停止后才结束，否则会很快又创建起来
            if (g_pc_status != sUserStop)
            {
                while (1)
                {
                    if (com_get_stub_tools_type() != STUB_PC_TOOL_ASQT_MODE)
                    {
                        return;
                    }

                    com_stub_read_cmd(STUB_CMD_ASQT_READ_STATUS, &pc_state, 4);
                    g_pc_status = (PC_curStatus_e) pc_state;

                    if (g_pc_status != sUserStart)
                    {
                        g_pc_pa_volume = 0xff;
                        break;
                    }

                    sys_os_time_dly(50);
                }
            }

            g_asqt_state = ASQT_STATUS_CONFIG;

            return;
        }
    }
    else
    {
        //nothing
    }

    if (g_asqt_configs.analysis_mode == 1)
    {
        g_disp_status = g_btcall_bt_info.status;

        if ((last_sco_created_flag == TRUE) && (g_btcall_bt_info.sco_created_flag == FALSE))
        {
            last_sco_created_flag = FALSE;
            goto stop_asqt_do;
        }

        last_sco_created_flag = g_btcall_bt_info.sco_created_flag;
    }

    //STUB LOAD SCO IN
    if (g_asqt_configs.analysis_mode == 0)
    {
        if (need_load_full == TRUE)
        {
            while ((g_sco_in_cache.length + 480)  < g_sco_in_cache.capacity)
            {
                if (com_get_stub_tools_type() != STUB_PC_TOOL_ASQT_MODE)
                {
                    return;
                }

                com_stub_read_cmd(STUB_CMD_ASQT_DOWNLOAD_SCOIN, (void *) STUB_RW_BUFFER_ADDR, 480);
                write_sco_in_cache(&g_sco_in_cache, (uint8 *) STUB_RW_BUFFER_ADDR, 480);
            }
            need_load_full = FALSE;
        }

        if ((g_sco_in_cache.length + 480)  < g_sco_in_cache.capacity)
        {
            com_stub_read_cmd(STUB_CMD_ASQT_DOWNLOAD_SCOIN, (void *) STUB_RW_BUFFER_ADDR, 480);
            write_sco_in_cache(&g_sco_in_cache, (uint8 *) STUB_RW_BUFFER_ADDR, 480);
        }
    }

    //STUB DUMP 需要容错，先读TAG，再读数据
    while (read_dump_tag_from_asqt_dump_buffer(&g_asqt_dump_buf, &g_asqt_dump_tag) == TRUE)
    {
        if (com_get_stub_tools_type() != STUB_PC_TOOL_ASQT_MODE)
        {
            return;
        }

        read_dump_data_from_asqt_dump_buffer(&g_asqt_dump_buf, &g_asqt_dump_tag, (uint8 *)(STUB_RW_BUFFER_ADDR + 8 + g_dump_data_index));
        g_dump_data_index += g_asqt_dump_tag.length;

        if ((g_asqt_dump_tag.bitmap == SP_ASQT_DUMP_SCO_IN) || (g_asqt_dump_tag.bitmap == SP_ASQT_DUMP_PRE_PLC))
        {
            //DUMP SCO IN/PRE PLC
            com_stub_write_cmd(STUB_CMD_ASQT_DUMP_SCOIN, (void *) STUB_RW_BUFFER_ADDR, g_dump_data_index);
            g_dump_data_index = 0;
        }
        else if (g_asqt_dump_tag.bitmap == SP_ASQT_DUMP_AEC_INFO)
        {
            //DUMP RESULT
            com_stub_write_cmd(STUB_CMD_ASQT_DUMP_RESULT, (void *) STUB_RW_BUFFER_ADDR, g_dump_data_index);
            g_dump_data_index = 0;
        }
        else
        {
            //nothing
        }
    }
}

#endif
