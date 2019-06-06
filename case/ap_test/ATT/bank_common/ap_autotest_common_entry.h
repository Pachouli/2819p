#ifndef __COMMON_ENTRY_H
#define __COMMON_ENTRY_H

#include "ap_test_head.h"

extern const uint32 bank_common_entry[];


enum 
{
	ENTRY_ID(act_test_start_deal) = 0,
	ENTRY_ID(act_test_get_btinfo) ,
	ENTRY_ID(act_test_modify_btinfo) ,
	ENTRY_ID(create_app_engine) ,
	ENTRY_ID(kill_current_app_engine) ,
	ENTRY_ID(act_test_stub_report_result) ,
	ENTRY_ID(act_local_test_report_result_ext) ,
	ENTRY_ID(create_log_file_name) ,
	ENTRY_ID(write_log_file) ,
	ENTRY_ID(att_test_add_result_to_sd_log_ext) ,
	ENTRY_ID(att_test_add_result_to_sd_log) ,
	ENTRY_ID(stub_cmd_print_log) ,
	ENTRY_ID(get_bank_id_of_test_id) ,
	ENTRY_ID(get_op_entry_of_test_id) ,
};


#if 1
bool act_test_get_btinfo(btengine_setinfo_t *p_btinfo);
#endif
#define GET_BT_INFO(a) 				((bool(*)(btengine_setinfo_t *p_btinfo))(bank_common_entry[ENTRY_ID(act_test_get_btinfo)]))((a))

#if 1
bool act_test_modify_btinfo(btengine_setinfo_t *p_btinfo);
#endif
#define MODIFY_BT_INFO(a) 			((bool(*)(btengine_setinfo_t *p_btinfo))(bank_common_entry[ENTRY_ID(act_test_modify_btinfo)]))((a))

#if 1
bool create_app_engine(uint8 engine_id, uint8 para);
#endif
#define CREATE_APP_ENGINE(a,b) 		((bool(*)(uint8 engine_id, uint8 para))(bank_common_entry[ENTRY_ID(create_app_engine)]))((a),(b))

#if 1
bool kill_current_app_engine(void);
#endif
#define KILL_CURRENT_APP_ENGINE() 	((bool(*)(void))(bank_common_entry[ENTRY_ID(kill_current_app_engine)]))()

#if 1
int32 act_test_stub_report_result(uint8 *write_data, uint16 payload_len);
#endif
#define STUB_REPORT_RESULT(a,b) 	((int32(*)(uint8 *write_data, uint16 payload_len))(bank_common_entry[ENTRY_ID(act_test_stub_report_result)]))((a),(b))

#if 1
void act_local_test_report_result_ext(uint8 *test_info, uint16 test_id, bool result, \
    uint32 test_mode, uint8 * log_buffer, uint32 * cur_total_len);
#endif
#define LOCAL_REPORT_RESULT_EXT(a,b,c,d,e,f) 	((void(*)(uint8 *test_info, uint16 test_id, bool result, \
    uint32 test_mode, uint8 * log_buffer, uint32 * cur_total_len))(bank_common_entry[ENTRY_ID(act_local_test_report_result_ext)]))((a),(b),(c),(d),(e),(f))

/*#if 0 
void att_load_test_items_from_atf_file(uint8* atf_file_name);
#endif
#define LOAD_TEST_ITEMS_FROM_ATF_FILE(a) 			((void(*)(uint8* atf_file_name))(bank_common_entry[ENTRY_ID(ACT_READ_TESTINFO_AND_ARGS)]))((a))
*/

#if 1
int32 act_test_start_deal(uint8 test_mode); 
#endif
#define ACT_TEST_START_DEAL(a) 			((int32(*)(uint8 test_mode))(bank_common_entry[ENTRY_ID(act_test_start_deal)]))(a)

#if 1
void create_log_file_name(uint8 *file_name, uint32 record_count);
#endif
#define CREATE_LOG_FILE_NAME(a,b) 		((void(*)(uint8 *file_name, uint32 record_count))(bank_common_entry[ENTRY_ID(create_log_file_name)]))((a),(b))

#if 1
void write_log_file(const uint8* file_name, uint8* log_buffer, uint32 write_len);
#endif
#define WRITE_LOG_FILE(a,b,c)			((void(*)(const uint8* file_name, uint8* log_buffer, uint32 write_len))(bank_common_entry[ENTRY_ID(write_log_file)]))((a),(b),(c))

#if 1
void att_test_add_result_to_sd_log_ext(uint8 *test_info, uint8* test_data, uint32 data_bytes, uint8 * log_buffer, uint32* cur_total_len);
#endif
#define ATT_TEST_ADD_RESULT_TO_SD_LOG_EXT(a,b,c,d,e)	((void(*)(uint8 *test_info, uint8* test_data, \
    uint32 data_bytes, uint8 * log_buffer, uint32* cur_total_len))(bank_common_entry[ENTRY_ID(att_test_add_result_to_sd_log_ext)]))((a),(b),(c),(d),(e))

#if 1
void att_test_add_result_to_sd_log(uint8 *test_info, bool test_result, uint8 * log_buffer, uint32* cur_total_len);
#endif
#define ATT_TEST_ADD_RESULT_TO_SD_LOG(a,b,c,d)		((void(*)(uint8 *test_info, bool test_result, \
    uint8 * log_buffer, uint32* cur_total_len))(bank_common_entry[ENTRY_ID(att_test_add_result_to_sd_log)]))((a),(b),(c),(d))

#if 1
int stub_cmd_print_log(const char* format, ...);
#endif
#define STUB_CMD_PRINT_LOG(a,...) 				((int(*)(const char* format, ...))(bank_common_entry[ENTRY_ID(stub_cmd_print_log)]))((a),##__VA_ARGS__)


#if 1
int32 get_bank_id_of_test_id(uint16 test_id, uint8* bin_id, uint8* bank_id);
#endif
#define GET_BANK_ID_OF_TEST_ID(a, b, c)			((int32(*)(uint16 test_id, uint8* bin_id, uint8* bank_id))(bank_common_entry[ENTRY_ID(get_bank_id_of_test_id)]))((a), (b), (c))


#if 1
test_func_ext_t get_op_entry_of_test_id(uint16 test_id);
#endif
#define GET_OP_ENTRY_OF_TEST_ID(a)			((test_func_ext_t(*)(uint16 test_id))(bank_common_entry[ENTRY_ID(get_op_entry_of_test_id)]))((a))


#if 0

#endif


#endif

