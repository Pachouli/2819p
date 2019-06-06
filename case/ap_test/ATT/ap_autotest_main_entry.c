#include "ap_test_head.h"

__section__(".op_entry") const uint32 ap_main_entry[]={
	(uint32)att_write_data,
	(uint32)att_read_data,
	(uint32)read_atf_file,
	(uint32)read_atf_sub_file_data,
	(uint32)check_fatal_result,
};
