/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_radio.h"

/* Radio UI ����ջ*/
INT8U prio = AP_FRONT_LOW_PRIO;

//���������Ƶ��
const uint32 max_freq[] = { MAX_US, MAX_JP, MAX_EU };

//��������СƵ��
const uint32 min_freq[] = { MIN_US, MIN_JP, MIN_EU };

//radio ui ����������Ϣ
radio_config_t g_radio_config;

//����״̬
FM_Drv_Status_t g_engine_status;

//����״̬�����ṹ��ָ��
fmeg_status_t g_fmeg_status;
play_status_e g_last_fmeg_status;

/* ָ��ǰ�����ŵĵ�̨�б����*/
uint32 *pfreq;

//��ʶ��ǰ����Ӧ�ó���
radio_scene_e g_radio_scene;

//���ݽ��а��Զ���̨ǰ��Ƶ��
uint32 startfreq;

//����Ӧ��ģʽ
uint8 g_enter_param;

//������ʾ
bool need_tts = TRUE;

uint32 g_radio_search_th;

uint8 g_freq_tts_sec_ids[10];

//�Զ���̨����
bool  g_radio_seek_audition_enable;
uint8 g_radio_seek_audition_time;

uint32 g_number_input_freq; //���ְ��������Ƶ��
uint32 g_number_input_timer; //8S��ʱ��ʱ��
uint8 g_number_input_high; //���ְ�������ĸ�����
uint8 g_number_input_keys; //�����������������뼸�����ְ������м����Next�Ȱ����ᱻ��0

#ifdef FM_THRESHOLD_TEST
int16 __section__(".FM_TEST_BSS") fm_threshold_values[512]; //����ֵ��¼
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_load_cfg(void)
 * \����Ӧ�ó�ʼ����Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_load_cfg(void)
{
    //��ȡ����ֵ
    com_get_config_struct(RADIO_SEEK_THRESHOLD, (uint8 *)(&g_radio_search_th), sizeof(g_radio_search_th));

    g_radio_seek_audition_enable = (uint8) com_get_config_default(RADIO_SEEK_AUDITION_ENABLE);
    g_radio_seek_audition_time = (uint8) com_get_config_default(RADIO_SEEK_AUDITION_TIME);

    //��ȡradio AP ������Ϣ
    if (sys_vm_read(&g_radio_config, VM_AP_RADIO, sizeof(radio_config_t)) < 0)
    {
        g_radio_config.magic = 0xffff;
    }
    if (g_radio_config.magic != VRAM_MAGIC(VM_AP_RADIO))
    {
        g_radio_config.magic = VRAM_MAGIC(VM_AP_RADIO);

        //��������Ԥ���̨�б�����
        libc_memset(g_radio_config.Auto_tab, 0, MAX_STATION_COUNT * 4);

        g_radio_config.band_mode = (radio_band_e) com_get_config_default(RADIO_AP_BAND_SEL);
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
        g_radio_config.FMStatus.station_count = 0;

        sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));
    }

    /* ָ��ָ��ǰ���ŵ�Ƶ�ʱ���*/
    pfreq = &g_radio_config.Auto_tab[0];
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_save_cfg(void)
{
    /*����VM����*/
    com_save_sys_comval();
    sys_vm_write(&g_radio_config, VM_AP_RADIO, sizeof(g_radio_config));

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_app_init(void)
 * \RadioUI Ӧ����Ϣ��ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    radio_load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_RADIO, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_app_exit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //������Ϣ��дVRAM
    radio_save_cfg();

    /*ִ��applib���ע������*/
    applib_quit(THIS_APP_TYPE);

    return TRUE;
}

void _kill_engine(void)
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

void _create_engine(void)
{
    msg_apps_t msg;

    //����radio ��̨�������
    msg.type = MSG_CREAT_APP_SYNC;
    msg.para[0] = APP_ID_FMENGINE;
    msg.para[1] = FM_EG_ENTER_NORMAL;
    msg.content.addr = NULL;
    //����ͬ����Ϣ
    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_scene_dispatch(uint8 enter_mode)
 * \����������
 * \param[in]    enter_mode ����ģʽ
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       ���ص��δ�
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radio_scene_dispatch(uint8 enter_mode)
{
    app_result_e scene_result = RESULT_NULL;
    bool first_flag = TRUE;
    g_radio_scene = FM_SCENE_PLAYING;

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_RADIO);

    //ע��Ӧ����Ϣ�ַ�����
    set_app_msg_dispatch_handle(radio_app_msg_callback);

    /* radioUI ����ѭ��*/
    while (g_radio_scene != FM_SCENE_EXIT)
    {
        switch (g_radio_scene)
        {
            /* ����RADIO ���ų���*/
            case FM_SCENE_PLAYING:
            scene_result = radio_scene_playing(first_flag);
            first_flag = FALSE;
            if (scene_result == RESULT_RADIO_AUTO_SEARCH)
            {
                g_radio_scene = FM_SCENE_AUTO_SEARCH;
            }
            else if (scene_result == RESULT_RADIO_MANUAL_UP_SEARCH)
            {
                g_radio_scene = FM_SCENE_MANUAL_UP_SEARCH;
            }
            else if (scene_result == RESULT_RADIO_MANUAL_DOWN_SEARCH)
            {
                g_radio_scene = FM_SCENE_MANUAL_DOWN_SEARCH;
            }
            else
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            break;

            /* FM�Զ���̨���� */
            case FM_SCENE_AUTO_SEARCH:
            scene_result = radio_scene_auto_search(DIR_UP);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            /* FM�ֶ���̨���� */
            case FM_SCENE_MANUAL_UP_SEARCH:
            scene_result = radio_scene_play_search_deal(DIR_UP);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            /* FM�ֶ���̨���� */
            case FM_SCENE_MANUAL_DOWN_SEARCH:
            scene_result = radio_scene_play_search_deal(DIR_DOWN);
            if (scene_result != RESULT_NULL)
            {
                g_radio_scene = FM_SCENE_EXIT;
            }
            else
            {
                g_radio_scene = FM_SCENE_PLAYING;
            }
            break;

            default:
            //�˳���������
            g_radio_scene = FM_SCENE_EXIT;
            break;
        }
    }

    _kill_engine();

    com_set_sound_volume_mode(VOLUME_MODE_MAIN, VOLUME_GAIN_MAIN);

    return scene_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const void *argv[])
 * \radioUI app ��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e retval;
    bool result;
    //����ģʽ
    uint8 enter_mode = (uint8) (uint32) argv[0];

    g_enter_param = enter_mode;
    result = radio_app_init(); //��ʼ��
    if (result == FALSE)
    {
        //��ʼ��ʧ�ܣ��л�����һ��Ӧ��
        retval = RESULT_NEXT_FUNCTION;
    }
    else
    {
        _kill_engine();
        com_set_mute (FALSE); //�������

        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
        com_speaker_on(SPK_PLAY);

        //�л�ΪAB�๦�ţ��Խ��͸��ţ�����FM����Ч��
        com_speaker_ab_d_switch(PA_SWITCH_AB);

        retval = radio_scene_dispatch(enter_mode); //����������

        //�л�ΪD�๦�ţ��Խ��͹���
        com_speaker_ab_d_switch(PA_SWITCH_D);
    }

    com_ap_switch_deal(retval);
    radio_app_exit(); //�˳�

    return retval;
}

