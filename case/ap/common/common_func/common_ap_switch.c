/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用切换功能，支持指定应用切换、循环切换下一个应用、返回上一个应用、ESD断点恢复。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"
#include "config_common.h"
#include "music_common.h"
#include "record_common.h"
#include "common_tws_inner.h"

typedef struct
{
    uint16 active_item; //活动AP菜单项编号
    uint16 total_item; //AP菜单总数
    uint16 ap_funcs[MAX_FUNCTON_CYCLE]; //支持应用编号的数组
} app_switch_config_t;

typedef struct
{
    uint8 ap_id; //当前应用id号,创建应用需要(不同的功能项可以相同,因为一个应用中可以包含多于1个功能项)
    uint8 param; //进入功能项需要的参数
    bool engine_do; //进入应用前是否将其他引擎关闭
    app_result_e result_val; //指定进入到哪个功能项的返回值(直接进入某个功能项)
    uint8 app_func_id; //要进入的功能项的id(配置使用)
} ap_switch_info_t;

static ap_switch_var_t g_ap_switch_var;

//应用所支持的功能项列表
//如果增加一个按mode键可以切换到的功能项,则放在 APP_ID_MAX 前面
//如果增加其他功能项,则放在 APP_ID_MAX 后面
//添加完之后要在config.txt中将该项的FUNC ID添加到数组
const ap_switch_info_t ap_switch_info[] =
{
    //循环功能项,按mode键切换得到的功能项,不能超过 MAX_FUNCTON_CYCLE 个
    { APP_ID_BTPLAY, 0, 1, RESULT_BLUETEETH_SOUND, APP_FUNC_BTPLAY },
    { APP_ID_MUSIC, PARAM_MUSIC_CARD_DIR, 1, RESULT_MUSIC_CPLAY, APP_FUNC_PLAYCARD },
    { APP_ID_MUSIC, PARAM_MUSIC_UHOST_DIR, 1, RESULT_MUSIC_UPLAY, APP_FUNC_PLAYUHOST },
    { APP_ID_MUSIC, PARAM_MUSIC_RES_DIR, 1, RESULT_MUSIC_MDISKPLAY, APP_FUNC_PLAYMDISK },
    { APP_ID_LINEIN, LINEIN_MODE_AUX, 1, RESULT_ENTER_LINEIN, APP_FUNC_PLAYLINEIN },
    { APP_ID_LINEIN, LINEIN_MODE_SPDIF, 1, RESULT_ENTER_SPDIF, APP_FUNC_SPDIF_PLAY },
    //{ APP_ID_LINEIN, LINEIN_MODE_I2SRX, 1, RESULT_ENTER_I2SRX, APP_FUNC_I2SRX_PLAY },
    //{ APP_ID_LINEIN, LINEIN_MODE_HDMI, 1, RESULT_ENTER_HDMI, APP_FUNC_HDMI_HDMI },
    //{ APP_ID_LINEIN, LINEIN_MODE_HDMI1, 1, RESULT_ENTER_HDMI1, APP_FUNC_HDMI_HDMI1 },
    //{ APP_ID_LINEIN, LINEIN_MODE_HDMI2, 1, RESULT_ENTER_HDMI2, APP_FUNC_HDMI_HDMI2 },
    { APP_ID_LINEIN, LINEIN_MODE_ARC, 1, RESULT_ENTER_HDMI_ARC, APP_FUNC_HDMI_ARC },
    { APP_ID_RADIO, 0, 1, RESULT_RADIO, APP_FUNC_RADIO },
    { APP_ID_USOUND, 0, 1, RESULT_USB_SOUND, APP_FUNC_USOUND },

    { APP_ID_MAX, 0, 0, _RESERVE_FOR_COMMON_RESULT_, APP_FUNC_MAX }, //循环功能项分隔符

    /*!以下为快捷键进入的功能选项,它们不在功能循环内,需要返回到上一次的状态*/
    { APP_ID_BTCALL, 0, 1, RESULT_BLUETEETH_HFP, APP_FUNC_BTCALL },
    { APP_ID_BTCALL, 1, 1, RESULT_ENTER_ASQT_TEST, APP_FUNC_BTCALL }, //asqt test
    { APP_ID_CONFIG, CONFIG_MODE_POWER_OFF, 1, RESULT_POWER_OFF, APP_FUNC_CONFIG }, //power off
    { APP_ID_CONFIG, CONFIG_MODE_LOWPOWER, 1, RESULT_LOW_POWER, APP_FUNC_CONFIG }, //low power
    { APP_ID_CONFIG, CONFIG_MODE_ALARM_OFF, 1, RESULT_ALARM_POWER_OFF, APP_FUNC_CONFIG }, //alarm off
    { APP_ID_CONFIG, CONFIG_MODE_CHARGE_LOOP, 1, RESULT_ENTER_CHARGE_LOOP, APP_FUNC_CONFIG }, //charge loop
    { APP_ID_CONFIG, CONFIG_MODE_JUST_OFF, 1, RESULT_JUST_POWER_OFF, APP_FUNC_CONFIG }, //just power off
    { APP_ID_UDISK, 0, 1, RESULT_USB_TRANS, APP_FUNC_UDISK }, //usb trans
    { APP_ID_UPGRADE, DISK_H, 1, RESULT_ENTER_CARD_UPGRADE, APP_FUNC_UPGRADE }, //card upgrade fw
    { APP_ID_UPGRADE, DISK_U, 1, RESULT_ENTER_UHOST_UPGRADE, APP_FUNC_UPGRADE }, //uhost upgrade fw

    { APP_ID_TEST, DISK_C, 1, RESULT_TESTAP_ATD_ENTER, APP_FUNC_TEST }, //auto test ap
    { APP_ID_TEST, DISK_H, 1, RESULT_TESTAP_CARD_ENTER, APP_FUNC_TEST }, //auto test ap

    { APP_ID_BTHID, 0, 1, RESULT_ENTER_PHOTO, APP_FUNC_PHOTO }, //bt hid take photo

    { APP_ID_LINEIN, LINEIN_MODE_MIC_C, 1, RESULT_ENTER_CARD_REC, APP_FUNC_CARD_REC }, //card record
    { APP_ID_LINEIN, LINEIN_MODE_MIC_U, 1, RESULT_ENTER_UHOST_REC, APP_FUNC_UHOST_REC }, //uhost record

    { APP_ID_MUSIC, PARAM_MUSIC_CARD_REPLAY,  1, RESULT_ENTER_CARD_REPLAY, APP_FUNC_CARD_REPLAY },
    { APP_ID_MUSIC, PARAM_MUSIC_UHOST_REPLAY, 1, RESULT_ENTER_UHOST_REPLAY, APP_FUNC_UHOST_REPLAY },

    { APP_ID_ALARM, 0, 1, RESULT_ENTER_ALARM, APP_FUNC_ALARM },

    { 0, 0, 0, RESULT_ESD_RESET, 0 }, //FOR ESD RESET
};

