/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_usound_eg.h"

//globle variable

//保存中间件句柄
void *pp_handle = NULL;
mmm_pp_status_t g_pp_status;

//引擎状态变量结构体指针
usoundeg_status_t g_eg_status;
uint8 uengine_para;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_BACK_LOW_PRIO;

buffer_rw_t g_usound_input_buf;
uint8 __section__(".bss.usound_input_buf") usound_input_buf_raw_data[USOUND_STREAM_BUFFER_LEN];
uint8 __section__(".bss.usound_stream_fifo") usound_stream_fifo[USOUND_STREAM_FIFO_LEN];

//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
bool g_stop_by_tts_flag;
bool g_start_play = FALSE;
bool g_auto_set_play = FALSE;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

static void get_usb_info(void)
{
    uint8 info_p[52];
    bool bret = FALSE;
    usound_vol_sync_e tmp_vol_sync;

    libc_memset(info_p, 0x0, 52);

    bret = com_get_config_struct(USB_VID, info_p, 6);
    if (bret != FALSE)
    {
        usound_set_usb_cfg(DEVICE_VID, info_p);
    }

    bret = com_get_config_struct(USB_SOUND_PID, info_p, 6);
    if (bret != FALSE)
    {
        usound_set_usb_cfg(DEVICE_PID, info_p);
    }

    bret = com_get_config_struct(INF_USB_VENDOR, info_p, 8);
    if (bret != FALSE)
    {
        usound_set_usb_cfg(DEVICE_MANUFNAME, info_p);
    }

    bret = com_get_config_struct(INF_SOUND_PRODUCT_NAME, info_p, 16);
    if (bret != FALSE)
    {
        usound_set_usb_cfg(DEVICE_PRODNAME, info_p);
    }

    //USB音箱音量同步：0不可调，1小机可调，2PC可调，3小机PC均可调且小机优先，4小机PC均可调且PC优先
    tmp_vol_sync = (uint8) com_get_config_default(INF_VOLUME_FLAG);
    if ((tmp_vol_sync == USOUND_VOL_SYNC_ONLY_PC)
        || (tmp_vol_sync == USOUND_VOL_SYNC_PC_FIRST))
    {
        info_p[0] = 1;
    }
    else
    {
        info_p[0] = 0;
    }
    info_p[1] = uengine_para;
    usound_set_usb_cfg(DEVICE_VOLUME_FLAG, info_p);

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \初始化引擎、读取配置信息、初始化文件选择器、中间件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_init(void)
{
    bool bret = TRUE;
    uint8 s_auto_play;

    //初始化applib库，后台AP
    applib_init(APP_ID_USOUND_EG, THIS_APP_TYPE);
    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    g_usound_input_buf.read_ptr = 0;
    g_usound_input_buf.write_ptr = 0;
    g_usound_input_buf.length = 0;
    g_usound_input_buf.capacity = sizeof(usound_input_buf_raw_data);
    g_usound_input_buf.full_error = FALSE;
    g_usound_input_buf.raw_data = usound_input_buf_raw_data;

    if (sys_shm_creat(SHARE_MEM_ID_USBPCMPIPE, &g_usound_input_buf, sizeof(buffer_rw_t)) == -1)
    {
        libc_printf("usound_input_buf shm create fail!\n");
        while (1)
        {
            ; //QAC
        }
    }

    if ((int32) sys_drv_install(DRV_GROUP_USB_DEVICE, usound_stream_fifo, "usound.drv") == -1)
    {
        while (1);
    }

    get_usb_info();
    usb_sound_start();

    g_eg_status.play_status = PauseSta;

    s_auto_play = (uint8) com_get_config_default(USB_SOUND_AUTO_PLAY_ENABLE);
    g_start_play = ((s_auto_play == 1) ? TRUE : FALSE);

    //ESD RESET 且断点状态为 StopSta/PauseSta 则不启动播放
    if ((g_p_esd_bk_info->reset_flag == 1) && (g_p_esd_bk_info->play_status == PauseSta))
    {
        //ESD RESET前是暂停状态，不要 _play
    }
    else if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK) && (g_p_global_var->play_status_before == PauseSta))
    {
        //打电话前是暂停状态，不要 _play
    }
    else
    {
        //正常进入且不允许自动播放，不能执行_play()
        if((FALSE == g_start_play) && (g_p_esd_bk_info->reset_flag == 0))
        {
            goto endfunc;
        }
        bret = _play(adc_sample);
        if (bret == TRUE)
        {
            g_eg_status.play_status = PlaySta;
        }
    }
    g_p_global_var->bg_app_sta = BG_STATUS_NULL;

    endfunc:

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \退出app的功能函数,保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //执行applib库的注销操作
    usb_sound_stop();

    sys_drv_uninstall(DRV_GROUP_USB_DEVICE);

    sys_shm_destroy(SHARE_MEM_ID_USBPCMPIPE);

    applib_quit(THIS_APP_TYPE);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const void *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    uengine_para = (uint8) (uint32) argv[0];
    app_result_e usoundeg_result = RESULT_NULL;

    _app_init();

    usoundeg_result = usoundeg_control_block();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");

    extern void usoundeg_record_update_record_file(bool force);
    usoundeg_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //退出
    _app_deinit();

    return 0;
}
