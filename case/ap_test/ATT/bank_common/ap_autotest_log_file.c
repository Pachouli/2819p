/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ��忨�Զ������Բ�����־����ӿڡ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"

/*
    ��ÿ���ֽ�ת����2��hex�ַ���ר����������ַ��ת��
*/
 void byte_to_ascii(uint8 *byte_value, uint8 *ascii_buffer, uint32 byte_length)
{
    uint8 t1, t2, dat;
    uint32 i;

    byte_value = byte_value + byte_length - 1;
    for (i = 0; i < byte_length; i++, byte_value--, ascii_buffer++)
    {
        dat = *byte_value;

        t1 = dat & 0x0f;
        t2 = (dat & (uint8)0xf0) >> 4;
        if (t2 < 10)
        {
            t2 = t2 + '0';
        }
        else
        {
            t2 = t2 - 10 + 'a';
        }
        if (t1 < 10)
        {
            t1 = t1 + '0';
        }
        else
        {
            t1 = t1 - 10 + 'a';
        }

        *ascii_buffer = t2;
        ascii_buffer++;
        *ascii_buffer = t1;
    }
}




 void create_log_file_name(uint8 *file_name, uint32 record_count)
{
    uint32 timer;

    file_name[0] = 0xff;
    file_name[1] = 0xfe;

    file_name[2] = 'A';
    file_name[4] = 'T';
    file_name[6] = 'T';
    file_name[8] = '_';

    timer = sys_get_ab_timer();

    //��ʾʱ��
    file_name[10] = (timer / 1000000) + '0';
    file_name[12] = ((timer % 1000000)/100000) + '0';
    file_name[14] = ((timer % 100000)/10000) + '0';
    file_name[16] = ((timer % 10000)/1000) + '0';
    file_name[18] = ((timer % 1000)/100) + '0';
    file_name[20] = ((timer % 100)/10) + '0';
    file_name[22] = (timer % 10) + '0';
    file_name[24] = '_';

    //��ʾcount
    file_name[26] = (record_count/1000000) + '0';
    file_name[28] = ((record_count%1000000)/100000) + '0';
    file_name[30] = ((record_count%100000)/10000) + '0';
    file_name[32] = ((record_count%10000)/1000) + '0';
    file_name[34] = ((record_count%1000)/100) + '0';
    file_name[36] = ((record_count%100)/10) + '0';
    file_name[38] = (record_count%10) + '0';

    file_name[40] = 0x2e;
    file_name[42] = 'L';
    file_name[44] = 'O';
    file_name[46] = 'G';
    file_name[48] = 0x0;
}


// ���ļ������ļ������ڣ��򴴽�֮
 int32 com_file_open_or_create(const uint8* file_name)
{
    uint32 file_handle;
        
    //�����ļ�ϵͳ,�򿪴���
    uint32 file_sys_id = com_file_open(DISK_H, NULL, 0);
    //�ڸ�Ŀ¼�´���log�ļ�
    vfs_cd(file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(file_sys_id, (uint8*)file_name, R_NORMAL_SEEK);
    if(0 == file_handle)
    {
        file_handle = vfs_file_create(file_sys_id, (uint8*)file_name, 0);
    }

    if ( 0 == file_handle){
        libc_printf("open or create file:%s fail\n", file_name);
    }

    return file_handle;
}




 void write_log_file(const uint8* file_name, uint8* log_buffer, uint32 write_len)
{
    if (NULL == file_name || NULL == log_buffer || 0 == write_len) {
        return;
    }

    uint32 file_handle = com_file_open_or_create(file_name);
    if(file_handle == 0) {
        return;
    }
    
    com_file_seek(0, SEEK_END, file_handle);
    com_file_write(log_buffer, write_len, file_handle);

    libc_printf("write file over!", 0, 0);

    //vfs_file_close(file_sys_id, file_handle);
    com_file_close(file_handle);

}

/*
    ���sd�����ԣ������Խ��ת����log�ı���Ϣ��append��������
����˵��:
    test_info:      ����������
    test_result:    ���Գɹ���ʧ��
    log_buffer:     ��־������׵�ַ
    cur_total_len:  ��ǰ�����ѱ������־���ݳ���
���棺
    �˺��������log_buffer��󻺳�����С��������ÿһ��������������ʱ��ʱ��������log����Ȼ����ջ�����
    ������ܻ���ɻ���������������쳣
*/
 void att_test_add_result_to_sd_log(uint8 *test_info, bool test_result, uint8 * log_buffer, uint32* cur_total_len)
{
    if (NULL == log_buffer || NULL == cur_total_len) return;

    uint8* begin_addr = log_buffer + (*cur_total_len);
    uint32 write_len = 0;
    
    libc_printf("%s %d\n", test_info, test_result);

    if (NULL != test_info)
    {
        while(*test_info != '\0')
        {
            begin_addr[write_len] = *test_info;

            test_info++;
            write_len++;
        }
    }

    if (FALSE == test_result)
    {
        libc_strncpy(&begin_addr[write_len], " FAIL", 5);
        //led_flash_fail();
    }
    else
    {
        libc_strncpy(&begin_addr[write_len], " PASS", 5);
        //led_flash_ok();
    }
    write_len += 5;

    begin_addr[write_len++] = 0x0D;
    begin_addr[write_len++] = 0x0A;

    (*cur_total_len) += write_len;
}


/*
    ���sd�����ԣ������Խ��ת����log�ı���Ϣ��appedn��������
����˵��:
    test_info:      ����������
    test_data:      ���Խ������(������)����ת����16�����ַ���ascii��
    log_buffer:     ��־������׵�ַ
    cur_total_len:  ��ǰ�����ѱ������־���ݳ���
���棺
    �˺��������log_buffer��󻺳�����С��������ÿһ��������������ʱ��ʱ��������log����Ȼ����ջ�����
    ������ܻ���ɻ���������������쳣
*/
 void att_test_add_result_to_sd_log_ext(uint8 *test_info, uint8* test_data, uint32 data_bytes, uint8 * log_buffer, uint32* cur_total_len)
{
    if (NULL == log_buffer || NULL == cur_total_len) return;

    uint8* begin_addr = log_buffer + (*cur_total_len);
    uint32 write_len = 0;

    if (NULL != test_info)
    {
        while(*test_info != '\0')
        {
            begin_addr[write_len] = *test_info;

            test_info++;
            write_len++;
        }
    }

    byte_to_ascii(test_data, &begin_addr[write_len], data_bytes);
    write_len += data_bytes * 2;

    begin_addr[write_len++] = 0x0D;
    begin_addr[write_len++] = 0x0A;

    (*cur_total_len) += write_len;
}


