#include "ap_test_head.h"
#include "ap_autotest_common_entry.h"

__section__(".op_entry") const uint32 bank_common_entry[]={
	(uint32)act_test_start_deal,
	(uint32)act_test_get_btinfo,
	(uint32)act_test_modify_btinfo,
	(uint32)create_app_engine,
	(uint32)kill_current_app_engine,
	(uint32)act_test_stub_report_result,
	(uint32)act_local_test_report_result_ext,
	(uint32)create_log_file_name,
	(uint32)write_log_file,
	(uint32)att_test_add_result_to_sd_log_ext,
	(uint32)att_test_add_result_to_sd_log,
	(uint32)stub_cmd_print_log,
	(uint32)get_bank_id_of_test_id,
	(uint32)get_op_entry_of_test_id,
};
