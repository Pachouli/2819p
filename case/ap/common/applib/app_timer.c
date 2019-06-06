/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������ʱ��ģ��ӿ�ʵ�֣������������޸ģ�ɾ�������ã�handleִ�У���ʼ���Ƚӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib_inner.h"

static app_timer_group_t g_app_timer_group_vector[APP_TIMER_GROUP_MAX];

//COMMON ģ���ڲ������ʱ�����鶨��
#define COMMON_APP_TIMER_COUNT  8
static app_timer_t *g_p_com_app_timer_vector;

/*! \cond COMMON_API */

//��ʱ����ʼ��
void app_timer_var_init(app_timer_group_id_e group_id)
{
    if (group_id >= APP_TIMER_GROUP_MAX)
    {
        libc_memset(g_app_timer_group_vector, 0x00, sizeof(g_app_timer_group_vector));
        if (g_p_com_app_timer_vector == NULL)
        {
            g_p_com_app_timer_vector = (app_timer_t *) sys_malloc(sizeof(app_timer_t)*COMMON_APP_TIMER_COUNT);
        }
    }
    else
    {
        if (group_id != APP_TIMER_GROUP_COMMON)
        {
            if (g_app_timer_group_vector[group_id].vector != NULL)
            {
                sys_free(g_app_timer_group_vector[group_id].vector);
            }
        }
        libc_memset(&g_app_timer_group_vector[group_id], 0x00, sizeof(app_timer_group_t));
    }
}

//COMMON��ʱ����ʼ��
void app_timer_common_group_init(void)
{
    init_app_timers_inner(APP_TIMER_GROUP_COMMON, COMMON_APP_TIMER_COUNT);
}

//�޸�ǰ̨��COMMON��ʱ��������־
void change_app_timer_tag_inner(uint8 tag)
{
    g_app_timer_group_vector[APP_TIMER_GROUP_GUI].tag = tag;
    g_app_timer_group_vector[APP_TIMER_GROUP_COMMON].tag = tag;
}

//��ȡ�Ƿ�������ʱ��ִ�е���
bool get_app_timer_in_handle(app_timer_group_id_e group_id)
{
    return g_app_timer_group_vector[group_id].in_handle;
}

static inline int8 __set_timer(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, timer_type_e type, uint16 timeout, timer_proc func_proc);

/******************************************************************************/
/*!
 * \par  Description:
 *    ������ͨӦ�ü���ʱ����
 * \param[in]    attrb ��ʱ�����ԣ�������һ�ַ��࣬�� control �� ui ��
 * \param[in]    timeout ��ʱ�����ڣ��� 1ms Ϊ��λ������Ϊ 0
 * \param[in]    func_proc ��ʱ���������̣�����Ϊ NULL
 * \param[out]   none
 * \return       int8
 * \retval           id ����ɹ������ض�ʱ��ID����ΧΪ 0 ~ count - 1��
 * \retval           -1 ����ʧ�ܣ���ʱ����Դ���꣬�� timeout Ϊ0���� proc ΪNULL��
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1������ 80ms �ַ���������ʱ��
 //������ʱ�� ISR
 static void __scroll_string_handle(void)
 {
 ui_show_listbox(NULL, NULL, LIST_DRAW_ACTIVE_SCROLL);
 }

 int8 scroll_timer = -1;
 //���� UI�� Ӧ�ü���ʱ����������ʱ������
 scroll_timer = set_app_timer_inner(APP_TIMER_ATTRB_UI, 80, __scroll_string_handle);
 if(scroll_timer == -1)
 {
 ����ʧ�ܣ�ԭ��Ϊ��ʱ���ѱ�������
 }

 //ɾ����ЧӦ�ü���ʱ��
 if(scroll_timer != -1)
 {
 kill_app_timer_inner(scroll_timer);
 scroll_timer = -1;
 }
 * \endcode
 * \note
 * \li  ��ͨ���͵���ʱ����ÿ�ζ�ʱ�������ú��´ζ�ʱʱ�䣬Ȼ��ִ�� ISR���Զ�����ִ�У�ֱ���û�����
 *      stop_app_timer_inner ���� kill_app_timer��
 * \li  ���� set_app_timer_inner ������ʱ���󣬱����ڲ���ʹ��ʱ���� kill_app_timer_inner ɾ��֮����������ڴ�й©��
 * \li  timeout����Ϊ0��func_proc����Ϊ�գ����򷵻�ʧ�ܡ�
 *******************************************************************************/
