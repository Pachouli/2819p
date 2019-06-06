#ifndef _BT_HOST_OP_INTERFACE_FOR_BT_CON_H__            //��ֹ�ض���
#define _BT_HOST_OP_INTERFACE_FOR_BT_CON_H__

typedef enum
{
    BT_CON_MALLOC,                      //�ڴ�����
    BT_CON_FREE,                        //�ڴ��ͷ�

    ENTER_LOW_POWER,                    //����͹���ģʽ
    EXIT_LOW_POWER,                     //�˳��͹���ģʽ

    HOST_BUF_REQ,                       //��host�����ڴ棬�������ڴ����ŵ���host_buf_commit���ͷŵ�
    HOST_BUF_COMMIT,                    //��host��������

} bt_cmd_for_con_e;      //bt_host_op_entry.c

#define BT_HOST_ENTRY_ADDR_FOR_CON(x)                       (BT_HOST_OP_ENTRY_FOR_CON + (4 * x))

#define BT_CON_MALLOC_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(BT_CON_MALLOC))))
#define BT_CON_FREE_ENTRY                                   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(BT_CON_FREE))))
#define ENTER_LOW_POWER_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(ENTER_LOW_POWER))))
#define EXIT_LOW_POWER_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(EXIT_LOW_POWER))))
#define HOST_BUF_REQ_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(HOST_BUF_REQ))))
#define HOST_BUF_COMMIT_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_CON(HOST_BUF_COMMIT))))

//#define bt_con_malloc(a,b)                                  ((void*(*)(uint8,uint32))BT_CON_MALLOC_ENTRY)((a),(b))
//#define bt_con_free(a)                                      ((void(*)(void*))BT_CON_FREE_ENTRY)((a))

//#define enter_low_power(a)                                  ((uint8(*)(uint8))ENTER_LOW_POWER_ENTRY)((a))
//#define exit_low_power(a)                                   ((uint8(*)(uint8))EXIT_LOW_POWER_ENTRY)((a))

//#define host_buf_req(a,b)                                   ((uint8 *(*)(uint8,uint32))HOST_BUF_REQ_ENTRY)((a), (b))
//#define host_buf_commit(a,b,c)                              ((uint8 (*)(uint8,uint8 *,uint32))HOST_BUF_COMMIT_ENTRY)((a), (b),(c))

#endif
