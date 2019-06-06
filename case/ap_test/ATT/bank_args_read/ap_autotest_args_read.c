/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：自动化测试测试项参数读取接口
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"
#include "bank_resident/ap_autotest_resident_entry.h"

//根据测试项行号，定位测试项的起始位置
uint16 *act_test_locate_test_item(uint16* cfg_file_buffer, uint32 cfg_file_size, uint32 line_number)
{

    uint16 *cur;
    uint16 *begin;
    uint16 *start;
    uint32 cur_line;
    
    cur = cfg_file_buffer;
    cur_line = 0;

    while(1)
    {
        start = cur;

        begin = cur;

        //搜索结束标志 00 0d 00 0a
        while (*cur != 0x000a)
        {
            cur++;
        }

        while ((*start != 0x003d) && (start != cur))
        {
            start++;
        }

        //搜索到'='，说明是有效的数据行，否则过滤该行数据
        if(*start == 0x003d)
        {
            cur_line++;
        }

        //搜索到指定行数，退出
        if(cur_line == line_number)
        {
            break;
        }
        else
        {
            cur++;

            if((uint32)(cur - cfg_file_buffer) >= cfg_file_size)
            {
                return 0;
            }
        }
    }

    return begin;
}

 uint8 unicode_to_uint8(uint16 widechar)
{
    uint8 temp_value;

    if((widechar >= '0') && (widechar <= '9'))
    {
        temp_value = (widechar - '0');
    }
    else if((widechar >= 'A') && (widechar <= 'F'))
    {
        temp_value = widechar + 10 - 'A';
    }
    else if((widechar >= 'a') && (widechar <= 'f'))
    {
        temp_value = widechar + 10 - 'a';
    }
    else
    {
        return 0;
    }

    return temp_value;
}

 int32 unicode_to_int(uint16 *start, uint16 *end, uint32 base)
{
    uint32 minus_flag;
    int32 temp_value = 0;

    minus_flag = FALSE;

    while(start != end)
    {
        if(*start == '-')
        {
            minus_flag = TRUE;
        }
        else
        {
            temp_value *= base;

            temp_value += unicode_to_uint8(*start);
        }
        start++;
    }

    if(minus_flag == TRUE)
    {
        temp_value = 0 - temp_value;
    }

    return temp_value;
}

 int32 unicode_encode_utf8(uint8 *s, uint16 widechar)
{
    int32 encode_len;

    if (widechar & 0xF800)
    {
        encode_len = 3;
    }
    else if (widechar & 0xFF80)
    {
        encode_len = 2;
    }
    else
    {
        encode_len = 1;
    }

    switch (encode_len)
    {
        case 1:
            *s = (char)widechar;
            break;

        case 2:
            *s++ = 0xC0 | (widechar >> 6);
            *s   = 0x80 | (widechar & 0x3F);
            break;

        case 3:
            *s++ = 0xE0 | (widechar >> 12);
            *s++ = 0x80 | ((widechar >> 6) & 0x3F);
            *s   = 0x80 | (widechar & 0x3F);
            break;
    }

    return encode_len;
}

 int32 unicode_to_utf8_str(uint16 *start, uint16 *end, uint8 *utf8_buffer, uint32 utf8_buffer_len)
{
    int32 encode_len;
    int32 encode_total_len;

    encode_len = 0;
    encode_total_len = 0;

    while(start != end)
    {
        encode_len = unicode_encode_utf8(utf8_buffer, *start);

        start++;

        if(encode_len + encode_total_len > utf8_buffer_len)
        {
            return FALSE;
        }

        encode_total_len += encode_len;

        utf8_buffer += encode_len;
    }

    //末尾添加结束符
    *utf8_buffer = 0;

    return TRUE;
}

