/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用自动化测试。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

#ifdef bt_auto_test
//定时器id
int8 get_statis_timer_id = -1;
uint8 test_mode;

#if 1
void deal_random_test(void)
{
    uint32 mode;
    while(1)
    {
        mode=sys_random()&0x07;
        if((mode!=0)&&(mode<6))
        {
            break;
        }
    }
    if((g_btplay_bt_info.status==STATUS_WAIT_PAIRED)&&(mode!=3))
    {
        mode=3;
    }
    libc_printf("\nrandom val %d",mode);
    switch (mode)
    {
        case 1:
        key_next_deal();
        break;

        case 2:
        hfp_outgo_call();
        break;

        case 3:
        com_btengine_connect_deal(CON_DEAL_CONNECT_LAST, NULL);
        break;
        case 4:
        key_play_deal();
        break;
        case 5:
        key_prev_deal();
        break;
        default:
        break;
    }
}
#else
void deal_random_test(void)
{

    if(g_btplay_bt_info.status==STATUS_NONE)
    {
        return;
    }
    libc_printf("\nbt status %d",g_btplay_bt_info.status);
    if(g_btplay_bt_info.status==STATUS_LINKED)
    {
        key_play_deal();
    }
    else
    {
        com_btengine_connect_deal(CON_DEAL_CONNECT_LAST, NULL);
    }
}

#endif
#endif
