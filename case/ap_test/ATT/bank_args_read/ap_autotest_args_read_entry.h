#ifndef __ARGS_READ_ENTRY_H
#define __ARGS_READ_ENTRY_H

#include "ap_test_head.h"

extern const uint32 bank_args_read_entry[];

enum {
	ENTRY_ID(act_read_testinfo_and_args) = 0,
};


#if 1
bool act_read_testinfo_and_args(uint32 test_mode, uint8* cfg_file_buffer, uint32 cfg_file_size, uint32* p_cur_line_num, \
    uint8 *arg_buffer, uint32 arg_len, autotest_test_info_t * p_test_info);
#endif
#define ACT_READ_TESTINFO_AND_ARGS(a,b,c,d,e,f,g)     ((bool(*)(uint32 test_mode, uint8* cfg_file_buffer, uint32 cfg_file_size, uint32* p_cur_line_num, \
    uint8 *arg_buffer, uint32 arg_len, autotest_test_info_t * p_test_info))(bank_args_read_entry[ENTRY_ID(act_read_testinfo_and_args)]))((a),(b),(c),(d),(e),(f),(g))

#if 0

#endif


#endif

