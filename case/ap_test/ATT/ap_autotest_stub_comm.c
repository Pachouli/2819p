/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：stub通信封装。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"

/*
    包装stub_ext_write函数调用
参数说明:
    cmd:            发送的基于stub协议的命令
    payload_len:    协议负载的待发送的数据部分的长度(4字节对齐)，不包括协议头(6个字节)和末尾的检验码(2字节)
    data_addr:      待发送的整个协议数据(包括协议头和负载数据部分)的起始地址
其他：
    调用att_write_data时确保data_addr中已填充了负载数据内容
*/
uint32 att_write_data(uint16 cmd, uint32 payload_len, uint8* data_addr)
{
    int32 ret_val;
    stub_ext_param_t ext_param;

    ext_param.opcode = cmd;
    ext_param.payload_len = payload_len;
    ext_param.rw_buffer = (uint8 *)data_addr;

    ret_val = stub_ext_write(&ext_param);

    return ret_val;
}

/*
    包装stub_ext_read函数调用
参数说明:
    payload_len:    待接收的负载数据部分的长度(4字节对齐)，不包括协议头(6个字节)
    data_addr:      用于接收整个协议数据(包括协议头和数据部分)的起始地址
*/
uint32 att_read_data(uint8* data_addr, uint32 payload_len)
{
    int32 ret_val;
    stub_ext_param_t ext_param;

//    ext_param.opcode = cmd;
    ext_param.payload_len = payload_len;
    ext_param.rw_buffer = (uint8 *)data_addr;

    ret_val = stub_ext_read(&ext_param);

    return ret_val;
}
