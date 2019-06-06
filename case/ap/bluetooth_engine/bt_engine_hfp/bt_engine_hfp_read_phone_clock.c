/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenuµÄmainº¯ÊýÄ£¿é
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "./../bluetooth_engine.h"

bt_read_phone_time_t bt_read_phone_time;

bt_read_phone_time_t *hfp_get_bt_read_phone_time(void)
{
    return &bt_read_phone_time;
}

static uint8 isquotes(uint8 ch)
{
    if (ch == '"')
    {
        return 1;
    }
    return 0;
}

static uint8 isslash(uint8 ch)
{
    if (ch == '/')
    {
        return 1;
    }
    return 0;
}

static uint8 iscolon(uint8 ch)
{
    if (ch == ':')
    {
        return 1;
    }
    return 0;
}

static uint8 iscomma(uint8 ch)
{
    if (ch == ',')
    {
        return 1;
    }
    return 0;
}

static bool at_cclk_get_year(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->date.year = (p_str[0] - '0') * 10 + (p_str[1] - '0') + 2000;
    HFP_DBG_PRINTF("\n<hfp> phone clock\nyear:%d\n",p_time->date.year);
    return TRUE;
}

static bool at_cclk_get_month(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->date.month = (p_str[0] - '0') * 10 + (p_str[1] - '0');
    HFP_DBG_PRINTF("month:%d\n",p_time->date.month);

    if ((p_time->date.month <= 0) || (p_time->date.month > 12))
    {
        HFP_PRINTF_WARNNING("phone clock parse err. \nmonth beyong date range\n");
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    return TRUE;
}

static bool at_cclk_get_day(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->date.day = (p_str[0] - '0') * 10 + (p_str[1] - '0');
    HFP_DBG_PRINTF("day:%d\n",p_time->date.day);

    if ((p_time->date.day <= 0) || (p_time->date.day > 31))
    {
        HFP_PRINTF_WARNNING("phone clock parse err. \nday beyong date range\n");
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    return TRUE;
}

static bool at_cclk_get_hour(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->time.hour = (p_str[0] - '0') * 10 + (p_str[1] - '0');
    HFP_DBG_PRINTF("hour:%d\n",bt_read_phone_time.time.hour);

    if ((p_time->time.hour <= 0) || (p_time->time.hour > 24))
    {
        HFP_PRINTF_WARNNING("phone clock parse err. \nhour beyong date range\n");
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }
    return TRUE;
}

static bool at_cclk_get_minute(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->time.minute = (p_str[0] - '0') * 10 + (p_str[1] - '0');
    HFP_DBG_PRINTF("min:%d\n",p_time->time.minute);

    if ((p_time->time.minute <= 0) || (p_time->time.minute > 60))
    {
        HFP_PRINTF_WARNNING("phone clock parse err. \nminute beyong date range\n");
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }
    return TRUE;
}

static bool at_cclk_get_second(bt_read_phone_time_t *p_time, uint8 *p_str)
{
    if ((isdigit(p_str[0]) == 0) || (isdigit(p_str[1]) == 0))
    {
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }

    p_time->time.second = (p_str[0] - '0') * 10 + (p_str[1] - '0');
    HFP_DBG_PRINTF("sec:%d\n",bt_read_phone_time.time.second);

    if ((p_time->time.second <= 0) || (p_time->time.second > 60))
    {
        HFP_PRINTF_WARNNING("phone clock parse err. \nsecond beyong date range\n");
        p_time->replay = RESP_PARSE_ERR;
        return FALSE;
    }
    return TRUE;
}

void parse_hfp_cclk_cmd_param(uint8 *buf)
{
    int8 ret, i;
    uint8 *p_cmd = buf + 2; //at cmd reply start with <CR>(\r\n)

    bt_read_phone_time_t *p_time_reply = hfp_get_bt_read_phone_time();

    ret = libc_memcmp(p_cmd, "+CCLK", 5);
    if (ret != 0)
    {
        return;
    }

    if (p_time_reply->replay == REQ_DATA_INVALID)
    {
        //libc_printf("\n<hfp> clcc resp\n");
        p_time_reply->replay = RESP_CMD_NOT_SUPPORT;
    }
    else
    {
        return;
    }

    i = 0;
    //"18/01/31, 11:29:55"
    while (p_cmd[i] != 0)
    {
        //may this a valid head
        if (isquotes(p_cmd[i]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_year(p_time_reply, &p_cmd[i + 1]) == FALSE)
        {
            goto continue_count;
        }

        if (isslash(p_cmd[i + 3]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_month(p_time_reply, &p_cmd[i + 4]) == FALSE)
        {
            goto continue_count;
        }

        if (isslash(p_cmd[i + 6]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_day(p_time_reply, &p_cmd[i + 7]) == FALSE)
        {
            goto continue_count;
        }

        if (iscomma(p_cmd[i + 9]) == 0)
        {
            goto continue_count;
        }

        if (isspace(p_cmd[i+10]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_hour(p_time_reply, &p_cmd[i + 11]) == FALSE)
        {
            goto continue_count;
        }

        if (iscolon(p_cmd[i + 13]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_minute(p_time_reply, &p_cmd[i + 14]) == FALSE)
        {
            goto continue_count;
        }

        if (iscolon(p_cmd[i + 16]) == 0)
        {
            goto continue_count;
        }

        if (at_cclk_get_second(p_time_reply, &p_cmd[i + 17]) == FALSE)
        {
            goto continue_count;
        }

        if (isquotes(p_cmd[i + 19]) == 0)
        {
            goto continue_count;
        }

        //all info parse success
        p_time_reply->replay = RESP_DATA_VALID;
        break;
        continue_count: i++;
    }
}

