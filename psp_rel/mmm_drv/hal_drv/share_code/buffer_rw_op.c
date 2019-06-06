/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：读写缓冲区数据读写等操作。
 * 作者：cailizhen
 ********************************************************************************/

#include <psp_includes.h>

//向buffer写入数据
bool buffer_rw_write_inner(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len)
{
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if ((p_buf_rw->length + w_len) > p_buf_rw->capacity)
    {
        p_buf_rw->full_error = TRUE;
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    if ((p_buf_rw->write_ptr + w_len) > p_buf_rw->capacity)
    {
        read_len = p_buf_rw->capacity - p_buf_rw->write_ptr;
        rem_len = w_len - read_len;
    }
    else
    {
        read_len = w_len;
        rem_len = 0;
    }
    libc_memcpy(p_buf_rw->raw_data + p_buf_rw->write_ptr, w_buf, read_len);

    if (rem_len != 0)
    {
        libc_memcpy(p_buf_rw->raw_data, w_buf + read_len, rem_len);
    }

    p_buf_rw->write_ptr += w_len;
    p_buf_rw->write_ptr %= p_buf_rw->capacity;

    p_buf_rw->length += w_len;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//从buffer读出数据
bool buffer_rw_read_inner(buffer_rw_t *p_buf_rw, uint8 *r_buf, uint16 r_len)
{
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if (p_buf_rw->length < r_len)
    {
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    if ((p_buf_rw->read_ptr + r_len) > p_buf_rw->capacity)
    {
        read_len = p_buf_rw->capacity - p_buf_rw->read_ptr;
        rem_len = r_len - read_len;
    }
    else
    {
        read_len = r_len;
        rem_len = 0;
    }

    libc_memcpy(r_buf, p_buf_rw->raw_data + p_buf_rw->read_ptr, read_len);

    if (rem_len != 0)
    {
        libc_memcpy(r_buf + read_len, p_buf_rw->raw_data, rem_len);
    }

    p_buf_rw->read_ptr += r_len;
    p_buf_rw->read_ptr %= p_buf_rw->capacity;
    p_buf_rw->length -= r_len;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}

//复用raw_data，使用完后直接更新读指针和长度
bool buffer_rw_update_inner(buffer_rw_t *p_buf_rw, uint16 r_len)
{
    uint32 temp_flag = sys_local_intc_save(0x01);

    if (p_buf_rw->length < r_len)
    {
        sys_local_intc_restore(temp_flag);
        return FALSE;
    }

    p_buf_rw->read_ptr += r_len;
    p_buf_rw->read_ptr %= p_buf_rw->capacity;
    p_buf_rw->length -= r_len;

    sys_local_intc_restore(temp_flag);

    return TRUE;
}

//清空buffer
bool buffer_rw_flush_inner(buffer_rw_t *p_buf_rw)
{
    uint32 temp_flag = sys_local_intc_save(0x01);

    p_buf_rw->length = 0;
    p_buf_rw->read_ptr = 0;
    p_buf_rw->write_ptr = 0;

    sys_local_intc_restore(temp_flag);
    return TRUE;
}
