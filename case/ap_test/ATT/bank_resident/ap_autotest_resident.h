#ifndef __BANK_RESIDENT_H
#define __BANK_RESIDENT_H


// test item overlay bank区加载的bank id，顺序不要变，否则要修改makefile中的bk_item.bin的合并顺序
typedef enum{
    TEST_ITEM_BANK_ID_BT_ATTR_MODIEY = 0,
    TEST_ITEM_BANK_ID_BT_ATTR_READ,
    TEST_ITEM_BANK_ID_BT_TEST,
    TEST_ITEM_BANK_ID_FM,
    TEST_ITEM_BANK_ID_LINEIN,
    TEST_ITEM_BANK_ID_MUSIC,
    TEST_ITEM_BANK_ID_MAX_NULL,
}test_item_bank_id_t;

// 扩张测试项部分的bank id
typedef enum{
	TEST_ITEM_EXT_BANK_ID_GPIO = 0,
	TEST_ITEM_EXT_BANK_ID_CHANNEL,
	TEST_ITEM_EXT_BANK_ID_MAX_NULL,
}test_item_ext_bank_id_t;

#endif

