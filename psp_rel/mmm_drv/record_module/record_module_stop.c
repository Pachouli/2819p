/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������¼��ģ�����¼���ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "record_module.h"

void destroy_write_file_thread(record_module_t *p_record_module)
{
    p_record_module->write_thread_terminate_flag = TRUE;

    while (p_record_module->write_thread_terminate_flag == TRUE)
    {
        sys_os_sched_unlock();
        sys_os_time_dly(1);
        sys_os_sched_lock();
    }
}

/* �������ֽ����� */
int32 stop_record_module(record_module_t *p_record_module)
{
    int32 ret = 0;

    p_record_module->stop_encode_flag = TRUE;

    if (p_record_module->record_module_status.status == RECORD_MODULE_STA_START)
    {
        write_record_file:
        while (p_record_module->encode_output_buf_w.length >= WRITE_SIZE_PER_TIME)
        {
            sys_os_sched_unlock();
            sys_os_time_dly(1);
            sys_os_sched_lock();

            if (p_record_module->record_module_status.status != RECORD_MODULE_STA_START)
            {
                break;
            }
        }
        //���ĩβ��ȷ�����б���������ݶ��������浽�ļ�
        if ((p_record_module->record_module_status.status == RECORD_MODULE_STA_START)
            && (p_record_module->encode_output_buf_w.length > 0))
        {
            uint8 *data_start = p_record_module->encode_output_buf_w.raw_data;
            uint16 write_ptr = p_record_module->encode_output_buf_w.write_ptr;
            uint16 data_len = p_record_module->encode_output_buf_w.length;

            libc_memset(data_start + write_ptr, 0, WRITE_SIZE_PER_TIME - data_len);

            p_record_module->encode_output_buf_w.length = WRITE_SIZE_PER_TIME;

            goto write_record_file;
        }
    }

    //״̬�޸�Ϊ RECORD_MODULE_STA_STOP��STOP���̽���
    p_record_module->record_module_status.status = RECORD_MODULE_STA_STOP;

    //д�ļ��߳�Ҳ������Ҫ������д�ļ��߳�
    destroy_write_file_thread(p_record_module);

    if (p_record_module->encode_para.encode_mode != ENCODE_BYPASS)
    {
        //�رձ����
        ret = audio_encoder_cmd(p_record_module->audioenc_handle, AE_CMD_CLOSE, 0);
        p_record_module->audioenc_handle = NULL;
        if (ret != 0)
        {
            libc_printf_error("AE_CMD_CLOSE fail!!\n");
        }

        //ж�ر����
        sys_drv_uninstall(REC_WAV_GROUP);
    }

    p_record_module->encode_output_buf_w.read_ptr = 0;
    p_record_module->encode_output_buf_w.write_ptr = 0;
    p_record_module->encode_output_buf_w.length = 0;

    return ret;
}

