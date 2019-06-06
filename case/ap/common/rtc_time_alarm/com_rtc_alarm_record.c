/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：闹钟记录管理模块，闹钟记录存储在VRAM中，需要通过接口进行间接处理，包括读取，
 *       搜索，修改，删除，增加等。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

static rtc_alarm_manager_info_t *g_p_rtc_alarm_manager_info;

static void com_rtc_alarm_ring_handle(void)
{
    uint16 msg;

    msg = MSG_RTCALARM;
    sys_mq_send(MQ_ID_SYS, &msg);
}


/*
函数功能：寻找最近的下一个闹钟
实现方法：遍历闹钟的last_time，找到离此时最近的一个闹钟
*/
uint8 com_search_next_alarm(void)
{
    uint32 tmp_cur_time,tmp_alarm_time;
    time_t alarm_time;
    uint32 tmp_diff_time,tmp_diff_time_min;
    uint8 alarm_index,alarm_available;
    alarm_record_t *p_cur_alarm_record;

    rtc_get_time(&alarm_time);

    //搜索最早打开了的闹钟
    //获取当前时间
    tmp_cur_time = (uint32) (alarm_time.hour * 3600);
    tmp_cur_time += alarm_time.minute * 60;
    tmp_cur_time += alarm_time.second;
    tmp_diff_time_min = 0xffffffff;
    alarm_available = 0xff;

    //遍历闹钟，找到下一个最近的闹钟
    for (alarm_index = 0; alarm_index < ALARM_RECORD_MAX; alarm_index++)
    {
        p_cur_alarm_record = &(g_p_rtc_alarm_manager_info->alarm_records[alarm_index]);

        if (p_cur_alarm_record->alarm.alarm_used == 1)//已使用+打开
        {
            if (p_cur_alarm_record->alarm.alarm_onoff == 1)
            {
                tmp_alarm_time = (uint32) (p_cur_alarm_record->last_time.hour * 3600);
                tmp_alarm_time += p_cur_alarm_record->last_time.minute * 60;
                tmp_alarm_time += p_cur_alarm_record->last_time.second;

                if (tmp_alarm_time > tmp_cur_time)
                {
                    tmp_diff_time = tmp_alarm_time - tmp_cur_time;
                }
                else
                {
                    tmp_diff_time = (tmp_alarm_time + 24 * 3600) - tmp_cur_time;
                }

                if (tmp_diff_time < tmp_diff_time_min)
                {
                    tmp_diff_time_min = tmp_diff_time;
                    alarm_available = alarm_index;
                }
            }
        }
    }

    return alarm_available;
}
/*
函数功能：
找到下一个闹钟之后，
1、如果有效，设置到寄存器
2、无效，关闭闹钟
*/
void com_set_next_alarm(void)
{
    alarm_record_t *p_alarm_record;
    uint8 alarm_available;

    //开始寻找下一个最近的闹钟wanglialarm
    alarm_available = com_search_next_alarm();
    if (alarm_available != 0xff)
    {
        p_alarm_record = &(g_p_rtc_alarm_manager_info->alarm_records[alarm_available]);

        libc_printf("SET ALARM[%02d:%02d:%02d], ALARM TIME IS:%02d:%02d:%02d\n",
                p_alarm_record->alarm.alarm_time.hour,\
                p_alarm_record->alarm.alarm_time.minute, p_alarm_record->alarm.alarm_time.second,
                p_alarm_record->last_time.hour,\
                p_alarm_record->last_time.minute, p_alarm_record->last_time.second);

        rtc_set_alarm_time(&(p_alarm_record->last_time));
        rtc_enable_alarm(com_rtc_alarm_ring_handle);
        g_p_rtc_alarm_manager_info->alarm_idx = alarm_available;
    }
    else//关闭闹钟
    {
        rtc_disable_alarm();
        g_p_rtc_alarm_manager_info->alarm_idx = 0xff;
    }

}


