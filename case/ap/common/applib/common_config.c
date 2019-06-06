/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：固件配置项模块接口实现，包括打开配置项文件，读取配置项等接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/*!
 *  \brief
 *  config_fp 配置脚本文件句柄
 */
static sd_handle config_fp;

// config 脚本文件名字
const char cfg_file[] = "config.bin";

/******************************************************************************/
/*!
 * \par  Description:
 *    打开应用配置脚本文件，并读取解释配置文件头。
 * \param[in]    config_file 应用配置脚本文件名称
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 打开应用配置脚本成功。
 * \retval           FALSE 打开应用配置脚本失败
 * \ingroup      common_func
 * \note
 * \li  该接口只在 manager ap的 globe_data_init 中调用一次，也就是说在系统启动时
 *      进行一次打开初始化，之后任何地方读取应用配置项时都无须打开。
 *******************************************************************************/
bool com_open_config_file(void)
{
    uint8 head[16];

    config_fp = sys_sd_fopen(cfg_file);
    if (config_fp == NULL)
    {
        return FALSE;
    }

    if (sys_sd_fread(config_fp, head, 16) == -1)
    {
        return FALSE;
    }

    if (libc_memcmp(head, "cfg", 3) != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭应用配置脚本文件。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 关闭应用配置脚本成功。
 * \retval           FALSE 关闭应用配置脚本失败。
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_close_config_file(void)
{
    if (sys_sd_fclose(config_fp) == -1)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    读取应用配置项。
 * \param[in]    config_id 配置参数ID
 * \param[in]    buf_len 参数输出缓冲区大小
 * \param[out]   buf 参数输出缓冲区
 * \return       bool
 * \retval           TRUE 读取配置项成功。
 * \retval           FALSE 读取配置项失败。
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 例子1：获取背光亮度时间参数结构体
 if(com_get_config_struct_inner(CONFIG_ID_LIGHT_TIME, config_buf, CONFIG_BUF_LEN) == TRUE)
 {
 config_nonlinear_t *config_param;

 config_param = (config_nonlinear_t *)config_buf;
 然后就可以对 config_param 按 config_nonlinear_t 结构进行访问
 }
 else
 {
 没有找到该应用配置项
 }
 * \endcode
 * \note
 * \li  取可变长的 config_string_t 和 config_nonlinear_t，必须先根据使用场合申请最大
 *      的config参数的 uint8 缓冲区并读取数据，读取成功后把该缓冲区强制转换为
 *      config_string_t 或 config_nonlinear_t 类型指针，然后就可以按照 config_string_t
 *      和 config_nonlinear_t类型进行访问。
 *******************************************************************************/
bool com_get_config_struct_inner(uint16 config_id, uint8 *buf, uint16 buf_len)
{
    bool ret = TRUE;
    config_index_t temp_index;

    if (config_fp == NULL)
    {
        return FALSE;
    }

    sys_os_sched_lock();

    //读取配置项索引结构体
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        ret = FALSE;
        goto fail_exit_0;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        ret = FALSE;
        goto fail_exit_0;
    }

    //读取配置项内容
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        ret = FALSE;
        goto fail_exit_0;
    }
    if (temp_index.length > buf_len)
    {
        if (sys_sd_fread(config_fp, buf, buf_len) == -1)
        {
            ret = FALSE;
            goto fail_exit_0;
        }
    }
    else
    {
        if (sys_sd_fread(config_fp, buf, temp_index.length) == -1)
        {
            ret = FALSE;
            goto fail_exit_0;
        }
    }

    fail_exit_0:
    sys_os_sched_unlock();

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    读取应用配置项默认值。
 * \param[in]    config_id 配置参数ID
 * \param[out]   none
 * \return       uint16
 * \retval           返回读取到的应用配置项默认值。
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 例子1：获取背光亮度时间默认值
 light_time = com_get_config_default_inner(CONFIG_ID_LIGHT_TIME);
 * \endcode
 * \note
 * \li  仅用于数值型参数，并由用户保证 config_id 正确，若找不到配置项则返回0。
 *******************************************************************************/
uint16 com_get_config_default_inner(uint16 config_id)
{
    config_index_t temp_index;
    uint16 default_value;

    if (config_fp == NULL)
    {
        return 0;
    }

    sys_os_sched_lock();

    //读取配置项索引结构体
    if (sys_sd_fseek(config_fp, SEEK_SET, 16 + config_id * sizeof(config_index_t)) == -1)
    {
        default_value = 0;
        goto fail_exit_1;
    }
    if (sys_sd_fread(config_fp, &temp_index, sizeof(config_index_t)) == -1)
    {
        default_value = 0;
        goto fail_exit_1;
    }

    //读取配置项内容
    if (sys_sd_fseek(config_fp, SEEK_SET, temp_index.offset) == -1)
    {
        default_value = 0;
        goto fail_exit_1;
    }
    if (sys_sd_fread(config_fp, &default_value, sizeof(uint16)) == -1)
    {
        default_value = 0;
        goto fail_exit_1;
    }

    fail_exit_1:
    sys_os_sched_unlock();

    return default_value;
}

uint8 __section__(".bss.vram_merge") vram_merge_temp_buffer[VRAM_MERGE_TEMP_BUFFER_LEN];

//空闲时主动尝试 vram merge
void com_vram_merge_inner(void)
{
    uint32 t1, t2;
    uint8 i;

    #if 0 //一般情况下后台蓝牙连接着进行VRAM Merge关中断几百ms是没有问题的，故屏蔽掉
    //不支持在蓝牙应用中merge
    if (get_bt_state_inner() == BT_STATE_WORK)
    {
        return;
    }
    #endif

    //VRAM Merge需要临时占用中间件及解码空间，故必须没有中间件时才能进行VRAM Merge
    if (get_mmm_state_inner() != MMM_STATE_NO_PLAY)
    {
        return;
    }

    //切换到 32KB 临时BUFFER
    base_set_config(BASE_SET_VRAM_BUFFER, (void *) vram_merge_temp_buffer);

    //执行merge
    for (i = 0; i < 3; i++)
    {
        t1 = sys_get_ab_timer();
        if (base_vram_merge() > 0)
        {
            t2 = sys_get_ab_timer();
            libc_printf_info("vram merge ok! %d\n", t2 - t1);
        }
        else
        {
            break;
        }
    }

    //恢复为默认地址
    base_set_config(BASE_SET_VRAM_BUFFER, NULL);
}

/*! \endcond */
