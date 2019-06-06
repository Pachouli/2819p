#ifndef _THREAD_START_H_
#define _THREAD_START_H_

typedef int (*ap_main_cbk)(int argc, const void* argv[]);

typedef struct
{
    INT8U prio; /*应用优先级*/
    OS_STK *ptos; /*栈顶*/
    INT16U stk_size; /*应用栈大小*/
    INT32U dstart; /*数据段起始地址*/
    INT32U dend; /*数据段结束地址*/
    ap_main_cbk main; /*线程Main函数*/
} process_param_t;

#define ap_start_entry(a,b)      ((int(*)(void*, process_param_t *))COM_COM_AP_START_ENTRY)((a),(b))

#endif