void com_load_rtc_alarm_manager_info(void)
{
    rtc_status_t temp_rtc_status;
    uint8 alarm_index;

    g_p_rtc_alarm_manager_info = (rtc_alarm_manager_info_t *) sys_malloc(sizeof(rtc_alarm_manager_info_t));
    if (g_p_rtc_alarm_manager_info == NULL)
    {
        libc_printf("g_p_rtc_alarm_manager_info\n");
        while (1);
    }

    libc_memset(g_p_rtc_alarm_manager_info, 0, sizeof(rtc_alarm_manager_info_t));
    if (sys_vm_read(g_p_rtc_alarm_manager_info, VM_AP_ALARMINFO, sizeof(rtc_alarm_manager_info_t)) < 0)
    {
        g_p_rtc_alarm_manager_info->magic = 0xffff;
    }

    if (g_p_rtc_alarm_manager_info->magic != VRAM_MAGIC(VM_AP_ALARMINFO))
    {
        g_p_rtc_alarm_manager_info->snooze_enable    = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_SNOOZE_ENABLE);
        g_p_rtc_alarm_manager_info->snooze_time      = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_SNOOZE_TIMER);
        g_p_rtc_alarm_manager_info->snooze_max_count = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_SNOOZE_MAX_TIMES);
        g_p_rtc_alarm_manager_info->overtime         = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_ALARM_OVERTIME);
        g_p_rtc_alarm_manager_info->overtime_deal    = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_ALARM_OVERTIME_DEAL);
        g_p_rtc_alarm_manager_info->alarm_volume     = (uint8) com_get_config_default_inner(SETTING_TIMEALARM_DEFAULT_VOLUME);

        g_p_rtc_alarm_manager_info->magic = VRAM_MAGIC(VM_AP_ALARMINFO);

        for (alarm_index = 0; alarm_index < ALARM_RECORD_MAX; alarm_index++)
        {
            g_p_rtc_alarm_manager_info->alarm_records[alarm_index].alarm.alarm_time.hour = 7 + alarm_index; //闹钟默认时间为早上7：00
        }

        sys_vm_write(g_p_rtc_alarm_manager_info, VM_AP_ALARMINFO, sizeof(rtc_alarm_manager_info_t));
    }

    //闹钟配置检查
    rtc_get_status(&temp_rtc_status);

    //如果尚未开启Calendar，估计是断过电，就会开机马上开启，时间设置为：2018-01-01 00:00:00，闹钟也直接关闭
    if (temp_rtc_status.calendar_en == 0)
    {
        date_t temp_date;
        time_t temp_time;

        rtc_phy_init();

        temp_date.year = 2018;
        temp_date.month = 1;
        temp_date.day = 1;

        temp_time.hour = 0;
        temp_time.minute = 0;
        temp_time.second = 0;

        rtc_set_date(&temp_date);
        rtc_set_time(&temp_time);

        //闹钟直接关闭
        for (alarm_index = 0; alarm_index < ALARM_RECORD_MAX; alarm_index++)
        {
            g_p_rtc_alarm_manager_info->alarm_records[alarm_index].alarm.alarm_onoff = FALSE;
            com_rtc_update_alarm_info(alarm_index, &g_p_rtc_alarm_manager_info->alarm_records[alarm_index].alarm, ALARM_UPDATE_MODIFY);
        }
    }

    //开机使能中断，所以在上电到这里的一小段时间如果闹钟时间到了是无法触发的，有小概率风险！！
    //可以考虑通过时间主动判断是否闹钟时间到了来规避该问题
    //设置下一个闹钟
    com_set_next_alarm();
}

void com_rtc_get_alarm_info(uint8 idx, alarm_info_t *alarm)
{
    alarm_record_t *p_alarm_record = &(g_p_rtc_alarm_manager_info->alarm_records[idx]);

    libc_memcpy(alarm, &(p_alarm_record->alarm), sizeof(alarm_info_t));
}

