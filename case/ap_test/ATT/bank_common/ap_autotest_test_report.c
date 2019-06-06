/*
    stub测试结果上报借口
*/


#include "ap_test_head.h"

/*
    向PC端汇报某个测试项的测试结果信息，write_data中需事先填充负载数据内容
*/
int32 act_test_stub_report_result(uint8 *write_data, uint16 payload_len)
{
    int ret_val;
    uint8 cmd_data[8];

    stub_ext_param_t ext_param;

    ((return_result_t*)write_data)->timeout = 5;  // 获取测试项超时

    while(1)
    {
        ext_param.opcode = STUB_CMD_ATT_REPORT_TRESULT;
        ext_param.payload_len = payload_len;
        ext_param.rw_buffer = write_data;

        ret_val = stub_ext_write(&ext_param);

        if(ret_val == 0)
        {
            ext_param.payload_len = 0;
            ext_param.rw_buffer = cmd_data;

            ret_val = stub_ext_read(&ext_param);

            if(ret_val == 0)
            {
                //确认是ACK数据
                if (cmd_data[1] == 0x04 && cmd_data[2] == 0xfe)
                {
                    break;
                }
                else
                {
                    stub_ioctrl_set(RESET_FIFO, 0 , 0);
                    stub_ioctrl_set(RESET_FIFO, 1 , 0);
                }
            }
        }
    }

    return ret_val;
}



/*
    汇报某个测试项的测试结果信息，与act_test_report_result功能类似；
    但act_local_test_report_result会根据当前模式判断如何汇报测试结果：
    1. 若为sd卡模式测试，则将测试结果保存到sd卡
    2. 若为usb模式测试， 则将测试结果提交给pc端 
*/
 void act_local_test_report_result_ext(uint8 *test_info, uint16 test_id, bool result, \
    uint32 test_mode, uint8 * log_buffer, uint32 * cur_total_len)
{   
    uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
    if (NULL == stub_rw_buffer){
        libc_printf("local_test_report: stub_rw_buffer null\n");
        while(1){}
    }

    return_result_t *return_data;

    if (test_mode != TEST_MODE_CARD)
    {
        return_data = (return_result_t *) (stub_rw_buffer);
        return_data->test_id = test_id;
        return_data->test_result = result;

        act_test_stub_report_result((uint8*)return_data, 4);
    }
    else
    {
        att_test_add_result_to_sd_log(test_info, result, log_buffer, cur_total_len);
    }

    sys_free(stub_rw_buffer);
}