int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc)
{
    return __set_timer(group_id, tag, attrb, TIMER_TYPE_NORMAL, timeout, func_proc);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��������Ӧ�ü���ʱ����
 * \param[in]    attrb ��ʱ�����ԣ�������һ�ַ��࣬�� control �� ui ��
 * \param[in]    timeout ��ʱ�����ڣ��� 1ms Ϊ��λ
 * \param[in]    func_proc ��ʱ����������
 * \param[out]   none
 * \return       int8
 * \retval           id ����ɹ������ض�ʱ��ID����ΧΪ 0 ~ count - 1��
 * \retval           -1 ����ʧ�ܣ���ʱ����Դ���꣬�� timeout Ϊ0���� proc ΪNULL��
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1������ 5s ��ʱ��ʱ������������ĳ���¼�
 //5s ��ʱ��ʱ�� ISR���ñ�־ g_timeout_flagd
 static void __timeout_handle(void)
 {
 g_timeout_flagd = TRUE;
 }

 bool g_timeout_flagd = FALSE;//ȫ�ֱ���
 int8 timeout_timer = -1;
 //���� control�� Ӧ�ü���ʱ��
 timeout_timer = set_single_shot_app_timer_inner(APP_TIMER_ATTRB_UI, 5000, __timeout_handle);
 if(timeout_timer == -1)
 {
 ����ʧ�ܣ�ԭ��Ϊ��ʱ���ѱ�������
 }

 //ɾ����ЧӦ�ü���ʱ��������ܹ�ȷ��������ʱ����ִ�е�����ô�Ͳ���Ҫִ��ɾ��������
 if(timeout_timer != -1)
 {
 kill_app_timer_inner(timeout_timer);//����ڴ�֮ǰ������ʱ��������ɾ�����᷵�ش��󣬵��ǲ������
 timeout_timer = -1;
 }
 * \endcode
 * \note
 * \li  ����Ӧ�ü���ʱ����Ψһһ�� ISR ִ��ʱ�ͱ��Զ����٣��û����Բ����Լ����١�
 * \li  �������ֶ�ʱ��Ҳ�����⣬�����û�������ȫ���ն�ʱ�����������ڣ�����ȷ����ʱ��
 *      �Ƿ������ˣ��������п����ظ�������ͬ�Ķ�ʱ����
 * \li  timeout����Ϊ0��func_proc����Ϊ�գ����򷵻�ʧ�ܡ�
 *******************************************************************************/
int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc)
{
    return __set_timer(group_id, tag, attrb, TIMER_TYPE_SINGLE_SHOT, timeout, func_proc);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �޸�Ӧ�ü���ʱ���Ķ�ʱ���ڡ�
 * \param[in]    timer_id ��ʱ��ID
 * \param[in]    timeout ���µĶ�ʱ�����ڣ��� 1ms Ϊ��λ
 * \param[out]   none
 * \return       bool
 * \retval           TRUE �޸Ķ�ʱ���ڳɹ���
 * \retval           FALSE �޸Ķ�ʱ����ʧ�ܣ����Ҳ���ָ��ID�Ķ�ʱ������ timeout �Ƿ���
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1���޸Ķ�ʱ����ʱʱ��Ϊ 5s��֮�����¼�ʱ
 if(modify_app_timer_inner(timer_id, 5000) == FALSE)
 {
 �޸�ʧ��
 }
 * \endcode
 * \note
 * \li  �޸���ʱ���Ķ�ʱ���ں󣬻����¿�ʼ��ʱ��
 * \li  timerout����Ϊ0�����򷵻�ʧ�ܡ�
 * \li  timer_id ����0 ~ count - 1�����з���FALSE��
 *******************************************************************************/
bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //����Խ��
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    if (0 == timeout)
    {
        return FALSE;
    }

    tm->timeout = timeout;
    tm->timeout_expires = sys_get_ab_timer() + timeout;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ֹͣӦ�ü���ʱ����ʱ������ʱ��״̬��Ϊ TIMER_STATE_STOPED��
 * \param[in]    timer_id ��ʱ��ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ֹͣ��ʱ�ɹ���
 * \retval           FALSE ֹͣ��ʱʧ�ܣ�ԭ��Ϊ�Ҳ���ָ��ID�Ķ�ʱ����
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1��ֹͣ��ʱ����ʱ��֮���ͨ�� restart_app_timer_inner �ָ�
 if(stop_app_timer_inner(timer_id) == FALSE)
 {
 ֹͣʧ��
 }
 * \endcode
 * \note
 * \li  ֹͣ��ʱ�����ٻָ���ֹͣʱ��������ʱ��ֻ�ܵ��� restart_app_timer_inner ���¼�ʱ��
 * \li  timer_id ����0 ~ count - 1�����з���FALSE��
 *******************************************************************************/
bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //����Խ��
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_STOPED;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ����Ӧ�ü���ʱ����ʱ������״̬��Ϊ TIMER_STATE_RUNNING��
 * \param[in]    timer_id ��ʱ��ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ������ʱ�ɹ���
 * \retval           FALSE ������ʱʧ�ܣ�ԭ��Ϊ�Ҳ���ָ��ID�Ķ�ʱ����
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1���ָ���ʱ����ʱ����ʱ������ͷ��ʼ��ʱ
 if(restart_app_timer_inner(timer_id) == FALSE)
 {
 �ָ���ʱʧ��
 }
 * \endcode
 * \note
 * \li  ������ʱ�������¿�ʼһ����ʱ���ڡ�
 * \li  timer_id ����0 ~ count - 1�����з���FALSE��
 *******************************************************************************/
bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //����Խ��
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_RUNNING;
    tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
    return TRUE;
}

