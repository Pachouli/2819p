/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������С�ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func_inner.h"

/*! �Ƿ���Ҫ����忨����ϵ���Ϣ */
static uint8 g_need_update_vm;

/*! IC PA ��ʼ���Ƿ���ɣ���ɺ���ܲ������� */
static bool g_ic_pa_inited;

/*! �Ƿ���Ҫ����DUT����ģʽ */
static uint8 g_enter_dut_test;

/*! �������� */
static sound_energy_t *g_sound_energy_p;

void set_need_update_vm_inner(uint8 flag)
{
    g_need_update_vm = flag;
}
uint8 get_need_update_vm_inner(void)
{
    return g_need_update_vm;
}

void set_ic_pa_inited_inner(bool inited)
{
    g_ic_pa_inited = inited;
}
bool get_ic_pa_inited_inner(void)
{
    return g_ic_pa_inited;
}

void set_enter_dut_test_inner(uint8 test)
{
    g_enter_dut_test = test;
}
uint8 get_enter_dut_test_inner(void)
{
    return g_enter_dut_test;
}

///���õ�ǰǰ̨AP function id
void set_cur_func_id_inner(uint8 func_id)
{
    g_app_info_state.cur_func_id = func_id;
}

//��ȡϵͳ��״̬
card_state_e get_card_state_inner(void)
{
    return g_app_info_state.card_state;
}

//�޸�ϵͳ��״̬
void change_card_state_inner(card_state_e card_state)
{
    g_app_info_state.card_state = card_state;
}

//��ȡϵͳLINEIN״̬
linein_state_e get_linein_state_inner(void)
{
    return g_app_info_state.linein_state;
}
//��ȡϵͳ USB ����״̬
uhost_state_e get_uhost_state_inner(void)
{
    return g_app_info_state.uhost_state;
}

//��ȡϵͳ USB ����״̬
cable_state_e get_cable_state_inner(void)
{
    return g_app_info_state.cable_state;
}
//��ȡϵͳ USB �����״̬
adaptor_state_e get_adaptor_state_inner(void)
{
    return g_app_info_state.adaptor_state;
}

//��ȡUHOST�豸����
usb_host_in_e get_uhost_type_inner(void)
{
    return g_app_info_state.uhost_type;
}
//�޸�UHOST�豸����
void change_uhost_type_inner(usb_host_in_e uhost_type)
{
    g_app_info_state.uhost_type = uhost_type;
}

uint8 com_get_stub_tools_type_inner(void)
{
    return g_app_info_state.stub_tools_type;
}

void com_set_stub_tools_type_inner(uint8 stub_tools_type)
{
    g_app_info_state.stub_tools_type = stub_tools_type;
}

uint8 get_bat_value_inner(void)
{
    return g_app_info_state.bat_value;
}

uint8 get_btdbg_ejtag_flag_inner(void)
{
    return g_app_info_state.btdbg_ejtag_flag;
}

/*! \cond COMMON_API */

freq_level_e com_adjust_sys_clk_inner(freq_level_e freq_level, bool sys_clk_work)
{
    freq_level_e ret_freq;

    change_sys_clk_work(sys_clk_work);

    if (get_sys_clk_work() == TRUE)
    {
        ret_freq = sys_adjust_clk(freq_level, SET_SYS_CLK);
    }
    else
    {
        if (get_ui_status() == UI_STATUS_BUSY)
        {
            ret_freq = sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
        }
        else
        {
            ret_freq = sys_adjust_clk(SYS_CLK_ON_IDLE, SET_SYS_CLK);
        }
    }

    return ret_freq;
}

/*
 *  ����ļ�ϵͳ������VRAM�е���Ϣ��
 */
void clear_fs_vram_info_inner(void)
{
    fs_vm_head_t fs_vm_head;
    //���¿���������ļ�ϵͳVRAM��Ϣ����ȷ���ܹ�����ɨ��fat��
    libc_memset(&fs_vm_head, 0x0, sizeof(fs_vm_head_t));
    sys_vm_write(&fs_vm_head, FS_VM_HEAD_INFO, sizeof(fs_vm_head_t));
}

//������������
void com_update_sound_energy_inner(sound_energy_t *p_energy)
{
    if (g_sound_energy_p == NULL)
    {
        g_sound_energy_p = (sound_energy_t *) sys_malloc(sizeof(sound_energy_t));
        if (g_sound_energy_p == NULL)
        {
            libc_printf("g_sound_energy_p\n");
            while (1);
        }
    }
    sys_os_sched_lock();
    if (p_energy != NULL)
    {
        libc_memcpy(g_sound_energy_p, p_energy, sizeof(sound_energy_t));
    }
    else
    {
        libc_memset(g_sound_energy_p, 0x00, sizeof(sound_energy_t));
    }
    sys_os_sched_unlock();
}

//��ȡ��������
void com_get_sound_energy_inner(sound_energy_t *p_energy)
{
    sys_os_sched_lock();
    if (g_sound_energy_p != NULL)
    {
        libc_memcpy(p_energy, g_sound_energy_p, sizeof(sound_energy_t));
    }
    else
    {
        libc_memset(p_energy, 0x00, sizeof(sound_energy_t));
    }
    sys_os_sched_unlock();
}

/*! \endcond */
