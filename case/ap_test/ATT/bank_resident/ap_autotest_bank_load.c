/*
	overlay bank代码加载，主要有两个overlay
	1. base_func: 基本功能bank区，分时加载args_read和common两个bank代码
	2. test_item: 测试项bank区，分时加载各个测试项代码
*/

#include "ap_test_head.h"
#include "ap_autotest_resident.h"
#include "ap_autotest_main_entry.h"

//加载在base func基本功能内存区上的"参数读取"bin文件
const uint8 BASE_FUNC_BANK_NAME_ARGS_READ[] = "bkargs.bin";

//加载在base func基本功能内存区上的"通用功能"bin文件
const uint8 BASE_FUNC_BANK_NAME_COMMON[] 	= "bkcom.bin";

//加载在test item测试项代码内存区上的"测试项"bin文件
//有部分测试项代码加载在test item ext代码内存区，该内存区有较多的内存空间可以使用，但是只针对不需要使用engine应用和中间件的测试项
const uint8 TEST_ITEM_BANK_NAME[TEST_ITEM_BIN_ID_MAX][16] = {
	"bkitem.bin", 
	"bkitex.bin"
};

// 各个测试项bin文件中的最大bank_id
const uint8 MAX_BANK_ID_IN_TEST_ITEM_BIN[TEST_ITEM_BIN_ID_MAX] = {
	TEST_ITEM_BANK_ID_MAX_NULL,
	TEST_ITEM_EXT_BANK_ID_MAX_NULL
};

/*
extern const uint32* ap_main_entry[];
static void show_ap_main_entry()
{
    libc_printf("print main entry: %x---------\n", (uint32)ap_main_entry);
    libc_printhex(ap_main_entry, 16, 0); 
}*/

int32 load_bank_on_base_func(uint8 bank_id, uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, \
	atf_head_t *p_atf_head, const uint8* atf_file_name)
{	
	static uint8 cur_bank_id = BASE_FUNC_BANK_ID_MAX_NULL;

	libc_printf("load_bank_on_base_func bank_id:%d cur_bank_id:%d\n", bank_id, cur_bank_id); 
	if (bank_id >= BASE_FUNC_BANK_ID_MAX_NULL){
		return -1;
	}
	if (cur_bank_id == bank_id){
		//避免重复加载
		return 0;
	}
	
	uint8 sub_file_name[30];
	libc_memset(sub_file_name, 0, sizeof(sub_file_name));

	switch(bank_id){
		case BASE_FUNC_BANK_ID_ARGS_READ:
		{
			libc_strncpy(sub_file_name, BASE_FUNC_BANK_NAME_ARGS_READ, sizeof(sub_file_name));
			break;
		}
		case BASE_FUNC_BANK_ID_COMMON:
		{
			libc_strncpy(sub_file_name, BASE_FUNC_BANK_NAME_COMMON, sizeof(sub_file_name));
			break;
		}
	}

	//show_ap_main_entry();

	int32 ret;
	ret = READ_ATF_SUB_FILE_DATA(test_mode, dst_addr, dst_buffer_len, p_atf_head, atf_file_name, sub_file_name, 0, -1);
	if ( ret >= 0){
		cur_bank_id = bank_id; 
	}

	return ret;
}

int32 load_bank_on_test_item(uint8 bin_id, uint8 bank_id, uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, atf_head_t *p_atf_head, \
    const uint8* atf_file_name, int32 sub_bank_size)
{	
	//各个测试项bin文件对应的代码空间均初始为未加载状态
	static uint8 cur_bank_id[TEST_ITEM_BIN_ID_MAX] = {
		TEST_ITEM_BANK_ID_MAX_NULL, 				
		TEST_ITEM_EXT_BANK_ID_MAX_NULL				
	};

	libc_printf("load_bank_on_test_item bin_id:%d bank_id:%d cur_bin_id:%d\n", bin_id, bank_id, cur_bank_id[bin_id]);
	if (bin_id >= TEST_ITEM_BIN_ID_MAX){
		return -1;
	}
	if (bank_id >= MAX_BANK_ID_IN_TEST_ITEM_BIN[bin_id]){
		return -1;
	}
	if (cur_bank_id[bin_id] == bank_id){
		//避免重复加载
		return 0;
	}
	
	//show_ap_main_entry();

	int ret;
	ret = READ_ATF_SUB_FILE_DATA(test_mode, dst_addr, dst_buffer_len, p_atf_head, atf_file_name, TEST_ITEM_BANK_NAME[bin_id], bank_id * sub_bank_size, sub_bank_size);
	if ( ret >= 0){
		cur_bank_id[bin_id] = bank_id;
	}

	return ret;
}

