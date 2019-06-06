/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Ӧ�ù���ģ��ӿ�ʵ�֣�����Ӧ��ע��ͳ�ʼ����Ӧ��ע������ȡӦ�ù���ṹ��ȡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

extern void message_key_flush_inner(void);
extern bool com_led_force_exit_msg_show(void);

/*!
 *  \brief
 *  g_app_info_vector ȫ��Ӧ����Ϣ�ṹ������
 */
static app_info_t g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  g_app_info_state_all ȫ��ϵͳ��ǰ״̬�ṹ��
 */
static app_info_state_all_t g_app_info_state_all;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע��ͳ�ʼ����
 * \param[in]    app_id Ӧ��ID���� APP_ID_MUSIC��APP_ID_MANAGER��APP_ID_MENGINE ��
 * \param[in]    type Ӧ�����ͣ��μ� app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע��ͳ�ʼ���ɹ���
 * \retval           FALSE ��ʾע��ͳ�ʼ��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 ����1��ǰ̨Ӧ�� music AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init_inner(APP_ID_MUSIC, APP_TYPE_GUI);

 ����2����̨����Ӧ�� music engine AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init_inner(APP_ID_MENGINE, APP_TYPE_CONSOLE);

 ����3�����̹���Ӧ�� manager AP ���� applib ģ��ע��ͳ�ʼ��
 applib_init_inner(APP_ID_MANAGER, APP_TYPE_PMNG);
 * \endcode
 * \note
 * \li  ��Ӧ�� main ������ڴ����ã��ڴ�֮ǰ������������� applib �ӿڡ�
 *******************************************************************************/
bool applib_init_inner(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    if (type == APP_TYPE_GUI)
    {
        g_app_info_state_all.app_id = app_id;
        g_app_info_state_all.sys_clk_work = FALSE;
        change_ui_status(UI_STATUS_BUSY);
        sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
    }

    //��������ֹ�������
    sys_os_sched_lock();

    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1; //��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    //cur_app_info->soft_watchdog_timeout_expires = 0;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }
    else if (type == APP_TYPE_GUI)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_DESK;
        g_app_info_state_all.app_state = APP_STATE_NO_PLAY; //ǰ̨APע�ᣬ��ʼ��Ϊ NO_PLAY ״̬

        //��� gui ��Ϣ����
        message_key_flush_inner();
        //��� Ӧ����Ϣ����
        sys_mq_flush (MQ_ID_DESK);
        message_redeal_app_msg_inner(NULL);//��NULL��0
    }
    else if (type == APP_TYPE_CONSOLE)
    {
        //����������������״̬
        cur_app_info->mq_id = (uint8) MQ_ID_EGN;
        g_app_info_state_all.engine_type = get_engine_type_by_appid_inner(app_id);
        g_app_info_state_all.engine_state = ENGINE_STATE_NULL;

        //��� Ӧ����Ϣ����
        sys_mq_flush (MQ_ID_EGN);
    }
    else //APP_TYPE_BTENGINE
    {
        cur_app_info->mq_id = (uint8) MQ_ID_BT;
        g_app_info_state_all.bt_state = BT_STATE_WORK;

        //��� Ӧ����Ϣ����
        sys_mq_flush (MQ_ID_BT);
    }

    //�����������������
    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵ�ע������
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʾע���ɹ���
 * \retval           FALSE ��ʾע��ʧ�ܡ�
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 ����1��Ӧ�ý��� applib ģ��ע��
 applib_quit_inner();
 * \endcode
 * \note
 * \li  ��Ӧ�� main �����˳������ã��ڴ�֮�󣬲���������� applib �ӿڡ�
 *******************************************************************************/
bool applib_quit_inner(app_type_e type)
{
    app_info_t *cur_app_info;

    if (type == APP_TYPE_GUI)
    {
        sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
    }

    //��¼��һ���˳���ǰ̨AP���Ա㷵��
    if (type == APP_TYPE_GUI)
    {
        g_app_info_state_all.last_app_id = g_app_info_state_all.app_id;
        g_app_info_state_all.app_id = APP_ID_MAX;
        g_app_info_state_all.sys_clk_work = FALSE;
        g_app_info_state_all.app_state = APP_STATE_NO_PLAY; //ǰ̨APע������ʼ��Ϊ NO_PLAY ״̬
        app_timer_var_init(APP_TIMER_GROUP_GUI);
        com_led_force_exit_msg_show();
        g_app_msg_dispatch_handle = NULL;
    }

    else if (type == APP_TYPE_CONSOLE)
    {
        //�޸ĺ�̨��������Ϊ NONE
        g_app_info_state_all.engine_type = ENGINE_NULL;
        g_app_info_state_all.engine_state = ENGINE_STATE_NULL;
        app_timer_var_init(APP_TIMER_GROUP_CONSOLE);
    }
    else if (type == APP_TYPE_BTENGINE)
    {
        g_app_info_state_all.bt_state = BT_STATE_NONE;
        app_timer_var_init(APP_TIMER_GROUP_BTENGINE);
    }
    else
    {
        //manager��Զ�����˳�
    }

    //�޸� app_info �ṹ��Ϊδ��ʹ��
    sys_os_sched_lock();

    cur_app_info = &g_app_info_vector[type];
    cur_app_info->used = 0;
    cur_app_info->app_id = APP_ID_MAX;

    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ȡ app_info_t
 * \param[in]    type Ӧ�����ͣ��μ� app_type_e ����
 * \param[out]   app_info_t *
 * \return       bool
 * \retval           TRUE ��ʾ��ȡ�ɹ���
 * \retval           FALSE ��ʾ��ȡʧ�ܡ�
 * \ingroup      app_manager
 *******************************************************************************/
bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info)
{
    sys_os_sched_lock();
    libc_memcpy(app_info, &(g_app_info_vector[type]), sizeof(app_info_t));
    sys_os_sched_unlock();

    return TRUE;
}

