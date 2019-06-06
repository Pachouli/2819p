/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ��ǰ̨Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

/*ȫ�ֱ�������*/

uint8 last_disp_status = STATUS_NONE;
uint8 g_disp_status = STATUS_NONE;
bool last_sco_created_flag;

btengine_info_t g_btcall_bt_info;

btcall_info_t g_btcall_info;

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_FRONT_LOW_PRIO;

bool g_chf_cag_exch_enable;

bool g_speech_play_flag;

bool g_speech_peq_flag;

void *callring_handle;
callring_status_e g_cr_status;
uint32 g_cr_timer;
bool callring_wait_sco_flag; //�����ȴ�2S�ӿ�������SCO�����������оͲ��ò���Ĭ������������
uint32 g_wait_sco_timer; //�����ȴ�2S��ʱ����Ҳ���Ǹս���ͨ��������ʱ���
uint8 ignore_default_ring_flag; //���Բ���Ĭ�������ı��
uint8 callin_hint_mode; //������ʾģʽ
uint8 callring_space_time; //Ĭ������������϶ʱ������λ100ms
uint8 callring_pb_space_time; //�绰���������ʱ�䣬��λ1S
bool g_pb_need_init = TRUE; //�Ƿ���Ҫ��ʼ��
phonebook_play_status_e g_pb_play_status; //�绰������״̬
uint32 g_pb_break_timer; //�ȴ����ʱ��

#if (SUPPORT_ASQT_TEST == 1)

asqt_state_e g_asqt_state = ASQT_STATUS_CONFIG;
PC_curStatus_e g_pc_status;
uint32 g_pc_state_timer;
uint8 g_pc_state_count;
uint8 g_pc_pa_volume = 0xff;

asqt_configs_t g_asqt_configs;
asqt_dump_buf_t g_asqt_dump_buf;
asqt_dump_tag_t g_asqt_dump_tag;
buffer_rw_t *p_sco_input_buf;
uint16 g_dump_data_index;

buffer_rw_t g_sco_in_cache;
bool need_load_full;

asqt_config_t g_asqt_config;
uint8 g_asqt_config_count;

#endif

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _read_var(void)
{
    g_chf_cag_exch_enable = (uint8) com_get_config_default(BTCALL_FRONT_CHF_CAG_EXCH_ENABLE);
    callin_hint_mode = (uint8) com_get_config_default(BTCALL_CALLIN_HINT_MODE);
    callring_space_time = (uint8) com_get_config_default(BTCALL_CALLIN_RING_SPACE_TIME);
    callring_pb_space_time = (uint8) com_get_config_default(BTCALL_CALLIN_PB_SPACE_TIME);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_init(void)
{
    /*����VM����*/
    _read_var();

    /*��ʼ��applib�⣬ǰ̨AP*/
    applib_init(APP_ID_BTCALL, THIS_APP_TYPE);

    /*��ʼ����ʱ��*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{
    //�˳�Ӧ��ʱȷ��������Ϊ44KHz
    set_dac_rate(44);

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    com_save_sys_comval();

    /*ִ��applib���ע������*/
    applib_quit(THIS_APP_TYPE);
}

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //�鿴��ǰ�����Ƿ�Ϊmusic
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //ע��radio��̨
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

void _create_btcall_engine(void)
{
    msg_apps_t msg;

    //����Ϣ����
    msg.para[0] = APP_ID_BTCALL_EG;
    msg.para[1] = 0;
    msg.content.addr = NULL;

    //��Ϣ����(����Ϣ����)
    msg.type = MSG_CREAT_APP_SYNC;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e result = RESULT_NULL;
    uint32 wait_status_update_timer;
    bool is_asqt_mode = FALSE;

    /*ap��ʼ��*/
    _app_init();

    _kill_conflict_engine();

    //��ͨ�绰������ͨ������
    com_set_sound_volume_mode(VOLUME_MODE_CALL, VOLUME_GAIN_CALL);

    com_led_breath(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    com_speaker_on(SPK_PLAY);
    change_app_state (APP_STATE_PLAYING);
    //�ر�TTS����
    //NOTE!!���ܿ���TTS��������Ĭ�������������绰���������ͻ!!
    com_tts_state_play_control(TTS_CTRL_IGNORE);

#if (SUPPORT_ASQT_TEST == 1)
    //����BT ENGINE
    if (com_get_stub_tools_type() == STUB_PC_TOOL_ASQT_MODE)
    {
        //�رհ�����
        keytone_set_on_off (FALSE);

        com_creat_bt_engine(0);
        sys_os_time_dly(300); //�ȴ�3S�������ȶ�����
        is_asqt_mode = TRUE;
    }
#endif

    //���� BTCALL ����
    _create_btcall_engine();

    result = get_message_loop();

#if (SUPPORT_ASQT_TEST == 1)
    //����BT ENGINE
    if (is_asqt_mode == TRUE)
    {
        _kill_conflict_engine();
        com_close_bt_engine();
        com_creat_bt_engine(0);
        sys_os_time_dly(300); //�ȴ�3S�������ȶ�����
    }
#endif

    //�ָ�TTS����
    com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
    change_app_state (APP_STATE_NO_PLAY);
    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    //�Ͽ�������Ҫ�������Ͽ����ӡ�TTS
    wait_status_update_timer = sys_get_ab_timer();
    while (1)
    {
        if ((sys_get_ab_timer() - wait_status_update_timer) >= 250)
        {
            break;
        }

        com_get_btengine_status(&g_btcall_bt_info);
        if (g_btcall_bt_info.status == STATUS_WAIT_PAIRED)
        {
            //�������������Ͽ�&�ȴ�����
            tts_play_ret_e temp_tts_ret;
            temp_tts_ret = com_tts_state_play(TTS_BT_DISCON, 0);
            if ((temp_tts_ret == TTS_PLAY_RET_BY_KEY) || (temp_tts_ret == TTS_PLAY_RET_BY_SYS))
            {
                //��������������Ͽ�������ֹ�ˣ���ô�ȴ����ӾͲ��ٲ�����
                break;
            }
            com_tts_state_play(TTS_WAIT_BT_CON, 0);
            break;
        }
    }

    com_ap_switch_deal(result);

    _app_exit();

    return 0;
}
