/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ��Զ���������ڣ���ʼ������
 * ���ߣ�zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "bank_resident/ap_autotest_resident_entry.h"

/*ȫ�ֱ�������*/

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_FRONT_LOW_PRIO;

//���Դ洢sd������ʱ�Ĳ��Խ����־�ļ� 1K
__section__(".bss.log_file_buffer") uint8 LOG_FILE_BUFFER[LOG_FILE_BUFFER_SIZE] ;

//���Դ洢atf�����ļ������в�������Ϣ, 1K
__section__(".bss.att_cfg_file_buf") uint8 CFG_FILE_BUFFER[CFG_FILE_BUFFER_SIZE] ;
//uint32 CFG_FILE_SIZE = 0;

//���Դ洢bank resident��פ���ܴ���,2K
__section__(".bss.bank_resident_buffer")        uint8 BANK_RESIDENT_BUFFER[BANK_RESIDENT_BUFFER_SIZE]; 

//���Դ洢bank base func�������ܴ���,4K
__section__(".bss.bank_base_func_buffer")       uint8 BANK_BASE_FUNC_BUFFER[BANK_BASE_FUNC_BUFFER_SIZE]; 

//���Դ洢bank��������ܴ��� 2K
__section__(".bss.bank_test_item_buffer")       uint8 BANK_TEST_ITEM_BUFFER[BANK_TEST_ITEM_BUFFER_SIZE];

//���Դ洢bank��������ܴ��� 2K
__section__(".bss.bank_test_item_ext_buffer")   uint8 BANK_TEST_ITEM_EXT_BUFFER[BANK_TEST_ITEM_EXT_BUFFER_SIZE];

//atf����ļ�
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

//atf�ļ��д���������������б���Ϣ���ļ�
const uint8 CONFIG_FILE_NAME[] = "config.txt";


/******************************************************************************/
/*!
 * \par  Description:
 *    applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_init(void)
{
    /*��ʼ��applib�⣬ǰ̨AP*/
    applib_init(APP_ID_CONFIG, THIS_APP_TYPE);

    /*��ʼ����ʱ��*/
    init_app_timers(THIS_APP_TIMER_GROUP, 2);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    /*ִ��applib���ע������*/
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

    //��atf�м���atf�ļ�ͷ
    //libc_memset((uint8*)atf_head, 0, sizeof(atf_head_t));
    ret = read_atf_file(test_mode, (uint8*)ATF_FILE_NAME, (uint8*)atf_head, sizeof(atf_head_t), 0, sizeof(atf_head_t));
    if( ret != sizeof(atf_head_t)){
        libc_printf("load atf head error!\n");
        while(1){}
    }

    //����resident bank��פ����
    ret = read_atf_sub_file_data(test_mode, BANK_RESIDENT_BUFFER, BANK_RESIDENT_BUFFER_SIZE, atf_head, (uint8*)ATF_FILE_NAME, "bkresi.bin", 0, -1);
    check_fatal_result(ret, "load bank_resident.bin error.");

    //�����sd��ģʽ������Ҫ����atf�е�config.txt��������Ϣ
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

    //������ֹ�ϵ�����
    g_p_esd_bk_info->flag = 0;
    g_p_global_var->att_flag = 1;

    /*ap��ʼ��*/
    _app_init();
    //�ر�TTS�����Ͱ�����
    //com_tts_state_play_control(TTS_CTRL_IGNORE);
    //keytone_set_on_off (FALSE);
    //ʹ�����Ȳ���
    com_speaker_on(SPK_PLAY);
    com_set_drc_onoff(FALSE);  //�ص�drc�������������Ų��Ի��쳣
    com_set_dae_onoff(FALSE); //�ص�DAE
    

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

    //׼����buffer������Ϣ
    buffer_info_t buffer_info;    
    buffer_info.log_file_buffer             = LOG_FILE_BUFFER;
    buffer_info.log_file_buffer_size        = LOG_FILE_BUFFER_SIZE;
    buffer_info.cfg_file_buffer             = CFG_FILE_BUFFER;
    buffer_info.cfg_file_length             = cfg_file_size;
    buffer_info.bank_base_func_buffer       = BANK_BASE_FUNC_BUFFER;
    buffer_info.bank_base_func_buffer_size  = BANK_BASE_FUNC_BUFFER_SIZE;

    //���������ռ�
    buffer_info.bank_test_item_buffer[TEST_ITEM_BIN_ID]             = BANK_TEST_ITEM_BUFFER;
    buffer_info.bank_test_item_buffer_size[TEST_ITEM_BIN_ID]        = BANK_TEST_ITEM_BUFFER_SIZE;
    //��չ���������ռ�
    buffer_info.bank_test_item_buffer[TEST_ITEM_BIN_ID_EXT]         = BANK_TEST_ITEM_EXT_BUFFER;
    buffer_info.bank_test_item_buffer_size[TEST_ITEM_BIN_ID_EXT]    = BANK_TEST_ITEM_EXT_BUFFER_SIZE;
    

    //�������
    TEST_ENTER(test_mode, ATF_FILE_NAME, atf_head, &buffer_info);

    sys_free(atf_head);
    
    com_set_dae_onoff(TRUE); //�ָ�DAE
    com_set_drc_onoff(TRUE);
    //�ر����Ȳ���
    com_speaker_off(SPK_PLAY);
    //�ָ�TTS�����Ͱ�����
    //com_tts_state_play_control(TTS_CTRL_CANCEL_IGNORE);
    //keytone_set_on_off(TRUE);

    _app_exit();

    return RESULT_NULL;
}




