/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�ǰ̨Ӧ��TTS��������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

void enter_bt_tts_play(tts_play_background_hook hook)
{
    //����������������
    if (g_esd_reset_ignore_enter_tts_flag == FALSE)
    {
        //����������������
        g_tts_enter_ret = com_tts_state_play_ext(TTS_ENTER_BT, 0, hook);
    }
    g_esd_reset_ignore_enter_tts_flag = FALSE;
}

void bt_tts_play(void)
{
    switch (g_disp_status)
    {
        case STATUS_WAIT_PAIRED:
        if (g_esd_reset_ignore_wait_tts_flag != FALSE)
        {
            g_esd_reset_ignore_wait_tts_flag = FALSE;
            break;
        }
        if (last_disp_status == STATUS_NONE)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //����Ӻ󣬸������貥���ȴ�����
            if (g_btplay_bt_info.dev_role == TWS_SLAVE)
            {
                break;
            }
#endif

            //���������ȴ�����
            if ((g_tts_enter_ret == TTS_PLAY_RET_BY_KEY) || (g_tts_enter_ret == TTS_PLAY_RET_BY_SYS))
            {
                //�����������������ʱ������ֹ�ˣ���ô�ȴ����ӾͲ��ٲ�����
                break;
            }
            com_tts_state_play(TTS_WAIT_BT_CON, 0);
        }
        else
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
        }
        break;

        /* for samsung S8500, link when play, maybe pause-status.  */
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        if (g_esd_reset_ignore_link_tts_flag != FALSE)
        {
            g_esd_reset_ignore_link_tts_flag = FALSE;
            break;
        }
        if ((last_disp_status == STATUS_WAIT_PAIRED) || (last_disp_status == STATUS_NONE))
        {
            //�������ӳɹ�
            com_tts_state_play(TTS_BT_CON_OK, 0);
        }
        break;

        default:
        break;
    }
}

