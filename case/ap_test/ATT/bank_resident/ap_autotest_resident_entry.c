#include "ap_test_head.h"
#include "ap_autotest_resident_entry.h"

__section__(".op_entry") const uint32 bank_resident_entry[]={
	(uint32)act_test_read_bt_addr_from_record_file,
	(uint32)att_test_read_count_from_record_file,
	(uint32)act_test_write_att_record_file,
	(uint32)load_bank_on_base_func,
	(uint32)load_bank_on_test_item,
	(uint32)test_enter,

};

