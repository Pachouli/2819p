/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASET音效调试模块，与PC ASET工具通信。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_aset_inner.h"
#include "common_func_inner.h"

#if (SUPPORT_ASET_TEST == 1)

#define UPDATE_PEQ_MODE_PEQ        0
#define UPDATE_PEQ_MODE_LIMITER    4
#define UPDATE_PEQ_MODE_PEQ2       11
#define UPDATE_PEQ_MODE_PEQ3       12
#define UPDATE_PEQ_MODE_DRC2       20

int32 aset_read_volume(void)
{
    int volume;
    int *p_volume = (int *) (g_aset_var.cmd_buffer + 8);

    int ret_val;
    uint8 cur_volume;

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_VOLUME, g_aset_var.cmd_buffer, sizeof(int));

    if (ret_val == 0)
    {
        libc_memcpy(&volume, p_volume, sizeof(int));

        //update volume
        cur_volume = (uint8) volume;//0-31

        if (cur_volume > VOLUME_VALUE_MAX)
        {
            cur_volume = VOLUME_VALUE_MAX;
        }

        com_volume_set_inner(SET_VOL_BT_TOOLS, cur_volume);
    }

    return 0;
}

int32 aset_read_main_switch(void)
{
    int main_switch;
    int *p_main_switch = (int *) (g_aset_var.cmd_buffer + 8);

    int ret_val;

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_MAIN_SWITCH, g_aset_var.cmd_buffer, sizeof(int));

    if (ret_val == 0)
    {
        libc_memcpy(&main_switch, p_main_switch, sizeof(int));

        g_aset_var.main_switch_flag = (bool) main_switch;

        if (g_aset_var.main_switch_flag == 0)
        {
            g_aset_var.p_dae_cfg->precut = 0;
            g_aset_var.p_dae_cfg->bypass = 1;
        }
        else
        {
            g_aset_var.p_dae_cfg->precut = g_aset_var.main_gain_value;
            g_aset_var.p_dae_cfg->bypass = 0;
        }
    }

    return 0;
}

void update_peq_point(dae_config_t *p_dae, int8 peq_point_start, aset_peq_point_t *peq_points, int8 peq_point_count)
{
    int i;

    for (i = 0; i < peq_point_count; i++)
    {
        if (peq_points[i].enable != 0)
        {
            p_dae->peq_bands[peq_point_start + i].cutoff = (uint16) peq_points[i].freq;
            p_dae->peq_bands[peq_point_start + i].q = (uint16) peq_points[i].Qval;
            p_dae->peq_bands[peq_point_start + i].gain = peq_points[i].gain;
            p_dae->peq_bands[peq_point_start + i].type = (uint16) peq_points[i].type;
            p_dae->music_eq_id[peq_point_start + i] = peq_points[i].enable;

            //libc_printf("cutoff: %d, Qval: %d, gain: %d, type: %d\n", p_dae->peq_bands[peq_point_start + i].cutoff,
            //p_dae->peq_bands[peq_point_start + i].q,
            //p_dae->peq_bands[peq_point_start + i].gain,
            //p_dae->peq_bands[peq_point_start + i].type);
        }
        else
        {
            p_dae->peq_bands[peq_point_start + i].gain = 0;
            p_dae->music_eq_id[peq_point_start + i] = 0;
        }
    }
}

