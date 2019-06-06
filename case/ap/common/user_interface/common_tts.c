/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������TTS���Žӿ�ʵ�֣�֧��һ��Ƭ�κͶ��Ƭ�β��ţ�֧����ʱ���ԡ�ǿ��ֹͣ��
 *       ��̬���ء���������TTS�л��ȹ��ܡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

tts_play_infor_t g_tts_play_infor;

/*!
 *  \brief
 *  g_tts_section_ids ����Ƭ��ID�б�
 */
uint16 g_tts_section_ids[TTS_SECTIONS_MAX];

void *tts_play_handle;
tts_play_background_hook g_tts_play_bg_hook;

static bool need_tts_state_play(void);
static bool com_tts_state_sync(uint8 mode);
static tts_play_ret_e wait_tts_status_play_exit(uint16 mode);

void tts_play_var_init(void)
{
    g_tts_play_infor.enable = (uint8) com_get_config_default_inner(SETTING_SOUND_ENABLE_TTS);
    g_tts_play_infor.control_enable = (uint8) com_get_config_default_inner(SETTING_SOUND_ENABLE_CONTROL_TTS);
    g_tts_play_infor.tts_ignore = 0;
    g_tts_play_infor.tts_force_stop = 0;
    g_tts_play_infor.tts_playing = 0;

    g_tts_play_infor.language = (uint8) com_get_config_default_inner(SETTING_SOUND_TTS_LANGUAGE);
    g_tts_play_infor.volmode = (uint8) com_get_config_default_inner(SETTING_SOUND_TTS_VOLMODE);
    g_tts_play_infor.min_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_TTS_MINVOL);
    g_tts_play_infor.max_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_TTS_MAXVOL);
}

void * com_sd_mp3_player_play_tts_inner(uint8 * sec_ids, uint8 sec_cnt)
{
    uint8 i;

    for (i = 0; i < sec_cnt; i++)
    {
        g_tts_section_ids[i] = sec_ids[i] * TTS_LANGUAGE_TOTAL + g_tts_play_infor.language;
    }

    tts_play_handle = com_sd_mp3_player_play_tts_do_inner(g_tts_section_ids, sec_cnt);

    return tts_play_handle;
}

tts_play_ret_e com_tts_state_play_inner(uint8 tts_id, uint16 mode)
{
    uint8 sec_ids[1];
    uint8 section_cnt;

    //�Ƿ�TTS������ֱ���˳�
    if (tts_id >= TTS_SENTENCE_MAX)
    {
        return TTS_PLAY_RET_NO_PLAY;
    }

    //������TTS������
    if ((g_tts_play_infor.control_enable == 0) && ((mode & TTS_MODE_CONTROL_TTS_MASK) == TTS_MODE_CONTROL_TTS_YES))
    {
        return TTS_PLAY_RET_NO_PLAY;
    }

    sec_ids[0] = tts_id;
    section_cnt = 1;

    return com_tts_state_play_list_inner(sec_ids, section_cnt, mode);
}