/*
    将unicode编码转换成uint8数组，专用于蓝牙测试项中提取蓝牙地址
参数说明:
    start,end:      待转换的unicode编码始末位置
    byte_buffer:    待填充的uint8数组位置，一般是蓝牙地址的首地址
    byte_index:     相对于byte_buffer的索引位置
    byte_len:       待读取的长度
其他：
    从unicode编码的参数内容依次读取并按从高到低的顺序填充到byte_buffer，即byte_buffer按蓝牙地址高位在高索引(高地址)，低位在低索引(低地址)
    例如unicode第0个字符填充到byte_buffer第5个字节，
        unicode第5个字符填充到byte_buffer第0个字节
*/
 int32 unicode_to_uint8_bytes(uint16 *start, uint16 *end, uint8 *byte_buffer, uint8 byte_index, uint8 byte_len)
{
    while(start != end)
    {
        byte_buffer[byte_index] = (unicode_to_uint8(*start) << 4);

        byte_buffer[byte_index] |= unicode_to_uint8(*(start + 1));

        byte_index--;

        byte_len--;

        if(byte_len == 0)
        {
            break;
        }

        start += 2;
    }

    return TRUE;
}

//在当前行提取test id
 uint32 act_test_read_test_id_from_line(uint16 *line_buffer)
{
    uint16 *start;
    uint16 *cur;
    uint32 test_id;

    start = line_buffer;
    cur = start;

    while (*cur != 0x003d)
    {
        cur++;
    }

    //test id都是十进制的字符串
    test_id = unicode_to_int(start, cur, 10);

    return test_id;
}