void update_peq_value(dae_config_t *p_dae, void *aset_data, uint8 update_mode)
{
    if (update_mode == UPDATE_PEQ_MODE_PEQ)
    {
        aset_peq_data_t *aset_peq_data = (aset_peq_data_t *) aset_data;

        //判断是否使能PEQ
        p_dae->peq_enable = aset_peq_data->peq_enable;

        //均衡器使能
        if (aset_peq_data->peq_enable == 1)
        {
            update_peq_point(p_dae, 0, aset_peq_data->peq_array, aset_peq_data->nPointTotal);
        }

        if (aset_peq_data->main_gain_enable == 1)
        {
            g_aset_var.main_gain_value = aset_peq_data->main_gain_value;

            p_dae->precut = g_aset_var.main_gain_value;
        }
        else
        {
            p_dae->precut = 0;
        }
    }
    else if (update_mode == UPDATE_PEQ_MODE_PEQ2)
    {
        aset_peq_data_2_3_t *aset_eq_data_2_3 = (aset_peq_data_2_3_t *) aset_data;

        if (EQMAXPOINTCOUNT_ALL >= EQMAXPOINTCOUNT*2)
        {
            update_peq_point(p_dae, EQMAXPOINTCOUNT, aset_eq_data_2_3->peq_array, EQMAXPOINTCOUNT);
        }
        else
        {
            update_peq_point(p_dae, EQMAXPOINTCOUNT, aset_eq_data_2_3->peq_array, EQMAXPOINTCOUNT_ALL - EQMAXPOINTCOUNT);
        }
    }
    else if (update_mode == UPDATE_PEQ_MODE_PEQ3)
    {
        aset_peq_data_2_3_t *aset_eq_data_2_3 = (aset_peq_data_2_3_t *) aset_data;

        update_peq_point(p_dae, EQMAXPOINTCOUNT*2, aset_eq_data_2_3->peq_array, EQMAXPOINTCOUNT_ALL - EQMAXPOINTCOUNT*2);
    }
    else if (update_mode == UPDATE_PEQ_MODE_LIMITER)
    {
        aset_limiter_data_t *aset_limiter_data = (aset_limiter_data_t *) aset_data;

        p_dae->limiter_threshold = aset_limiter_data->threhold;
        p_dae->limiter_attack_time = aset_limiter_data->attack_time;
        p_dae->limiter_release_time = aset_limiter_data->release_time;
        p_dae->limiter_enable = aset_limiter_data->limiter_enable;
    }
    else if (update_mode == UPDATE_PEQ_MODE_DRC2)
    {
        DRC_all_data_t *aset_drc_data = (DRC_all_data_t *) aset_data;

        p_dae->drc_enable        = aset_drc_data->bDrcEnable;
        p_dae->drc_crossover     = aset_drc_data->crossover;
        p_dae->pre_softvol_gain  = aset_drc_data->input_gain;
        p_dae->post_softvol_gain = aset_drc_data->output_gain;
        libc_memcpy(&(p_dae->drc_bands[0]), &(aset_drc_data->drc_bands[0]), sizeof(drc_band_t));
        libc_memcpy(&(p_dae->drc_bands[1]), &(aset_drc_data->drc_bands[1]), sizeof(drc_band_t));
    }
    else
    {
        ; //nothing
    }
}

int32 aset_read_eq_data(void)
{
    int ret_val;

    aset_peq_data_t *eq_data_buffer = (aset_peq_data_t *) (g_aset_var.cmd_buffer + 8);
    aset_peq_data_2_3_t *eq_data_2_3_buffer = (aset_peq_data_2_3_t *) (g_aset_var.cmd_buffer + 8);

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_EQ_DATA, g_aset_var.cmd_buffer, sizeof(aset_peq_data_t));

    if (ret_val == 0)
    {
        //update eq
        update_peq_value(g_aset_var.p_dae_cfg, eq_data_buffer, UPDATE_PEQ_MODE_PEQ);
    }

    if (EQMAXPOINTCOUNT_ALL > EQMAXPOINTCOUNT)
    {
        ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_EQ2_DATA, g_aset_var.cmd_buffer, sizeof(aset_peq_data_2_3_t));

        if (ret_val == 0)
        {
            //update eq
            update_peq_value(g_aset_var.p_dae_cfg, eq_data_2_3_buffer, UPDATE_PEQ_MODE_PEQ2);
        }
    }

    if (EQMAXPOINTCOUNT_ALL > EQMAXPOINTCOUNT*2)
    {
        ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_EQ3_DATA, g_aset_var.cmd_buffer, sizeof(aset_peq_data_2_3_t));

        if (ret_val == 0)
        {
            //update eq
            update_peq_value(g_aset_var.p_dae_cfg, eq_data_2_3_buffer, UPDATE_PEQ_MODE_PEQ3);
        }
    }

    return 0;
}

int32 aset_read_limiter_data(void)
{
    int ret_val;

    aset_limiter_data_t *limiter_data_buffer = (aset_limiter_data_t *) (g_aset_var.cmd_buffer + 8);

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_LIMITER_DATA, g_aset_var.cmd_buffer, sizeof(aset_limiter_data_t));

    if (ret_val == 0)
    {
        //update eq
        update_peq_value(g_aset_var.p_dae_cfg, limiter_data_buffer, UPDATE_PEQ_MODE_LIMITER);
    }

    return 0;
}

