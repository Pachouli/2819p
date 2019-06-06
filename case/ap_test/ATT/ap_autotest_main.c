/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：自动化测试入口，初始化程序。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_resident/ap_autotest_resident_entry.h"

/*全局变量定义*/

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

//用以存储sd卡测试时的测试结果日志文件 1K
__section__(".bss.log_file_buffer") uint8 LOG_FILE_BUFFER[LOG_FILE_BUFFER_SIZE] ;

//用以存储atf配置文件中所有测试项信息, 1K
__section__(".bss.att_cfg_file_buf") uint8 CFG_FILE_BUFFER[CFG_FILE_BUFFER_SIZE] ;
//uint32 CFG_FILE_SIZE = 0;

//用以存储bank resident常驻功能代码,2K
__section__(".bss.bank_resident_buffer")        uint8 BANK_RESIDENT_BUFFER[BANK_RESIDENT_BUFFER_SIZE]; 

//用以存储bank base func基础功能代码,4K
__section__(".bss.bank_base_func_buffer")       uint8 BANK_BASE_FUNC_BUFFER[BANK_BASE_FUNC_BUFFER_SIZE]; 

//用以存储bank测试项处理功能代码 2K
__section__(".bss.bank_test_item_buffer")       uint8 BANK_TEST_ITEM_BUFFER[BANK_TEST_ITEM_BUFFER_SIZE];

//用以存储bank测试项处理功能代码 2K
__section__(".bss.bank_test_item_ext_buffer")   uint8 BANK_TEST_ITEM_EXT_BUFFER[BANK_TEST_ITEM_EXT_BUFFER_SIZE];

//atf打包文件
const uint8 ATF_FILE_NAME[] =
{
    0xff, 0xfe,
    'a',  0x00,
    'c',  0x00,
    't',  0x00,
    'i',  0x00,
    'o',  0x00,
    'n',  0x00,
    's',  0x00,
    't',  0x00,
    'e',  0x00,
    's',  0x00,
    't',  0x00,
    '_',  0x00,
    '2',  0x00,
    '8',  0x00,
    '1',  0x00,
    'b',  0x00,
    '.',  0x00,
    'a',  0x00,
    't',  0x00,
    'f',  0x00,
    0x00, 0x00
};

//atf文件中打包的描述测试项列表信息的文件
const uint8 CONFIG_FILE_NAME[] = "config.txt";


/******************************************************************************/
/*!
 * \par  Description:
 *    applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_init(void)
{
    /*初始化applib库，前台AP*/
    applib_init(APP_ID_CONFIG, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 2);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib退出
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);
}

void check_fatal_result(int ret, const uint8* print_err_info, ...)
{
    if (ret < 0){
        va_list args;
        va_start(args, print_err_info); 
        libc_printf(print_err_info, args);
        va_end(args);

        while(1){}
    }
}

uint8 test_init(uint8 att_para, atf_head_t * atf_head, uint32 *cfg_file_size)
{   
    int32 ret;
    uint8 test_mode;

    if (DISK_H == att_para)
    {
        test_mode = TEST_MODE_CARD;
    }
    else
    {
        test_mode = TEST_MODE_USB;
    }

    //从atf中加载atf文件头
    //libc_memset((uint8*)atf_head, 0, sizeof(atf_head_t));
    ret = read_atf_file(test_mode, (uint8*)ATF_FILE_NAME, (uint8*)atf_head, sizeof(atf_head_t), 0, sizeof(atf_head_t));
    if( ret != sizeof(atf_head_t)){
        libc_printf("load atf head error!\n");
        while(1){}
    }

    //加载resident bank常驻代码
    ret = read_atf_sub_file_data(test_mode, BANK_RESIDENT_BUFFER, BANK_RESIDENT_BUFFER_SIZE, atf_head, (uint8*)ATF_FILE_NAME, "bkresi.bin", 0, -1);
    check_fatal_result(ret, "load bank_resident.bin error.");

    //如果是sd卡模式，则需要加载atf中的config.txt测试项信息
    if (TEST_MODE_CARD == test_mode){
        ret = read_atf_sub_file_data(test_mode, CFG_FILE_BUFFER, CFG_FILE_BUFFER_SIZE, atf_head, (uint8*)ATF_FILE_NAME, (uint8*)CONFIG_FILE_NAME, 0, -1);
        check_fatal_result(ret, "load config.txt error.");
        if (ret >= 0){
            *cfg_file_size = ret;
        }
    }


    return test_mode;
}

int main(int argc, const char *argv[])
{   
    uint8 att_para = (uint8) (uint32) argv[0];

    //重启禁止断点续播
    g_p_esd_bk_info->flag = 0;
    g_p_global_var->att_flag = 1;

    /*ap初始化*/
    _app_init();
    //关闭TTS播报和按键音
    //com_tts_state_play_control(TTS_CTRL_IGNORE);
    //keytone_set_on_off (FALSE);
    //使能喇叭播放
    com_speaker_on(SPK_PLAY);
    com_set_drc_onoff(FALSE);  //关掉drc，否则蓝牙播放测试会异常
    com_set_dae_onoff(FALSE); //关掉DAE
    

    atf_head_t * atf_head = sys_malloc(sizeof(atf_head_t));
    if (NULL == atf_head){
        libc_printf("fail to sys_malloc for atf_head_t!\n");
        while(1){}
    }else{
        libc_printf("sys_malloc for atf_head_t ok!\n");
    }
    
    uint8 test_mode;
    uint32 cfg_file_size;
    test_mode = test_init(att_para, atf_head, &cfg_file_size);

    //准备好buffer参数信息
    buffer_info_t buffer_info;    
    buffer_info.log_file_buffer             = LOG_FILE_BUFFER;
    buffer_info.log_file_buffer_size        = LOG_FILE_BUFFER_SIZE;
    buffer_info.cfg_file_buffer             = CFG_FILE_BUFFER;
    buffer_info.cfg_file_length             = cfg_file_size;
    buffer_info.bank_base_func_buffer       = BANK_BASE_FUNC_BUFFER;
    buffer_info.bank_base_func_buffer_size  = BANK_BASE_FUNC_BUFFER_SIZE;

    //测试项代码空间
    buffer_info.bank_test_item_buffer[TEST_ITEM_BIN_ID]             = BANK_TEST_ITEM_BUFFER;
    buffer_info.bank_test_item_buffer_size[TEST_ITEM_BIN_ID]        = BANK_TEST_ITEM_BUFFER_SIZE;
    //扩展测试项代码空间
    buffer_info.bank_test_item_buffer[TEST_ITEM_BIN_ID_EXT]         = BANK_TEST_ITEM_EXT_BUFFER;
    buffer_info.bank_test_item_buffer_size[TEST_ITEM_BIN_ID_EXT]    = BANK_TEST_ITEM_EXT_BUFFER_SIZE;
    

    //进入测试
    TEST_ENTER(test_mode, ATF_FILE_NAME, atf_head, &buffer_info);

    sys_free(atf_head);
    
    com_set_dae_onoff(TRUE); //恢复DAE
    com_set_drc_onoff(TRUE);
    //关闭喇叭播放
    com_speaker_off(SPK_PLAY);
    //恢复TTS播报和按键音
    //com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
    //keytone_set_on_off(TRUE);

    _app_exit();

    return RESULT_NULL;
}




