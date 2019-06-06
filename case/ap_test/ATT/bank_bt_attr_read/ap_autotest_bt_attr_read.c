/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ���ȡ������ַ���ԣ���ȡ�����豸���Ʋ��ԡ�
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_include/ap_autotest_char_convert.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"


//�ò��������ڶ�ȡ������ַ���ṩ��PC���߽���У�飬ȷ�ϵ�ַ�Ƿ���ȷ
int act_test_read_bt_addr_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    return_result_t *return_data;
    uint16 trans_bytes;

    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(btengine_setinfo_t));
    GET_BT_INFO(&bt_info);
    //�˳�������̨
    com_close_bt_engine();

    if(test_mode != TEST_MODE_CARD)
    {
        STUB_CMD_PRINT_LOG("bt addr: %x:%x:%x:%x:%x:%x", bt_info.bt_addr[5], bt_info.bt_addr[4], 
            bt_info.bt_addr[3], bt_info.bt_addr[2], bt_info.bt_addr[1], bt_info.bt_addr[0]);

        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer){
            libc_printf("read_bt_addr_ext: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *)(stub_rw_buffer);

        return_data->test_id = TESTID_READ_BTADDR;

        return_data->test_result = 1;

        trans_bytes = 0;

        //������ַתunicode��ʽ
        bytes_to_unicode(bt_info.bt_addr, 5, 6, return_data->return_arg, (uint16*)&trans_bytes);

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
        ATT_TEST_ADD_RESULT_TO_SD_LOG("read bt addr", TRUE, log_buffer, cur_log_len);
    }

    return TRUE;
}

int act_test_read_bt_name_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    uint32 ret_val = TRUE;
    return_result_t *return_data;
    read_btname_test_arg_t *read_btname_arg;

    read_btname_arg = (read_btname_test_arg_t *) arg_buffer;

    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(btengine_setinfo_t));
    GET_BT_INFO(&bt_info);
    //�˳�������̨
    com_close_bt_engine();

    if (read_btname_arg->cmp_btname_flag == TRUE)
    {
        if(0 != libc_strncmp(bt_info.bt_name, read_btname_arg->cmp_btname, TEST_BTNAME_MAX_LEN))
        {
            ret_val = FALSE;
        }
    }

    if (test_mode != TEST_MODE_CARD)
    {
        STUB_CMD_PRINT_LOG("read bt name: %s", bt_info.bt_name);

        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer){
            libc_printf("read_bt_name_ext: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *) (stub_rw_buffer);

        return_data->test_id = TESTID_READ_BTNAME;

        return_data->test_result = ret_val;

        STUB_REPORT_RESULT((uint8*)return_data, 4);

        sys_free(stub_rw_buffer);
    }
    else
    {   
        ATT_TEST_ADD_RESULT_TO_SD_LOG("read bt name", ret_val, log_buffer, cur_log_len);
    }

    return ret_val;
}



