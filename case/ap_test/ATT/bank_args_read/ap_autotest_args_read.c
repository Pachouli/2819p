/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ��Զ������Բ����������ȡ�ӿ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"
#include "bank_resident/ap_autotest_resident_entry.h"

//���ݲ������кţ���λ���������ʼλ��
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

        //����������־ 00 0d 00 0a
        while (*cur != 0x000a)
        {
            cur++;
        }

        while ((*start != 0x003d) && (start != cur))
        {
            start++;
        }

        //������'='��˵������Ч�������У�������˸�������
        if(*start == 0x003d)
        {
            cur_line++;
        }

        //������ָ���������˳�
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

    //ĩβ��ӽ�����
    *utf8_buffer = 0;

    return TRUE;
}

/*
    ��unicode����ת����uint8���飬ר������������������ȡ������ַ
����˵��:
    start,end:      ��ת����unicode����ʼĩλ��
    byte_buffer:    ������uint8����λ�ã�һ����������ַ���׵�ַ
    byte_index:     �����byte_buffer������λ��
    byte_len:       ����ȡ�ĳ���
������
    ��unicode����Ĳ����������ζ�ȡ�����Ӹߵ��͵�˳����䵽byte_buffer����byte_buffer��������ַ��λ�ڸ�����(�ߵ�ַ)����λ�ڵ�����(�͵�ַ)
    ����unicode��0���ַ���䵽byte_buffer��5���ֽڣ�
        unicode��5���ַ���䵽byte_buffer��0���ֽ�
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

//�ڵ�ǰ����ȡtest id
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

    //test id����ʮ���Ƶ��ַ���
    test_id = unicode_to_int(start, cur, 10);

    return test_id;
}


/*
    ������ǰ�У���ȡָ��������ʼĩλ��
    1. sd����stubЭ���ȡ�Ĳ�������unicode����
*/
 uint8 *locate_para_from_test_item_line(uint8 test_mode, uint16 *line_buffer, uint8 arg_number, uint16 **start, uint16 **end)
{
    uint16 *cur;
    //uint32 test_id;
    uint8 cur_arg_num;

    cur = line_buffer;

    //��ģʽ��Ҫ���˰���'='��ǰ���ַ�
    //USBģʽֱ�ӽ�ȡ�ַ�������
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

        //���Ա�ǵ�ǰ�����Ƿ������һ������
        bool b_cur_is_last_arg = FALSE;

        //����','��������һ������������0x0d0a��ʾһ�н���
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
            //��ǰ�Ѷ������һ��������������������
            break;
        }else{
            //����','
            cur++;
        }
    }

    //��������������㣬��ѭ���˳�ʱ����ǰ��������Ԥ�ڲ�������ǰ������Ĳ�������
    if (cur_arg_num != arg_number){
        //��������
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

    //��һ������Ϊ������ַ��3�ֽ�
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

    //��һ������Ϊ������ַ��3�ֽ�
    locate_para_from_test_item_line(test_mode, line_buffer, 1, &start, &end);

    //��ȡbyte5, byte4, byte3���ݣ�һ��Ϊ������ַ�̶��ֽ�
    unicode_to_uint8_bytes(start, end, bt_addr_arg->bt_addr, 5, 3);

    //���ڿ�����ģʽ�����ȴӴ�����bt_addr.log�ļ���ȡ������ַ�����û��
    //�ٴ������ļ���ȡ
    if(test_mode == TEST_MODE_CARD)
    {
        if(READ_BT_ADDR_FROM_RECORD_FILE(bt_addr_arg->bt_addr) == FALSE)
        {   
            //�ڶ�������Ϊ������ַ��3�ֽ�
            locate_para_from_test_item_line(test_mode, line_buffer, 2, &start, &end);

            //��ȡbyte2, byte1, byte0���ݣ�һ��Ϊ���޸ĵ��ֽ�
            unicode_to_uint8_bytes(start, end, bt_addr_arg->bt_addr, 2, 3);
        }
    }
    else
    {
        //�ǿ����Է�����ֱ�ӽ���pc������������
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

    //��ȡ6�ֽڵ�������ַ
    unicode_to_uint8_bytes(start, end, btplay_arg->bt_transmitter_addr, 5, 6);

    //������ģʽ��Ҫ�Թ������5��������ַ������ȡ���һ������ģʽ����
    //PC����ģʽ��������PC���䣬����Ҫ���д���
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

    //��ȡ����ģʽ
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

/* gpio������������� */
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

/* ͨ�����Բ������� */
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
    ����test_id������stubЭ���е������������
����˵��:
    line_buffer:    stubЭ���еĸ����������ݣ�����Ϊ�������
    arg_buffer:     ���ڴ洢������Ľṹ�������Ļ�����
    arg_len:        ����������
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

    //�ļ�������ϣ��������
    if(line_buffer == 0)
    {
        *test_id = TESTID_ID_QUIT;
        return TRUE;
    }

    *test_id = (uint16)act_test_read_test_id_from_line(line_buffer);

    return act_test_read_test_arg(test_mode, *test_id, line_buffer, arg_buffer, arg_len);
}


/*
    ��ȡ�µĲ�������Ϣ�����Բ������нṹ������
����˵��:
    arg_buffer/arg_len: ���ڷ��ص�ǰ������Ľ�����Ľṹ������
    p_test_info:        �洢��ǰ�����������Ϣ
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

    //���Ͳ�������Ϣ�������ݰ�
    cmd_data = (uint8*)stub_rw_buffer;
    //�ó�ʱʱ������Ϊ��λ��ȡ�������������ʹ�õ�ʱ��
    cmd_data[6] = ATT_ITEM_TEST_TIMEOUT;    // �����ʱ
    cmd_data[7] = 0;
    cmd_data[8] = 0;
    cmd_data[9] = 0;

    if (ATT_WRITE_DATA(STUB_CMD_ATT_GET_TEST_ID, 4, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }

    //��ȡ��������Ϣ��
    if (ATT_READ_DATA((uint8*)stub_rw_buffer, 4) != 0){
        goto read_error;
    }

    cmd_data = (uint8 *)stub_rw_buffer;
    p_test_info->test_id = (cmd_data[6] | (cmd_data[7] << 8));
    p_test_info->arg_len = (cmd_data[8] | (cmd_data[9] << 8));

    //�ظ�ACK
    if (ATT_WRITE_DATA(STUB_CMD_ATT_ACK, 0, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }

    libc_printf("read_testinfo_stub testid:%d raw_arg_len:%d\n", p_test_info->test_id,p_test_info->arg_len);

    if(p_test_info->arg_len == 0){
        goto read_success;
    }
    
    //���ͻ�ȡ������Ϣ����
    if (ATT_WRITE_DATA(STUB_CMD_ATT_GET_TEST_ARG, 0, (uint8*)stub_rw_buffer) != 0){
        goto read_error;
    }
   
    //��ȡ������Ϣ
    if (ATT_READ_DATA((uint8*)stub_rw_buffer, p_test_info->arg_len) != 0){
        goto read_error;
    }
    
    //����ʹ��STUB_ATT_RW_TEMP_BUFFER����Ϊ��������Ҫʹ�ø�buffer��������
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
    ��ȡ�µĲ�������Ϣ�����Բ������нṹ������
����˵��:
    cfg_file_buffer:    config.txt�������
    p_cur_line_num:     ��ǰ��ȡ�Ĳ������к�
    arg_buffer/arg_len: ���ڷ��ص�ǰ������Ľ�����Ľṹ������
    p_test_info:        �洢��ǰ�����������Ϣ
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
    ��ȡ�����������Ϣ�Ͳ���
����:
    test_mode:        ��ǰ����ģʽ������ѡ��ͬ�Ĳ���������ʽ
    cfg_file_buffer:  config.txt�����ļ����ݻ���
    cfg_file_size:    �����ļ���С
    p_cur_line_num:   config.txt��ÿ��һ��������,��ֵָʾ��ǰ��������������
    arg_buffer:       ���ؽṹ������ֵ
    arg_len:          ���������С����ֹ���
    p_test_info:      ���ز��Ի�����Ϣ
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




