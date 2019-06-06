#ifndef __RESIDENT_ENTRY_H
#define __RESIDENT_ENTRY_H

#include "ap_test_head.h"

extern const uint32 bank_resident_entry[];

enum {
	ENTRY_ID(act_test_read_bt_addr_from_record_file) = 0,
	ENTRY_ID(att_test_read_count_from_record_file) ,
	ENTRY_ID(act_test_write_att_record_file) ,
	ENTRY_ID(load_bank_on_base_func) ,
	ENTRY_ID(load_bank_on_test_item) ,
	ENTRY_ID(test_enter),
};


#if 1
int32 act_test_read_bt_addr_from_record_file(uint8 *byte_buffer);
#endif
#define READ_BT_ADDR_FROM_RECORD_FILE(a) 		((int32(*)(uint8 *byte_buffer))(bank_resident_entry[ENTRY_ID(act_test_read_bt_addr_from_record_file)]))((a))

#if 1
uint32 att_test_read_count_from_record_file(void);
#endif
#define ATT_TEST_READ_COUNT_FROM_RECORD_FILE() 	((uint32(*)(void))(bank_resident_entry[ENTRY_ID(att_test_read_count_from_record_file)]))()

#if 1
int32 act_test_write_att_record_file(uint8 *byte_buffer, uint32 record_cnt, uint32 mode);
#endif
#define WRITE_ATT_RECORD_FILE(a,b,c)        ((int32(*)(uint8 *byte_buffer, uint32 record_cnt, uint32 mode))(bank_resident_entry[ENTRY_ID(act_test_write_att_record_file)]))((a),(b),(c))


#if 1
int32 load_bank_on_base_func(uint8 bank_id, uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, \
	atf_head_t *p_atf_head, const uint8* atf_file_name);
#endif
#define LOAD_BANK_ON_BASE_FUNC(a,b,c,d,e,f)			((int32(*)(uint8 bank_id, uint32 test_mode, uint8* dst_addr, \
	uint32 dst_buffer_len, atf_head_t *p_atf_head, const uint8* atf_file_name))(bank_resident_entry[ENTRY_ID(load_bank_on_base_func)]))((a),(b),(c),(d),(e),(f))


#if 1
int32 load_bank_on_test_item(uint8 bin_id, uint8 bank_id, uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, atf_head_t *p_atf_head, \
    const uint8* atf_file_name, int32 sub_bank_size);
#endif
#define LOAD_BANK_ON_TEST_ITEM(a,b,c,d,e,f,g,h)		((int32(*)(uint8 bin_id, uint8 bank_id, uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, \
	atf_head_t *p_atf_head, const uint8* atf_file_name, int32 sub_bank_size))(bank_resident_entry[ENTRY_ID(load_bank_on_test_item)]))((a),(b),(c),(d),(e),(f),(g),(h))


#if 1
void test_enter(uint8 test_mode, const uint8* atf_file_name, atf_head_t * atf_head, buffer_info_t* buffer_info);
#endif
#define TEST_ENTER(a,b,c,d)        ((void(*)(uint8 test_mode, const uint8* atf_file_name, atf_head_t * atf_head, \
	buffer_info_t* buffer_info))(bank_resident_entry[ENTRY_ID(test_enter)]))((a),(b),(c),(d))


#if 0

#endif

#if 0

#endif


#endif

