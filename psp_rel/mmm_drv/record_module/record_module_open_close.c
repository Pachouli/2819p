/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：录音模块打开和关闭。
 * 作者：cailizhen
 ********************************************************************************/

#include "record_module.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    打开音频播放中间件
 * \param[in]   param  打开音乐播放中间件的参数，目前设置为空
 * \return       操作句柄
 * \retval           others sucess
 * \retval           NULL failed
 *******************************************************************************/
void *record_module_open(void *param)
{
    record_module_t *p_record_module = &g_record_module_global;

    libc_printf_info("RECORD MODULE OPEN!\n");

    p_record_module->record_module_status.status = RECORD_MODULE_STA_OPEN;
    p_record_module->print_bytes_used = TRUE;

    return (void *) p_record_module;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭音乐播放中间件
 * \param[in]   handel  打开的音乐播放中间件操作句柄
 * \param[out]  param 关闭中间件所需保存数据的指针，目前设置为空
 * \return       the result
 * \retval           0 sucess
 * \retval           -1 failed
 *******************************************************************************/
int32 record_module_close(record_module_t *p_record_module, void *param)
{
    libc_printf_info("RECORD MODULE CLOSE!\n");

    p_record_module->record_module_status.status = RECORD_MODULE_STA_CLOSE;

    return 0;
}
