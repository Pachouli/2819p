/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������
 ********************************************************************************/
#include "ap_usound.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_send_msg(void *param, uint32 msg_type)
 * \��ȡ������ز�������
 * \param[in]    input_param �������ָ��
 * \param[in]    msg_type  ��Ϣ����
 * \param[out]   output_param �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool usound_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_volume_add(void)
 * \�Ӵ�����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_volume_add(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_VOL_ADD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_volume_sub(void)
 * \��С����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_volume_sub(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_VOL_SUB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_pause(void)
 * \��ͣ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_set_pause(void)
{
    bool bret;
    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_pause(void)
 * \����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_set_play(void)
{
    bool bret;
    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_RESUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_next(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_play_next(void)
{
    bool bret;

    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_NEXT_SONG_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_prev(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_play_prev(void)
{
    bool bret;

    bret = usound_send_msg(NULL, NULL, MSG_USOUNDEG_PREV_SONG_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_prev(void)
 * \��ȡ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool usound_get_play_status(usoundeg_status_t* ptr)
{
    bool bret;

    bret = usound_send_msg(NULL, ptr, MSG_USOUNDEG_GET_STATUS_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_prev(void)
 * \����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/
bool mannul_set_play(void)
{
    bool bret = FALSE;
    g_need_volume_sync = TRUE;
    com_speaker_on(SPK_PLAY);
    bret = usound_set_play();
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool usound_play_prev(void)
 * \��ͣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      usound_control.c
 * \note
 */
/*******************************************************************************/

bool mannul_set_pause(void)
{
    bool bret = FALSE;

    bret = usound_set_pause();
    return bret;
}

//WIN7��100��������0������ÿ�����
//NOTE��XPÿ�������72������XPӳ��������˥���ñ�WIN7��
const uint8 volume_delta_value[100] =
{
    33, 33, 33, 34, 34, 35, 35, 35, 35, 36,
    36, 36, 37, 37, 38, 38, 38, 38, 39, 40,
    39, 41, 40, 41, 42, 41, 43, 43, 43, 44,
    44, 45, 45, 46, 46, 47, 48, 48, 49, 49,
    50, 51, 51, 52, 52, 54, 54, 55, 56, 56,
    57, 59, 59, 60, 61, 61, 63, 64, 65, 66,
    68, 68, 70, 71, 72, 73, 75, 77, 78, 79,
    82, 83, 85, 86, 89, 91, 93, 95, 97, 100,
    103, 106, 108, 112, 115, 118, 122, 126, 130, 135,
    140, 144, 151, 156, 163, 170, 178, 187, 195, 207
};

void pc_set_volume_sync(void)
{
    uint16 pc_volume = 0;

    if ((g_cur_play_status.vol_sync.volume_chg != 0) || (g_need_volume_sync == TRUE))
    {
        //max volume 0xfff0, min volume 0xe39a
        uint32 pc_set_volume = g_cur_play_status.vol_sync.volume_value;
        uint32 pc_volume_max = g_cur_play_status.vol_sync.volume_max;
        uint32 pc_volume_min = g_cur_play_status.vol_sync.volume_min;
        uint32 delta_value;

        if ((pc_set_volume >= pc_volume_min) && (pc_set_volume <= pc_volume_max))
        {
            #if 0
            //�ʺ�XP����ϵͳ������ֵ���㷽ʽ
            pc_volume = ((pc_set_volume - pc_volume_min) / 72); //72.54 = (pc_volume_max - pc_volume_min) / 100
            #endif

            #if 1
            //�ʺ�WIN7����ϵͳ������ֵ���㷽ʽ
            uint8 i;
            uint32 delta_add;

            delta_value = pc_volume_max - pc_set_volume;

            delta_add = 0;
            for (i = 0; i < 100; i++)
            {
                if (delta_value <= delta_add)
                {
                    break;
                }

                delta_add += volume_delta_value[i];
            }

            pc_volume = 100 - i;
            #endif
        }
        else if (pc_set_volume < pc_volume_min)
        {
            pc_volume = 0;
        }
        else
        {
            pc_volume = 100;
        }

        libc_printf_info("usound pc volume:%d\n", pc_volume);

        com_set_sound_volume(SET_VOL_BY_PC_VOL, pc_volume);
        g_need_volume_sync = FALSE;
    }
}