tts_play_ret_e com_tts_state_play_ext_inner(uint8 tts_id, uint16 mode, tts_play_background_hook bg_hook)
{
    uint8 sec_ids[1];
    uint8 section_cnt;
    tts_play_ret_e tts_play_ret;

    if (g_tts_play_infor.tts_playing == 1)
    {
        //TTS����Ƕ������
        return TTS_PLAY_RET_NO_PLAY;
    }

    g_tts_play_bg_hook = bg_hook;

    //�Ƿ�TTS������ֱ���˳�
    if (tts_id >= TTS_SENTENCE_MAX)
    {
        tts_play_ret = TTS_PLAY_RET_NO_PLAY;
        goto tts_play_list_end;
    }

    //������TTS������
    if ((g_tts_play_infor.control_enable == 0) && ((mode & TTS_MODE_CONTROL_TTS_MASK) == TTS_MODE_CONTROL_TTS_YES))
    {
        tts_play_ret = TTS_PLAY_RET_NO_PLAY;
        goto tts_play_list_end;
    }

    sec_ids[0] = tts_id;
    section_cnt = 1;

    tts_play_ret = com_tts_state_play_list_inner(sec_ids, section_cnt, mode);

    tts_play_list_end:

    //���û�гɹ����������������˳�����֮ǰִ��
    if (g_tts_play_bg_hook != NULL)
    {
        //ִ�к�̨����ص�����
        g_tts_play_bg_hook();
        g_tts_play_bg_hook = NULL;
    }

    return tts_play_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ״̬��������
 * \param[in]    tts_id ״̬���
 * \param[in]    mode ����ģʽ
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  ״̬������ʹ�� 0x37000 ~ 0x3fc00 ������ⲿ�ֿռ������� bank ���룬��Ҫ
 ������� BANK ID��������� sys_bank_flush ���
 *******************************************************************************/
tts_play_ret_e com_tts_state_play_list_inner(uint8 *tts_list, uint8 section_cnt, uint16 mode)
{
    uint8 volume_backup, volume_backup_flag;
    uint8 volume_mode_backup, volume_gain_backup;
    uint8 dacclk_bk;
    tts_play_ret_e tts_play_ret;
    uint8 drc_mode_backup;

    if (need_tts_state_play() == FALSE)
    {
        return TTS_PLAY_RET_NO_PLAY;
    }

    //���󲥷�TTS
    if (com_tts_state_sync(1) == FALSE)
    {
        return TTS_PLAY_RET_NO_PLAY;
    }

    libc_printf_info("tts play:%d\n", tts_list[0]);

    g_tts_play_infor.tts_playing = 1;

    com_speaker_on_inner(SPK_SYS);

    dacclk_bk = get_dac_rate();

    volume_mode_backup = com_get_sound_volume_mode();
    volume_gain_backup = com_get_sound_volume_gain_mode();
    com_set_sound_volume_mode_inner(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    if (g_tts_play_infor.volmode == 0)
    {
        //������С����
        volume_backup_flag = 0;
        volume_backup = 0;
        if (com_get_sound_volume_inner() < g_tts_play_infor.min_volume)
        {
            volume_backup = com_get_sound_volume_inner();
            volume_backup_flag = 1;
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_tts_play_infor.min_volume);
        }
        //�����������
        else if (com_get_sound_volume_inner() > g_tts_play_infor.max_volume)
        {
            volume_backup = com_get_sound_volume_inner();
            volume_backup_flag = 1;
            com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_tts_play_infor.max_volume);
        }
    }
    else
    {
        volume_backup = com_get_sound_volume_inner();
        volume_backup_flag = 1;
        com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_tts_play_infor.max_volume);
    }

    //TTS����NormalģʽDRC
    drc_mode_backup = com_switch_drc_mode(DRC_MODE_NORMAL);

    //��ʼ����TTS
    tts_play_handle = com_sd_mp3_player_play_tts_inner(tts_list, section_cnt);
    if (tts_play_handle == NULL)
    {
        tts_play_ret = TTS_PLAY_RET_NO_PLAY;
        libc_printf_error("tts play fail!\n");
        goto _tts_state_play_list_fail_ret;
    }

    if (g_tts_play_bg_hook != NULL)
    {
        //ִ�к�̨����ص�����
        g_tts_play_bg_hook();
        g_tts_play_bg_hook = NULL;
    }

    //�ȴ�tts�������
    tts_play_ret = wait_tts_status_play_exit(mode);

    com_sd_mp3_player_stop_inner(tts_play_handle);
    tts_play_handle = NULL;

    _tts_state_play_list_fail_ret:

    //�ָ�DRCģʽ
    com_switch_drc_mode(drc_mode_backup);

    set_dac_rate(dacclk_bk);

    com_speaker_off_inner(SPK_SYS);

    //�ָ�����
    if (volume_backup_flag == 1)
    {
        com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, volume_backup);
    }

    com_set_sound_volume_mode_inner(volume_mode_backup, volume_gain_backup);

    //USB�������⴦��
    com_set_usbbox_volume_ctrl_inner(USBBOX_VOL_CTRL_RESET);

    g_tts_play_infor.tts_playing = 0;
    g_tts_play_infor.tts_force_stop = 0;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_tts_play_infor.tts_slave_limit_delay_flag == 1)
    {
        com_set_volume_limit_inner(g_tts_play_infor.slave_limit_delay);
        g_tts_play_infor.tts_slave_limit_delay_flag = 0;
        libc_printf("delay set limit\n");
    }

    if (g_tts_play_infor.tts_slave_volume_delay_flag == 1)
    {
        com_set_sound_volume_inner(SET_VOL_BY_SYSTEM, g_tts_play_infor.slave_volume_delay);
        g_tts_play_infor.tts_slave_volume_delay_flag = 0;
        libc_printf("delay set volume\n");
    }
