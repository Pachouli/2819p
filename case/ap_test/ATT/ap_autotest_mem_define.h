/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ����������䶨��ͷ�ļ���
 * ���ߣ�zhouxl
 ********************************************************************************/

#ifndef __AP_AUTOTEST_MEM_DEFINE_H
#define __AP_AUTOTEST_MEM_DEFINE_H

#define STUB_ATT_RW_TEMP_BUFFER_LEN (256)
//extern uint8 * STUB_ATT_RW_TEMP_BUFFER;

#define STUB_ATT_RETURN_DATA_LEN    (256)
//extern uint8 * STUB_ATT_RETURN_DATA_BUFFER;

//�忨����ʱ����Ҫ�������Լ�¼��ǰ���Խ�����Ϣ���ļ����˻����������ļ��Ķ�д���ļ���"att_record.bin"��2K
//#define RECORD_FILE_BUFFER_LEN     (0x800)
//extern uint8 TEST_RECORD_FILE_BUFFER[RECORD_FILE_BUFFER_LEN]; 

//�忨����ʱ������ÿ�β��Խ����Ϣ���ļ����ļ�����Ӧ"ATT_XXX_XXX.LOG"��1K
#define LOG_FILE_BUFFER_SIZE                (0x200)
extern uint8 LOG_FILE_BUFFER[LOG_FILE_BUFFER_SIZE]; 

//�忨����ʱ������sd��atf�ļ��д����config.txt�������б���Ϣ��1K
#define CFG_FILE_BUFFER_SIZE         (0x400)
extern uint8 CFG_FILE_BUFFER[CFG_FILE_BUFFER_SIZE];

//bank ��פ����ռ� 2K
#define BANK_RESIDENT_BUFFER_SIZE	((512)*5)
extern uint8 BANK_RESIDENT_BUFFER[BANK_RESIDENT_BUFFER_SIZE]; 

//bank �������ܴ���ռ� 2.5K
#define BANK_BASE_FUNC_BUFFER_SIZE	((512)*5)
extern uint8 BANK_BASE_FUNC_BUFFER[BANK_BASE_FUNC_BUFFER_SIZE]; 

//bank ���������ռ� 1.5K
#define BANK_TEST_ITEM_BUFFER_SIZE	(512*3)
extern uint8 BANK_TEST_ITEM_BUFFER[BANK_TEST_ITEM_BUFFER_SIZE];

//��չ���������ռ� 8K
#define BANK_TEST_ITEM_EXT_BUFFER_SIZE (8*1024)
extern uint8 BANK_TEST_ITEM_EXT_BUFFER[BANK_TEST_ITEM_EXT_BUFFER_SIZE];

#endif



