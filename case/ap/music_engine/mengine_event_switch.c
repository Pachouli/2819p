/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ���Ų�������(��һ������һ�����л�����Ŀ¼��)
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_switch(private_msg_t *msg_ptr, uint8 mode)
 * \�л���������
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    mode �л�ģʽ  TRUE ��һ�� FALSE ��һ��
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
static app_result_e mengine_play_switch(private_msg_t *msg_ptr, switch_mode_e switch_mode)
{
    bool bret;
    //play_mode_e play_mode;//�����Ƿ����ڲ���
    switch_result_e switch_ret; //�л��Ƿ�ɹ�

    if (g_eg_status_p->play_status != PlaySta) //���ڲ���
    {
        //����ϵ�
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }

    switch_ret = _mengine_file_switch(STOP_NORMAL, switch_mode, PLAY_NO_PLAY);

    if (switch_ret == SWITCH_NO_ERR)
    {
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }

    mengine_reply_msg(msg_ptr, bret);

    return RESULT_NULL;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_next(void* msg_ptr)
 * \�л�����һ��
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_next(private_msg_t *msg_ptr)
{
    if (g_mengine_enter_mode == ENTER_RECORD_PLAY)
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
    }
    else
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_NEXT);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_prev(void* msg_ptr)
 * \�л�����һ��
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_prev(private_msg_t *msg_ptr)
{
    play_status_e play_status = g_eg_status_p->play_status;
    bool bret = TRUE;
    bool replay_flag = (bool) com_get_config_default(MUSIC_REPLAY_BY_PREV_KEY);

    if ((replay_flag == TRUE) && (g_eg_playinfo_p->cur_time > 5000))
    {
        g_eg_playinfo_p->cur_time = 0;

        //����ϵ�
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));

        if (play_status == PlaySta)
        {
            //ֹͣ����
            bret = _stop(STOP_NORMAL);

            //�ȱ�����ͣ����ǰ̨�ָ����ţ�����ǰ̨�����ڲ���֮ǰ�岥TTS

            g_eg_playinfo_p->cur_file_switch |= 1;
        }
        if (bret == TRUE)
        {
            mengine_reply_msg(msg_ptr, bret);
            return RESULT_NULL;
        }
    }

    if (g_mengine_enter_mode == ENTER_RECORD_PLAY)
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_NEXT);
    }
    else
    {
        return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
    }
}