#endif

    //TTS�������
    com_tts_state_sync(0);

    libc_printf_info("tts play over:%d\n", tts_play_ret);

    return tts_play_ret;
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void com_tts_state_play_tws_slave_inner(uint8 tts_id, uint16 mode)
{
    bool ret_val;

    if (tts_id != (uint8) -1)
    {
        if (g_tts_play_infor.slave_tts_count < 3)
        {
            g_tts_play_infor.slave_tts_ids[g_tts_play_infor.slave_tts_count] = tts_id;
            g_tts_play_infor.slave_tts_count++;
        }
    }

    if (g_tts_play_infor.slave_tts_count == 0)
    {
        return;
    }

    if (need_tts_state_play() == FALSE)
    {
        return;
    }

    if (g_tts_play_infor.slave_tts_count > 0)
    {
        tts_id = g_tts_play_infor.slave_tts_ids[0];

        g_tts_play_infor.slave_tts_count--;
        if (g_tts_play_infor.slave_tts_count > 0)
        {
            uint8 i;
            for (i = 0; i < g_tts_play_infor.slave_tts_count; i++)
            {
                g_tts_play_infor.slave_tts_ids[i] = g_tts_play_infor.slave_tts_ids[i+1];
            }
        }
    }

    //�Ƿ�TTS������ֱ���˳�
    if (tts_id >= TTS_SENTENCE_MAX)
    {
        return;
    }

    //������TTS������
    if ((g_tts_play_infor.control_enable == 0) && ((mode & TTS_MODE_CONTROL_TTS_MASK) == TTS_MODE_CONTROL_TTS_YES))
    {
        return;
    }

    libc_printf_info("slave tts play:%d\n", tts_id);

    //���󲥷�TTS
    if (com_tts_state_sync(1) == FALSE)
    {
        return;
    }

    com_tws_m2s_tts_play_req(tts_id);

    while (1)
    {
        tws_slave_tts_state_e slave_tts_state;

        ret_val = com_tws_m2s_tts_status_req(&slave_tts_state);
        if (ret_val == FALSE)
        {
            break;
        }
        if (slave_tts_state == TWS_SLAVE_STA_STOP)
        {
            break;
        }
    }

    //TTS�������
    com_tts_state_sync(0);

    libc_printf_info("slave tts play over\n");
}

#endif

