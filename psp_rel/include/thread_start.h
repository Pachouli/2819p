#ifndef _THREAD_START_H_
#define _THREAD_START_H_

typedef int (*ap_main_cbk)(int argc, const void* argv[]);

typedef struct
{
    INT8U prio; /*Ӧ�����ȼ�*/
    OS_STK *ptos; /*ջ��*/
    INT16U stk_size; /*Ӧ��ջ��С*/
    INT32U dstart; /*���ݶ���ʼ��ַ*/
    INT32U dend; /*���ݶν�����ַ*/
    ap_main_cbk main; /*�߳�Main����*/
} process_param_t;

#define ap_start_entry(a,b)      ((int(*)(void*, process_param_t *))COM_COM_AP_START_ENTRY)((a),(b))

#endif
