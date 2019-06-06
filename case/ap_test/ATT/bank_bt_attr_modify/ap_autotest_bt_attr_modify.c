/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ��޸�������ַ���޸������豸���ơ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_include/ap_autotest_char_convert.h"
#include "ap_autotest_main_entry.h"
#include "bank_resident/ap_autotest_resident_entry.h"
#include "bank_common/ap_autotest_common_entry.h"



//����������ͨ�����õĽṹ�壬���ڴ洢��ѯ����������Ϣ�ʹ����͵�������Ϣ
//btengine_setinfo_t g_bt_info;

int32 utf8str_to_unicode(uint8 *utf8, uint32 utf8Len, uint16 *unicode, uint32 *unicode_len)
{
    int32 count = 0;
    uint8 c0, c1;
    uint32 scalar;

    while(--utf8Len >= 0)
    {
        c0 = *utf8;
        utf8++;

        if (c0 < 0x80)
        {
            *unicode = c0;

            if(*unicode == 0x00)
            {
                count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        /*��ascii�ַ�������ĵ�һ���ֽڲ�Ӧ��Ϊ10xxxxxx*/
        if ((c0 & 0xc0) == 0x80)
        {
            return -1;
        }

        scalar = c0;
        if (--utf8Len < 0)
        {
            return -1;
        }

        c1 = *utf8;
        utf8++;
        /*��2���ֽڵı����ʽӦ��Ϊ10xxxxxx*/
        if ((c1 & 0xc0) != 0x80)
        {
            return -1;
        }
        scalar <<= 6;
        scalar |= (c1 & 0x3f);

        /*���r0 & 0x20����0��˵����utf���볤��Ϊ2����������볤�ȴ���2*/
        if (!(c0 & 0x20))
        {
            if ((scalar != 0) && (scalar < 0x80))
            {
                /*unicode����С��0x80��utf8���볤��ȴΪ2��over encoding*/
                return -1;
            }
            *unicode = (unsigned short)scalar & 0x7ff;
            if(*unicode == 0x00)
            {
                count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        if (--utf8Len < 0)
        {
            return -1;
        }

        c1 = *utf8;
        utf8++;
        /*��3���ֽڵı����ʽӦ��Ϊ10xxxxxx*/
        if ((c1 & 0xc0) != 0x80)
        {
            return -1;
        }
        scalar <<= 6;
        scalar |= (c1 & 0x3f);

        /*���r0 & 0x10����0��˵����utf���볤��Ϊ3����������볤�ȴ���3*/
        if (!(c0 & 0x10))
        {
            if (scalar < 0x800)
            {
                return -1;
            }
            if ((scalar >= 0xd800) && (scalar < 0xe000))
            {
                return -1;
            }
            *unicode = (unsigned short)scalar & 0xffff;
            if(*unicode == 0x00)
            {
                count += 2;
                break;
            }
            unicode++;
            count += 2;
            continue;
        }

        return -1;
    }

    //��֤�ַ������ֽڶ���
    if((count % 4) != 0)
    {
        unicode++;
        count+= 2;
        *unicode = 0x00;
    }

    *unicode_len = count;
    return 0;
}

int act_test_report_modify_bt_addr_result_ext(bt_addr_arg_t *bt_addr_arg, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    int ret_val;
    return_result_t *return_data;
    uint16 trans_bytes;

    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(btengine_setinfo_t));

    GET_BT_INFO(&bt_info);
    com_close_bt_engine();

    if(libc_memcmp(bt_addr_arg->bt_addr, bt_info.bt_addr, 6) != 0)
    {
        ret_val = FALSE;
    }
    else
    {
        ret_val = TRUE;
    }

    if(test_mode != TEST_MODE_CARD)
    {
        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer){
            libc_printf("report_modify_bt_addr: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *)(stub_rw_buffer);

        return_data->test_id = TESTID_MODIFY_BTADDR;

        return_data->test_result = ret_val;

        //ע��trans_bytes��uint16���ͣ�������unicode����
        trans_bytes = 0;

        //������ַתunicode��ʽ
        bytes_to_unicode(bt_info.bt_addr, 5, 6, return_data->return_arg, &trans_bytes);

        //��Ӳ����ָ���','
        return_data->return_arg[trans_bytes++] = 0x002c;

        bytes_to_unicode(&(bt_addr_arg->bt_addr_add_mode), 0, 1, &(return_data->return_arg[trans_bytes]), &trans_bytes);

        //��Ӳ����ָ���','
        return_data->return_arg[trans_bytes++] = 0x002c;

        bytes_to_unicode(&(bt_addr_arg->bt_burn_mode), 0, 1, &(return_data->return_arg[trans_bytes]), &trans_bytes);

        //��ӽ�����
        return_data->return_arg[trans_bytes++] = 0x0000;

        //�������δ���ֽڶ��룬Ҫ���ֽڶ��봦��
        if((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }

        STUB_REPORT_RESULT((uint8*)return_data, trans_bytes*2 + 4);

        sys_free(stub_rw_buffer);
    }
    else
    {   
        ATT_TEST_ADD_RESULT_TO_SD_LOG_EXT("bt address:", bt_info.bt_addr, 6, log_buffer, cur_log_len);
        
        ATT_TEST_ADD_RESULT_TO_SD_LOG("modify bt addr", ret_val, log_buffer, cur_log_len);

        WRITE_ATT_RECORD_FILE(bt_addr_arg->bt_addr, 0, bt_addr_arg->bt_addr_add_mode);
    }

    return ret_val;
}


int32 act_test_modify_bt_addr_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    int ret_val;
    bt_addr_arg_t *bt_addr_arg = (bt_addr_arg_t *)arg_buffer;

    btengine_setinfo_t bt_info;
    libc_memcpy(bt_info.bt_addr, bt_addr_arg->bt_addr, 6);
    bt_info.bt_addr_type = bt_addr_arg->bt_addr_add_mode;
    bt_info.force_write = bt_addr_arg->bt_burn_mode;
    bt_info.modify_mode = BT_MODIFY_ADDR;

    ret_val = MODIFY_BT_INFO(&bt_info);

    ret_val = act_test_report_modify_bt_addr_result_ext(bt_addr_arg, test_mode, log_buffer, cur_log_len);

    return ret_val;
}


int act_test_report_modify_bt_name_result_ext(uint8 *bt_name_arg, uint16 bt_name_arg_len, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{   
    int ret_val = TRUE;
    return_result_t *return_data;
    uint32 trans_bytes = 0;

    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(btengine_setinfo_t));
    GET_BT_INFO(&bt_info);

    com_close_bt_engine();

    int32 bt_name_cur_len = libc_strlen(bt_info.bt_name);
    if( bt_name_cur_len != bt_name_arg_len){
        libc_printf("fail to modify bt name. bt_name_cur_len:%d bt_name_arg_len:%d\n", bt_name_cur_len, bt_name_arg_len);
        ret_val = FALSE;
    }else if(libc_strncmp(bt_info.bt_name, bt_name_arg, bt_name_arg_len) != 0){
        libc_printf("fail to modify bt name. bt_name_cur:%s bt_name_arg:%s\n", bt_info.bt_name, bt_name_arg);
        ret_val = FALSE;
    }

    if(test_mode != TEST_MODE_CARD)
    {   
        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer){
            libc_printf("modify_bt_name: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *)(stub_rw_buffer);

        return_data->test_id = TESTID_MODIFY_BTNAME;

        return_data->test_result = ret_val;

        //utf8תunicode���������buffer�϶࣬�����������֮ǰ�ַ���buffer��2��
        //�����Ҫһ����һ���buffer�����ַ���
        utf8str_to_unicode(bt_info.bt_name, sizeof(bt_info.bt_name), return_data->return_arg, &trans_bytes);

        STUB_REPORT_RESULT((uint8*)return_data, trans_bytes + 4);

        sys_free(stub_rw_buffer);
    }
    else
    {
        ATT_TEST_ADD_RESULT_TO_SD_LOG("modify bt name", ret_val, log_buffer, cur_log_len);
    }

    return ret_val;
}


int32 act_test_modify_bt_name_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    int ret_val;
    uint32 name_length;
    name_length = libc_strlen(arg_buffer);
    
    btengine_setinfo_t bt_info;
    bt_info.bt_name_len = name_length;
    libc_strncpy(bt_info.bt_name, arg_buffer, name_length + 1);
    bt_info.force_write = 0;
    bt_info.modify_mode = BT_MODIFY_NAME;

    ret_val = MODIFY_BT_INFO(&bt_info);
    
    ret_val = act_test_report_modify_bt_name_result_ext((uint8*)arg_buffer, name_length, test_mode, log_buffer, cur_log_len);
    
    return ret_val;
}





