/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  �����潻���ӿڹ���ʵ��
 ********************************************************************************/


#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
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
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
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
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \��ȡ��ǰ�����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    bool bret;
    bret = music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \��ȡ��ǰ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t *play_status)
{
    bool bret;
    bret = music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* loop_mode)
 * \��ȡ��ǰ����ģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8 *loop_mode)
{
    bool bret;
    bret = music_send_msg(NULL, loop_mode, MSG_MENGINE_GET_PLAYMODE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filename(music_file_name_t *music_file_name)
 * \��ȡ��ǰ�ļ�����
 * \param[in]    music_file_name �ļ����ṹ�建����
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filename(music_file_name_t *music_file_name)
{
    bool bret;
    bret = music_send_msg(NULL, music_file_name, MSG_MENGINE_GET_FILENAME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_playmode(uint8 playmode)
 * \����ѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret;
    bret = music_send_msg(&playmode, NULL, MSG_MENGINE_SET_PLAYMODE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_volume(uint8 volume)
 * \��������
 * \param[in]    volume �����ȼ�
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool music_set_volume(uint8 volume)
{
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play(void)
 * \��ʼ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_pause(void)
 * \��ͣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_pause(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_resume(void)
 * \�ָ���������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_resume(void)
{
    bool bret;
    com_speaker_on(SPK_PLAY);
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_stop(void)
 * \ֹͣ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STOP_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_forward(void)
 * \���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_forward(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)���
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FFWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_backward(void)
 * \����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_backward(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)����
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FBWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_cancel_ffb(void)
 * \ȡ�������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_cancel_ffb(void)
{
    bool bret;
    //��Ϣ����(����Ϣ����)ȡ�������
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CANCEL_FFB_SYNC);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_next(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_play_next(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_NEXT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_prev(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_play_prev(void)
{
    bool bret;

    //����ͬ����Ϣ
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_PREV_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bbool music_clear_error(void)
 * \��������
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_clear_error(void)
{
    bool bret = TRUE;

    //����ͬ����Ϣ
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CLEAR_ERR_SYNC);

    return bret;
}

//��ȡ�ļ���Ϣ�������ļ���ʱ�䣬�����ʣ������ʵ�
bool music_get_fileinfo(mmm_mp_file_info_t *fileinfo)
{
    return music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_sortnumber(uint16 destnum)
 * \�����������ļ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_get_disknumber(uint16 *disk_num)
{
    bool bret;
    bret = music_send_msg(NULL, disk_num, MSG_MENGINE_GET_DISKNUMBER_SYNC);
    return bret;
}