int32 aset_read_drc_data(void)
{
    int ret_val;

    DRC_all_data_t *drc_data_buffer = (DRC_all_data_t *) (g_aset_var.cmd_buffer + 8);

    ret_val = com_stub_read_ext_cmd_inner(STUB_CMD_ASET_READ_DRC2_DATA, g_aset_var.cmd_buffer, sizeof(DRC_all_data_t));

    if (ret_val == 0)
    {
        //update drc
        if (com_get_config_default_inner(DAE_DRC_ENABLE) == 1)
        {
            update_peq_value(g_aset_var.p_dae_cfg, drc_data_buffer, UPDATE_PEQ_MODE_DRC2);
        }
    }

    return 0;
}

int32 aset_cmd_deal(aset_status_t *aset_status)
{
    int32 sound_effect_changed;

    if (aset_status->upload_case_info == TRUE)
    {
        aset_write_case_info();
    }
    else if (aset_status->state == 1)
    {
        sound_effect_changed = FALSE;

        if (aset_status->volume_changed == TRUE)
        {
            aset_read_volume();

            //sound_effect_changed = TRUE;
        }

        if (aset_status->eq_data_changed == TRUE)
        {
            aset_read_eq_data();

            sound_effect_changed = TRUE;
        }

        if (aset_status->limiter_data_changed == TRUE)
        {
            aset_read_limiter_data();

            sound_effect_changed = TRUE;
        }

        if (aset_status->drc_data_changed == TRUE)
        {
            aset_read_drc_data();

            if (com_get_config_default_inner(DAE_DRC_ENABLE) == 1)
            {
                sound_effect_changed = TRUE;
            }
        }

        if (aset_status->main_switch_changed == TRUE)
        {
            aset_read_main_switch();

            sound_effect_changed = TRUE;
        }

        if (sound_effect_changed == TRUE)
        {
            com_update_dae_config(g_aset_var.p_dae_cfg);
        }
    }
    else
    {
        ; //nothing for QAC
    }

    return 0;
}

static const int8 project_ver[] = "281B101";

static void config_pc_view(aset_interface_config_t *pCfg)
{
    pCfg->bEQ_v_1_1 = 1;
    pCfg->bLimiter_v_1_0 = 1;
    pCfg->bMS_v_1_0 = 1;
    pCfg->bMDRC_v_2_0 = 1;
    libc_memcpy(pCfg->project_ver, project_ver, 7);
    pCfg->project_ver[7] = 0x00;
}

int32 aset_write_case_info(void)
{
    uint8 *cmd_buf_rev_8bhead_2btail = g_aset_var.cmd_buffer;
    uint8 *cmd_buf_payload = g_aset_var.cmd_buffer + 8;
    aset_case_info_t *aset_case_info = (aset_case_info_t *) (cmd_buf_payload);

    libc_memset(aset_case_info, 0x00, sizeof(aset_case_info_t));
    aset_case_info->peq_point_num = EQMAXPOINTCOUNT_ALL;
    aset_case_info->max_volume = VOLUME_VALUE_MAX;
    config_pc_view(&aset_case_info->stInterface);

    com_stub_write_ext_cmd_inner(STUB_CMD_ASET_WRITE_STATUS, cmd_buf_rev_8bhead_2btail, sizeof(aset_case_info_t));

    return 0;
}

//0-non aux mode, 1-aux mode
int32 aset_switch_aux_mode_inner(uint8 mode)
{
    uint8 *cmd_buf_rev_8bhead_2btail = g_aset_var.cmd_buffer;
    uint8 *cmd_buf_payload = g_aset_var.cmd_buffer + 8;
    application_properties_t *app_pro_info = (application_properties_t *) (cmd_buf_payload);

    libc_memset(app_pro_info, 0x00, sizeof(application_properties_t));
    app_pro_info->aux_mode = mode;

    com_stub_write_ext_cmd_inner(STUB_CMD_ASET_SWTICH_AUX_MODE, cmd_buf_rev_8bhead_2btail, sizeof(application_properties_t));

    return 0;
}

#endif