//��ʱ��STANDBY�˳����������ж�ʱ��
bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id)
{
    app_timer_t *tm;
    uint8 i;

    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);

        if (tm->state == TIMER_STATE_RUNNING)
        {
            tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ɾ��Ӧ�ü���ʱ��������ʱ��״̬��Ϊ TIMER_STATE_NOUSED��
 * \param[in]    timer_id ��ʱ��ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ɾ����ʱ���ɹ���
 * \retval           FALSE ɾ����ʱ��ʧ�ܣ�ԭ��Ϊ�Ҳ���ָ��ID�Ķ�ʱ����
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 ����1��ɾ����ʱ��
 if(kill_app_timer_inner(timer_id) == FALSE)
 {
 ɾ����ʱ��ʧ��
 }
 * \endcode
 * \note
 * \li  timer_id ����0 ~ count - 1�����з���FALSE��
 *******************************************************************************/
bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //����Խ��
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_NOUSED;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʼ��Ӧ�ü���ʱ������ģ�飬�����ж�ʱ��״̬��Ϊ TIMER_STATE_NOUSED��
 * \param[in]    count Ӧ�ò㶨�����ʱ���������ʱ����Ŀ
 * \param[out]   none
 * \return       bool
 * \retval           TRUE ��ʼ���ɹ���
 * \retval           FALSE ��ʼ��ʧ�ܣ�ԭ��Ϊ�����Ƿ���
 * \ingroup      app_timer
 * \par          exmaple code
 * \note
 * \li  ��Ӧ�� main ������ڴ����ã��ڴ�֮ǰ������������� app_timer �ӿڡ�
 * \li  �ýӿ��ڽ���AP�󣬵����� applib_init_inner �ӿں���á�
 *******************************************************************************/
bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count)
{
    app_timer_t *tm;
    uint8 i;

    if (count == 0) //��ʱ����Ŀ����Ϊ0
    {
        return FALSE;
    }

    if (group_id != APP_TIMER_GROUP_COMMON)
    {
        g_app_timer_group_vector[group_id].vector = (app_timer_t *) sys_malloc(sizeof(app_timer_t)*count);
    }
    else
    {
        g_app_timer_group_vector[group_id].vector = g_p_com_app_timer_vector;
    }
    g_app_timer_group_vector[group_id].count = count;

    for (i = 0; i < count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);
        tm->state = TIMER_STATE_NOUSED;
    }

    //��ʼ��Ϊ AP ����
    g_app_timer_group_vector[group_id].tag = APP_TIMER_TAG_MAIN;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ�ü���ʱ�� handle ������Ӧ����Ϣѭ����ѭ�����ã�ѭ��ɨ�����ж�ʱ����ִ��
 *    ��ʱ����Ķ�ʱ���ķ������� proc�������ݶ�ʱ�����ͽ������á�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      app_timer
 * \note
 *******************************************************************************/
