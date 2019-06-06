/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：bank代码加载，需要支持sd卡和usb模式。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"


/* 
函数功能：
    从atf配置文件中 offset 位置读取 total_len 大小的数据，拷贝到 dstAddr地址中
    1. 用 STUB_ATT_RW_TEMP_BUFFER 发送数据，buffer大小256字节
    2. 考虑到 STUB_ATT_RW_TEMP_BUFFER 中有一部分空间要保存stub协议头和校验码，所以
       最大一次性读取大小 MAX_READ_LEN 必须小于 STUB_ATT_RW_TEMP_BUFFER 缓冲区大小，此处定为 200 字节
    3. 如果数据量比较大，需要分批读取
参数说明：
    atf_offset: 待读取的相对于atf文件首地址的位移
    total_len:  待读取的数据大小
    dstAddr:    拷贝数据的目的地址，需要保证此地址对应的缓存区大小至少为total_len
返回:
    如果读取遇到错误，则返回-1； 否则返回读取大小

atf文件格式说明:
    1. 512字节的文件头 atf_head_t, 描述了所打包的最多15个子文件的偏移位置和文件内容长度
    2. 每个子文件的偏移地址以256字节对齐方式，同时也意味着每个文件所占用的空间也是以256字节对齐，不足之处补0
*/
int read_atf_file_by_stub(const uint8* atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, uint32 atf_offset, uint32 total_len)
{
#define MAX_READ_LEN 200
    if ( 0 == total_len ) return 0;

    if (total_len > dst_buffer_len) return -1;

    uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
    if (NULL == stub_rw_buffer){
        libc_printf("read_atf_file_by_stub: stub_rw_buffer null\n");
        while(1){}
    }

    int ret_val = 0;

    atf_file_read_t     * sendData = (atf_file_read_t*)stub_rw_buffer;
    atf_file_read_ack_t * recvData = (atf_file_read_ack_t*)stub_rw_buffer; 

    uint32 cur_file_offset  = atf_offset;
    int32 left_len          = total_len;

    PRINTF("data_info -> atf_offset:%d total_len:%d\n",atf_offset,total_len);
    while(left_len > 0)
    {   
        //判断数据读取大小是否过大
        uint32 read_len = left_len;
        if (read_len >= MAX_READ_LEN) read_len = MAX_READ_LEN;

        //必须保证实际读取的负载数据大小按四字节对齐，否则读取失败
        uint32 aligned_len = (read_len + 3)/4*4;
        sendData->offset = cur_file_offset;
        sendData->readLen = aligned_len;

        //发送读取命令
        ret_val = att_write_data(STUB_CMD_ATT_FREAD, (sizeof(atf_file_read_t)-sizeof(stub_ext_cmd_t)), (uint8*)sendData);
        if (ret_val != 0){
            //libc_printf("%s line:%d att_write_data ret:%d", __FUNCTION__, __LINE__, ret_val);
            PRINTF("att_write_data ret:%d\n", ret_val);
            ret_val = -1;
            break;
        }

        //读取数据
        ret_val = att_read_data((uint8*)recvData,aligned_len);
        if (ret_val != 0){
            //libc_printf("%s line:%d att_read_data ret:%d", __FUNCTION__, __LINE__, ret_val);
            PRINTF("att_read_data ret:%d\n",ret_val);
            ret_val = -1;
            break;
        }

        //拷贝并更新相应变量,只拷贝需要的数据而不拷贝因对齐而补齐的部分
        libc_memcpy(dstAddr,recvData->payload, read_len);
        
        dstAddr  += read_len;
        left_len -= read_len;
        cur_file_offset += read_len;
    }

    PRINTF("read:%d bytes ok!\n",total_len);
    sys_free(stub_rw_buffer);

    return (ret_val < 0) ? ret_val : total_len;
}

/*  
    与read_atf_file_by_stub类似

返回：
    如果读取遇到错误，则返回-1； 否则返回读取大小
*/
int read_atf_file_by_sd(const uint8 *atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, uint32 atf_offset, uint32 total_len)
{   
    if ( 0 == total_len ) return 0;

    if (total_len > dst_buffer_len) return -1;

    PRINTF("data_info -> atf_offset:%d total_len:%d\n",atf_offset,total_len);

    int32 ret = -1;
    uint32 file_handle = com_file_open(DISK_H, (uint8*)atf_file_name, R_NORMAL_SEEK);
    if (0 == file_handle) {
        libc_printf("com_file_open sd error\n");
        return ret;
    }

    if (!com_file_seek(atf_offset, SEEK_SET, file_handle)) {
        com_file_close(file_handle);
        return ret;
    }

    ret = com_file_read(dstAddr, total_len, file_handle);
    com_file_close(file_handle);

    if ( 0 == ret ) {
        ret = -1;
    }else{
        PRINTF("read:%d bytes ok!\n",ret);
    }
    return ret;
}

