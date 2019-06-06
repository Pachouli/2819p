/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ��Զ����������̿���ģ�顣
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_common/ap_autotest_common_entry.h"
#include "bank_args_read/ap_autotest_args_read_entry.h"
#include "bank_resident/ap_autotest_resident_entry.h"
#include "ap_autotest_main_entry.h"


/******************************************************************************/
/*!
 * \par  Description:
 *    Ap test init
 * \param[in]   att_para    att enter mode: DISK_C: ATT tools(USB); DISK_H:card test
 * \param[out]   none
 * \return       none
 * \retval           none
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
*******************************************************************************/

#if 1

void sys_reboot(void)
{
    libc_printf("please reboot...", 0, 0);

    //ENABLE_WATCH_DOG;

    while(1);
}

#define KT_MODE_TEST_PASS  ((KT_FREQ_1000HZ<<KT_FREQ_SHIFT) | (KT_PERIOD_48<<KT_PERIOD_SHIFT) | KT_BLOCK_YES)
#define KT_MODE_TEST_FAIL  ((KT_FREQ_250HZ<<KT_FREQ_SHIFT) | (KT_PERIOD_80<<KT_PERIOD_SHIFT) | KT_BLOCK_YES)

void test_exit(uint8 test_mode, bool att_ok_flag)
{
    if (TEST_MODE_CARD == test_mode)
    {
        set_dac_rate(44);
        if (att_ok_flag == TRUE)
        {
            keytone_play (KT_MODE_TEST_PASS);
        }
        else
        {
            keytone_play (KT_MODE_TEST_FAIL);
        }
    }else {
        if ( !att_ok_flag ){
            while(1){}
        }
    }

    libc_printf("test ap end...\n", 0, 0);

    sys_reboot();
}


/*
    ��ȡ��һ��������ִ�в��Դ���
����˵����
    test_mode: ��ǰ����ģʽ
    atf_head:  �洢atf�ļ�ͷ���ṩ����bank����Ҫ����Ϣ
    cur_line_num: ��sd��ģʽ����Ч����ʾҪ��config.txt�еĵڼ��ж�ȡ������id�Ͳ���
    log_write_len: ��¼д�뵽log�����е���־����
���أ�
    TRUE:  ��ʾ��ǰ�����������ɣ�������ִ����һ��������
    FALSE: ��ʾ���в��������ɻ�ǰ�������������󣬴�ʱ�������ټ���������һ����ԡ�
*/
bool test_loop(uint8 test_mode, const uint8* atf_file_name, atf_head_t * atf_head, buffer_info_t* buffer_info, \
    const uint8* log_file_name, uint32 *log_write_len)
{
#define ATT_CMD_ARGS_BUFFER_SIZE 80

    int ret_val;
    bool b_test_res = TRUE;

    uint32 cur_line_num = 1;        //�������б��е�ǰ����ȡ��������ÿ��ȡһ���������ֵ+1

    test_func_ext_t test_item_op_entry = NULL;

    autotest_test_info_t test_info;                             //�洢��ǰ�����������Ϣ������������id�Ͷ�Ӧ�������ȵ�
    //uint8 att_cmd_args_buffer[ATT_CMD_ARGS_BUFFER_SIZE];      //�洢��ǰ����id��Я���Ĳ������Ǿ���������Ľṹ������
    uint8 * att_cmd_args_buffer = sys_malloc(ATT_CMD_ARGS_BUFFER_SIZE);
    if (NULL == att_cmd_args_buffer)
    {
        libc_printf("att_cmd_args_buffer null\n");
        while(1){}
    }

    while(1){
        //����bank_args_read.bin��֧�ֲ�����ȡ�ͽ�����ע���bank�� bank_common��ͻ��֮��Ҫ���¼���bank_common
        ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_ARGS_READ, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
        CHECK_FATAL_RESULT(ret_val, "load bkargs.bin error.");

        //��ȡ�����������Ϣ��args��Ϣ��argsΪ�ṹ��ת����Ĳ�����Ϣ
        if ( !ACT_READ_TESTINFO_AND_ARGS(test_mode, buffer_info->cfg_file_buffer, buffer_info->cfg_file_length, &cur_line_num, att_cmd_args_buffer, ATT_CMD_ARGS_BUFFER_SIZE, &test_info) )
        {   
            libc_printf("read args error!\n");
            b_test_res = FALSE;
            break;
        }

        //���¼���bank_common.bin��֧�ָ�������
        ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_COMMON, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
        CHECK_FATAL_RESULT(ret_val, "load bkcom.bin error.");

        if(test_info.test_id == TESTID_ID_WAIT)
        {
            sys_mdelay(500);
            continue;
        }

        if(test_info.test_id == TESTID_ID_QUIT)
        {
            break;
        }
        
        uint8 bank_id;
        uint8 bin_id;
        ret_val = GET_BANK_ID_OF_TEST_ID(test_info.test_id, &bin_id, &bank_id);
        CHECK_FATAL_RESULT(ret_val, "unknown test_id.\n");

        test_item_op_entry = GET_OP_ENTRY_OF_TEST_ID(test_info.test_id);
        if ( NULL == test_item_op_entry ){
            libc_printf("not find test_item_op_entry of test_id:%d\n",test_info.test_id);
            b_test_res = FALSE;
            break;
        }

        //�����Ӳ�����bank
        ret_val = LOAD_BANK_ON_TEST_ITEM(bin_id, bank_id, test_mode, buffer_info->bank_test_item_buffer[bin_id], buffer_info->bank_test_item_buffer_size[bin_id], \
            atf_head, atf_file_name, buffer_info->bank_test_item_buffer_size[bin_id]);
        CHECK_FATAL_RESULT(ret_val, "load bank error.\n");

        //���ò��������
        if ( (*test_item_op_entry)(att_cmd_args_buffer, test_mode, buffer_info->log_file_buffer, log_write_len) == FALSE){
            libc_printf("test id:%d test fail\n", test_info.test_id);
            b_test_res = FALSE;
            break;
        }else{
            libc_printf("test id:%d test ok\n", test_info.test_id);
        }

        //sd��ģʽ�£����log����ϴ�(����log�����һ��)����ʱˢ��log��sd���ļ�����������������
        //ע�⣬��Ҫ��һ������������������д̫������
        if (TEST_MODE_CARD == test_mode && *log_write_len >= (buffer_info->log_file_buffer_size/2) )
        {
            WRITE_LOG_FILE(log_file_name, buffer_info->log_file_buffer, *log_write_len);
            *log_write_len = 0;
        }
    }

    sys_free(att_cmd_args_buffer);

    return b_test_res;
}

