/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ��ǰ̨Ӧ���Զ������ԡ�
 * ���ߣ�cailizhen
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
            //����ʱ��next/prev�ܾ�����
            hfp_reject_call();
        }
        else if (g_disp_status == STATUS_CALL_HFP)
        {
            //���ڽ���ʱ��next/prev�л���PHONE����
            if (g_chf_cag_exch_enable == 1)
            {
                hfp_change_to_phone();
            }
        }
        else if (g_disp_status == STATUS_CALL_PHONE)
        {
            //���ڽ���ʱ��next/prev�л����������
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
            //����ʱ��play����
            hfp_accept_call();
        }
        else
        {
            //���ʱ/���ڽ���ʱ��play�Ҷ�
            hfp_hang_up_call();
        }
        break;
        default:
        break;
    }
}
#endif