//����Ӧ�����ͻ�ȡӦ����Ϣ
app_info_t *applib_get_app_info_by_type(app_type_e type)
{
    return &(g_app_info_vector[type]);
}

//����Ӧ��ID��ȡӦ����Ϣ
app_info_t *applib_get_app_info_by_app_id(uint8 app_id)
{
    app_info_t *app;
    uint8 i;

    if (app_id >= APP_ID_MAX)
    {
        return NULL;
    }

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_id == app_id))
        {
            return app;
        }
    }

    return NULL;
}

//��ȡ��̨��������
engine_type_e get_engine_type_inner(void)
{
    return g_app_info_state_all.engine_type;
}

//�޸ĺ�̨��������
void change_engine_type(engine_type_e type)
{
    g_app_info_state_all.engine_type = type;
}

//��ȡ����Э��ջ����״̬
bt_state_e get_bt_state_inner(void)
{
    return g_app_info_state_all.bt_state;
}

tws_mode_e get_tws_mode_inner(void)
{
    return g_app_info_state_all.tws_mode;
}

void change_tws_mode_inner(tws_mode_e mode)
{
    g_app_info_state_all.tws_mode = mode;
}

//��ȡ��̨����״̬
engine_state_e get_engine_state(void)
{
    return g_app_info_state_all.engine_state;
}

//�ĺ�̨����״̬
void change_engine_state_inner(engine_state_e state)
{
    g_app_info_state_all.engine_state = state;
}

//��ȡǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
app_state_e get_app_state(void)
{
    return g_app_info_state_all.app_state;
}

//�޸�ǰ̨Ӧ�� UI ״̬�������ڲ���UI�ͷǲ���UI
void change_app_state_inner(app_state_e state)
{
    g_app_info_state_all.app_state = state;
}

//��ȡ���ǰ̨AP
uint8 get_last_app_id_inner(void)
{
    return g_app_info_state_all.last_app_id;
}

//��ȡ��ǰǰ̨AP
uint8 get_cur_app_id_inner(void)
{
    return g_app_info_state_all.app_id;
}

//��ȡ ui_status
ui_status_e get_ui_status(void)
{
    return g_app_info_state_all.ui_status;
}

//�ı� ui_status
void change_ui_status(ui_status_e ui_sta)
{
    g_app_info_state_all.ui_status = ui_sta;
}

//��ȡ ui_status
bool get_sys_clk_work(void)
{
    return g_app_info_state_all.sys_clk_work;
}

//�ı� ui_status
void change_sys_clk_work(bool sys)
{
    g_app_info_state_all.sys_clk_work = sys;
}

//��ȡ mmm_state
mmm_state_e get_mmm_state_inner(void)
{
    return g_app_info_state_all.mmm_state;
}

//�ı� mmm_state
void change_mmm_state_inner(mmm_state_e mmm_sta)
{
    g_app_info_state_all.mmm_state = mmm_sta;
}

//��ȡ mmm_state
mmm_ktp_engine_t get_ktp_status_inner(void)
{
    return g_app_info_state_all.ktp_status;
}

//�ı� mmm_state
void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta)
{
    g_app_info_state_all.ktp_status = ktp_sta;
}


void applib_var_init(void)
{
    uint8 i;

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        libc_memset(&(g_app_info_vector[i]), 0x00, sizeof(app_info_t));
        g_app_info_vector[i].app_id = APP_ID_MAX;
    }

    libc_memset(&g_app_info_state_all, 0x00, sizeof(g_app_info_state_all));
    g_app_info_state_all.engine_type = ENGINE_NULL;
    g_app_info_state_all.engine_state = ENGINE_STATE_NULL;
    g_app_info_state_all.app_state = APP_STATE_NO_PLAY;
    g_app_info_state_all.bt_state = BT_STATE_NONE;
    g_app_info_state_all.last_app_id = APP_ID_MAX;

    g_app_info_state_all.ui_status = UI_STATUS_BUSY;
    g_app_info_state_all.sys_clk_work = FALSE;
}

/*! \endcond */