//数组有几个元素
#define ARR_COUNT(arr)  (sizeof(arr) / sizeof((arr)[0]))

#define AP_SWITCH_INFO_COUNT    (ARR_COUNT(ap_switch_info))

static uint8 ap_switch_func_id2index(uint8 app_func_id);
static uint8 switch_seq_func_id2index(uint8 app_func_id);
static bool com_check_function_in(uint8 func_id);
static uint8 com_app_switch_next_func_index(uint8 last_index);
static uint8 com_app_switch_any_func_index(uint8 cur_index);
static void com_send_msg_to_manager(uint32 msg_type, uint8 param1, uint8 param2, bool synch_flag);
static uint8 com_ap_switch_next_index(uint8 last_func_index, app_result_e app_result);
static uint8 com_ap_switch_cycle_count(void);
static uint8 com_ap_switch_default_index(void);

//返回循环功能数量
uint8 com_ap_switch_cycle_count(void)
{
    uint8 i;

    for (i = 0; i < AP_SWITCH_INFO_COUNT; i++)
    {
        if (ap_switch_info[i].ap_id == APP_ID_MAX)
        {
            break;
        }
    }

    if (i > MAX_FUNCTON_CYCLE)
    {
        return MAX_FUNCTON_CYCLE;
    }

    return i;
}

//返回默认功能FUNC ID对应的index；找不到返回0xff
uint8 com_ap_switch_default_index(void)
{
    uint8 i;

    for (i = 0; i < g_ap_switch_var.app_switch_cycle_count; i++)
    {
        if (ap_switch_info[i].app_func_id == g_ap_switch_var.app_default_func_id)
        {
            return i;
        }
    }

    return 0xff;
}