/******************************************************************************/
/*bool mengine_special_switch(uint8 direct,uint16 dest_num)
 * \par  Description:
 * \ʵ���ļ��л�,�������ֵ���Ŀ¼�л�
 * \param[in]    direct param1 0 --���ֵ�� 0x80--Ŀ¼���,0x81--Ŀ¼��ǰ
 * \param[in]    dest_num param2 �������
 * \param[in]
 * \param[out]   none
 * \return       bool
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
bool mengine_special_switch(uint8 direct, uint16 dest_num)
{
    bool ret;

    //ֹͣ��ǰ��������
    ret = _stop(STOP_NORMAL);
    if (ret == FALSE)
    {
        return ret;
    }

    //�������Ƿ����
    ret = _mengine_check_disk_in(g_eg_cfg_p->location.dirlocation.disk);
    //����Ѿ��γ�����
    if (ret == FALSE)
    {
        //�������״̬
        g_eg_status_p->err_status = EG_ERR_DISK_OUT;

        return ret;
    }

    //ǿ���л�
    if (g_mengine_enter_mode == ENTER_MUSIC_START)
    {
        fsel_set_mode((g_eg_cfg_p->shuffle_flag | FSEL_MODE_LOOPALL));
    }

    if (direct == 0)
    {
        //���ֵ��
        ret = fsel_get_byno((void *) g_eg_cfg_p->location.dirlocation.filename, dest_num);
    }
    else if (direct == 0x80)
    {
        //Ŀ¼���
        ret = fsel_get_nextdir((void *) g_eg_cfg_p->location.dirlocation.filename);
    }
    else
    {
        //Ŀ¼��ǰ
        ret = fsel_get_prevdir((void *) g_eg_cfg_p->location.dirlocation.filename);
    }

    //�ָ�ѭ��ģʽ
    if (g_mengine_enter_mode == ENTER_MUSIC_START)
    {
        fsel_set_mode((uint8) (g_eg_cfg_p->shuffle_flag | g_eg_cfg_p->repeat_mode));
    }

    //��ȡ��ǰ������location
    if (ret == TRUE)
    {
        fsel_get_location(&(g_eg_cfg_p->location.dirlocation));

        if (direct != 0)
        {
            g_eg_playinfo_p->cur_file_switch |= 0x01;
        }
        //����ϵ���Ϣ
        libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

#ifdef DIR_NAME_SUPPORT
        fsel_get_dir_name(g_dir_name_buf, 32, &(g_eg_cfg_p->location.dirlocation));
#endif
        //get file name
        vfs_get_name(g_file_sys_id, g_eg_cfg_p->name_buf, 16);
    }
    else
    {
        libc_memset(g_eg_cfg_p->name_buf, 0, 32);
    }

    mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);
    //��ͨ�����л�ʱ��ǰʱ��������
    g_eg_playinfo_p->cur_time = 0;

    //if (direct == 0)
    //{
    //ֻҪ�л������ͻῪʼ����
    //ret = _play(PLAY_RESUME);
    //}

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_dolder_next(void* msg_ptr)
 * \Ŀ¼�л�
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_folder_next(private_msg_t *msg_ptr)
{

    bool ret_val;

    ret_val = mengine_special_switch(0x80, 1);

    //���سɹ�
    mengine_reply_msg(msg_ptr, ret_val);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_dolder_prev(void* msg_ptr)
 * \Ŀ¼�л�
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //ͬ����־��1B
 *                 sem_id_t sem_id;           //�ź���ID��1B
 *                 msg_apps_t msg;          //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //��Ϣ���ͣ�2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_switch.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_folder_prev(private_msg_t *msg_ptr)
{

    bool ret_val;

    ret_val = mengine_special_switch(0x81, 1);

    //���سɹ�
    mengine_reply_msg(msg_ptr, ret_val);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _next_or_prev(switch_mode_e switch_mode)
 * \�л�������
 * \param[in]    switch_mode �л�ģʽ
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _next_or_prev(switch_mode_e switch_mode)
{
    uint8 loop_mode;
    bool bret = TRUE;
    freq_level_e decode_clk;

    uint8 direct = (uint8) (switch_mode & 0x01);

    uint8 force_mod = (uint8) ((switch_mode & 0x02) >> 1);

    g_eg_status_p->err_status = EG_ERR_NONE;

    loop_mode = g_eg_cfg_p->shuffle_flag; //ѭ��ģʽshuffle

    //�и���ʱ��������Ƶ�ʣ���ʱû���ڽ���
    decode_clk = sys_adjust_clk(FREQ_156M, SET_DECODE_CLK);

    //ǿ���л�
    //if ((force_mod == TRUE) && (g_mengine_enter_mode == ENTER_MUSIC_START))
    if (force_mod == TRUE)
    {
        if (g_mengine_enter_mode == ENTER_MUSIC_START)
        {
            loop_mode |= FSEL_MODE_LOOPALL;
        }
        else
        {
            loop_mode |= FSEL_MODE_LOOPONEDIR;
        }

        fsel_set_mode(loop_mode);
    }
    if (g_open_disk != DISK_C)
    {
        //¼���ط����򲥷�
        if ((switch_mode == END_SWITCH) && (g_mengine_enter_mode == ENTER_RECORD_PLAY))
        {
            direct = 1;
        }
        if (direct == 0)
        {
            //��ȡ��һ��
            bret = fsel_get_nextfile((void *) g_eg_cfg_p->location.dirlocation.filename);
        }
        else
        {
            //��ȡ��һ��
            bret = fsel_get_prevfile((void *) g_eg_cfg_p->location.dirlocation.filename);
        }
    }
#if (SUPPORT_RES_DIR == 1)
    else
    {
        if (direct == 0)
        {
            //��ȡ��һ��
            bret = get_next_file(g_file_name);
        }
        else
        {
            //��ȡ��һ��
            bret = get_prev_file(g_file_name);
        }
    }
#else if (SUPPORT_EXTLIB_PLAY == 1)
    else
    {
        if (direct == 0)
        {
            //��ȡ��һ��
            bret = extlib_get_next_file(g_file_name);
        }
        else
        {
            //��ȡ��һ��
            bret = extlib_get_prev_file(g_file_name);
        }
    }
#endif

    //��ȡ��ǰ������location
    if (bret == TRUE)
    {
        if (g_open_disk != DISK_C)
        {
            fsel_get_location(&(g_eg_cfg_p->location.dirlocation));
        }
#if (SUPPORT_RES_DIR == 1)
        else
        {
            g_eg_cfg_p->location.dirlocation.disk = DISK_C;
            g_eg_cfg_p->location.dirlocation.file_total = get_disk_total_num();
            g_eg_cfg_p->location.dirlocation.file_total_disk = g_eg_cfg_p->location.dirlocation.file_total;

            int8 ret = get_current_file_num(g_file_name, &play_file_num);
            if (ret != -1)
            {
                g_eg_cfg_p->location.dirlocation.file_num = play_file_num;
            }

        }
#elif (SUPPORT_EXTLIB_PLAY == 1)
        else
        {
            g_eg_cfg_p->location.dirlocation.disk = DISK_C;
            g_eg_cfg_p->location.dirlocation.file_total = extlib_get_disk_total_num();

            int8 ret = extlib_get_current_file_num(g_file_name, &play_file_num);
            if (ret != -1)
            {
                g_eg_cfg_p->location.dirlocation.file_num = play_file_num;
            }
        }
#endif

        g_eg_playinfo_p->cur_file_switch |= 0x01;

        //��ͨ�����л�ʱ��ǰʱ��������
        g_eg_playinfo_p->cur_time = 0;

#ifdef DIR_NAME_SUPPORT
        fsel_get_dir_name(g_dir_name_buf, 32, &(g_eg_cfg_p->location.dirlocation));
#endif
    }

    //ǿ���л�
    //if ((force_mod == TRUE) && (g_mengine_enter_mode == ENTER_MUSIC_START))
    if (force_mod == TRUE)
    {
        loop_mode = g_eg_cfg_p->shuffle_flag; //�ָ�ѭ��ģʽ
        loop_mode |= (uint8) g_eg_cfg_p->repeat_mode;
        fsel_set_mode(loop_mode);
    }

    //����ϵ���Ϣ
    libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

    sys_adjust_clk(decode_clk, SET_DECODE_CLK);
    return bret;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \ʵ���ļ��л��������������β����һ�������˵�ͷ����һ������
 * \param[in]    stop_mode_e stop_mode ֹͣģʽ STOP_NORMAL/STOP_PAUSE
 * \param[in]    switch_mode_e force_mode �л�ģʽ
 * \li NORMAL_SWITCH_NEXT ��������һ��
 * \li NORMAL_SWITCH_PREV ��������һ��
 * \li FORCE_SWITCH_NEXT  ǿ������һ��
 * \li FORCE_SWITCH_NEXT  ǿ������һ��
 * \li END_SWITCH         ���ŵ��ļ�β����һ��
 * \param[in]    play_mode_e play_mode ����ģʽ
 * \param[out]   none
 * \return       switch_result_e
 * \retval       SWITCH_NO_ERR success
 * \retval       other failed
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
switch_result_e _mengine_file_switch(stop_mode_e stop_mode, switch_mode_e switch_mode, play_mode_e play_mode)
{
    bool ret;

    uint8 read_err;
    switch_result_e switch_result = SWITCH_NO_ERR;

    //ֹͣ��ǰ��������
    ret = _stop(stop_mode);

    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_STOP;
        goto switch_end;
    }
    if (g_open_disk != DISK_C)
    {
        if (g_file_sys_id == 0)
        {
            return SWITCH_NO_ERR;
        }

        //�������Ƿ����
        ret = _mengine_check_disk_in(g_eg_cfg_p->location.dirlocation.disk);
        //�Ƿ��Ƕ��ļ�����
        read_err = fsel_get_error();

        if ((ret == FALSE) || (read_err == 2))
        {
            switch_result = SWITCH_ERR_DISK_OUT;
            //�������״̬
            g_eg_status_p->err_status = EG_ERR_DISK_OUT;

            goto switch_end;
        }
    }
    //�л�������

    ret = _next_or_prev(switch_mode);
    if (ret == FALSE) //&& (g_mengine_enter_mode != ENTER_RECORD_PLAY))
    {
        //�л�ʧ��ʱ����Ƿ񲥷ŵ����һ�׸���
        if (switch_mode == END_SWITCH)
        {
            //ǿ���л�����һ�׸�����ֹͣ���ţ�ֻ�ǻ�ȡ��Ϣ
            _next_or_prev(FORCE_SWITCH_NEXT);
            play_mode = PLAY_NO_PLAY;
        }
        else
        {
            switch_result = SWITCH_ERR_SWITCH;
            goto switch_end;
        }
    }
    //}
    //get file name
    if (g_open_disk != DISK_C)
    {
        vfs_get_name(g_file_sys_id, g_eg_cfg_p->name_buf, 16);
    }

    //try vram merge
    com_vram_merge();
    //�л��ļ���Ϊ�ϵ���Ч
    g_eg_cfg_p->bk_infor_invalid_flag = 1;
    mengine_vram_write(g_eg_cfg_p->location.dirlocation.disk);

    if ((play_mode != PLAY_NO_PLAY) && (ret == TRUE))
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif

        //�л�������ʼ����
        ret = _play(play_mode);
    }

    if (ret == FALSE)
    {
        switch_result = SWITCH_ERR_PLAY;
    }
    switch_end:
    return switch_result;
}