void test_enter(uint8 test_mode, const uint8* atf_file_name, atf_head_t * atf_head, buffer_info_t* buffer_info)
{
    //att���Գɹ���������
    bool att_ok_flag = TRUE;
    
    //libc_printf("test ap running...\n", 0, 0);
    
    int ret_val;
      
    int32  record_count = 0;        //��������ɲ��Ե�������ÿ���һ��С�����ԣ���ֵ+1�������浽record�ļ���

    uint32 log_write_len = 0;       // ��ǰlog��������־���ݵĳ���
    //uint8  log_file_name[52];       // log �ļ���
    uint8 * log_file_name = sys_malloc(52);
    if (NULL == log_file_name)
    {
        libc_printf("log_file_name null!\n");
        while(1){}
    }
    libc_memset(log_file_name, 0, 52);

    //����bank_commmonģ����֧�ֽ������Ĺ��ܵ���
    ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_COMMON, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
    CHECK_FATAL_RESULT(ret_val, "load bkcom.bin error.");

//    stub_ioctrl_set(SET_TIMEOUT, 500, 0);
    if (ACT_TEST_START_DEAL(test_mode) < 0){
        libc_printf("ACT_TEST_START_DEAL error!\n");
        while(1){}
    }

    //sd��ģʽ����Ҫ����дrecord��log�ļ�
    if (TEST_MODE_CARD == test_mode)
    {   
        //�޸�record.bin�ļ������Ѳ�������+1
        record_count = ATT_TEST_READ_COUNT_FROM_RECORD_FILE();
        WRITE_ATT_RECORD_FILE(0, record_count+1, TEST_RECORD_TESTCOUNT);

        //����log file�ļ���
        CREATE_LOG_FILE_NAME(log_file_name, record_count);
    }

    att_ok_flag = test_loop(test_mode, atf_file_name, atf_head, buffer_info, log_file_name, &log_write_len);

    //sd��ģʽ��дlog�ļ�
    if (TEST_MODE_CARD == test_mode)
    {   
        //�˴�Ҫ��֤����bank_common.bin���������߼��Ͽ�����test_loop�˳�ʱ��BASE_FUNC�ڴ�����Ȼ������bank_common���룬������Ҫ���¼��ظ�bank
        //ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_COMMON, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
        //CHECK_FATAL_RESULT(ret_val, "load bkcom.bin error.");

        WRITE_LOG_FILE(log_file_name, buffer_info->log_file_buffer, log_write_len);
    }   

    sys_free(log_file_name);

    test_exit(test_mode, att_ok_flag);
   
}

#endif


