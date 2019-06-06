/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话前台应用自动化测试。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

#ifdef bt_auto_test

int get_statis_timer_id=-1;
uint8 g_get_status_flag;

void deal_random_test(void)
{
    uint32 mode;
    while(1)
    {
        mode=sys_random()&0x03;
        if((mode<3)&&(mode!=0))
        {
            break;
        }
    }
    libc_printf("\nhfp nrandom val %d",mode);
    switch (mode)
    {
        case 1:
        if (g_disp_status == STATUS_CALL_INCOMING)
        {
            //来电时按next/prev拒绝接听
            hfp_reject_call();
        }
        else if (g_disp_status == STATUS_CALL_HFP)
        {
            //正在接听时按next/prev切换到PHONE接听
            if (g_chf_cag_exch_enable == 1)
            {
                hfp_change_to_phone();
            }
        }
        else if (g_disp_status == STATUS_CALL_PHONE)
        {
            //正在接听时按next/prev切换到音箱接听
            if (g_chf_cag_exch_enable == 1)
            {
                hfp_change_to_hfp();
            }
        }
        else
        {
            //nothing for QAC
        }
        break;

        case 2:
        if (g_disp_status == STATUS_CALL_INCOMING)
        {
            //来电时按play接听
            hfp_accept_call();
        }
        else
        {
            //打出时/正在接听时按play挂断
            hfp_hang_up_call();
        }
        break;
        default:
        break;
    }
}
#endif

