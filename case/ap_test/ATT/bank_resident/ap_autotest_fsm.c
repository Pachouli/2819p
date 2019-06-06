/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：自动化测试流程控制模块。
 * 作者：zhouxl
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
    获取下一项测试项，并执行测试处理
参数说明：
    test_mode: 当前测试模式
    atf_head:  存储atf文件头，提供加载bank所需要的信息
    cur_line_num: 在sd卡模式下有效，表示要从config.txt中的第几行读取测试项id和参数
    log_write_len: 记录写入到log缓冲中的日志长度
返回：
    TRUE:  表示当前测试项测试完成，将继续执行下一个测试项
    FALSE: 表示所有测试项都已完成或当前测试项遇到错误，此时，将不再继续进行下一项测试。
*/
bool test_loop(uint8 test_mode, const uint8* atf_file_name, atf_head_t * atf_head, buffer_info_t* buffer_info, \
    const uint8* log_file_name, uint32 *log_write_len)
{
#define ATT_CMD_ARGS_BUFFER_SIZE 80

    int ret_val;
    bool b_test_res = TRUE;

    uint32 cur_line_num = 1;        //测试项列表中当前待读取的行数，每读取一个测试项，该值+1

    test_func_ext_t test_item_op_entry = NULL;

    autotest_test_info_t test_info;                             //存储当前测试项基本信息，包括测试项id和对应参数长度等
    //uint8 att_cmd_args_buffer[ATT_CMD_ARGS_BUFFER_SIZE];      //存储当前命令id所携带的参数，是经过解析后的结构化参数
    uint8 * att_cmd_args_buffer = sys_malloc(ATT_CMD_ARGS_BUFFER_SIZE);
    if (NULL == att_cmd_args_buffer)
    {
        libc_printf("att_cmd_args_buffer null\n");
        while(1){}
    }

    while(1){
        //加载bank_args_read.bin以支持参数读取和解析，注意该bank与 bank_common冲突，之后要重新加载bank_common
        ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_ARGS_READ, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
        CHECK_FATAL_RESULT(ret_val, "load bkargs.bin error.");

        //读取测试项基本信息和args信息，args为结构化转换后的参数信息
        if ( !ACT_READ_TESTINFO_AND_ARGS(test_mode, buffer_info->cfg_file_buffer, buffer_info->cfg_file_length, &cur_line_num, att_cmd_args_buffer, ATT_CMD_ARGS_BUFFER_SIZE, &test_info) )
        {   
            libc_printf("read args error!\n");
            b_test_res = FALSE;
            break;
        }

        //重新加载bank_common.bin以支持辅助功能
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

        //加载子测试项bank
        ret_val = LOAD_BANK_ON_TEST_ITEM(bin_id, bank_id, test_mode, buffer_info->bank_test_item_buffer[bin_id], buffer_info->bank_test_item_buffer_size[bin_id], \
            atf_head, atf_file_name, buffer_info->bank_test_item_buffer_size[bin_id]);
        CHECK_FATAL_RESULT(ret_val, "load bank error.\n");

        //调用测试项处理函数
        if ( (*test_item_op_entry)(att_cmd_args_buffer, test_mode, buffer_info->log_file_buffer, log_write_len) == FALSE){
            libc_printf("test id:%d test fail\n", test_info.test_id);
            b_test_res = FALSE;
            break;
        }else{
            libc_printf("test id:%d test ok\n", test_info.test_id);
        }

        //sd卡模式下，如果log缓冲较大(超过log缓冲的一半)，则及时刷新log到sd卡文件，避免产生缓冲溢出
        //注意，不要在一个测试项内往缓冲区写太多数据
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
    //att测试成功与否结果标记
    bool att_ok_flag = TRUE;
    
    //libc_printf("test ap running...\n", 0, 0);
    
    int ret_val;
      
    int32  record_count = 0;        //保存已完成测试的数量，每完成一个小机测试，该值+1，并保存到record文件中

    uint32 log_write_len = 0;       // 当前log缓冲中日志数据的长度
    //uint8  log_file_name[52];       // log 文件名
    uint8 * log_file_name = sys_malloc(52);
    if (NULL == log_file_name)
    {
        libc_printf("log_file_name null!\n");
        while(1){}
    }
    libc_memset(log_file_name, 0, 52);

    //加载bank_commmon模块已支持接下来的功能调用
    ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_COMMON, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
    CHECK_FATAL_RESULT(ret_val, "load bkcom.bin error.");

//    stub_ioctrl_set(SET_TIMEOUT, 500, 0);
    if (ACT_TEST_START_DEAL(test_mode) < 0){
        libc_printf("ACT_TEST_START_DEAL error!\n");
        while(1){}
    }

    //sd卡模式，需要考虑写record和log文件
    if (TEST_MODE_CARD == test_mode)
    {   
        //修改record.bin文件，将已测试数量+1
        record_count = ATT_TEST_READ_COUNT_FROM_RECORD_FILE();
        WRITE_ATT_RECORD_FILE(0, record_count+1, TEST_RECORD_TESTCOUNT);

        //创建log file文件名
        CREATE_LOG_FILE_NAME(log_file_name, record_count);
    }

    att_ok_flag = test_loop(test_mode, atf_file_name, atf_head, buffer_info, log_file_name, &log_write_len);

    //sd卡模式，写log文件
    if (TEST_MODE_CARD == test_mode)
    {   
        //此处要保证加载bank_common.bin，不过从逻辑上看，当test_loop退出时，BASE_FUNC内存区仍然加载了bank_common代码，否则需要重新加载该bank
        //ret_val = LOAD_BANK_ON_BASE_FUNC(BASE_FUNC_BANK_ID_COMMON, test_mode, buffer_info->bank_base_func_buffer, buffer_info->bank_base_func_buffer_size, atf_head, atf_file_name);
        //CHECK_FATAL_RESULT(ret_val, "load bkcom.bin error.");

        WRITE_LOG_FILE(log_file_name, buffer_info->log_file_buffer, log_write_len);
    }   

    sys_free(log_file_name);

    test_exit(test_mode, att_ok_flag);
   
}

#endif


