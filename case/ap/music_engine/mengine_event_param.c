/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 设置播放信息
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_vram_write(uint8 save_disk)
 * \写vram
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
            //写vram
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
 * \读vram
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
 * \更换磁盘,始化文件选择器
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
    //文件选择器初始化参数
    fsel_param_t switch_init_param;
    //后缀名buffer
    uint8 exname[4];
    //文件选择类型
    //fsel_type_e file_source;
    //路径信息指针
    file_location_t *loact_ptr = &g_eg_cfg_p->location.dirlocation;
    //初始化文件选择器结果
    bool init_result = FALSE;

    g_file_sys_id = 0;
    if (_mengine_check_disk_in(loact_ptr->disk) == FALSE)
    {
        goto switch_init_over;
    }

    //文件选择器初始化接口的参数设置

    //盘符信息
    switch_init_param.disk = loact_ptr->disk;

    switch_init_param.fsel_type = g_eg_cfg_p->fsel_type;

    g_eg_cfg_p->shuffle_flag &= (uint8) 0xf0;

    //循环模式
    switch_init_param.fsel_mode = FSEL_MODE_LOOPALL;

    //循环模式
    //switch_init_param.fsel_mode = FSEL_MODE_LOOPALL;

    //bitmap 设置
    switch_init_param.file_type_bit = MUSIC_BITMAP;

    //文件选择器初始化
    g_file_sys_id = fsel_init(&switch_init_param, 0);
    //初始化成功
    if (g_file_sys_id != 0)
    {

        //设置上次退出的文件
        init_result = fsel_set_location(loact_ptr);

        if (init_result == TRUE)
        {
            //比较后缀名
            vfs_get_name(g_file_sys_id, exname, 0);

            if (libc_strncmp(exname, loact_ptr->filename, sizeof(exname)) != 0)
            {
                init_result = FALSE;
            }
        }

        //设置失败
        if (init_result == FALSE)
        {
            //清除断点信息
            libc_memset(&g_eg_cfg_p->bk_infor, 0, sizeof(mmm_mp_bp_info_t));

            //获取当前模式下的第一个可播放文件
            init_result = fsel_get_nextfile(loact_ptr->filename);

            //找到可播放文件
            if (init_result == TRUE)
            {
                //获取当前文件的location信息
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
        //没有可播放文件
        //loact_ptr->file_total = 0;

        //设置状态为出错无文件
        g_eg_status_p->err_status = EG_ERR_NO_FILE;
    }
    return init_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_playmode(void* msg_ptr)
 * \设置循环模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_playmode(private_msg_t *msg_ptr)
{
    uint8 cur_mode;

    //获取当前循环模式
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

    //录音 such special dir 播放时不需要all循环模式
    if (g_mengine_enter_mode != ENTER_MUSIC_START)
    {
        if (cur_mode == FSEL_MODE_LOOPALL)
        {
            cur_mode = FSEL_MODE_LOOPONEDIR;
        }
    }

    //保存shuffle模式
    //g_eg_cfg_p->shuffle_flag = (uint8) (cur_mode & 0xf0);
    //保存循环模式,当前为播放状态则无法直接设置
    g_eg_cfg_p->repeat_mode = cur_mode;

    //暂停或则停止时设置循环模式
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        fsel_set_mode((uint8) (g_eg_cfg_p->shuffle_flag | g_eg_cfg_p->repeat_mode)); //设置循环模式
    }

    //ret_back:
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uinapp_result_e mengine_set_filepath(void* msg_ptr)
 * \设置播放文件路径信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_filepath(private_msg_t *msg_ptr)
{
    bool ret_vals;
    //路径信息指针
    file_path_info_t *locat_info = (file_path_info_t *) msg_ptr->msg.content.addr;
    //盘符
    /*uint8 disk_old = g_eg_cfg_p->location.dirlocation.disk;
     uint8 disk_new = locat_info->file_path.dirlocation.disk;*/

    fsel_type_e cur_type = locat_info->file_source;

    ret_vals = _stop(STOP_NORMAL); //停止播放

    g_eg_cfg_p->fsel_type = cur_type; //更改引擎保存的文件选择类型

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

        ret_vals = switch_disk_file_init(); //重新初始化
    }

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_clear_error(void* msg_ptr)
 * \清除错误号
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_clear_error(private_msg_t *msg_ptr)
{
    g_eg_status_p->err_status = EG_ERR_NONE; //错误号清除

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_tts_start(void* msg_ptr)
 * \开始语音播报,做好互斥准备
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口后，要保证不再使用 0x37000 以上的空间，直到调用 mengine_tts_stop 为止
 */
/*******************************************************************************/
app_result_e mengine_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status_p->play_status == PlayFast) || (g_eg_status_p->play_status == PlaySta))
    {
        //先暂停播放
        ret_vals = _stop(STOP_PAUSE); //暂停播放
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
 * \语音播报完成,可恢复备份
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口，表示 0x37000 以上空间可以完全由music应用支配
 */
/*******************************************************************************/
app_result_e mengine_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        ret_vals = _play(PLAY_RESUME); //续播

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

    //备份现场，准备进入睡眠模式
    libc_printf_info("engine standby enter\n", 0, 0);

    //回复前台，然后接着跑就会睡眠
    mengine_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //从睡眠模式回来，恢复现场
    libc_printf_info("engine standby exit\n", 0, 0);

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_fileinfo(void* msg_ptr)
 * \获取文件的播放信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //获取文件信息，包括文件总时间，波特率，采样率等
 */
/*******************************************************************************/
app_result_e mengine_get_fileinfo(private_msg_t *msg_ptr)
{
    if (mp_handle != NULL)
    {
        _get_file_info();
    }

    //copy当前文件信息到消息指针指向的数据区
    libc_memcpy((msg_ptr->reply->content), &g_mengine_info.eg_file_info, sizeof(mmm_mp_file_info_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_fileinfo(void* msg_ptr)
 * \获取文件的播放信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //获取文件信息，包括文件总时间，波特率，采样率等
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
 * \获取文件的播放信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //获取文件信息，包括文件总时间，波特率，采样率等
 */
/*******************************************************************************/
app_result_e mengine_get_disknumber(private_msg_t *msg_ptr)
{
    *(uint16 *) (msg_ptr->reply->content) = fsel_get_fileno();

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