void com_rtc_update_alarm_info(uint8 idx, alarm_info_t *alarm, uint8 mode)
{
    alarm_record_t *p_alarm_record = &(g_p_rtc_alarm_manager_info->alarm_records[idx]);
    time_t temp_cur_time;

    rtc_get_time(&temp_cur_time);

    update_again:

    if (mode == ALARM_UPDATE_MODIFY)
    {
        if ((alarm != NULL) && (alarm != &(p_alarm_record->alarm))) //开机时检测到异常会重新设置闹钟，指针会相等
        {
            libc_memcpy(&(p_alarm_record->alarm), alarm, sizeof(alarm_info_t));
        }
        p_alarm_record->alarm.alarm_used = 1;
        if (p_alarm_record->alarm.alarm_onoff == TRUE)
        {
            p_alarm_record->snooze = 0;
            libc_memcpy(&(p_alarm_record->last_time), &(p_alarm_record->alarm.alarm_time), sizeof(time_t));
        }
        else
        {
            p_alarm_record->snooze = 0;
            libc_memset(&(p_alarm_record->last_time), 0, sizeof(time_t));
        }
    }
    else if (mode == ALARM_UPDATE_DELETE)
    {
        libc_memset(p_alarm_record, 0, sizeof(alarm_record_t));
    }
    else if (mode == ALARM_UPDATE_OVERTIME)
    {
        if (g_p_rtc_alarm_manager_info->overtime_deal == 0)
        {
            libc_printf_info("ALARM CLOSE!\n");
            mode = ALARM_UPDATE_CLOSE;
        }
        else
        {
            libc_printf_info("ALARM SNOOZE!\n");
            mode = ALARM_UPDATE_SNOOZE;
        }
        goto update_again;
    }
    else if (mode == ALARM_UPDATE_CLOSE)
    {
        p_alarm_record->snooze = 0;
        libc_memcpy(&(p_alarm_record->last_time), &(p_alarm_record->alarm.alarm_time), sizeof(time_t));
    }
    else if (mode == ALARM_UPDATE_SNOOZE)
    {
        if ((g_p_rtc_alarm_manager_info->snooze_enable == 1)
            && (p_alarm_record->snooze < g_p_rtc_alarm_manager_info->snooze_max_count))
        {
            p_alarm_record->last_time.second = temp_cur_time.second;
            p_alarm_record->last_time.minute = (temp_cur_time.minute + g_p_rtc_alarm_manager_info->snooze_time) % 60;
            p_alarm_record->last_time.hour   = temp_cur_time.hour;
            if ((temp_cur_time.minute + g_p_rtc_alarm_manager_info->snooze_time) >= 60)
            {
                p_alarm_record->last_time.hour = (p_alarm_record->last_time.hour + 1) % 24;
            }
            p_alarm_record->snooze++;
        }
        else
        {
            mode = ALARM_UPDATE_CLOSE;
            goto update_again;
        }
    }

    //设置下一个闹钟
    com_set_next_alarm();

    sys_vm_write(g_p_rtc_alarm_manager_info, VM_AP_ALARMINFO, sizeof(rtc_alarm_manager_info_t));
}

void com_rtc_get_alarm_detail_inner(now_alarm_detail_t *p_alarm_detail)
{
    alarm_record_t *p_alarm_record = &(g_p_rtc_alarm_manager_info->alarm_records[g_p_rtc_alarm_manager_info->alarm_idx]);

    libc_memcpy(&(p_alarm_detail->start_time), &(p_alarm_record->last_time), sizeof(time_t));
    p_alarm_detail->overtime = g_p_rtc_alarm_manager_info->overtime;
    p_alarm_detail->volume = g_p_rtc_alarm_manager_info->alarm_volume;
    p_alarm_detail->ring_type = p_alarm_record->alarm.ring_type;
}

void com_rtc_update_alarm_inner(uint8 mode)
{
    com_rtc_update_alarm_info(g_p_rtc_alarm_manager_info->alarm_idx, NULL, mode);
}
