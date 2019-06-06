/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：缓冲区分配定义头文件。
 * 作者：zhouxl
 ********************************************************************************/

#ifndef __AP_AUTOTEST_MEM_DEFINE_H
#define __AP_AUTOTEST_MEM_DEFINE_H

#define STUB_ATT_RW_TEMP_BUFFER_LEN (256)
//extern uint8 * STUB_ATT_RW_TEMP_BUFFER;

#define STUB_ATT_RETURN_DATA_LEN    (256)
//extern uint8 * STUB_ATT_RETURN_DATA_BUFFER;

//插卡测试时，需要保存用以记录当前测试进度信息的文件，此缓冲区用以文件的读写，文件名"att_record.bin"，2K
//#define RECORD_FILE_BUFFER_LEN     (0x800)
//extern uint8 TEST_RECORD_FILE_BUFFER[RECORD_FILE_BUFFER_LEN]; 

//插卡测试时，保存每次测试结果信息的文件，文件名对应"ATT_XXX_XXX.LOG"，1K
#define LOG_FILE_BUFFER_SIZE                (0x200)
extern uint8 LOG_FILE_BUFFER[LOG_FILE_BUFFER_SIZE]; 

//插卡测试时，保存sd卡atf文件中打包的config.txt测试项列表信息，1K
#define CFG_FILE_BUFFER_SIZE         (0x400)
extern uint8 CFG_FILE_BUFFER[CFG_FILE_BUFFER_SIZE];

//bank 常驻代码空间 2K
#define BANK_RESIDENT_BUFFER_SIZE	((512)*5)
extern uint8 BANK_RESIDENT_BUFFER[BANK_RESIDENT_BUFFER_SIZE]; 

//bank 基础功能代码空间 2.5K
#define BANK_BASE_FUNC_BUFFER_SIZE	((512)*5)
extern uint8 BANK_BASE_FUNC_BUFFER[BANK_BASE_FUNC_BUFFER_SIZE]; 

//bank 测试项代码空间 1.5K
#define BANK_TEST_ITEM_BUFFER_SIZE	(512*3)
extern uint8 BANK_TEST_ITEM_BUFFER[BANK_TEST_ITEM_BUFFER_SIZE];

//扩展测试项代码空间 8K
#define BANK_TEST_ITEM_EXT_BUFFER_SIZE (8*1024)
extern uint8 BANK_TEST_ITEM_EXT_BUFFER[BANK_TEST_ITEM_EXT_BUFFER_SIZE];

#endif



