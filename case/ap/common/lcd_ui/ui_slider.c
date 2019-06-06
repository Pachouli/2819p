/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������LCD�����������˿ؼ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
static uint8 g_slider_overtime;

const region_t bar_region = { 0, 16, SCR_WIDTH, 16 }; //�ؼ���������
const region_t num_region = { 44, 8, 40, 8 }; //��������

const key_map_t slider_key_map_list[] =
{
    /*! �̰�PLAYȷ����ֵ */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_UI_CONFORM },
    /*! �̰�F1ȡ������ */
    { { KEY_F1, 0, KEY_TYPE_SHORT_UP }, EVENT_UI_CANCEL },

    /*! ���ڼ� */
    { { KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_ADD },
    { { KEY_VADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_ADD },
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_ADD },

    /*! ���ڼ� */
    { { KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_SUB },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_SUB },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_UI_VALUE_SUB },

    /*! ������־ */
    { { KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL },
};

void com_slider_timer_handle(void)
{
    g_slider_overtime++;
}

/*
 ********************************************************************************
 * Description : �����˿ؼ�
 *
 * Arguments   : slider, �����˽ṹ
 callback, �����˵Ļص�����,=NULL ��ʾû�лص�����
 *
 * Returns     : �����Ϣ, ����0
 *
 * Notes       : �ؼ�����û���ػ�����, ��Ϊ�ؼ������ػ�����
 *
 ********************************************************************************
 */
app_result_e ui_slider_inner(slider_t *slider, slider_callback s_cbk)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t pri_msg;
    int8 slider_timer_id;

    uint8 start, end; //x����
    bool need_draw = TRUE;
    bool value_changed = TRUE; //ֵ�Ƿ�ı�, ��Ҫcallback

    change_app_timer_tag(APP_TIMER_TAG_SUB);

    slider_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SUB, APP_TIMER_ATTRB_CONTROL, \
        500, com_slider_timer_handle);

    while (1)
    {
        if (need_draw == TRUE)
        {
            lcd_clear_screen(&bar_region);

            //show pic
            lcd_show_image(POSITION_SLIDERBAR, 42, SBAR);
            start = POSITION_SLIDERBAR + 1;
            end = (POSITION_SLIDERBAR - 1)
                 + SLIDER_BAR_WIDTH * (slider->value - slider->min) / (slider->max - slider->min);
            //�ֶ���ʾ����
            while ((start + SLIDER_BG_WIDTH) < end)
            {
                lcd_show_image(start, 42, SBKGRD);
                start += SLIDER_BG_WIDTH;
            }
            // �ڱ����ľ�ͷ����ʾ�ϰ�
            lcd_show_image(start, 42, SSLIDER);

            //��ʾ����
            lcd_show_image(48, 7, LNUM0+(slider->value/10));
            lcd_show_image(64, 7, LNUM0+(slider->value%10));

            lcd_update_screen(NULL);

            need_draw = FALSE;
        }

        if (g_slider_overtime >= UI_OVERTIME)
        {
            result = RESULT_UI_CANCEL;
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            g_slider_overtime = 0;

            if (com_key_mapping(&gui_msg, &gui_event, slider_key_map_list) == TRUE)
            {
                switch (gui_event)
                {
                case EVENT_UI_CONFORM:
                    result = RESULT_UI_CONFIRM;
                    goto slider_exit;

                case EVENT_UI_CANCEL:
                    result = RESULT_UI_CANCEL;
                    goto slider_exit;

                case EVENT_UI_VALUE_ADD:
                    if ((slider->value + slider->step) <= slider->max)
                    {
                        slider->value += slider->step;
                        value_changed = TRUE;
                        need_draw = TRUE;
                    }
                    break;

                case EVENT_UI_VALUE_SUB:
                    if ((slider->value - slider->step) >= slider->min)
                    {
                        slider->value -= slider->step;
                        value_changed = TRUE;
                        need_draw = TRUE;
                    }
                    break;

                default:
                    result = com_message_box(gui_event);
                    break;
                }
            }
        }
        else
        {
            if (g_app_msg_dispatch_handle != NULL)
            {
                if (get_app_msg(APP_TYPE_GUI, &pri_msg) == TRUE)
                {
                    result = g_app_msg_dispatch_handle(&pri_msg);
                }
            }
        }

        //handle callback
        if ((s_cbk != NULL) && (value_changed == TRUE))
        {
            s_cbk(slider->value);
            value_changed = FALSE;
        }

        if (result != RESULT_NULL)
        {
            break;
        }
    }//while(1)

    slider_exit:

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, slider_timer_id);
    slider_timer_id = -1;

    change_app_timer_tag(APP_TIMER_TAG_MAIN);

    return result;
}

void contrast_changed_callback(int16 value)
{
    lcd_set_contrast(value);
}

app_result_e ui_set_contrast(void)
{
    app_result_e result = RESULT_NULL;
    slider_t contrast_slider;

    contrast_slider.min = 0;
    contrast_slider.max = 15;
    contrast_slider.value = g_com_env_var.contrast;
    contrast_slider.step = 1;

    result = ui_slider_inner(&contrast_slider, contrast_changed_callback);

    if (result == RESULT_UI_CONFIRM)
    {
        g_com_env_var.contrast = contrast_slider.value;
    }

    if ((result == RESULT_NULL) || (result == RESULT_UI_CONFIRM) || (result == RESULT_UI_CANCEL))
    {
        result = RESULT_REDRAW;
    }

    return result;
}
#endif
