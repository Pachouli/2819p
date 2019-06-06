#include "ap_test_head.h"
#include "ap_autotest_bt_attr_read_entry.h"

__section__(".op_entry") const uint32 bank_bt_attr_read_entry[]={
	(uint32)act_test_read_bt_addr_ext,
	(uint32)act_test_read_bt_name_ext,
};

