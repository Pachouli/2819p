/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������̼�������ģ��ӿ�ʵ�֣��������������ļ�����ȡ������Ƚӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

/*! \cond COMMON_API */

/*!
 *  \brief
 *  config_fp ���ýű��ļ����
 */
static sd_handle config_fp;

// config �ű��ļ�����
const char cfg_file[] = "config.bin";

/******************************************************************************/
/*!
 * \par  Description:
 *    ��Ӧ�����ýű��ļ�������ȡ���������ļ�ͷ��
 * \param[in]    config_file Ӧ�����ýű��ļ�����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��Ӧ�����ýű��ɹ���
 * \retval           FALSE ��Ӧ�����ýű�ʧ��
 * \ingroup      common_func
 * \note
 * \li  �ýӿ�ֻ�� manager ap�� globe_data_init �е���һ�Σ�Ҳ����˵��ϵͳ����ʱ
 *      ����һ�δ򿪳�ʼ����֮���κεط���ȡӦ��������ʱ������򿪡�
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
 *    �ر�Ӧ�����ýű��ļ���
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �ر�Ӧ�����ýű��ɹ���
 * \retval           FALSE �ر�Ӧ�����ýű�ʧ�ܡ�
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
 *    ��ȡӦ�������
 * \param[in]    config_id ���ò���ID
 * \param[in]    buf_len ���������������С
 * \param[out]   buf �������������
 * \return       bool
 * \retval           TRUE ��ȡ������ɹ���
 * \retval           FALSE ��ȡ������ʧ�ܡ�
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1����ȡ��������ʱ������ṹ��
 if(com_get_config_struct_inner(CONFIG_ID_LIGHT_TIME, config_buf, CONFIG_BUF_LEN) == TRUE)
 {
 config_nonlinear_t *config_param;

 config_param = (config_nonlinear_t *)config_buf;
 Ȼ��Ϳ��Զ� config_param �� config_nonlinear_t �ṹ���з���
 }
 else
 {
 û���ҵ���Ӧ��������
 }
 * \endcode
 * \note
 * \li  ȡ�ɱ䳤�� config_string_t �� config_nonlinear_t�������ȸ���ʹ�ó����������
 *      ��config������ uint8 ����������ȡ���ݣ���ȡ�ɹ���Ѹû�����ǿ��ת��Ϊ
 *      config_string_t �� config_nonlinear_t ����ָ�룬Ȼ��Ϳ��԰��� config_string_t
 *      �� config_nonlinear_t���ͽ��з��ʡ�
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

    //��ȡ�����������ṹ��
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

    //��ȡ����������
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
 *    ��ȡӦ��������Ĭ��ֵ��
 * \param[in]    config_id ���ò���ID
 * \param[out]   none
 * \return       uint16
 * \retval           ���ض�ȡ����Ӧ��������Ĭ��ֵ��
 * \ingroup      common_func
 * \ingroup      common_func
 * \par          exmaple code
 * \code
 ����1����ȡ��������ʱ��Ĭ��ֵ
 light_time = com_get_config_default_inner(CONFIG_ID_LIGHT_TIME);
 * \endcode
 * \note
 * \li  ��������ֵ�Ͳ����������û���֤ config_id ��ȷ�����Ҳ����������򷵻�0��
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

    //��ȡ�����������ṹ��
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

    //��ȡ����������
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

//����ʱ�������� vram merge
void com_vram_merge_inner(void)
{
    uint32 t1, t2;
    uint8 i;

    #if 0 //һ������º�̨���������Ž���VRAM Merge���жϼ���ms��û������ģ������ε�
    //��֧��������Ӧ����merge
    if (get_bt_state_inner() == BT_STATE_WORK)
    {
        return;
    }
    #endif

    //VRAM Merge��Ҫ��ʱռ���м��������ռ䣬�ʱ���û���м��ʱ���ܽ���VRAM Merge
    if (get_mmm_state_inner() != MMM_STATE_NO_PLAY)
    {
        return;
    }

    //�л��� 32KB ��ʱBUFFER
    base_set_config(BASE_SET_VRAM_BUFFER, (void *) vram_merge_temp_buffer);

    //ִ��merge
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

    //�ָ�ΪĬ�ϵ�ַ
    base_set_config(BASE_SET_VRAM_BUFFER, NULL);
}

/*! \endcond */
