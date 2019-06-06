/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_lineineg.h"

//globle variable

//保存中间件句柄
void *pp_handle = NULL;
mmm_pp_status_t g_pp_status;

//引擎状态变量结构体指针
lineineg_status_t g_eg_status;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_BACK_LOW_PRIO;


//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
bool g_stop_by_tts_flag;

bool g_lowenergy_poweroff_enable;

bool g_need_auto_play_flag;

uint8 g_linein_play_chan;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

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
int _app_init(void)
{
    //初始化applib库，后台AP
    applib_init(APP_ID_LINEIN_EG, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    g_lowenergy_poweroff_enable = (uint8) com_get_config_default(LINEIN_LOWENERGY_POWEROFF_ENABLE);

    return TRUE;
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
    applib_quit(THIS_APP_TYPE);

    return TRUE;
}

bool _app_play(void)
{
    bool init_fsel_ret = TRUE;

    g_eg_status.play_status = StopSta;

    //ESD RESET 且断点状态为 StopSta/PauseSta 则不启动播放
    if ((g_p_esd_bk_info->reset_flag == 1)
        && ((g_p_esd_bk_info->play_status == StopSta) || (g_p_esd_bk_info->play_status == PauseSta)))
    {
        //ESD RESET前是暂停状态，不要 _play
    }
    else if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK)
        && ((g_p_global_var->play_status_before == StopSta) || (g_p_global_var->play_status_before == PauseSta)))
    {
        //打电话前是暂停状态，不要 _play
    }
    else if (g_need_auto_play_flag == TRUE)
    {
        init_fsel_ret = _play();
        if (init_fsel_ret == TRUE)
        {
            g_eg_status.play_status = PlaySta;
        }
    }
    g_p_global_var->bg_app_sta = BG_STATUS_NULL;

    return init_fsel_ret;
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
    app_result_e lineineg_result = RESULT_NULL;
    g_need_auto_play_flag = ((bool) (uint32) argv[0]) & 0x0f;
    g_linein_play_chan = ((uint8) (uint32) argv[0]) >> 4;

    //初始化
    _app_init();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (_record_create_file() == FALSE)
        {
            g_need_auto_play_flag = FALSE;
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
#endif

    _app_play();

    //消息循环
    lineineg_result = lineineg_control_block();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");
    extern void lineineg_record_update_record_file(bool force);
    lineineg_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //退出
    _app_deinit();

    return 0;
}