//根据模式选择不同的atf文件读取功能
/*
返回：
    如果读取遇到错误，则返回-1； 否则返回读取大小
*/
int read_atf_file(uint32 test_mode, const uint8* atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, uint32 atf_offset, uint32 total_len)
{
    if (total_len > dst_buffer_len) return -1;

    int32 ret = -1;
    switch(test_mode){
        case TEST_MODE_CARD:
        {
            ret = read_atf_file_by_sd(atf_file_name, dstAddr, dst_buffer_len, atf_offset, total_len);
            break;
        }
        case TEST_MODE_USB:
        {
            ret = read_atf_file_by_stub(atf_file_name, dstAddr, dst_buffer_len, atf_offset, total_len);
            break;
        }
        default: 
        {
            break;
        }
    }

    return ret;
}

/*
    atf文件中包裹了多个文件, 该函数从atf文件头中获取指定文件的基本属性
参数说明:
    sub_file_name:  待获取属性的文件名
    p_offset:  用于返回指定文件相对于atf文件的位移
    p_length:  用于返回指定文件的数据长度
*/
 int atf_read_sub_file_attr(atf_head_t *p_atf_head, const uint8 * sub_file_name, uint32 * p_offset, uint32 * p_length)
{
    if(NULL == p_atf_head || NULL == sub_file_name || NULL == p_offset || NULL == p_length) return -1;

    int i;
    for(i = 0; i < ATF_MAX_SUB_FILE_NUM; i++){
        if (libc_strncmp(sub_file_name, (p_atf_head->atf_dir)[i].filename,ATF_MAX_SUB_FILENAME_LENGTH) == 0){
            *p_offset = (p_atf_head->atf_dir)[i].offset;
            *p_length = (p_atf_head->atf_dir)[i].length;
            return 0;
        }
    }

    PRINTF("can't find %s\n",sub_file_name);
    return -1;
}

/*
    从atf文件的指定子文件中读取文件内容
参数说明:
    test_mode:  测试模式，用以决定从何种方式读物数据，例如usb,sd卡
    dst_addr:   保存读取数据的目的地址
    dst_bufferlen: 目的缓冲最大长度，用以判断是否越界
    atf_file_name: 待读取的atf配置文件名
    sub_file_name: 待读取的atf中的子文件名
    p_atf_head:    atf文件头信息，可从中获取子文件的偏移位置和长度
    offset_from_sub_file: 相对于子文件起始地址的偏移量
    read_len:       待读取的数据长度，若 read_len<0 则表示一直读取到子文件末尾
返回：
    读取失败返回-1，否则返回读取长度
*/
int read_atf_sub_file_data(uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, atf_head_t *p_atf_head, \
    const uint8* atf_file_name, const uint8 * sub_file_name, int32 offset_from_sub_file, int32 read_len)
{
    int32 sub_file_offset, sub_file_length;

    int ret = atf_read_sub_file_attr(p_atf_head, sub_file_name, &sub_file_offset, &sub_file_length);
    if (ret != 0 ) return -1;

    if (read_len < 0){
        read_len = sub_file_length - offset_from_sub_file;
    }

    if (read_len < 0){
        libc_printf("read_len <= 0\n");
        return -1;
    }

    if (read_len > dst_buffer_len){
        libc_printf("read_len:%d > dst_buffer_len:%d\n",read_len,dst_buffer_len);
        return -1;
    }

    if (offset_from_sub_file + read_len > sub_file_length) {
        libc_printf("atf_file_name:%s, sub_file_name%s, offset_from_sub_file:%d + read_len:%d > sub_file_length:%d\n",\
            atf_file_name, sub_file_name, offset_from_sub_file, read_len, sub_file_length);
        return -1;
    }

    return read_atf_file(test_mode, atf_file_name, dst_addr, dst_buffer_len, sub_file_offset + offset_from_sub_file, read_len);
}

