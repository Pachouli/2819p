/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：录音模块命令API入口。
 * 作者：cailizhen
 ********************************************************************************/

#include "record_module.h"

record_module_t g_record_module_global;

const uint32 __section__(".rodata.entry") record_module_cmd_entry[] =
{
    (uint32) audio_record_module_cmd,
};

/*! \cond MMM_MR_API */

int32 audio_record_module_cmd(void *handel, record_module_cmd_e cmd, uint32 args)
{
    record_module_t *p_record_module = NULL;
    int32 ret = 0;

    //RECORD_MODULE_OPEN命令用于获取handle，所以此时handle实际上为一个存放p_record_module_global指针的地址
    if (cmd != RECORD_MODULE_OPEN)
    {
        p_record_module = (record_module_t *) handel;
    }

    if ((g_record_module_global.record_module_status.status == RECORD_MODULE_STA_CLOSE) && (cmd != RECORD_MODULE_OPEN))
    {
        libc_printf_warning("send invlaid cmd without init\n");
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case RECORD_MODULE_OPEN:
        {
            p_record_module = (record_module_t *)record_module_open((void*)args);
            if (p_record_module == NULL)
            {
                libc_printf("record module cmd->%d err\n", RECORD_MODULE_OPEN);
                *((record_module_t **) handel) = NULL;
                ret = -1;
                break;
            }
            *((record_module_t **) handel) = p_record_module;
        }
        break;

        case RECORD_MODULE_CLOSE:
        //非法状态
        if ((p_record_module->record_module_status.status != RECORD_MODULE_STA_STOP)
            && (p_record_module->record_module_status.status != RECORD_MODULE_STA_OPEN))
        {
            ret = -1;
            break;
        }

        ret = record_module_close(handel, NULL);
        break;

        case RECORD_MODULE_START:
        {
            if (p_record_module->record_module_status.status != RECORD_MODULE_STA_OPEN)
            {
                ret = -1;
                break;
            }

            start_record_module(p_record_module);
        }
        break;

        case RECORD_MODULE_STOP:
        {
            if ((p_record_module->record_module_status.status != RECORD_MODULE_STA_START)
                &&(p_record_module->record_module_status.status != RECORD_MODULE_STA_ERROR))
            {
                libc_printf_warning("record module already stop!\n");
                ret = 0;
                break;
            }

            ret = stop_record_module(p_record_module);
        }
        break;

        case RECORD_MODULE_SET_FS:
        libc_memcpy(&(p_record_module->record_fs), args, sizeof(record_module_fs_para_t));
        if (p_record_module->record_fs.file_handle == NULL)
        {
            libc_printf_error("record module file is null!\n");
            while (1);
            ret = -1;
        }
        break;

        case RECORD_MODULE_SET_ENCODE:
        libc_memcpy(&(p_record_module->encode_para), args, sizeof(record_module_encode_para_t));
        break;

        case RECORD_MODULE_SET_BUFFER:
        libc_memcpy(&(p_record_module->buffer_set), args, sizeof(record_module_buffer_set_t));
        break;

        case RECORD_MODULE_GET_STATUS:
        {
            record_module_status_t *p_status = (record_module_status_t *)args;
            libc_memcpy(p_status, &p_record_module->record_module_status, sizeof(record_module_status_t));
        }
        break;

        case RECORD_MODULE_GET_ENCODE_DETAIL:
        {
            record_module_encode_detail_t *p_encode_detail = (record_module_encode_detail_t *)args;;
            p_encode_detail->encode_frame_len = p_record_module->encode_frame_len;
        }
        break;

        case RECORD_MODULE_FRAME_DEAL:
        {
            if (p_record_module->record_module_status.status == RECORD_MODULE_STA_ERROR)
            {
                ret = -1;
                break;
            }

            if (p_record_module->record_module_status.status != RECORD_MODULE_STA_START)
            {
                libc_printf_warning("record module not start yet!\n");
                ret = -1;
                break;
            }

            ret = record_module_encode_deal(p_record_module, args);
        }
        break;

        default:
        ret = -1;
        libc_printf_warning("RECORD MODULE cmd not support:%d\n", cmd);
        break;
    }

    sys_os_sched_unlock();

    return ret;
}

int32 record_module_rcode_init(void)
{
    g_record_module_global.record_module_status.status = RECORD_MODULE_STA_CLOSE;

    return 0;
}

int32 record_module_rcode_exit(void)
{
    g_record_module_global.record_module_status.status = RECORD_MODULE_STA_CLOSE;

    return 0;
}

module_init (record_module_rcode_init)
module_exit(record_module_rcode_exit)

/*! \endcond */

