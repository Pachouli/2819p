/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ԥ��ģʽ�ӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#if (SUPPORT_FAST_PREVIEW == 1)

#define FAST_PREVIEW_ENTER_TIME   300  //��λ ms����һ�ΰ����󣬼���೤ʱ���ٴΰ����������Ԥ��ģʽ
#define FAST_PREVIEW_EXIT_TIME    500  //��λ ms��ֹͣ�����೤ʱ���˳�����Ԥ��ģʽ

typedef struct
{
    uint32 last_tick;             //��һ���л�����ʱ�������λms
    fast_preview_status_e fp_sta; //�Ƿ���������л�Ӧ�õİ����̰�����
    uint8 last_func_index;        //�����func index����preview��һ��func index
    uint8 last_func_id;           //�����func id����preview��һ��func id
    bool need_switch_ap;          //�����л�ǰ̨Ӧ�ñ�־
    uint8 switch_func_index;      //�����л���ǰ̨Ӧ��func index
} fast_preview_info_t;

static fast_preview_info_t g_fast_preview_info;

extern uint8 com_fast_preview_ask_next_func(uint8 last_func_id, app_result_e app_result, uint8 *next_func_index);
extern void com_fast_preview_show_ap_welcome_ui(uint8 func_id);
extern void com_fast_preview_load_ap_welcome_ui(void);

void com_fast_preview_init(void)
{
    libc_memset(&g_fast_preview_info, 0, sizeof(g_fast_preview_info));
    g_fast_preview_info.last_func_index = get_cur_func_index();
    g_fast_preview_info.last_func_id = get_cur_func_id();
    com_fast_preview_load_ap_welcome_ui();
}

fast_preview_status_e com_fast_preview_get_status_inner(void)
{
    return g_fast_preview_info.fp_sta;
}

bool com_fast_preview_need_switch_ap(uint8 *func_index)
{
    if (g_fast_preview_info.need_switch_ap == TRUE)
    {
        *func_index = g_fast_preview_info.switch_func_index;
        g_fast_preview_info.need_switch_ap = FALSE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void _fast_preview_tick(void)
{
    uint32 cur_ab_timer = sys_get_ab_timer();

    if (FAST_PREVIEW_STA_RUNNING == g_fast_preview_info.fp_sta)
    {
        if ((cur_ab_timer - g_fast_preview_info.last_tick) > FAST_PREVIEW_EXIT_TIME)
        {
            g_fast_preview_info.fp_sta = FAST_PREVIEW_STA_NULL;
            libc_printf_info("exit fast preview\n");

            if (g_fast_preview_info.last_func_index != get_cur_func_index())
            {
                g_fast_preview_info.switch_func_index = g_fast_preview_info.last_func_index;
                g_fast_preview_info.need_switch_ap = TRUE;
            }
        }
    }
    else if (FAST_PREVIEW_STA_READY == g_fast_preview_info.fp_sta)
    {
        if ((cur_ab_timer - g_fast_preview_info.last_tick) > FAST_PREVIEW_ENTER_TIME)
        {
            g_fast_preview_info.fp_sta = FAST_PREVIEW_STA_NULL;
            libc_printf("fast preview continuous key shot OVERTIME\n");
        }
    }

    if (FAST_PREVIEW_STA_NULL == g_fast_preview_info.fp_sta)
    {
        g_fast_preview_info.last_func_index = get_cur_func_index();
        g_fast_preview_info.last_func_id = get_cur_func_id();

        //�л���Ŀ��Ӧ�ú�Ž����ʾ  NOTE���忨��U��AUX���¼���Ҳ������fast preview�ķ�ʽ���л�Ӧ�ã�����������©��
        if (g_fast_preview_info.switch_func_index == get_cur_func_index())
        {
            g_app_info_state.ui_display_forbidden = FALSE;
        }
    }
}

static void _fast_preview_key_shot(app_result_e switch_result)
{
    uint8 next_func_id;
    uint8 next_func_index;
    uint32 cur_ab_timer = sys_get_ab_timer();

    if (FAST_PREVIEW_STA_NULL == g_fast_preview_info.fp_sta)
    {
        g_fast_preview_info.fp_sta = FAST_PREVIEW_STA_READY;
    }
    else if (FAST_PREVIEW_STA_READY == g_fast_preview_info.fp_sta)
    {
        g_fast_preview_info.fp_sta = FAST_PREVIEW_STA_RUNNING;
        libc_printf_info("enter fast preview\n");
    }
    g_fast_preview_info.last_tick = cur_ab_timer;

    next_func_id = com_fast_preview_ask_next_func(g_fast_preview_info.last_func_index, switch_result, &next_func_index);

    if (next_func_index != g_fast_preview_info.last_func_index)
    {
        libc_printf_info("fast previer : [%d,%d] -> [%d,%d]\n", g_fast_preview_info.last_func_id,
                g_fast_preview_info.last_func_index, next_func_id, next_func_index);

        g_app_info_state.ui_display_forbidden = FALSE;
        com_fast_preview_show_ap_welcome_ui(next_func_id);
        g_app_info_state.ui_display_forbidden = TRUE;

        g_fast_preview_info.last_func_index = next_func_index;
        g_fast_preview_info.last_func_id = next_func_id;
    }

    if ((FAST_PREVIEW_STA_READY == g_fast_preview_info.fp_sta)
        && (g_fast_preview_info.last_func_index != get_cur_func_index()))
    {
        g_fast_preview_info.switch_func_index = g_fast_preview_info.last_func_index;
        g_fast_preview_info.need_switch_ap = TRUE;
    }
}

void com_fast_preview_cbk_inner(input_gui_msg_t *input_msg)
{
    if (input_msg == NULL) //ιtick
    {
        null_or_not_switch_key_shot:

        _fast_preview_tick();
    }
    else if ((uint32) input_msg == 1) //��ʾ����fast preview
    {
        com_fast_preview_init();
    }
    else //�а����̰�����
    {
        bool has_key_shot = FALSE;
        app_result_e switch_result = RESULT_NULL;

        //��Ҫͬ key_shortcut_list �л�Ӧ�ð���ӳ�䱣��һ��
        switch (input_msg->data.kmsg.val)
        {
        case KEY_MODE:

            switch_result = RESULT_NEXT_FUNCTION;

        #if 0
        case KEY_BT:
        case KEY_LINEIN:
        case KEY_AUXIN:
        case KEY_OPT:
        case KEY_COAX:
        case KEY_ARC:
        case KEY_HDMI_0:
        #endif

            if (KEY_TYPE_DOWN == input_msg->data.kmsg.type)
            {
                has_key_shot = TRUE;
            }
            else
            {
                input_msg->data.kmsg.type = KEY_TYPE_NULL;
            }

            //�滻Ϊ KEY_DUMMY
            input_msg->data.kmsg.val = KEY_DUMMY;

            break;

        default:
            goto null_or_not_switch_key_shot;
        }

        if (has_key_shot == TRUE)
        {
            _fast_preview_key_shot(switch_result);
        }
    }
}

void com_fast_preview_next_app_inner(app_result_e switch_result)
{
    _fast_preview_key_shot(switch_result);
}

#endif

