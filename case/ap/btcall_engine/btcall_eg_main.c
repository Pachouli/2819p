/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话引擎应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_btcall_engine.h"

void *sp_handle;
mmm_sp_status_t sp_status;
btengine_info_t btcall_bt_info;

btcall_info_t g_btcall_info;

btcall_speech_play_para_t g_play_para_backup;

INT8U prio = AP_BACK_LOW_PRIO;

//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
bool g_stop_by_tts_flag;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{

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
int _app_init(void)
{
    //读取配置信息 (包括vm信息)
    _load_cfg();

    //初始化applib库，后台AP
    applib_init(APP_ID_BTCALL_EG, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

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
    //save config 写vram
    _save_cfg();

    //执行applib库的注销操作
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
    //初始化
    _app_init();

    //告诉蓝牙协议栈蓝牙免提已开始
    com_btengine_set_hfp_call(TRUE);

    //循环
    btcall_engine_control_block(); //app功能处理

    //告诉蓝牙协议栈蓝牙免提已停止
    com_btengine_set_hfp_call(FALSE);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");

    extern void btcall_engine_record_update_record_file(bool force);
    btcall_engine_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //退出
    _app_deinit();

    return 0;
}
