/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������¼��ģ��򿪺͹رա�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "record_module.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ƶ�����м��
 * \param[in]   param  �����ֲ����м���Ĳ�����Ŀǰ����Ϊ��
 * \return       �������
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
 *    �ر����ֲ����м��
 * \param[in]   handel  �򿪵����ֲ����м���������
 * \param[out]  param �ر��м�����豣�����ݵ�ָ�룬Ŀǰ����Ϊ��
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
