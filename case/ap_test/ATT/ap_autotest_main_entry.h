#ifndef __AP_MAIN_ENTRY_H
#define __AP_MAIN_ENTRY_H

#include "ap_test_head.h"

extern const uint32* ap_main_entry[];

enum {
	ENTRY_ID(att_write_data) = 0,
	ENTRY_ID(att_read_data) ,
	ENTRY_ID(read_atf_file) ,
	ENTRY_ID(read_atf_sub_file_data) ,
	ENTRY_ID(check_fatal_result) ,
};

#if 0
uint32 att_write_data(uint16 cmd, uint32 payload_len, uint8* data_addr);
#endif
#define ATT_WRITE_DATA(a,b,c) 		((uint32(*)(uint16 cmd, uint32 payload_len, uint8* data_addr))(ap_main_entry[ENTRY_ID(att_write_data)]))((a),(b),(c))

#if 0
uint32 att_read_data(uint8* data_addr, uint32 payload_len);
#endif
#define ATT_READ_DATA(a,b) 			((uint32(*)(uint8* data_addr, uint32 payload_len))(ap_main_entry[ENTRY_ID(att_read_data)]))((a),(b))

#if 0
int read_atf_file(uint32 test_mode, uint8* atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, uint32 atf_offset, uint32 total_len);
#endif
#define READ_ATF_FILE(a,b,c,d,e,f)  	((int(*)(uint32 test_mode, uint8* atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, \
	uint32 atf_offset, uint32 total_len))(ap_main_entry[ENTRY_ID(read_atf_file)]))((a),(b),(c),(d),(e),(f))

#if 0
int read_atf_sub_file_data(uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, atf_head_t *p_atf_head, \
    const uint8* atf_file_name, const uint8 * sub_file_name, int32 offset_from_sub_file, int32 read_len)
#endif
#define READ_ATF_SUB_FILE_DATA(a,b,c,d,e,f,g,h)  	((int(*)(uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, \
	atf_head_t *p_atf_head, const uint8* atf_file_name, const uint8 * sub_file_name, int32 offset_from_sub_file, \
    int32 read_len))(ap_main_entry[ENTRY_ID(read_atf_sub_file_data)]))((a),(b),(c),(d),(e),(f),(g),(h))


#if 0
void check_fatal_result(int ret, const uint8* print_err_info, ...);
#endif
#define CHECK_FATAL_RESULT(a,b,...) 			((void(*)(int ret, const uint8* print_err_info))(ap_main_entry[ENTRY_ID(check_fatal_result)]))((a),(b),##__VA_ARGS__)


#endif