static tts_play_ret_e wait_tts_status_play_exit(uint16 mode)
{
    input_gui_msg_t ui_msg;
    private_msg_t app_msg;
    tts_play_ret_e tts_play_ret = TTS_PLAY_RET_NORMAL;

    while (1)
    {
        if (com_sd_mp3_player_is_stop_inner(tts_play_handle) == TRUE)
        {
            break;
        }

        if ((mode & TTS_MODE_KEY_OPTION_MASK) != TTS_MODE_KEY_NORECV)
        {
            if (get_gui_msg_inner(&ui_msg) == TRUE)
            {
                if ((mode & TTS_MODE_KEY_OPTION_MASK) == TTS_MODE_KEY_BREAK_REDEAL)
                {
                    tts_play_ret = TTS_PLAY_RET_BY_KEY;
                    message_redeal_gui_msg_inner(&ui_msg); //���ݻ�����Ϣ
                    libc_printf_info("tts stop by key!%d\n", ui_msg.data.kmsg.val);
                    break;
                }
                else if ((mode & TTS_MODE_KEY_OPTION_MASK) == TTS_MODE_KEY_BREAK_FILTER)
                {
                    tts_play_ret = TTS_PLAY_RET_BY_KEY;
                    libc_printf_info("tts stop by key!%d\n", ui_msg.data.kmsg.val);
                    break;
                }
                else
                {
                    //ֱ�ӹ��˰�����Ϣ
                }
            }
        }

        if ((mode & TTS_MODE_SYS_OPTION_MASK) != TTS_MODE_SYS_NORECV)
        {
            if (get_app_msg_hotplug(APP_TYPE_GUI, &app_msg) == TRUE)
            {
                //ֻ���ڻᷢ��Ӧ���л�ʱ����ֹTTS����Ӧ
                if ((mode & TTS_MODE_SYS_OPTION_MASK) == TTS_MODE_SYS_BREAK_COND)
                {
                    //TODO ����ͨ������ message_box �ӿڻ�ȡ�Ƿ���Ҫ�л�Ӧ�õ���Ϣ
                    if (((app_msg.msg.type == MSG_LINEIN_OUT) && (get_cur_func_id() != APP_FUNC_PLAYLINEIN))
                        || ((app_msg.msg.type == MSG_SD_OUT) && (get_cur_func_id() != APP_FUNC_PLAYCARD))
                        || ((app_msg.msg.type == MSG_UH_OUT) && (get_cur_func_id() != APP_FUNC_PLAYUHOST))
                        || ((g_com_comval.support_usbbox == 1)
                            && (app_msg.msg.type == MSG_USB_UNSTICK) && (get_cur_func_id() != APP_FUNC_USOUND))
                        || ((g_com_comval.support_usbbox == 0)
                            && ((app_msg.msg.type == MSG_USB_STICK) || (app_msg.msg.type == MSG_USB_UNSTICK))
                            && (get_cur_app_id_inner() != APP_FUNC_CONFIG))
                        || (app_msg.msg.type == MSG_ADAPTOR_IN)
                        || (app_msg.msg.type == MSG_ADAPTOR_OUT))
                    {
                        continue;
                    }
                }

                tts_play_ret = TTS_PLAY_RET_BY_SYS;
                message_redeal_app_msg_inner(&app_msg); //���ݻ�����Ϣ
                libc_printf_info("tts stop by msg!0x%x\n", app_msg.msg.type);
                break;
            }
        }

        if (g_tts_play_infor.tts_force_stop == 1)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    return tts_play_ret;
}

static bool need_tts_state_play(void)
{
    if (g_tts_play_infor.enable == FALSE)
    {
        //��֧��״̬������������
        return FALSE;
    }

    if (g_tts_play_infor.tts_ignore == 1)
    {
        //ĳЩ������ʱ�ر�TTS����
        return FALSE;
    }

    if (g_tts_play_infor.tts_playing == 1)
    {
        //TTS����Ƕ������
        return FALSE;
    }

    if ((get_app_timer_in_handle(APP_TIMER_GROUP_GUI) == TRUE)
            || (get_app_timer_in_handle(APP_TIMER_GROUP_COMMON) == TRUE))
    {
        //��ʱ��handle�в���ִ��TTS
        libc_printf_warning("ignore tts in app timer handle!\n");
        return FALSE;
    }

    if (get_mute_enable() == TRUE)
    {
        //����״̬�£�ֱ�ӷ���
        return FALSE;
    }

    return TRUE;
}

//modeΪ1��ʾ��ʼ����������TRUE��ʾ���Բ���������FALSE��ʾ���ܲ�����
//modeΪ0��ʾ������ɣ�����ֵ������
static bool com_tts_state_sync(uint8 mode)
{
    msg_apps_t msg;
    msg_reply_t reply;
    bool bret = TRUE;

    switch (mode)
    {
        //��ͨTTS�������
        case 0:
        msg.type = MSG_TTS_STOP_SYNC;
        break;

        //��ͨTTS������ʼ
        case 1:
        msg.type = MSG_TTS_START_SYNC;
        break;

        default:
        break;
    }

    //����ͬ����Ϣ
    if (get_engine_type_inner() != ENGINE_NULL)
    {
        bret = send_sync_msg_inner(APP_ID_THEENGINE, &msg, &reply, 0);
        if ((mode == 1) && (reply.type != MSG_REPLY_SUCCESS))
        {
            bret = FALSE;
        }
    }

    return bret;
}

void com_tts_state_play_control_inner(tts_control_opr_e opr)
{
    switch (opr)
    {
        case TTS_CTRL_IGNORE:
        g_tts_play_infor.tts_ignore = 1;
        break;

        case TTS_CTRL_CANCEL_IGNORE:
        g_tts_play_infor.tts_ignore = 0;
        break;

        case TTS_CTRL_ENABLE_SWITCH:
        if (g_tts_play_infor.enable == FALSE)
        {
            g_tts_play_infor.enable = TRUE;
            //TTS��ʾ��������֮��
        }
        else
        {
            //TTS��ʾ��������֮ǰ
            g_tts_play_infor.enable = FALSE;
        }
        break;

        case TTS_CTRL_FORCE_STOP:
        g_tts_play_infor.tts_force_stop = 1;
        break;

        case TTS_CTRL_CANCEL_FORCE_STOP:
        g_tts_play_infor.tts_force_stop = 0;
        break;

        case TTS_CTRL_LANGUAGE_SWITCH:
        if (g_tts_play_infor.language < (TTS_LANGUAGE_TOTAL - 1))
        {
            g_tts_play_infor.language++;
        }
        else
        {
            g_tts_play_infor.language = 0;
        }
        break;

        default:
        break;
    }
}
