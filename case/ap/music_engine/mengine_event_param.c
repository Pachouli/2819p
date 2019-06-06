/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ���ò�����Ϣ
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_vram_write(uint8 save_disk)
 * \дvram
 * \param[in]    void  para1
 * \param[out]   none
 * \return
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
void mengine_vram_write(uint8 save_disk)
{

    //back up show file num
    uint16 file_num_back = g_eg_cfg_p->location.dirlocation.file_num;

    //now num
    if(save_disk != DISK_C)
    {
        uint16 play_num = fsel_get_fileno();
        if (play_num != 0)
        {
            g_eg_cfg_p->location.dirlocation.file_num = play_num;
        }
    }

    //card or uhost all play
    if (g_mengine_enter_mode == ENTER_MUSIC_START)
    {
        if (save_disk == DISK_H)
        {
            //дvram
            g_eg_cfg_p->normal_exit_flag = 0;
            if (g_p_global_var->att_flag == 0)
            {
                sys_vm_write(g_eg_cfg_p, VM_CARD_BKINFO, sizeof(mengine_config_t));
            }
        }
        else if (save_disk == DISK_U)
        {
            g_eg_cfg_p->normal_exit_flag = 0;
            if (g_p_global_var->att_flag == 0)
            {
                sys_vm_write(g_eg_cfg_p, VM_UHOST_BKINFO, sizeof(mengine_config_t));
            }
        }
        else if (save_disk == DISK_C)
        {
            g_eg_cfg_p->normal_exit_flag = 0;
            if (g_p_global_var->att_flag == 0)
            {
                sys_vm_write(g_eg_cfg_p, VM_RES_BKINFO, sizeof(mengine_config_t));
            }
        }
        else
        {
            ;
        }
    }

    g_eg_cfg_p->location.dirlocation.file_num = file_num_back;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int32 mengine_vram_read(uint8 restore_disk,mengine_config_t * p_cfg)
 * \��vram
 * \param[in]    void  para1
 * \param[out]   none
 * \return
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
int32 mengine_vram_read(uint8 restore_disk, mengine_config_t * p_cfg)
{
    int32 ret = 0;

    if (g_mengine_enter_mode == ENTER_MUSIC_START)
    {
        if (restore_disk == DISK_H)
        {
            ret = sys_vm_read(p_cfg, VM_CARD_BKINFO, sizeof(mengine_config_t));
        }
        else if (restore_disk == DISK_U)
        {
            ret = sys_vm_read(p_cfg, VM_UHOST_BKINFO, sizeof(mengine_config_t));
        }
        else if (restore_disk == DISK_C)
        {
            ret = sys_vm_read(p_cfg,VM_RES_BKINFO,sizeof(mengine_config_t));
        }
        else
        {
            ;
        }
    }
    return (ret);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool switch_disk_file_init(void)
 * \��������,ʼ���ļ�ѡ����
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
bool switch_disk_file_init(void)
{
    //�ļ�ѡ������ʼ������
    fsel_param_t switch_init_param;
    //��׺��buffer
    uint8 exname[4];
    //�ļ�ѡ������
    //fsel_type_e file_source;
    //·����Ϣָ��
    file_location_t *loact_ptr = &g_eg_cfg_p->location.dirlocation;
    //��ʼ���ļ�ѡ�������
    bool init_result = FALSE;

    g_file_sys_id = 0;
    if (_mengine_check_disk_in(loact_ptr->disk) == FALSE)
    {
        goto switch_init_over;
    }

    //�ļ�ѡ������ʼ���ӿڵĲ�������

    //�̷���Ϣ
    switch_init_param.disk = loact_ptr->disk;

    switch_init_param.fsel_type = g_eg_cfg_p->fsel_type;

    g_eg_cfg_p->shuffle_flag &= (uint8) 0xf0;

    //ѭ��ģʽ
    switch_init_param.fsel_mode = FSEL_MODE_LOOPALL;

    //ѭ��ģʽ
    //switch_init_param.fsel_mode = FSEL_MODE_LOOPALL;

    //bitmap ����
    switch_init_param.file_type_bit = MUSIC_BITMAP;

    //�ļ�ѡ������ʼ��
    g_file_sys_id = fsel_init(&switch_init_param, 0);
    //��ʼ���ɹ�
    if (g_file_sys_id != 0)
    {

        //�����ϴ��˳����ļ�
        init_result = fsel_set_location(loact_ptr);

        if (init_result == TRUE)
        {
            //�ȽϺ�׺��
            vfs_get_name(g_file_sys_id, exname, 0);

            if (libc_strncmp(exname, loact_ptr->filename, sizeof(exname)) != 0)
            {
                init_result = FALSE;
            }
        }

        //����ʧ��
        if (init_result == FALSE)
        {
            //����ϵ���Ϣ
            libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

            //��ȡ��ǰģʽ�µĵ�һ���ɲ����ļ�
            init_result = fsel_get_nextfile(loact_ptr->filename);

            //�ҵ��ɲ����ļ�
            if (init_result == TRUE)
            {
                //��ȡ��ǰ�ļ���location��Ϣ
                init_result = fsel_get_location(loact_ptr);
            }
        }

#ifdef DIR_NAME_SUPPORT
        if(init_result == TRUE)
        {
            fsel_get_dir_name(g_dir_name_buf, 32, &(g_eg_cfg_p->location.dirlocation));
        }
#endif
    }
    //loop mode set

    switch_init_over: if (init_result == FALSE)
    {
        //û�пɲ����ļ�
        //loact_ptr->file_total = 0;

        //����״̬Ϊ�������ļ�
        g_eg_status_p->err_status = EG_ERR_NO_FILE;
    }
    return init_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_playmode(void* msg_ptr)
 * \����ѭ��ģʽ
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

 * \param[out]   msg_ptr->msg.content.addr //����ѭ����ʽ��ָ��
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_playmode(private_msg_t *msg_ptr)
{
    uint8 cur_mode;

    //��ȡ��ǰѭ��ģʽ
    cur_mode = *(uint8 *) (msg_ptr->msg.content.addr);

    //shuffle
    if (cur_mode == FSEL_MODE_RANOM)
    {
        g_eg_cfg_p->shuffle_flag = 0x10;

        cur_mode = FSEL_MODE_LOOPALL;

    }
    else
    {
        g_eg_cfg_p->shuffle_flag = 0;
    }

    //¼�� such special dir ����ʱ����Ҫallѭ��ģʽ
    if (g_mengine_enter_mode != ENTER_MUSIC_START)
    {
        if (cur_mode == FSEL_MODE_LOOPALL)
        {
            cur_mode = FSEL_MODE_LOOPONEDIR;
        }
    }

    //����shuffleģʽ
    //g_eg_cfg_p->shuffle_flag = (uint8) (cur_mode & 0xf0);
    //����ѭ��ģʽ,��ǰΪ����״̬���޷�ֱ������
    g_eg_cfg_p->repeat_mode = cur_mode;

    //��ͣ����ֹͣʱ����ѭ��ģʽ
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        fsel_set_mode((uint8) (g_eg_cfg_p->shuffle_flag | g_eg_cfg_p->repeat_mode)); //����ѭ��ģʽ
    }

    //ret_back:
    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uinapp_result_e mengine_set_filepath(void* msg_ptr)
 * \���ò����ļ�·����Ϣ
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
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_filepath(private_msg_t *msg_ptr)
{
    bool ret_vals;
    //·����Ϣָ��
    file_path_info_t *locat_info = (file_path_info_t *) msg_ptr->msg.content.addr;
    //�̷�
    /*uint8 disk_old = g_eg_cfg_p->location.dirlocation.disk;
     uint8 disk_new = locat_info->file_path.dirlocation.disk;*/

    fsel_type_e cur_type = locat_info->file_source;

    ret_vals = _stop(STOP_NORMAL); //ֹͣ����

    g_eg_cfg_p->fsel_type = cur_type; //�������汣����ļ�ѡ������

    if (cur_type == FSEL_TYPE_SDFILE)
    {
        libc_memcpy(g_file_name, locat_info->file_path.dirlocation.filename, 12);
        ret_vals = TRUE;
    }
    else
    {
        libc_memcpy(&g_eg_cfg_p->location, &locat_info->file_path, sizeof(file_location_t));
        ret_vals = fsel_set_location(&g_eg_cfg_p->location);
    }

    if (ret_vals == FALSE)
    {
        fsel_exit();

        ret_vals = switch_disk_file_init(); //���³�ʼ��
    }

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_clear_error(void* msg_ptr)
 * \��������
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
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_clear_error(private_msg_t *msg_ptr)
{
    g_eg_status_p->err_status = EG_ERR_NONE; //��������

    //���سɹ�
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_start(void* msg_ptr)
 * \��ʼ��������,���û���׼��
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
 * \ingroup      mengine_event_param.c
 * \note  //���øýӿں�Ҫ��֤����ʹ�� 0x37000 ���ϵĿռ䣬ֱ������ mengine_tts_stop Ϊֹ
 */
/*******************************************************************************/
app_result_e mengine_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status_p->play_status == PlayFast) || (g_eg_status_p->play_status == PlaySta))
    {
        //����ͣ����
        ret_vals = _stop(STOP_PAUSE); //��ͣ����
        if (ret_vals == TRUE)
        {
            g_stop_by_tts_flag = TRUE;
        }
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif
    }

    mengine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_stop(void* msg_ptr)
 * \�����������,�ɻָ�����
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
 * \ingroup      mengine_event_param.c
 * \note  //���øýӿڣ���ʾ 0x37000 ���Ͽռ������ȫ��musicӦ��֧��
 */
/*******************************************************************************/
app_result_e mengine_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        ret_vals = _play(PLAY_RESUME); //����

        g_stop_by_tts_flag = FALSE;
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() != MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_play();
        }
#endif
    }

    mengine_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e mengine_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //�����ֳ���׼������˯��ģʽ
    libc_printf_info("engine standby enter\n", 0, 0);

    //�ظ�ǰ̨��Ȼ������ܾͻ�˯��
    mengine_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //��˯��ģʽ�������ָ��ֳ�
    libc_printf_info("engine standby exit\n", 0, 0);

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_fileinfo(void* msg_ptr)
 * \��ȡ�ļ��Ĳ�����Ϣ
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
 * \ingroup      mengine_event_param.c
 * \note  //��ȡ�ļ���Ϣ�������ļ���ʱ�䣬�����ʣ������ʵ�
 */
/*******************************************************************************/
app_result_e mengine_get_fileinfo(private_msg_t *msg_ptr)
{
    if (mp_handle != NULL)
    {
        _get_file_info();
    }

    //copy��ǰ�ļ���Ϣ����Ϣָ��ָ���������
    libc_memcpy((msg_ptr->reply->content), &g_mengine_info.eg_file_info, sizeof(mmm_mp_file_info_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_fileinfo(void* msg_ptr)
 * \��ȡ�ļ��Ĳ�����Ϣ
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
 * \ingroup      mengine_event_param.c
 * \note  //��ȡ�ļ���Ϣ�������ļ���ʱ�䣬�����ʣ������ʵ�
 */
/*******************************************************************************/
app_result_e mengine_set_sortnumber(private_msg_t *msg_ptr)
{
    uint16 set_num;

    set_num = *(uint16 *) (msg_ptr->msg.content.addr);

    //change cur number
    g_eg_cfg_p->location.dirlocation.file_num = set_num;

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_disknumber(void* msg_ptr)
 * \��ȡ�ļ��Ĳ�����Ϣ
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
 * \ingroup      mengine_event_param.c
 * \note  //��ȡ�ļ���Ϣ�������ļ���ʱ�䣬�����ʣ������ʵ�
 */
/*******************************************************************************/
app_result_e mengine_get_disknumber(private_msg_t *msg_ptr)
{
    *(uint16 *) (msg_ptr->reply->content) = fsel_get_fileno();

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

