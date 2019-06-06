#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"

/*
    测试ap启动时会执行的一个函数，在stub测试模式时，一般情况下是执行启动命令，但是若是测试了固件烧录测试项，
    说明测试早已开启，并刚刚执行了固件烧录，此时将汇报测试结果，而不是重新开启测试.
*/
int32 act_test_start_deal(uint8 test_mode)
{   
    if (test_mode == TEST_MODE_CARD)  // 卡模式?
        return 0;

    int32 ret_val;
    att_start_arg_t *att_start_arg;
    att_pc_test_info_t *att_pc_test_info;
    uint32 i;
    uint16 test_id = 0;

    uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
    if (NULL == stub_rw_buffer){
        libc_printf("start_deal: stub_rw_buffer null\n");
        while(1){}
    }
    
    if (ATT_WRITE_DATA(STUB_CMD_ATT_READ_TESTINFO, 0, stub_rw_buffer) != 0){
        goto deal_error;
    }

    if (ATT_READ_DATA(stub_rw_buffer, (sizeof(pc_test_info_t))) != 0){
        goto deal_error;
    }

    att_pc_test_info = (att_pc_test_info_t *)stub_rw_buffer;
    test_id = att_pc_test_info->pc_test_info.last_test_id;
    
    if (ATT_WRITE_DATA(STUB_CMD_ATT_ACK, 0, stub_rw_buffer) != 0){
        goto deal_error;
    }

    //如果上一项是量产测试，则说明是由量产重启，直接进行下一项测试
    //测试成功不发送START命令
    if(test_id == TESTID_PRODUCT_TEST)
    {
        /* 上报量产成功结果
         */
        return_result_t *return_data = (return_result_t *)(stub_rw_buffer);

        return_data->test_id = TESTID_PRODUCT_TEST;
        return_data->test_result = 1;

        act_test_stub_report_result((uint8*)return_data, 4);

        goto deal_success;
    }

    //准备发送开启stub测试命令
    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(bt_info));
    act_test_get_btinfo(&bt_info);
    
    att_start_arg = (att_start_arg_t *)stub_rw_buffer;
    libc_memset(att_start_arg, 0, sizeof(att_start_arg_t));
    for (i = 0; i < 6; i++)
    {
        att_start_arg->bdaddr[i] = bt_info.bt_addr[5 - i];
    }
    libc_printf("act_test_get_btinfo  btname:%s  btaddr:",bt_info.bt_name);
    libc_printhex(bt_info.bt_addr,6,0);
    
    //测试通过USB线连接，PC与DUT直接连接
    att_start_arg->dut_connect_mode = DUT_CONNECT_MODE_DIRECT;
    att_start_arg->dut_work_mode = DUT_WORK_MODE_NORMAL;
    //att_start_arg->dut_work_mode = DUT_WORK_MODE_SPECIAL;

    att_start_arg->timeout  = 5;  // 获取测试项超时
    att_start_arg->reserved = 0;

    if (ATT_WRITE_DATA(STUB_CMD_ATT_START, 32, (uint8*)att_start_arg) != 0)
    {
        goto deal_error;
    }
    if (ATT_READ_DATA(stub_rw_buffer, 0) != 0)
    {
        goto deal_error;
    }

deal_success:
    sys_free(stub_rw_buffer);
    return 0;

deal_error:
    if (stub_rw_buffer != NULL){
        sys_free(stub_rw_buffer);        
    }
    return -1;
}

/*int32 att_fw_swtch_deal(void)
{
    int32 ret_val;
    att_swtich_fw_arg_t *att_switch_fw_arg;

    libc_memset((uint8 *)STUB_ATT_RW_TEMP_BUFFER, 0, 40);

    att_switch_fw_arg = (att_swtich_fw_arg_t *)STUB_ATT_RW_TEMP_BUFFER;

    att_switch_fw_arg->reboot_timeout = 45;

    ret_val = ATT_WRITE_DATA(STUB_CMD_ATT_SWTICH_FW, 32, (uint32)STUB_ATT_RW_TEMP_BUFFER);

    if(ret_val == 0)
    {
        ret_val = ATT_READ_DATA((uint32)STUB_ATT_RW_TEMP_BUFFER, 0);

        return TRUE;
    }

    return FALSE;
}*/