//检查是否支持某个应用
bool com_ap_switch_check_inner(uint8 app_func_id)
{
    return ((g_ap_switch_var.app_func_support & (1 << app_func_id)) != 0);
}

//返回下一个Func id
uint8 com_fast_preview_ask_next_func(uint8 last_func_index, app_result_e app_result, uint8 *next_func_index)
{
    uint8 next_index;

    next_index = com_ap_switch_next_index(last_func_index, app_result);
    *next_func_index = next_index;
    if (next_index < AP_SWITCH_INFO_COUNT)
    {
        return ap_switch_info[next_index].app_func_id;
    }

    *next_func_index = g_ap_switch_var.app_default_func_index;
    return g_ap_switch_var.app_default_func_id;
}
//返回即将切换到哪个FUNCTION
uint8 com_ap_switch_ask_next_func_inner(app_result_e app_result)
{
    uint8 next_func_index;
    return com_fast_preview_ask_next_func(get_cur_func_index(), app_result, &next_func_index);
}

/******************************************************************************/
/*!
 * \par  Description:
 *\ ap切换功能
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
void com_ap_switch_deal_inner(app_result_e app_result)
{
    msg_apps_t msg;
    uint8 deal_ap_id;
    uint8 deal_func_id;
    uint8 ap_param;
    uint8 cur_index; //ap_func_table[cur_index]即要切入的APP_FUNC 的ID
    //(只在最后,cur_index变成了ap_switch_info的下标)
    uint8 last_index; //对应到ap_func_table的下标
    uint8 default_index = 0; //对应到ap_func_table数组的下标

    //得到缺省APP_FUNC在ap_switch_info数组中的下标
    default_index = g_ap_switch_var.app_default_func_index;

    //当前的index 作为上一次的保存
    last_index = get_cur_func_index();

    if ((app_result == RESULT_BLUETEETH_HFP) || (app_result == RESULT_ENTER_ALARM))
    {
        g_p_global_var->bg_app_sta = BG_STATUS_IN;
    }
    else
    {
        if (g_p_global_var->bg_app_sta == BG_STATUS_IN)
        {
            g_p_global_var->bg_app_sta = BG_STATUS_BACK;
        }
        else
        {
            g_p_global_var->bg_app_sta = BG_STATUS_NULL;
        }
    }

    //快速预览模式已经确定要切换到哪个应用
    g_p_global_var->fp_switch_jump_welcome = FALSE;
    if (app_result == RESULT_FAST_PREVIEW_SWITCH_AP)
    {
        g_p_global_var->fp_switch_jump_welcome = TRUE;
    }

    //找一些检查，如果不行，cur_index就会变成下一个满足条件的，
    //但cur_index仍然是ap_switch_info数组里面的下标
    cur_index = com_ap_switch_next_index(last_index, app_result);

    //for valid set
    if (cur_index >= AP_SWITCH_INFO_COUNT)
    {
        //check index
        cur_index = default_index;
        last_index = default_index;
    }

    if (ap_switch_info[cur_index].engine_do == TRUE)
    {
        //check当前引擎，关闭
        if (get_engine_type_inner() != ENGINE_NULL)
        {
            //杀死引擎
            msg.type = MSG_KILL_APP_SYNC;
            msg.para[0] = APP_ID_THEENGINE;
            //发送同步消息
            send_sync_msg_inner(APP_ID_MANAGER, &msg, NULL, 0);
        }
    }

    //保存当前的index,作为上一次的序号
    set_last_func_index(last_index);

    //save cur function index
    set_cur_func_index(cur_index);

    //save cur function id
    set_cur_func_id_inner(ap_switch_info[cur_index].app_func_id);

    libc_printf_info("ap switch func:%d\n", ap_switch_info[cur_index].app_func_id);

    deal_ap_id = ap_switch_info[cur_index].ap_id;
    deal_func_id = ap_switch_info[cur_index].app_func_id;
    ap_param = ap_switch_info[cur_index].param;

#if (SUPPORT_HDMI_MODULE == 1)
    if ((deal_func_id != APP_FUNC_CONFIG)
        && (deal_func_id != APP_FUNC_UPGRADE)
        && (deal_func_id != APP_FUNC_TEST)
        && (deal_func_id != APP_FUNC_BTCALL))
    {
        hdmi_switch_input(deal_func_id);
    }
#endif

    //try vram merge
    com_vram_merge_inner();

    //如果支持后台蓝牙，只有 ap_config 关机，ap_cardreader, ap_usound 与 bt engine 冲突，需要杀死 bt engine，
    //其他应用，除了 ap_btplay ，ap_btcall，ap_bthid，ap_test 在进入到应用后创建之外，需要在这里先创建 bt engine
    if (g_com_comval.support_bt_inback == 1)
    {
        bool bt_inback_conflict_flag = FALSE;
        bool bt_inback_create_self_flag = FALSE;

        if ((deal_ap_id == APP_ID_CONFIG) || (deal_ap_id == APP_ID_UDISK) || (deal_ap_id == APP_ID_UPGRADE))
        {
            bt_inback_conflict_flag = TRUE;
        }

        if ((deal_ap_id == APP_ID_BTPLAY) || (deal_ap_id == APP_ID_BTCALL) || (deal_ap_id == APP_ID_BTHID)
            || (deal_ap_id == APP_ID_TEST))
        {
            bt_inback_create_self_flag = TRUE;
        }

        if (get_bt_state_inner() == BT_STATE_WORK)
        {
            if (bt_inback_conflict_flag == TRUE)
            {
                com_close_bt_engine_inner();
            }
        }
        else
        {
            if ((bt_inback_conflict_flag == FALSE) && (bt_inback_create_self_flag == FALSE))
            {
                com_creat_bt_engine_inner(0);
            }
        }
    }

    //try vram merge
    com_vram_merge_inner();

    //向manager发消息创建前台应用
    com_send_msg_to_manager(MSG_CREAT_APP, deal_ap_id, ap_param, FALSE);

    //正常应用切换时清除ESD RESET FLAG；应用断点备份
    if (app_result != RESULT_ESD_RESET)
    {
        g_p_esd_bk_info->reset_flag = 0;

        g_p_esd_bk_info->flag = ESD_BK_FLAG;
        g_p_esd_bk_info->app_id = deal_ap_id;
        g_p_esd_bk_info->func_id = ap_switch_info[cur_index].app_func_id;
        g_p_esd_bk_info->func_index = cur_index;
        g_p_esd_bk_info->app_para = ap_param;
    }
}

//返回：app_func_id在ap_switch_info数组里面的下标；如果没找到，就返回app_default_func_index
static uint8 ap_switch_func_id2index(uint8 app_func_id)
{
    uint8 i;

    for (i = 0; i < AP_SWITCH_INFO_COUNT; i++)
    {
        if (ap_switch_info[i].app_func_id == app_func_id)
        {
            return i;
        }
    }

    return g_ap_switch_var.app_default_func_index;
}

//返回：app_func_id是“config.txt的SETTING_APP_SWITCH_SEQUENCE项”中的第几个；如果没找到，就返回0xff
static uint8 switch_seq_func_id2index(uint8 app_func_id)
{
    uint8 i;

    for (i = 0; i < g_ap_switch_var.app_switch_count; i++)
    {
        if (g_ap_switch_var.app_switch_seq[i] == app_func_id)
        {
            return i;
        }
    }

    return 0xff;
}

/******************************************************************************/
/*!
 * \par  Description:当前功能项硬件是否ok，能否进入
 *\ check cur app function hardware is ready and can realy enter
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/

static bool com_check_function_in(uint8 func_id)
{
    //check if hard ware is not ready
    switch (func_id)
    {
        case APP_FUNC_PLAYCARD:
        //卡不存在
        if (get_card_state_inner() != CARD_STATE_CARD_IN)
        {
            return FALSE;
        }
        break;

        case APP_FUNC_PLAYUHOST:
        //UHOST不存在
        if (get_uhost_state_inner() != UHOST_IN)
        {
            return FALSE;
        }

        //如果是phone则不切换应用
#if (SUPPORT_PHONE_CHARGING == 1)
        if ((g_com_comval.support_iphonecharge == 1) && (get_uhost_type_inner() != UDEVICE_TYPE))
        {
            return FALSE;
        }
#endif

        if (com_get_stub_tools_type_inner() != 0)
        {
            return FALSE;
        }
        break;

        case APP_FUNC_PLAYLINEIN:
        //linein不在
        if (get_linein_state_inner() != LINEIN_STATE_LINEIN_IN)
        {
            return FALSE;
        }
        break;

        case APP_FUNC_USOUND:
        //当前无USB线插入
        if (get_cable_state_inner() != CABLE_STATE_CABLE_IN)
        {
            return FALSE;
        }

        if (com_get_stub_tools_type_inner() != 0)
        {
            return FALSE;
        }
        break;

        default:
        break;
    }

    return TRUE;
}

//根据config.txt切换到下一个APP_FUNC
//返回：该APP_FUNC在ap_switch_info数组里面的下标
//last_index: 在ap_switch_info数组里面的下标
static uint8 com_app_switch_next_func_index(uint8 last_index)
{
    uint8 last_switch_seq_index;
    uint8 next_func_id;

    if (last_index >= g_ap_switch_var.app_switch_cycle_count)
    {
        last_index = g_ap_switch_var.app_default_func_index;
        last_switch_seq_index = g_ap_switch_var.app_default_seq_index;

        goto check_default_seq_index;
    }

    //是“config.txt的SETTING_APP_SWITCH_SEQUENCE项”中的第几个
    last_switch_seq_index = switch_seq_func_id2index(ap_switch_info[last_index].app_func_id);
    if (last_switch_seq_index == 0xff)
    {
        last_index = g_ap_switch_var.app_default_func_index;
        last_switch_seq_index = g_ap_switch_var.app_default_seq_index;
        if (last_switch_seq_index == 0xff)
        {
            libc_printf_error("app switch error!!\n");
            while (1)
            {
                ; //nothing for QAC
            }
        }
    }

    find_next_index:
    //按照config.txt切到下一个APP_FUNC
    last_switch_seq_index++;

    if (last_switch_seq_index >= g_ap_switch_var.app_switch_count)
    {
        last_switch_seq_index = 0;
    }

    check_default_seq_index:

    next_func_id = g_ap_switch_var.app_switch_seq[last_switch_seq_index];
    if (((g_ap_switch_var.app_func_support & (uint32)(1 << next_func_id)) == 0)
            || (com_check_function_in(next_func_id) == FALSE))
    {
        goto find_next_index;
    }

    //返回该APP_FUNC在ap_switch_info数组里面的下标
    return ap_switch_func_id2index(next_func_id);
}

//cur_index是ap_switch_info数组里面的下标
//这里做了一些检查
//返回值仍然是ap_switch_info数组里面的下标
static uint8 com_app_switch_any_func_index(uint8 cur_index)
{
    uint8 cur_switch_seq_index;

    if (cur_index >= AP_SWITCH_INFO_COUNT)
    {
        cur_index = g_ap_switch_var.app_default_func_index;
    }

    if ((g_ap_switch_var.app_func_support & (uint32)(1 << ap_switch_info[cur_index].app_func_id)) == 0)
    {
        cur_index = g_ap_switch_var.app_default_func_index;
    }

    if (cur_index < g_ap_switch_var.app_switch_cycle_count)
    {
        //cur_switch_seq_index表示config.txt里面的第几个APP_FUNC
        cur_switch_seq_index = switch_seq_func_id2index(ap_switch_info[cur_index].app_func_id);
        if (cur_switch_seq_index == 0xff)
        {
            cur_switch_seq_index = g_ap_switch_var.app_default_seq_index;
            cur_index = g_ap_switch_var.app_default_func_index;
        }

        if (((g_ap_switch_var.app_func_support & (uint32)(1 << ap_switch_info[cur_index].app_func_id)) == 0)
                || (com_check_function_in(g_ap_switch_var.app_switch_seq[cur_switch_seq_index]) == FALSE))
        {
            cur_index = com_app_switch_next_func_index(cur_index);
        }
    }

    return cur_index;
}

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:发送消息给manager
 *\ check cur app function hardware is ready and can realy enter
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
static void com_send_msg_to_manager(uint32 msg_type, uint8 param1, uint8 param2, bool synch_flag)
{
    msg_apps_t msg;

    //message type
    msg.type = msg_type;
    //param
    msg.para[0] = param1;

    msg.para[1] = param2;
    //whether sych message
    if (synch_flag == TRUE)
    {
        //发送同步消息
        send_sync_msg_inner(APP_ID_MANAGER, &msg, NULL, 0);
    }
    else
    {
        //msg.content.data[1] = param2;
        //发送异步消息
        send_async_msg_inner(APP_ID_MANAGER, &msg);
    }
}

//返回即将切换到哪个INDEX
static uint8 com_ap_switch_next_index(uint8 last_func_index, app_result_e app_result)
{
    uint8 cur_index; //ap_func_table[cur_index]即要切入的APP_FUNC 的ID
    //(只在最后,cur_index变成了ap_switch_info的下标)
    uint8 last_index; //对应到ap_func_table的下标
    uint8 default_index = 0; //对应到ap_func_table数组的下标

    //得到缺省APP_FUNC在ap_switch_info数组中的下标
    default_index = g_ap_switch_var.app_default_func_index;

    //当前的index 作为上一次的保存
    last_index = last_func_index;

    result_dispatch:

    //快速预览模式已经确定要切换到哪个应用
    if (app_result == RESULT_FAST_PREVIEW_SWITCH_AP)
    {
        return g_p_global_var->fp_switch_func_index;
    }
    else if ((app_result == RESULT_ESD_RESET) && (g_p_esd_bk_info->reset_flag == 1))
    {
        //ESD RESET 回来重启前应用
        if (g_p_esd_bk_info->app_id == APP_ID_BTCALL)
        {//蓝牙打电话应用先进入蓝牙推歌，然后连接起来后会自动切换
            app_result = RESULT_BLUETEETH_SOUND;
            goto result_dispatch;
        }
        cur_index = g_p_esd_bk_info->func_index;
    }
    else if (app_result == RESULT_LASTPLAY)
    {
        //上一次的apindex
        cur_index = get_last_func_index();
        if (cur_index >= g_ap_switch_var.app_switch_cycle_count)
        {
            cur_index = g_ap_switch_var.app_default_seq_index;
        }
    }
    else if (app_result == RESULT_NEXT_FUNCTION)
    {
        //进入下一个应用
        //cur_index是按照config.txt切换APP_FUNC后，这个APP_FUNC在ap_switch_info数组里面的下标
        cur_index = com_app_switch_next_func_index(last_index);
    }
    else
    {
        //根据返回值确定索引值
        for (cur_index = 0; cur_index < AP_SWITCH_INFO_COUNT; cur_index++)
        {
            //check enter which one
            if (ap_switch_info[cur_index].result_val == app_result)
            {
                break;
            }
        }

        if (cur_index >= AP_SWITCH_INFO_COUNT)
        {
            cur_index = default_index;
        }
    }

    //找一些检查，如果不行，cur_index就会变成下一个满足条件的，
    //但cur_index仍然是ap_switch_info数组里面的下标
    cur_index = com_app_switch_any_func_index(cur_index);

    return cur_index;
}

//加载并初始化应用管理配置项
void common_load_app_config(void)
{
    app_switch_config_t app_switch_config;
    uint8 i, j, valid_index;

    libc_memset(&g_ap_switch_var, 0x00, sizeof(ap_switch_var_t));

    com_get_config_struct_inner(SETTING_APP_SWITCH_SEQUENCE, (uint8 *) &app_switch_config, sizeof(app_switch_config_t));

    g_ap_switch_var.app_switch_cycle_count = com_ap_switch_cycle_count();

    g_ap_switch_var.app_switch_count = app_switch_config.total_item;
    if (g_ap_switch_var.app_switch_count > g_ap_switch_var.app_switch_cycle_count)
    {
        g_ap_switch_var.app_switch_count = g_ap_switch_var.app_switch_cycle_count;
    }

    valid_index = 0;
    for (i = 0; i < g_ap_switch_var.app_switch_count; i++)
    {
        bool valid_func_id = FALSE;

        for (j = 0; j < g_ap_switch_var.app_switch_cycle_count; j++)
        {
            if (ap_switch_info[j].app_func_id == (uint8) app_switch_config.ap_funcs[i])
            {
                valid_func_id = TRUE;
                break;
            }
        }

        if (valid_func_id == TRUE)
        {
            g_ap_switch_var.app_switch_seq[valid_index] = (uint8) app_switch_config.ap_funcs[i];
            valid_index++;
        }
    }
    if (valid_index == 0)
    {
        libc_printf_error("NO find one valid app!!\n");
        while (1);
    }
    g_ap_switch_var.app_switch_count = valid_index;

    g_ap_switch_var.app_func_support = 0xffffffff;

    //卡播歌支持
#if (SUPPORT_CARD_HARDWARE == 1)
    if (((uint8) com_get_config_default_inner(SETTING_HARDWARE_SUPPORT_CARD) == 0)
            || ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_CARDPLAY) == 0))
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYCARD));
    }
#else
    g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYCARD));
#endif
    //U盘播歌支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_UHOSTPLAY) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYUHOST));
    }
    //主盘播歌支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_MDISKPLAY) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYMDISK));
    }
    //LINEIN播歌支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_LINEIN) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_PLAYLINEIN));
    }
    //SPDIF播歌支持
#if (SUPPORT_SPDIF_PLAY == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_SPDIF_PLAY));
    }
#endif
    //HDMI播歌支持
#if (SUPPORT_HDMI_MODULE == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_HDMI_HDMI));
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_HDMI_HDMI1));
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_HDMI_HDMI2));
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_HDMI_ARC));
    }
#endif
#if (SUPPORT_FM_FUNC == 1)
    //FM 收音机支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_FMRADIO) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_RADIO));
    }
#else
    g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_RADIO));
#endif
    //蓝牙播歌支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_BTPLAY) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_BTPLAY));
    }
    //蓝牙打电话支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_BTCALL) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_BTCALL));
    }
    //USB音箱支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_USBBOX) == 0)
    {
        g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_USOUND));
    }
    //大容量磁盘支持
    if ((uint8) com_get_config_default_inner(SETTING_APP_SUPPORT_USBMSC) == 0)
    {
        //没有强制使能读卡器模式
        if (g_p_esd_bk_info->force_enable_udisk != 1)
        {
            g_ap_switch_var.app_func_support &= (~(1 << APP_FUNC_UDISK));
        }
    }

    //检查app_switch_seq是否合法，即所有打开的循环模式应用都在app_switch_seq中，如果不在会切不到这些循环应用
    for (i = 0; i < AP_SWITCH_INFO_COUNT; i++)
    {
        uint8 func_id;
        bool valid_func_id = FALSE;

        if (ap_switch_info[i].ap_id == APP_ID_MAX)
        {
            break;
        }

        func_id = ap_switch_info[i].app_func_id;
        if ((g_ap_switch_var.app_func_support & (1<<func_id)) != 0)
        {
            for (j = 0; j < g_ap_switch_var.app_switch_count; j++)
            {
                if (func_id == g_ap_switch_var.app_switch_seq[j])
                {
                    valid_func_id = TRUE;
                    break;
                }
            }

            if (valid_func_id == FALSE)
            {
                g_ap_switch_var.app_func_support &= ~(1<<func_id);
                libc_printf_warning("Func id %d NOT in SETTING_APP_SWITCH_SEQUENCE!! Please add it!!\n", func_id);
            }
        }
    }

    //确定默认应用
    g_ap_switch_var.app_default_func_id = (uint8) app_switch_config.active_item;
    if ((g_ap_switch_var.app_func_support & (1<<g_ap_switch_var.app_default_func_id)) == 0)
    {
        for (i = 0; i < g_ap_switch_var.app_switch_count; i++)
        {
            if ((g_ap_switch_var.app_func_support & (1<<g_ap_switch_var.app_switch_seq[i])) != 0)
            {
                g_ap_switch_var.app_default_func_id = g_ap_switch_var.app_switch_seq[i];
                g_ap_switch_var.app_default_seq_index = i;
                break;
            }
        }
        if (i == g_ap_switch_var.app_switch_count)
        {
            libc_printf_error("NO find one normal app!!\n");
            while (1);
        }
    }
    g_ap_switch_var.app_default_func_index = com_ap_switch_default_index();
    if (g_ap_switch_var.app_default_func_index == 0xff)
    {
        libc_printf_error("NO find one normal app INDEX!!\n");
        while (1);
    }
}
