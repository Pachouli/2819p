/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件缓冲区读写接口；ASQT DUMP 接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

//ASQT DUMP DATA
bool dump_data_to_asqt_dump_buffer(asqt_dump_buf_t *p_asqt_dump_buf, uint8 bitmap, uint8 *dump_buf,
        uint16 dump_length)
{
    asqt_dump_tag_t *p_asqt_dump_tag;
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if ((p_asqt_dump_buf->dump_bitmap & bitmap) == 0)
    {
        sys_local_intc_restore(temp_flag);
        return TRUE;
    }

    if ((p_asqt_dump_buf->dump_capacity - p_asqt_dump_buf->dump_length) < (dump_length + 4))
    {
        sys_local_intc_restore(temp_flag);
        libc_printf_warning("dump full!\n");
        return FALSE;
    }

    p_asqt_dump_tag = (asqt_dump_tag_t *) (p_asqt_dump_buf->dump_buf + p_asqt_dump_buf->dump_write_ptr);

    p_asqt_dump_tag->tag = SP_ASQT_DUMP_TAG;
    p_asqt_dump_tag->bitmap = bitmap;
    p_asqt_dump_tag->length = dump_length;

    p_asqt_dump_buf->dump_write_ptr += 4;
    p_asqt_dump_buf->dump_write_ptr %= p_asqt_dump_buf->dump_capacity;
    p_asqt_dump_buf->dump_length += 4;

    if ((p_asqt_dump_buf->dump_write_ptr + dump_length) > p_asqt_dump_buf->dump_capacity)
    {
        read_len = p_asqt_dump_buf->dump_capacity - p_asqt_dump_buf->dump_write_ptr;
        rem_len = dump_length - read_len;
    }
    else
    {
        read_len = dump_length;
        rem_len = 0;
    }
    libc_memcpy(p_asqt_dump_buf->dump_buf + p_asqt_dump_buf->dump_write_ptr, dump_buf, read_len);

    if (rem_len != 0)
    {
        libc_memcpy(p_asqt_dump_buf->dump_buf, dump_buf + read_len, rem_len);
    }

    p_asqt_dump_buf->dump_write_ptr += dump_length;
    p_asqt_dump_buf->dump_write_ptr %= p_asqt_dump_buf->dump_capacity;
    p_asqt_dump_buf->dump_length += dump_length;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//FILL SCO IN PIPE
bool write_raw_data_to_sco_in(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len)
{
    uint32 temp_flag = sys_local_intc_save(0x01);
    sco_err_report_info_t *p_sco_err_rpt_info;
    uint8 i;

    if (buffer_rw_write(p_buf_rw, w_buf, w_len) == FALSE)
    {
    	sys_local_intc_restore(temp_flag);
    	return FALSE;
    }

    //保存sco error report信息
    p_sco_err_rpt_info = (sco_err_report_info_t *) p_buf_rw->ext_data;
    for (i = 0; i < (w_len/120); i++)
    {
        if (p_sco_err_rpt_info->length >= p_sco_err_rpt_info->count)
        {
            libc_printf_error("sco error report save error!\n");
            while (1)
            {
                ; //for QAC
            }
        }

        p_sco_err_rpt_info->err_report_data[p_sco_err_rpt_info->write_ptr].pkg_status = 0;
        p_sco_err_rpt_info->err_report_data[p_sco_err_rpt_info->write_ptr].pkg_length = 120;

        p_sco_err_rpt_info->write_ptr++;
        p_sco_err_rpt_info->write_ptr %= p_sco_err_rpt_info->count;
        p_sco_err_rpt_info->length++;
    }

    sys_local_intc_restore(temp_flag);
    return TRUE;
}