/*
    解析当前行，获取指定参数的始末位置
    1. sd卡和stub协议获取的参数都是unicode编码
*/
 uint8 *locate_para_from_test_item_line(uint8 test_mode, uint16 *line_buffer, uint8 arg_number, uint16 **start, uint16 **end)
{
    uint16 *cur;
    //uint32 test_id;
    uint8 cur_arg_num;

    cur = line_buffer;

    //卡模式需要过滤包括'='的前面字符
    //USB模式直接截取字符串即可
    if(test_mode == TEST_MODE_CARD)
    {
        while(*cur != 0x003d)
        {
            cur++;
        }

        //skip '='
        cur++;
    }

    cur_arg_num = 0;

    while(cur_arg_num < arg_number)
    {
        *start = cur;

        //用以标记当前参数是否是最后一个参数
        bool b_cur_is_last_arg = FALSE;

        //遇到','表明解析一个参数，遇到0x0d0a表示一行结束
        while(1)
        {
            if(*cur == 0x002c){
                break;
            }else if((*cur == 0x000d) || (*cur == 0x0000)){
                b_cur_is_last_arg = TRUE;
                break;
            }
            else{
                cur++;
            }            
        }
        /*while((*cur != 0x002c) && (*cur != 0x000d) && (*cur != 0x0000))
        {
            cur++;
        }*/

        *end = cur;

        cur_arg_num++;

        if(TRUE == b_cur_is_last_arg){
            //当前已读完最后一个参数，不再往下搜索
            break;
        }else{
            //跳过','
            cur++;
        }
    }

    //如果参数数量不足，则循环退出时，当前参数不是预期参数，当前测试项的参数错误
    if (cur_arg_num != arg_number){
        //参数不足
        libc_printf("fail to parse arg_number:%d \n", arg_number);
        while(1){}
    }

    return 0;
}

 int32 parse_args_on_none(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
 {
    return TRUE;
 }

 int32 parse_args_on_modify_bt_name(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start;
    uint16 *end;

    bt_name_arg_t *bt_name_arg = (bt_name_arg_t *)arg_buffer;

    if(arg_len < sizeof(bt_name_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    //第一个参数为蓝牙地址高3字节
    locate_para_from_test_item_line(test_mode, line_buffer, 1, &start, &end);

    unicode_to_utf8_str(start, end, bt_name_arg->bt_name, TEST_BTNAME_MAX_LEN);

    PRINTF("bt_name:%s\n", bt_name_arg->bt_name);

    return TRUE;
}


/*
static int32 act_test_read_ble_name_arg(uint16 *line_buffer, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start;
    uint16 *end;

    ble_name_arg_t *ble_name_arg = (ble_name_arg_t *)arg_buffer;

    if(arg_len < sizeof(ble_name_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    locate_para_from_test_item_line(line_buffer, 1, &start, &end);

    unicode_to_utf8_str(start, end, ble_name_arg->bt_ble_name, TEST_BTBLENAME_MAX_LEN);

    libc_printf( ble_name_arg->bt_ble_name, 0, 0);

    return TRUE;
}
*/



 int32 parse_args_on_modify_bt_addr(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start;
    uint16 *end;

    bt_addr_arg_t *bt_addr_arg = (bt_addr_arg_t *)arg_buffer;

    if(arg_len < sizeof(bt_addr_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    //第一个参数为蓝牙地址高3字节
    locate_para_from_test_item_line(test_mode, line_buffer, 1, &start, &end);

    //读取byte5, byte4, byte3数据，一般为蓝牙地址固定字节
    unicode_to_uint8_bytes(start, end, bt_addr_arg->bt_addr, 5, 3);

    //对于卡测试模式，首先从创建的bt_addr.log文件读取蓝牙地址，如果没有
    //再从配置文件读取
    if(test_mode == TEST_MODE_CARD)
    {
        if(READ_BT_ADDR_FROM_RECORD_FILE(bt_addr_arg->bt_addr) == FALSE)
        {   
            //第二个参数为蓝牙地址低3字节
            locate_para_from_test_item_line(test_mode, line_buffer, 2, &start, &end);

            //读取byte2, byte1, byte0数据，一般为会修改的字节
            unicode_to_uint8_bytes(start, end, bt_addr_arg->bt_addr, 2, 3);
        }
    }
    else
    {
        //非卡测试方案，直接解析pc发过来的数据
        locate_para_from_test_item_line(test_mode, line_buffer, 2, &start, &end);

        unicode_to_uint8_bytes(start, end, bt_addr_arg->bt_addr, 2, 3);
    }
    locate_para_from_test_item_line(test_mode, line_buffer, 4, &start, &end);

    bt_addr_arg->bt_addr_add_mode = unicode_to_int(start, end, 16);
//    libc_printf("btaddr mode:%d", bt_addr_arg->bt_addr_add_mode);

    locate_para_from_test_item_line(test_mode, line_buffer, 5, &start, &end);

    bt_addr_arg->bt_burn_mode = unicode_to_int(start, end, 16);

    locate_para_from_test_item_line(test_mode, line_buffer, 6, &start, &end);

    bt_addr_arg->bt_print_flag = unicode_to_int(start, end, 16);

    return TRUE;
}




 int32 parse_args_on_bt_play(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start = 0;
    uint16 *end = 0;
    uint8 arg_num = 1;

    btplay_test_arg_t *btplay_arg = (btplay_test_arg_t *)arg_buffer;

    if(arg_len < sizeof(btplay_test_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    //读取6字节的蓝牙地址
    unicode_to_uint8_bytes(start, end, btplay_arg->bt_transmitter_addr, 5, 6);

    //卡测试模式需要略过后面的5个蓝牙地址参数，取最后一个测试模式参数
    //PC测试模式，参数由PC分配，不需要进行处理
    if(test_mode == TEST_MODE_CARD)
    {
        arg_num += 5;
/*
        libc_printf("byte0 %x\n", btplay_arg->bt_transmitter_addr[0]);
        libc_printf("byte1 %x\n", btplay_arg->bt_transmitter_addr[1]);
        libc_printf("byte2 %x\n", btplay_arg->bt_transmitter_addr[2]);
        libc_printf("byte3 %x\n", btplay_arg->bt_transmitter_addr[3]);
        libc_printf("byte4 %x\n", btplay_arg->bt_transmitter_addr[4]);
        libc_printf("byte5 %x\n", btplay_arg->bt_transmitter_addr[5]);
*/
    }
    else
    {
/*
        print_log("bt addr: %02x:%02x:%02x:%02x:%02x:%02x",  btplay_arg->bt_transmitter_addr[5], \
        btplay_arg->bt_transmitter_addr[4], btplay_arg->bt_transmitter_addr[3], \
        btplay_arg->bt_transmitter_addr[2], btplay_arg->bt_transmitter_addr[1], \
        btplay_arg->bt_transmitter_addr[0]);
*/
    }

    //读取测试模式
    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    btplay_arg->bt_test_mode = unicode_to_int(start, end, 16);

    return TRUE;
}




 int32 parse_args_on_read_bt_name(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start = 0;
    uint16 *end = 0;
    uint8 arg_num;

    read_btname_test_arg_t *read_btname_arg = (read_btname_test_arg_t *) arg_buffer;

    if(arg_len < sizeof(read_btname_test_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    arg_num = 1;

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    read_btname_arg->cmp_btname_flag = (uint8) unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    unicode_to_utf8_str(start, end, read_btname_arg->cmp_btname, TEST_BTNAME_MAX_LEN);
/*
    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    read_btname_arg->cmp_blename_flag = (uint8) unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);

    unicode_to_utf8_str(start, end, read_btname_arg->cmp_blename, TEST_BTBLENAME_MAX_LEN);
*/
    return TRUE;
}




 int32 parse_args_on_fm_play(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start;
    uint16 *end;

    fmplay_test_arg_t *p_fmplay_test_arg = (fmplay_test_arg_t *)arg_buffer;

    if(arg_len < sizeof(fmplay_test_arg_t))
    {
        libc_printf("argument too long", 0, 0);
        while(1);
    }

    locate_para_from_test_item_line(test_mode, line_buffer, 1, &start, &end);

    p_fmplay_test_arg->freq = unicode_to_int(start, end, 10);
    p_fmplay_test_arg->freq *= 100;

    locate_para_from_test_item_line(test_mode, line_buffer, 2, &start, &end);

    p_fmplay_test_arg->threshold = unicode_to_int(start, end, 10);

    return TRUE;
}

/* gpio测试项参数解析 */
int32 parse_args_on_gpio(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *start = 0;
    uint16 *end = 0;

    gpio_test_arg_t *gpio_arg = (gpio_test_arg_t *) arg_buffer;

    if(arg_len < sizeof(gpio_test_arg_t))
    {
       libc_printf("argument too long", 0, 0);
       while(1);
    }

    locate_para_from_test_item_line(test_mode, line_buffer, 1, &start, &end);

    gpio_arg->gpioA_value = unicode_to_int(start, end, 16);

    locate_para_from_test_item_line(test_mode, line_buffer, 2, &start, &end);

    gpio_arg->gpioB_value = unicode_to_int(start, end, 16);

    locate_para_from_test_item_line(test_mode, line_buffer, 3, &start, &end);

    gpio_arg->gpioSIO_value = unicode_to_int(start, end, 16);

    return TRUE;
}

/* 通道测试参数解析 */
int32 parse_args_on_read_channel(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len)
{
    uint16*  start = NULL;
    uint16*  end   = NULL;
    uint8 arg_num;
    channel_test_arg_t *channel_test_arg = (channel_test_arg_t *)arg_buffer;
    
    if(arg_len < sizeof(channel_test_arg_t))
    {
       libc_printf("argument too long", 0, 0);
       while(1);
    }

    arg_num = 1;

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->test_left_ch = (uint8)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->left_ch_power_threadshold = (uint32)unicode_to_int(start, end, 10);    

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->test_left_ch_SNR = (uint8)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->left_ch_SNR_threadshold = (uint32)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->left_ch_max_sig_point = unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->test_right_ch = (uint8)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->right_ch_power_threadshold = (uint32)unicode_to_int(start, end, 10);    

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->test_right_ch_SNR = (uint8)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->right_ch_SNR_threadshold = (uint32)unicode_to_int(start, end, 10);

    locate_para_from_test_item_line(test_mode, line_buffer, arg_num++, &start, &end);
    channel_test_arg->right_ch_max_sig_point = (uint8)unicode_to_int(start, end, 10);

    return TRUE;
}

const att_task_read_arg_t autotest_readarg_ops[] =
{
    {TESTID_MODIFY_BTNAME,          parse_args_on_modify_bt_name},

    {TESTID_MODIFY_BTADDR,          parse_args_on_modify_bt_addr},

    {TESTID_READ_BTADDR,            parse_args_on_none},

    {TESTID_READ_BTNAME,            parse_args_on_read_bt_name},

    {TESTID_BT_TEST,                parse_args_on_bt_play},

    {TESTID_FM_TEST,                parse_args_on_fm_play},

    {TESTID_GPIO_TEST,              parse_args_on_gpio},

    {TESTID_LINEIN_TEST,            parse_args_on_none},

    {TESTID_SDCARD_TEST,            parse_args_on_none},

    {TESTID_UHOST_TEST,             parse_args_on_none},

    {TESTID_LINEIN_CH_TEST,         parse_args_on_read_channel},

    {TESTID_MIC_CH_TEST,            parse_args_on_read_channel},
};



/*
    根据test_id来解析stub协议中的命令参数数据
参数说明:
    line_buffer:    stub协议中的负载数据内容，具体为命令参数
    arg_buffer:     用于存储解析后的结构化参数的缓冲区
    arg_len:        缓冲区长度
*/
 bool act_test_read_test_arg(uint8 test_mode, uint16 test_id, uint16 *line_buffer, uint8 *arg_buffer, uint32 arg_len)
{
    uint32 i;

    for(i = 0; i < sizeof(autotest_readarg_ops) / sizeof(att_task_read_arg_t); i++)
    {
        if(autotest_readarg_ops[i].test_id == test_id)
        {
            return autotest_readarg_ops[i].read_arg_func(line_buffer, test_mode, arg_buffer, arg_len);
        }
    }

    return FALSE;
}




 bool act_test_read_test_info(uint8 test_mode, uint8* cfg_file_buffer, uint32 cfg_file_size, uint8 read_line, uint16 *test_id, uint8 *arg_buffer, uint32 arg_len)
{
    uint16 *line_buffer;

    line_buffer = act_test_locate_test_item((uint16*)cfg_file_buffer, cfg_file_size, read_line);

    //文件解析完毕，测试完成
    if(line_buffer == 0)
    {
        *test_id = TESTID_ID_QUIT;
        return TRUE;
    }

    *test_id = (uint16)act_test_read_test_id_from_line(line_buffer);

    return act_test_read_test_arg(test_mode, *test_id, line_buffer, arg_buffer, arg_len);
}


/*
    读取新的测试项信息，并对参数进行结构化解析
参数说明:
    arg_buffer/arg_len: 用于返回当前测试项的解析后的结构化参数
    p_test_info:        存储当前测试项基本信息
*/
 bool act_read_testinfo_and_args_on_stub(uint8 test_mode, uint8 *arg_buffer, uint32 arg_len, autotest_test_info_t * p_test_info)
{   
    if (NULL == arg_buffer || p_test_info == NULL) return FALSE;

    uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
    if (NULL == stub_rw_buffer){
        libc_printf("read_testinfo: stub_rw_buffer null\n");
        while(1){}
    }

    int ret_val;
    uint8 *cmd_data;
    uint8 temp_data[8];

    if(TEST_MODE_USB != test_mode){
        goto read_error;
    }

    //发送测试项信息请求数据包
    cmd_data = (uint8*)stub_rw_buffer;
    //该超时时间以秒为单位，取决于最长测试项所使用的时间
    cmd_data[6] = ATT_ITEM_TEST_TIMEOUT;    // 测试项超时
    cmd_data[7] = 0;
    cmd_data[8] = 0;
    cmd_data[9] = 0;

    if (ATT_WRITE_DATA(STUB_CMD_ATT_GET_TEST_ID, 4, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }

    //获取测试项信息包
    if (ATT_READ_DATA((uint8*)stub_rw_buffer, 4) != 0){
        goto read_error;
    }

    cmd_data = (uint8 *)stub_rw_buffer;
    p_test_info->test_id = (cmd_data[6] | (cmd_data[7] << 8));
    p_test_info->arg_len = (cmd_data[8] | (cmd_data[9] << 8));

    //回复ACK
    if (ATT_WRITE_DATA(STUB_CMD_ATT_ACK, 0, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }

    libc_printf("read_testinfo_stub testid:%d raw_arg_len:%d\n", p_test_info->test_id,p_test_info->arg_len);

    if(p_test_info->arg_len == 0){
        goto read_success;
    }
    
    //发送获取参数信息请求
    if (ATT_WRITE_DATA(STUB_CMD_ATT_GET_TEST_ARG, 0, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }
   
    //读取参数信息
    if (ATT_READ_DATA((uint8*)stub_rw_buffer, p_test_info->arg_len) != 0){
        goto read_error;
    }
    
    //避免使用STUB_ATT_RW_TEMP_BUFFER，因为后续还需要使用该buffer解析参数
    if (ATT_WRITE_DATA(STUB_CMD_ATT_ACK, 0, (uint8*)temp_data) != 0){
        goto read_error;
    }

    if (act_test_read_test_arg(test_mode, p_test_info->test_id, (uint16*)(stub_rw_buffer + sizeof(stub_ext_cmd_t)), arg_buffer, arg_len) != TRUE){
        goto read_error;
    }

read_success:
    {
        if(stub_rw_buffer != NULL){
            sys_free(stub_rw_buffer);    
        }
        return TRUE;
    }
read_error:
    {
        if(stub_rw_buffer != NULL){
            sys_free(stub_rw_buffer);    
        }
        return FALSE;
    }
}

/*
    读取新的测试项信息，并对参数进行结构化解析
参数说明:
    cfg_file_buffer:    config.txt测试项缓冲
    p_cur_line_num:     当前读取的测试项行号
    arg_buffer/arg_len: 用于返回当前测试项的解析后的结构化参数
    p_test_info:        存储当前测试项基本信息
*/
 int32 act_read_testinfo_and_args_on_sd(uint8 test_mode, uint8* cfg_file_buffer, uint32 cfg_file_size, uint32* p_cur_line_num, uint8 *arg_buffer, uint32 arg_len, autotest_test_info_t * p_test_info)
{   
    if (NULL == arg_buffer || p_test_info == NULL) return FALSE;

    bool b_res = act_test_read_test_info(test_mode, cfg_file_buffer, cfg_file_size, (*p_cur_line_num), &(p_test_info->test_id), arg_buffer, arg_len);
    if (!b_res){
        return FALSE;
    }

    if(TESTID_ID_QUIT!= p_test_info->test_id)
    {
        (*p_cur_line_num)++;
    }

    return TRUE;
}


/*
    读取测试项基本信息和参数
参数:
    test_mode:        当前测试模式，用以选择不同的参数解析方式
    cfg_file_buffer:  config.txt配置文件内容缓冲
    cfg_file_size:    配置文件大小
    p_cur_line_num:   config.txt中每行一个测试项,该值指示当前测试项所在行数
    arg_buffer:       返回结构化参数值
    arg_len:          参数缓冲大小，防止溢出
    p_test_info:      返回测试基本信息
*/
bool act_read_testinfo_and_args(uint32 test_mode, uint8* cfg_file_buffer, uint32 cfg_file_size, uint32* p_cur_line_num, uint8 *arg_buffer, uint32 arg_len, autotest_test_info_t * p_test_info)
{
    switch(test_mode){
        case TEST_MODE_CARD:
        {
            return act_read_testinfo_and_args_on_sd(test_mode, cfg_file_buffer, cfg_file_size, p_cur_line_num, arg_buffer, arg_len, p_test_info);
            break;
        }
        case TEST_MODE_USB:
        {
            return act_read_testinfo_and_args_on_stub(test_mode, arg_buffer, arg_len, p_test_info);
            break;
        }
        default: 
        {
            return FALSE;
            break;
        }
    }

    return FALSE;
}




