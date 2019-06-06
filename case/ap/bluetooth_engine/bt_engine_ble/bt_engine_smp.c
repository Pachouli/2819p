/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_smp.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

struct SMP_KeyStoreStru smp_store_info;

void APP_SMP_Callback(UINT8 event, void *arg) /* APP UI Callback */
{
    BLE_PRINTF("SMP_Callback event code %d\n", event);
    switch (event)
    {
        case SMP_EV_TK_OOB:
        BLE_PRINTF("TK_OOB\n")
        ;
        break;

        case SMP_EV_TK_DISPLAY: //FIXME, display TK.
        break;

        case SMP_EV_TK_PASSKEY_ENTRY:
        BLE_PRINTF("PASSKEY_ENTRY\n")
        ;
        break;

        case SMP_EV_KEY_STORE:
        BLE_PRINTF("KEY_STORE\n")
        ;
        libc_memcpy(&smp_store_info, arg, sizeof(struct SMP_KeyStoreStru));
        break;

        case SMP_EV_KEY_MISS:
        {
            struct SMP_KeyMissStru *key = arg; /* FIXME, simple solution */
            SMP_PeerKeys_Remove(key);
            arg = NULL; /* Reuse arg */
        }
        break;

        case SMP_EV_SECURITY_FAILED:
        BLE_PRINTF("SECURITY_FAILED\n")
        ;
        break;

        case SMP_EV_SECURITY_OK:
        BLE_PRINTF("SECURITY_OK\n")
        ;
        break;

        default:
        BLE_PRINTF("SECURITY_UNKNOWN\n")
        ;
        break;
    }

    FREE(arg);
}

void app_smp_reglocalcap(void *sm_cbk)
{
    struct SMP_RegLocalCapStru *in;

    in = NEW(sizeof(struct SMP_RegLocalCapStru));
    in->min_encry_keysize = 7;
    in->max_encry_keysize = 16; //test
    in->cbk = sm_cbk;
    SMP_RegLocalCap(in);
}

/*---------------------------------------------------------------------------
 Description:
 Initialize, read keys from file, recover to SMP.
 ---------------------------------------------------------------------------*/
void APP_PeerNode_SMPRecoverKeys(void)
{
    /* Recover SMP Keys from file */
    struct ListStru *list = NEW(sizeof(struct ListStru));
    struct SMP_KeyStoreStru *key;
    libc_memset(list, 0, sizeof(struct ListStru));
    {
        //uint8 i;
        BTSRC_List_AddTail(list, key = BTSRC_List_NodeNew(sizeof(struct SMP_KeyStoreStru)));
        libc_memcpy(key, &smp_store_info, sizeof(struct SMP_KeyStoreStru));
    }
    SMP_PeerKeys_Recover(list);
    FREE(list);
}

#endif //end of #ifdef BT_SUPPORT_BLE
