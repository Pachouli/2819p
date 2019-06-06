/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  ǰ̨Ӧ�ó�ʼ����������
 ********************************************************************************/

#include "app_music.h"

//globle variable

//����music�ķ�ʽ
uint8 g_enter_mode;
//�������������
uint16 g_error_num = 0;

//��ǰ��
uint16 g_switch_prev = 0;

//ȫ�����л��̱�־
bool g_all_error_switch = FALSE;

//�ļ�·����Դ��Ϣ���棬����ʱ����
file_path_info_t g_file_path_info;

//��ǰ����״̬��Ϣ
mengine_status_t g_music_status;
play_status_e last_play_status;

//��ǰ����ʱ��ͱ�������Ϣ
mengine_playinfo_t g_play_info;

//music��ʱ����Ϣ
mmm_mp_file_info_t g_music_fileinfo;
//music ui����������Ϣ
music_config_t g_music_config;

//��ǰ������ʱ��
uint32 g_total_time;

//��ǰ����ʱ��
uint32 g_cur_time;

//ǰһ�μ�¼����ʱ��
uint32 g_prev_time;

uint8 last_minute, last_col;

//�����и赽�ڼ���
uint16 g_manul_switch_song_seq;

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_FRONT_LOW_PRIO;

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
uint8 g_fname_buffer[32];
bool  g_fname_scroll_flag;
bool  g_scroll_flag;
#endif

#define  MODE_MAX_COUNTER          (9)

const mode2disk_t g_mode2disk[MODE_MAX_COUNTER] =
{
    { PARAM_MUSIC_CARD_DIR,        DISK_H },
    { PARAM_MUSIC_UHOST_DIR,       DISK_U },
    { PARAM_MUSIC_RES_DIR,         DISK_C },
    { PARAM_MUSIC_CARD_REPLAY,     DISK_H },
    { PARAM_MUSIC_UHOST_REPLAY,    DISK_U },
    
    { PARAM_MUSIC_UHOST_DIR1_PLAY, DISK_U },
    { PARAM_MUSIC_UHOST_DIR2_PLAY, DISK_U },
    { PARAM_MUSIC_CARD_DIR1_PLAY,  DISK_H },
    { PARAM_MUSIC_CARD_DIR2_PLAY,  DISK_H },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    //AP_MUSIC_PRIO music ������ȡ
    if (sys_vm_read(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t)) < 0)
    {
        g_music_config.magic = 0xffff;
    }
    if (g_music_config.magic != VRAM_MAGIC(VM_AP_MUSIC))
    {
        g_music_config.magic = VRAM_MAGIC(VM_AP_MUSIC);

        //eq mode
        g_music_config.eq_type = EQ_NOR;

        //repeate mode
        g_music_config.repeat_mode = FSEL_MODE_LOOPALL;
        //��������
        g_music_config.card_open = (uint8) com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*����VM����*/
    sys_vm_write(&g_music_config, VM_AP_MUSIC, sizeof(g_music_config));
    com_save_sys_comval();
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  ��ʼ���ļ�ѡ����ʱ����Ĭ���̷�Ϊ���̣��ļ�����ΪCOMMONDIR
 * \li  �����̨��music���Ż��browser�Ƚ��룬ͬ��location
 * \li  music uiĬ��û���̷����䣬��playlist������˳����̷�д��browser VM����
 */
/*******************************************************************************/
bool _app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_MUSIC, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    //��ʼ���ļ�ѡ���� ��̨���д˲���

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //�˳�Ӧ��ʱȷ��������Ϊ44KHz
    set_dac_rate(44);

    //save config дvram

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    //save config дvram
    _save_cfg();

    return TRUE;
}

void _filter_sd_in_out_msg(void)
{
    msg_apps_type_e type;
    uint8 i = 0;
    //clr msf queue
    while (i < 5)
    {
        //sys_mq_flush(MQ_ID_HOTPLUG);
        if (sys_mq_receive(MQ_ID_HOTPLUG, (void *) &type) == 0)
        {
            if ((type != MSG_SD_IN) && (type != MSG_SD_OUT))
            {
                sys_mq_send(MQ_ID_HOTPLUG, (void *) &type);
            }
        }
        else
        {
            break;
        }

        i++;
    }
}

int8 set_play_disk_info(uint8 enter_mode)
{
    uint8 i=0;

    for(i=0;i<MODE_MAX_COUNTER;i++)
    {
        if(g_mode2disk[i].enter_mode == enter_mode)
        {
            break;
        }
    }

    if(i == MODE_MAX_COUNTER)
    {
        libc_printf_error("invalid enter mode!");
        return -1;
    }

    g_file_path_info.file_path.dirlocation.disk = g_mode2disk[i].play_disk;
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e retval;
    g_enter_mode = (uint8) (uint32) argv[0];

    _filter_sd_in_out_msg();

    //����
    if(0 != set_play_disk_info(g_enter_mode))
    {
        retval = RESULT_NEXT_FUNCTION;
        goto endpro;
    }

    //��ʼ��
    if (_app_init() == TRUE)
    {
        retval = music_scene_playing();
        g_p_global_var->display_time = g_cur_time;
    }
    else
    {
        //��ʾ����(��ʾERRO)
        retval = RESULT_NEXT_FUNCTION;
    }

    endpro:
    com_ap_switch_deal(retval);

    //�˳�
    _app_deinit();

    return retval;
}