void handle_timers(app_timer_group_id_e group_id)
{
    app_timer_t *tm;
    uint32 cur_time_ms = sys_get_ab_timer(); //���û��ִ����ʱ���������̣���Ϊ��ǰʱ�䲻��
    uint32 next_time_ms;
    uint32 i;

    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);
        if ((tm->state == TIMER_STATE_RUNNING) //��ʱ���������в���Ҫִ��handle
                && ((tm->tag == g_app_timer_group_vector[group_id].tag) || (tm->tag == APP_TIMER_TAG_SYS))) //����У��
        {
            next_time_ms = cur_time_ms + tm->timeout;
            if (cur_time_ms >= tm->timeout_expires) //��ʱʱ���ѵ�
            {
                //������ʱ����ʱʱ��
                if (tm->timeout > (cur_time_ms - tm->timeout_expires))
                {
                    tm->timeout_expires += tm->timeout; //ֱ����expires�ϼӶ�ʱ���ڣ���ȷ����ζ�ʱʱ��׼ȷ
                }
                else
                {
                    tm->timeout_expires = next_time_ms;
                }

                //������ʱ��һ��ִ����� kill ��
                if (tm->type == TIMER_TYPE_SINGLE_SHOT)
                {
                    tm->state = TIMER_STATE_NOUSED;
                }

                //������ʱ����������
                if (tm->func_proc != NULL)
                {
                    g_app_timer_group_vector[group_id].in_handle = TRUE;
                    tm->func_proc();
                    g_app_timer_group_vector[group_id].in_handle = FALSE;
                }

                //ִ�й���ʱ���������̣����¼��ص�ǰʱ�䣬����ʱ���Ϊ׼ȷ
                cur_time_ms = sys_get_ab_timer();
            }

            if (next_time_ms <= tm->timeout_expires)
            {
                //���ϵͳʱ����ĳ��ԭ�����ڳ�ʱʱ�����棬��ô�����޸���ʱ���
                tm->timeout_expires = next_time_ms;
            }
        }
    }
}


/*! \endcond */

/*! \cond */

//����Ӧ�ü���ʱ��
static inline int8 __set_timer(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, timer_type_e type, uint16 timeout, timer_proc func_proc)
{
    app_timer_t *tm;
    int8 timer_id = -1;
    int8 i;

    //�������̲���ΪNULL
    if (func_proc == NULL)
    {
        return -1;
    }

    //��ʱ���ڲ���Ϊ0
    if (0 == timeout)
    {
        return -1;
    }

    //�������������Ŀ�����ʱ��
    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);

        if (tm->state == TIMER_STATE_NOUSED)
        {
            timer_id = i;

            //��ʼ����ʱ��
            tm->attrb = attrb;
            tm->timeout = timeout;
            tm->timeout_expires = sys_get_ab_timer() + timeout;
            tm->func_proc = func_proc;
            tm->type = type;
            tm->tag = tag;
            tm->state = TIMER_STATE_RUNNING;
            break;
        }
    }

    if (timer_id < 0)
    {
        libc_printf_error("set app timer fail!!\n");
        while (1);
    }

    return timer_id;
}

/*! \endcond */
