#ifndef _BT_HOST_OP_INTERFACE__FOR_BT_ENGINE_H__            //防止重定义
#define _BT_HOST_OP_INTERFACE__FOR_BT_ENGINE_H__

#define BLE_INTERFACE_SUPPORT 1
#define SPP_INTERFACE_SUPPORT 1

typedef enum
{
    //BR/BDR HCI Interface total(9) offset(0)
    HCI_EXT_INQ_RSP_ENC,                    //HCI额外响应包编码，目前用于LE的广播包与广播响应包编码
    HCI_EXT_INQ_RSP_DEC,                    //HCI额外响应包解码，目前用于LE的广播包与广播响应包解码
    HCI_COMPARE_ADDR,                       //蓝牙地址比较
    HCI_CBK_REG,                            //HCI回调函数注册
    HCI_CBK_UNREG,                          //HCI回调函数反注册
    HCI_DISCONNECT,                         //断开HCI连接
    HCI_GET_ACL_PACKET_NUM,                 //获取该ACL链路中接收包数
    HCI_SET_ACL_PACKET_NUM,                 //设置该ACL链路中最大接收包数
    HCI_DEAL_ACLPKG_IRQ_LOCK,               //acl pkg 中断锁

    //BR/BDR List Operation Interface total(5) offset(9)
    LIST_REMOVEHEAD,                        //列表取出表头
    LIST_NODENEW,                           //创建节点
    LIST_ADDTAIL,                           //添加节点至列表尾部
    LIST_REMOVEAT,                          //取出对应位置节点
    LIST_COUNT,                             //计算当前列表长度

    //BR/BDR Gap Operation Interface total(12) offset(14)
    GAP_REGISTERINDCBK,
    GAP_RESETHARDWAREA,
    GAP_SETLOCALNAMEA,
    GAP_SETVISUALMODEA,
    GAP_SETLOCALDEVICECLASSA,
    GAP_SETIOCAPABILITY,
    GAP_REGISTERTRANSPORTLAYERA,
    GAP_UNREGISTERTRANSPORTLAYERA,
    GAP_EXECUTECOMMANDA,
    GAP_LINKKEYREPLY,
    GAP_DEVICEDISCOVERY,
    GAP_CANCELDEVICEDISCOVERY,

    //BR/BDR Transport layer Interface total(1) offset(26)
    INITRXLOOP,

    //BR/BDR A2dp Profile Interface total(10) offset(27)
    A2DP_REGCBK,
    A2DP_REGISTERSERVICE,
    A2DP_UNREGISTERSERVICE,
    A2DP_REGISTER_SEP,
    A2DP_UNREGISTER_SEP,
    A2DP_CONNECT,
    A2DP_DISCONNECT,
    A2DP_STREAMSTART,
    A2DP_STREAMSUSPEND,
    A2DP_DELAYREPORTING,

    //BR/BDR AVRCP Profile Interface total(13) offset(37)
    AVRCP_CONNECTREQ,
    AVRCP_DISCONNECTREQ,
    AVRCP_SERVERSTART,
    AVRCP_SERVERSTOP,
    AVRCP_REGCBK,
    AVRCP_PASSTHROUGH,
    AVRCP_FREEEVENTPARAM,
    AVRCP_ADVANCEDCONTROL_REQ,
    AVRCP_PASSTHROUGH_RSP,
    AVRCP_ADVANCEDCONTROL_REQ,
    AVRCP_ADVANCEDCONTROL_RSP,
    AVRCP_REGISTERNOTIF_RSP,
    AVRCP_GENERALREJECT_RSP,

    //BR/BDR HFP Profile Interface total(15) offset(50)
    HFP_SERVER_START,
    HFP_SERVER_STOP,
    HFP_CLIENT_START,
    HFP_CLIENT_STOP,
    HFP_HF_DIAL,
    HFP_HF_TXDTMF,
    HFP_HF_AUDIOCONNTRANS,
    HFP_HF_ANSWERCALL,
    HFP_HFP_HF_CANCELCALL,
    HFP_HF_HOLDINCOMINGCALL,
    HFP_HF_3WAYCALLINGHANDLER,
    HFP_HF_GETCURRENTCALLS,
    HFP_HF_VOICERECOGNITIONREQ,
    HFP_HF_DISABLENREC,
    HFP_EXTENDCMD,

    //BR/BDR HID Profile Interface total(6) offset(65)
    APP_HID_START,
    APP_HID_STARTSERVERREQ,
    HID_PACKET_SEND_INTERRUPT,
    HID_PACKET_SEND_CONTROL,
    HID_CONNECTREQ,
    HID_DISCONNECTREQ,

    //BR/BDR User Define Interface total(20) offset(71)
    BT_INIT,
    BT_DEINIT,
    BTDEV_LIBEXC,
    BTDEV_RXREADTOHCI,
    SET_EXIT_SCHEDULE_FLAG,
    REGITER_CALLBACK_FUNC,
    SET_AVDTP_FRAME_BUF,
    GET_RXREADTOHCI_FLAG,
    SET_RXREADTOHCI_FLAG,
    SCO_MEM_MALLOC,
    SCO_MEM_FREE,
    MEM_MALLOC,
    MEM_FREE,
    MY_INITSTATICMEM,
    MY_DONESTATICMEM,
    GET_BTMEM_FREE_SIZE,
    SEND,
    CHECK_HW_ERROR_NO,
    L2CAP_GETREMOTECID,
    HAS_AVDTP_PKG_LOST,

#if (SPP_INTERFACE_SUPPORT == 1)
    //BR/BDR SPP Add List Operation interface total(1) offset(91)
    LIST_REMOVEALL,

    //BR/BDR SPP Add Sdap interface total(1) offset(92)
    SDAP_MODIFYSTRINGATTRIB,

    //BR/BDR SPP Interface total(9) offset(93)
    APP_SPP_START,
    APP_SPP_STOP,
    APP_SPP_STARTSERVERREQ,
    APP_SPP_STOPSERVERREQ,
    APP_SPP_CONNECTREQ,
    APP_SPP_DISCONNECTREQ,
    APP_SPP_DATAREQ,
    APP_SPP_GETCFG,
    APP_SPP_GET_CREDITS,
#endif

#if (BLE_INTERFACE_SUPPORT == 1)
    //BLE l2cap Interface total(1) offset(102)
    L2CAP_CONNECTION_PARAMETER_UPDATE_REQ,

    //BLE smp Interface total(4) offset(103)
    SMP_PEERKEYS_REMOVE,
    SMP_PEERKEYS_RECOVER,
    SMP_COMPAREADDR,
    SMP_REGLOCALCAP,

    //BLE GATT Interface total(23) offset(107)
    GATT_SERVER_PENDIND_RESPONSE,
    GATT_EXCHANGE_MTU,
    GATT_WRITE_VALUE,
    GATT_READ_VALUE,
    GATT_TL_DISCONNECT,
    GATT_DISCOVER_CHARACTERISTICS,
    GATT_DISCOVER_SERVICES,
    GATT_FREE_SERVICE,
    GATT_SERVER_UNREGTREE,
    GATT_FREE_CHARACTER,
    GATT_FREE_DESCRIPTOR,
    GATT_ASSEMBLE_VALUE,
    GATT_SERVER_NOTIFYINDICATE,
    GATT_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS,
    GATT_GETHANDLE_BYUUID,
    GATT_SERVER_MODIFYVALUE,
    GATT_ATTCHANNELCBK_REG,
    GATT_NOTIFICATIONCBK_REG,
    GATT_ADD_CHARACTERISTIC,
    GATT_ADD_DESCRIPTOR_CCC,
    GATT_ADD_DESCRIPTOR_REPORTREFERENCE,
    GATT_SERVER_REGTREE,
    GATT_SERVER_SERVICECHANGED,

    //BLE HCI_LE Interface total(5) offset(130)
    LE_SET_SCAN_PARMETERS,
    LE_SET_SCAN_ENABLE,
    LE_SET_ADVERTISING_PARAMETERS,
    LE_SET_ADVERTISE_ENABLE,
    LE_GAP_SERVICEREG,

    //BLE ATT UUID Interface total(4) offset(135)
    ATT_UUID_GETU2,
    ATT_UUID_GET,
    ATT_UUID_SETU16,
    ATT_UUID_SETU2,

    //BLE HCI Interface total(2) offset(139)
    HCI_COMMAND_SENDINTER,
    HCI_LE_CAPACITY_GET,
#endif
} bt_cmd_for_bt_engine_e;

#define BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(x)                 (BT_HOST_OP_ENTRY_VA_START  + (4 * x))

#define HCI_EXT_INQ_RSP_ENC_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_EXT_INQ_RSP_ENC))))
#define HCI_EXT_INQ_RSP_DEC_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_EXT_INQ_RSP_DEC))))
#define HCI_COMPARE_ADDR_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_COMPARE_ADDR))))
#define HCI_CBK_REG_ENTRY                                   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_CBK_REG))))
#define HCI_CBK_UNREG_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_CBK_UNREG))))
#define HCI_DISCONNECT_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_DISCONNECT))))
#define HCI_GET_ACL_PACKET_NUM_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_GET_ACL_PACKET_NUM))))
#define HCI_SET_ACL_PACKET_NUM_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_SET_ACL_PACKET_NUM))))
#define HCI_DEAL_ACLPKG_IRQ_LOCK_ENTRY                      (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_DEAL_ACLPKG_IRQ_LOCK))))

#define LIST_REMOVEHEAD_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_REMOVEHEAD))))
#define LIST_NODENEW_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_NODENEW))))
#define LIST_ADDTAIL_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_ADDTAIL))))
#define LIST_REMOVEAT_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_REMOVEAT))))
#define LIST_COUNT_ENTRY                                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_COUNT))))

#define GAP_REGISTERINDCBK_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_REGISTERINDCBK))))
#define GAP_RESETHARDWAREA_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_RESETHARDWAREA))))
#define GAP_SETLOCALNAMEA_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_SETLOCALNAMEA))))
#define GAP_SETVISUALMODEA_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_SETVISUALMODEA))))
#define GAP_SETLOCALDEVICECLASSA_ENTRY                      (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_SETLOCALDEVICECLASSA))))
#define GAP_SETIOCAPABILITY_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_SETIOCAPABILITY))))
#define GAP_REGISTERTRANSPORTLAYERA_ENTRY                   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_REGISTERTRANSPORTLAYERA))))

#define GAP_UNREGISTERTRANSPORTLAYERA_ENTRY                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_UNREGISTERTRANSPORTLAYERA))))
#define GAP_EXECUTECOMMANDA_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_EXECUTECOMMANDA))))

#define GAP_LINKKEYREPLY_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_LINKKEYREPLY))))
#define GAP_DEVICEDISCOVERY_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_DEVICEDISCOVERY))))
#define GAP_CANCELDEVICEDISCOVERY_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GAP_CANCELDEVICEDISCOVERY))))

#define INITRXLOOP_ENTRY                                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(INITRXLOOP))))

#define A2DP_REGCBK_ENTRY                                   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_REGCBK))))
#define A2DP_REGISTERSERVICE_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_REGISTERSERVICE))))

#define A2DP_UNREGISTERSERVICE_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_UNREGISTERSERVICE))))

#define A2DP_REGISTER_SEP_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_REGISTER_SEP))))
#define A2DP_UNREGISTER_SEP_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_UNREGISTER_SEP))))

#define A2DP_CONNECT_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_CONNECT))))
#define A2DP_DISCONNECT_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_DISCONNECT))))
#define A2DP_STREAMSTART_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_STREAMSTART))))
#define A2DP_STREAMSUSPEND_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_STREAMSUSPEND))))
#define A2DP_DELAYREPORTING_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(A2DP_DELAYREPORTING))))

#define AVRCP_CONNECTREQ_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_CONNECTREQ))))
#define AVRCP_DISCONNECTREQ_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_DISCONNECTREQ))))
#define AVRCP_SERVERSTART_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_SERVERSTART))))
#define AVRCP_SERVERSTOP_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_SERVERSTOP))))
#define AVRCP_REGCBK_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_REGCBK))))
#define AVRCP_PASSTHROUGH_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_PASSTHROUGH))))
#define AVRCP_FREEEVENTPARAM_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_FREEEVENTPARAM))))
#define AVRCP_ADVANCEDCONTROL_REQ_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_ADVANCEDCONTROL_REQ))))
#define AVRCP_PASSTHROUGH_RSP_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_PASSTHROUGH_RSP))))
#define AVRCP_ADVANCEDCONTROL_REQ_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_ADVANCEDCONTROL_REQ))))
#define AVRCP_ADVANCEDCONTROL_RSP_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_ADVANCEDCONTROL_RSP))))
#define AVRCP_REGISTERNOTIF_RSP_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_REGISTERNOTIF_RSP))))
#define AVRCP_GENERALREJECT_RSP_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(AVRCP_GENERALREJECT_RSP))))

#define HFP_SERVER_START_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_SERVER_START))))

#define HFP_SERVER_STOP_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_SERVER_STOP))))
#define HFP_CLIENT_START_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_CLIENT_START))))
#define HFP_CLIENT_STOP_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_CLIENT_STOP))))
#define HFP_HF_DIAL_ENTRY                                   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_DIAL))))
#define HFP_HF_TXDTMF_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_TXDTMF))))
#define HFP_HF_AUDIOCONNTRANS_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_AUDIOCONNTRANS))))
#define HFP_HF_ANSWERCALL_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_ANSWERCALL))))
#define HFP_HFP_HF_CANCELCALL_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HFP_HF_CANCELCALL))))
#define HFP_HF_HOLDINCOMINGCALL_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_HOLDINCOMINGCALL))))
#define HFP_HF_3WAYCALLINGHANDLER_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_3WAYCALLINGHANDLER))))
#define HFP_HF_GETCURRENTCALLS_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_GETCURRENTCALLS))))
#define HFP_HF_VOICERECOGNITIONREQ_ENTRY                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_VOICERECOGNITIONREQ))))
#define HFP_HF_DISABLENREC_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_HF_DISABLENREC))))

#define HFP_EXTENDCMD_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HFP_EXTENDCMD))))

#define APP_HID_START_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_HID_START))))
#define APP_HID_STARTSERVERREQ_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_HID_STARTSERVERREQ))))
#define HID_PACKET_SEND_INTERRUPT_ENTRY                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HID_PACKET_SEND_INTERRUPT))))
#define HID_PACKET_SEND_CONTROL_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HID_PACKET_SEND_CONTROL))))
#define HID_CONNECTREQ_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HID_CONNECTREQ))))
#define HID_DISCONNECTREQ_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HID_DISCONNECTREQ))))

#define BT_INIT_ENTRY                                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(BT_INIT))))
#define BT_DEINIT_ENTRY                                     (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(BT_DEINIT))))
#define BTDEV_LIBEXC_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(BTDEV_LIBEXC))))
#define BTDEV_RXREADTOHCI_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(BTDEV_RXREADTOHCI))))
#define SET_EXIT_SCHEDULE_FLAG_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SET_EXIT_SCHEDULE_FLAG))))
#define REGITER_CALLBACK_FUNC_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(REGITER_CALLBACK_FUNC))))
#define SET_AVDTP_FRAME_BUF_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SET_AVDTP_FRAME_BUF))))
#define GET_RXREADTOHCI_FLAG_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GET_RXREADTOHCI_FLAG))))
#define SET_RXREADTOHCI_FLAG_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SET_RXREADTOHCI_FLAG))))
#define SCO_MEM_MALLOC_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SCO_MEM_MALLOC))))
#define SCO_MEM_FREE_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SCO_MEM_FREE))))
#define MEM_MALLOC_ENTRY                                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(MEM_MALLOC))))
#define MEM_FREE_ENTRY                                      (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(MEM_FREE))))
#define MY_INITSTATICMEM_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(MY_INITSTATICMEM))))
#define MY_DONESTATICMEM_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(MY_DONESTATICMEM))))
#define GET_BTMEM_FREE_SIZE_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GET_BTMEM_FREE_SIZE))))
#define SEND_ENTRY                                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SEND))))

#define CHECK_HW_ERROR_NO_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(CHECK_HW_ERROR_NO))))

#define L2CAP_GETREMOTECID_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(L2CAP_GETREMOTECID))))
#define L2CAP_CONNECTION_PARAMETER_UPDATE_REQ_ENTER         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(L2CAP_CONNECTION_PARAMETER_UPDATE_REQ))))

#define HAS_AVDTP_PKG_LOST_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HAS_AVDTP_PKG_LOST))))

#if (SPP_INTERFACE_SUPPORT == 1)
#define LIST_REMOVEALL_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LIST_REMOVEALL))))
#define SDAP_MODIFYSTRINGATTRIB_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SDAP_MODIFYSTRINGATTRIB))))
#define APP_SPP_START_ENTRY                                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_START))))
#define APP_SPP_STOP_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_STOP))))
#define APP_SPP_STARTSERVERREQ_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_STARTSERVERREQ))))
#define APP_SPP_STOPSERVERREQ_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_STOPSERVERREQ))))
#define APP_SPP_CONNECTREQ_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_CONNECTREQ))))
#define APP_SPP_DISCONNECTREQ_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_DISCONNECTREQ))))
#define APP_SPP_DATAREQ_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_DATAREQ))))
#define APP_SPP_GETCFG_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_GETCFG))))
#define APP_SPP_GET_CREDITS_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(APP_SPP_GET_CREDITS))))
#endif

#if (BLE_INTERFACE_SUPPORT == 1)
#define SMP_PEERKEYS_REMOVE_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SMP_PEERKEYS_REMOVE))))
#define SMP_PEERKEYS_RECOVER_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SMP_PEERKEYS_RECOVER))))
#define SMP_COMPAREADDR_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SMP_COMPAREADDR))))
#define SMP_REGLOCALCAP_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(SMP_REGLOCALCAP))))
#define GATT_SERVER_PENDIND_RESPONSE_ENTRY                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_PENDIND_RESPONSE))))
#define GATT_EXCHANGE_MTU_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_EXCHANGE_MTU))))
#define GATT_WRITE_VALUE_ENTRY                              (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_WRITE_VALUE))))
#define GATT_READ_VALUE_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_READ_VALUE))))
#define GATT_TL_DISCONNECT_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_TL_DISCONNECT))))
#define GATT_DISCOVER_CHARACTERISTICS_ENTRY                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_DISCOVER_CHARACTERISTICS))))
#define GATT_DISCOVER_SERVICES_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_DISCOVER_SERVICES))))
#define GATT_FREE_SERVICE_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_FREE_SERVICE))))
#define GATT_SERVER_UNREGTREE_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_UNREGTREE))))
#define GATT_FREE_CHARACTER_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_FREE_CHARACTER))))
#define GATT_FREE_DESCRIPTOR_ENTRY                          (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_FREE_DESCRIPTOR))))
#define GATT_ASSEMBLE_VALUE_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_ASSEMBLE_VALUE))))
#define GATT_SERVER_NOTIFYINDICATE_ENTRY                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_NOTIFYINDICATE))))
#define GATT_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS_ENTRY   (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS))))
#define GATT_GETHANDLE_BYUUID_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_GETHANDLE_BYUUID))))
#define GATT_SERVER_MODIFYVALUE_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_MODIFYVALUE))))
#define GATT_ATTCHANNELCBK_REG_ENTRY                        (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_ATTCHANNELCBK_REG))))
#define GATT_NOTIFICATIONCBK_REG_ENTRY                      (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_NOTIFICATIONCBK_REG))))
#define GATT_ADD_CHARACTERISTIC_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_ADD_CHARACTERISTIC))))
#define GATT_ADD_DESCRIPTOR_CCC_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_ADD_DESCRIPTOR_CCC))))
#define GATT_ADD_DESCRIPTOR_REPORTREFERENCE_ENTRY           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_ADD_DESCRIPTOR_REPORTREFERENCE))))
#define GATT_SERVER_REGTREE_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_REGTREE))))
#define GATT_SERVER_SERVICECHANGED_ENTRY                    (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(GATT_SERVER_SERVICECHANGED))))

#define LE_SET_SCAN_PARMETERS_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LE_SET_SCAN_PARMETERS))))
#define LE_SET_SCAN_ENABLE_ENTRY                            (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LE_SET_SCAN_ENABLE))))
#define LE_SET_ADVERTISING_PARAMETERS_ENTRY                 (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LE_SET_ADVERTISING_PARAMETERS))))
#define LE_SET_ADVERTISE_ENABLE_ENTRY                       (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LE_SET_ADVERTISE_ENABLE))))
#define LE_GAP_SERVICEREG_ENTRY                             (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(LE_GAP_SERVICEREG))))

#define ATT_UUID_GETU2_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(ATT_UUID_GETU2))))
#define ATT_UUID_GET_ENTRY                                  (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(ATT_UUID_GET))))
#define ATT_UUID_SETU16_ENTRY                               (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(ATT_UUID_SETU16))))
#define ATT_UUID_SETU2_ENTRY                                (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(ATT_UUID_SETU2))))
#define HCI_COMMAND_SENDINTER_ENTRY                         (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_COMMAND_SENDINTER))))
#define HCI_LE_CAPACITY_GET_ENTRY                           (*((uint32*)(BT_HOST_ENTRY_ADDR_FOR_BT_ENGINE(HCI_LE_CAPACITY_GET))))
#endif

/**
 * Hci 扩展扫描响应数据打包
 * 将adv_struct_list中各成员的数据打包成一个完整连续的数据块
 * 输入参数：
 *       UINT8 adv_struct_list (struct ListStru*)
 *       UINT8 *enc_data_head     
 *       UINT8 *enc_data_tail
 * 返回值:UINT8 enc_data_length
 */
#define BTSRC_HCI_Extended_Inquiry_Response_Encode(a,b,c)   ((UINT8(*)(struct ListStru* adv_struct_list, UINT8* enc_data_head, UINT8* enc_data_tail))HCI_EXT_INQ_RSP_ENC_ENTRY)((a), (b), (c))
#define BTSRC_HCI_Extended_Inquiry_Response_Decode(a,b,c)   ((void(*)(struct ListStru*, UINT8*, UINT8*))HCI_EXT_INQ_RSP_DEC_ENTRY)((a), (b), (c))
#define BTSRC_HCI_CompareAddr(a,b)                          ((UINT8(*)(struct HCI_AddressStru*, struct HCI_AddressStru*))HCI_COMPARE_ADDR_ENTRY)((a), (b))
#define BTSRC_HCI_Cbk_Reg(a,b)                              ((void(*)(struct HCI_CbkLineStru*, UINT32))HCI_CBK_REG_ENTRY)((a), (b))
#define BTSRC_HCI_Cbk_UnReg(a)                              ((void(*)(struct HCI_CbkLineStru*))HCI_CBK_UNREG_ENTRY)((a))
#define HCI_Disconnect(a)                                   ((void(*)(uint16))HCI_DISCONNECT_ENTRY)((a))

#define HCI_Get_Acl_packet_num(a)                           ((uint32(*)(uint16))HCI_GET_ACL_PACKET_NUM_ENTRY)((a))
#define HCI_Set_Acl_packet_num(a,b,c)                       ((void(*)(UINT16,UINT16,UINT8))HCI_SET_ACL_PACKET_NUM_ENTRY)((a),(b),(c))
#define HCI_Deal_Aclpkg_Irq_Lock(a)                         ((uint8(*)(uint8))HCI_DEAL_ACLPKG_IRQ_LOCK_ENTRY)((a))

/* BT list UI.  */
//取出列表中head指针（不保留取出的内容）并更新list.head
#define BTSRC_List_RemoveHead(a)                            ((void*(*)(struct ListStru *))LIST_REMOVEHEAD_ENTRY)((a))
//获取一个新的节点
#define BTSRC_List_NodeNew(a)                               ((void*(*)(uint32))LIST_NODENEW_ENTRY)((a))
//将节点添加到列表尾部
#define BTSRC_List_AddTail(a,b)                             ((void(*)(struct ListStru*, void*))LIST_ADDTAIL_ENTRY)((a), (b))
#define BTSRC_List_RemoveAt(a,b)                            ((void(*)(struct ListStru*, void*))LIST_REMOVEAT_ENTRY)((a),(b))
#define BTSRC_List_Count(a)                               ((uint32(*)(struct ListStru*))LIST_COUNT_ENTRY)((a))

//gap interface
#define BTSRC_GAP_RegisterIndCbk(a,b,c)                     ((void(*)(void *, tGAP_Ind_Type, GAP_IndicationCbk*))GAP_REGISTERINDCBK_ENTRY)((a), (b),(c))
#define BTSRC_GAP_ResetHardwareA(a,b,c)                     ((void(*)(HANDLE, void*, GAP_ResetHardwareCfmCbk*))GAP_RESETHARDWAREA_ENTRY)((a), (b),(c))
#define BTSRC_GAP_SetLocalNameA(a,b,c,d,e)                  ((void(*)(HANDLE, void*, UINT8*, UINT16, GAP_ConfirmationCbk*))GAP_SETLOCALNAMEA_ENTRY)((a), (b),(c), (d),(e))
#define BTSRC_GAP_SetVisualModeA(a,b,c,d)                   ((void(*)(HANDLE, void*, UINT16, GAP_ConfirmationCbk*))GAP_SETVISUALMODEA_ENTRY)((a), (b),(c), (d))
#define BTSRC_GAP_SetLocalDeviceClassA(a,b,c,d)             ((void(*)(HANDLE, void*, UINT8*, GAP_ConfirmationCbk*))GAP_SETLOCALDEVICECLASSA_ENTRY)((a), (b),(c), (d))
#define BTSRC_GAP_SetIOCapability(a)                        ((UINT16(*)(UINT8))GAP_SETIOCAPABILITY_ENTRY)((a))
#define BTSRC_GAP_RegisterTransportLayerA(a,b,c)            ((void(*)(HANDLE, TransportLayerStru*, GAP_RegisterTLCfmCbk*))GAP_REGISTERTRANSPORTLAYERA_ENTRY)((a), (b),(c))
#define BTSRC_GAP_UnregisterTransportLayerA(a,b)            ((void(*)(HANDLE, GAP_UnregisterTLCfmCbk*))GAP_UNREGISTERTRANSPORTLAYERA_ENTRY)((a), (b))
#define BTSRC_GAP_ExecuteCommandA(a,b,c,d,e,f)              ((void(*)(HANDLE, void*, UINT32, void*, UINT32, GAP_ExecuteCommandCfmCbk*))GAP_EXECUTECOMMANDA_ENTRY)((a), (b),(c), (d),(e), (f))
#define BTSRC_GAP_LinkKeyReply(a,b)                         ((UINT8(*)(HANDLE, struct HCI_Security_Link_Key_Request_ReplyStru*))GAP_LINKKEYREPLY_ENTRY)((a), (b))
#define BTSRC_GAP_DeviceDiscovery(a,b,c,d)                  ((UINT16(*)(HANDLE, void*, struct GAP_DeviceDiscoveryReqStru*, GAP_DeviceDiscoveryCfmCbk*))GAP_DEVICEDISCOVERY_ENTRY)((a), (b),(c), (d))
#define BTSRC_GAP_CancelDeviceDiscovery(a,b)                ((UINT16(*)(HANDLE, void*))GAP_CANCELDEVICEDISCOVERY_ENTRY)((a), (b))

/* BT GAP task.  */
#define BTSRC_InitRxLoop()                                  ((void(*)(void))INITRXLOOP_ENTRY)()

//a2dp profile interface
#define BTSRC_A2DP_RegCbk(a)                                ((void(*)(struct A2DP_RegCbkStru*))A2DP_REGCBK_ENTRY)((a))
#define BTSRC_A2DP_RegisterService(a,b)                     ((uint32(*)(UINT8, UINT16))A2DP_REGISTERSERVICE_ENTRY)((a), (b))
#define BTSRC_A2DP_UnregisterService(a)                     ((void(*)(UINT32))A2DP_UNREGISTERSERVICE_ENTRY)((a))
#define BTSRC_A2DP_Register_SEP(a)                          ((void(*)(struct A2DP_SEPStru *))A2DP_REGISTER_SEP_ENTRY)((a))
#define BTSRC_A2DP_UnRegister_SEP(a)                        ((void(*)(UINT8))A2DP_UNREGISTER_SEP_ENTRY)((a))
#define BTSRC_A2DP_Connect(a)                               ((void(*)(struct A2DP_ConnectReqStru *))A2DP_CONNECT_ENTRY)((a))
#define BTSRC_A2DP_Disconnect(a)                            ((void(*)(void*))A2DP_DISCONNECT_ENTRY)((a))
#define BTSRC_A2DP_StreamStart(a)                           ((void(*)(void*))A2DP_STREAMSTART_ENTRY)((a))
#define BTSRC_A2DP_StreamSuspend(a)                         ((void(*)(void*))A2DP_STREAMSUSPEND_ENTRY)((a))
#define BTSRC_A2DP_Delayreporting(a)                        ((int(*)(void*))A2DP_DELAYREPORTING_ENTRY)(a)

//avrcp profile interface
#define BTSRC_AVRCP_ConnectReq(a)                           ((void(*)(struct AVRCP_ConnectReqStru*))AVRCP_CONNECTREQ_ENTRY)((a))
#define BTSRC_AVRCP_DisconnectReq(a)                        ((void(*)(UINT8*))AVRCP_DISCONNECTREQ_ENTRY)((a))
#define BTSRC_AVRCP_ServerStart(a,b)                        ((UINT8(*)(UINT8, UINT16))AVRCP_SERVERSTART_ENTRY)((a),(b))
#define BTSRC_AVRCP_ServerStop()                            ((UINT8(*)(void))AVRCP_SERVERSTOP_ENTRY)()
#define BTSRC_AVRCP_RegCbk(a)                               ((void(*)(UINT8*))AVRCP_REGCBK_ENTRY)((a))
#define BTSRC_AVRCP_PassThrough_Req(a,b)                    ((void(*)(UINT8*, struct AVRCP_PassThroughStru*))AVRCP_PASSTHROUGH_ENTRY)((a),(b))
#define BTSRC_AVRCP_FreeEventParam(a,b)                     ((void(*)(UINT32, UINT8*))AVRCP_FREEEVENTPARAM_ENTRY)((a),(b))
#define BTSRC_AVRCP_AdvancedControl_Req(a,b,c,d)            ((void(*)(UINT8*, UINT32, void*, UINT16))AVRCP_ADVANCEDCONTROL_REQ_ENTRY)((a),(b),(c),(d))
#define AVRCP_PassThrough_Rsp(a,b,c)                        ((UINT8(*)(UINT8*, struct AVRCP_PassThroughStru*, UINT32))AVRCP_PASSTHROUGH_RSP_ENTRY)((a),(b),(c))
#define AVRCP_AdvancedControl_Req(a,b,c,d)                  ((void(*)(UINT8*, UINT32, void*, UINT16))AVRCP_ADVANCEDCONTROL_REQ_ENTRY)((a),(b),(c),(d))
#define AVRCP_AdvancedControl_Rsp(a,b,c,d)                  ((void(*)(UINT8*, UINT32, void*, UINT16))AVRCP_ADVANCEDCONTROL_RSP_ENTRY)((a),(b),(c),(d))
#define AVRCP_RegisterNotif_Rsp(a,b,c)                      ((void(*)(UINT32, struct AVRCP_RegisterNotifRspStru*, UINT16))AVRCP_REGISTERNOTIF_RSP_ENTRY)((a),(b),(c))
#define AVRCP_GeneralReject_Rsp(a,b,c)                      ((void(*)(UINT8*, UINT8, UINT32))AVRCP_GENERALREJECT_RSP_ENTRY)((a),(b),(c))

//hfp profile interface
#define BTSRC_HFP_Server_Start(a,b,c,d,e,f)                 ((void(*)(UINT32, UINT8, UINT32*, UINT8, UINT8, HFP_Callback))HFP_SERVER_START_ENTRY)((a),(b),(c),(d),(e),(f))
#define BTSRC_HFP_Server_Stop(a)                            ((void(*)(void*))HFP_SERVER_STOP_ENTRY)((a))
#define BTSRC_HFP_Client_Start(a,b,c,d,e)                   ((void(*)(void*, UINT8*, UINT8*, UINT8, HFP_Callback))HFP_CLIENT_START_ENTRY)((a),(b),(c),(d),(e))
#define BTSRC_HFP_Client_Stop(a)                            ((void(*)(UINT8*))HFP_CLIENT_STOP_ENTRY)((a))
#define HFP_HF_Dial(a,b)                                    ((void(*)(UINT8*,UINT8*))HFP_HF_DIAL_ENTRY)((a),(b))
#define HFP_HF_TxDTMF(a,b)                                  ((void(*)(UINT8*,UINT8))HFP_HF_TXDTMF_ENTRY)((a),(b))
#define HFP_HF_AudioConnTrans(a)                            ((void(*)(UINT8*))HFP_HF_AUDIOCONNTRANS_ENTRY)((a))
#define BTSRC_HFP_HF_AnswerCall(a)                          ((void(*)(UINT8*))HFP_HF_ANSWERCALL_ENTRY)((a))
#define BTSRC_HFP_HF_CancelCall(a)                          ((void(*)(UINT8*))HFP_HFP_HF_CANCELCALL_ENTRY)((a))
#define BTSRC_HFP_HF_HoldIncomingCall(a)                    ((void(*)(UINT8*))HFP_HF_HOLDINCOMINGCALL_ENTRY)((a))
#define HFP_HF_3WayCallingHandler(a,b,c)                    ((void(*)(UINT8*, UINT8, UINT8))HFP_HF_3WAYCALLINGHANDLER_ENTRY)((a),(b),(c))
#define HFP_HF_GetCurrentCalls(a)                           ((void(*)(UINT8*))HFP_HF_GETCURRENTCALLS_ENTRY)((a))
#define HFP_HF_VoiceRecognitionReq(a,b)                     ((void(*)(UINT8*, UINT8))HFP_HF_VOICERECOGNITIONREQ_ENTRY)((a),(b))
#define HFP_HF_DisableNREC(a)                               ((void(*)(UINT8*))HFP_HF_DISABLENREC_ENTRY)((a))
#define HFP_ExtendCmd(a,b,c,d)                              ((void(*)(UINT8*, void*, UINT16, UINT32))HFP_EXTENDCMD_ENTRY)((a),(b),(c),(d))
//hid profile interface
#define APP_HID_Start()                                     ((void(*)(void))APP_HID_START_ENTRY)()
#define App_HID_StartServerReq(a,b)                         ((void(*)(tApp_Hid_Callback, struct HID_RegInfoStru*))APP_HID_STARTSERVERREQ_ENTRY)((a),(b))
#define HID_Packet_Send_Interrupt(a,b)                      ((void(*)(uint8*, uint32))HID_PACKET_SEND_INTERRUPT_ENTRY)((a),(b))
#define HID_Packet_Send_Control(a,b)                        ((void(*)(uint8*, uint32))HID_PACKET_SEND_CONTROL_ENTRY)((a),(b))
#define HID_ConnectReq(a)                                   ((void(*)(struct HID_ConnectReqStru*))HID_CONNECTREQ_ENTRY)((a))
#define HID_DisconnectReq(a)                                ((void(*)(struct HID_ConnectReqStru*))HID_DISCONNECTREQ_ENTRY)((a))

//bt host interface
#define bt_init(a)                                          ((void(*)(bt_cfg_info_t*))BT_INIT_ENTRY)((a))
#define bt_deinit()                                         ((int(*)(void))BT_DEINIT_ENTRY)()
#define BtDev_LibExc()                                      ((void(*)(void))BTDEV_LIBEXC_ENTRY)()
#define BtDev_RxReadToHCI(a,b)                              ((void(*)(void*,void*))BTDEV_RXREADTOHCI_ENTRY)((a),(b))
#define set_exit_schedule_flag(a)                           ((void(*)(uint8))SET_EXIT_SCHEDULE_FLAG_ENTRY)((a))
#define regiter_callback_func(a)                            ((void(*)(regiter_func_t*))REGITER_CALLBACK_FUNC_ENTRY)((a))
#define set_avdtp_frame_buf(a,b)                            ((void(*)(uint8 *, uint8))SET_AVDTP_FRAME_BUF_ENTRY)((a),(b))
#define get_RxReadToHCI_flag()                              ((uint8(*)(void))GET_RXREADTOHCI_FLAG_ENTRY)()
#define set_RxReadToHCI_flag(a)                             ((void(*)(uint8))SET_RXREADTOHCI_FLAG_ENTRY)((a))
#define sco_mem_malloc(a)                                   ((void*(*)(uint32))SCO_MEM_MALLOC_ENTRY)((a))
#define sco_mem_free(a)                                     ((void*(*)(void*))SCO_MEM_FREE_ENTRY)((a))
#define mem_malloc(a)                                       ((void *(*)(uint32))MEM_MALLOC_ENTRY)((a))
#define mem_free(a)                                         ((void(*)(void *))MEM_FREE_ENTRY)((a))
#define MY_InitStaticMem(a,b)                               ((void(*)(uint32,uint16))MY_INITSTATICMEM_ENTRY)((a),(b))
#define MY_DoneStaticMem()                                  ((void(*)(void))MY_DONESTATICMEM_ENTRY)()
#define get_btmem_free_size()                               ((uint32(*)(void))GET_BTMEM_FREE_SIZE_ENTRY)()
#define Send(a,b,c)                                         ((int(*)(HANDLE, uint8*, uint32))SEND_ENTRY)((a),(b),(c))

#define check_hw_error_no(a)                                ((void(*)(bt_hw_error_no_e*))CHECK_HW_ERROR_NO_ENTRY)((a))

#define L2CAP_GetRemoteCID(a)                               ((uint16(*)(uint16))L2CAP_GETREMOTECID_ENTRY)((a))
#define L2CAP_Connection_Parameter_Update_Req(a)            ((void(*)(struct L2CAP_ConnectionParameterUpdateStru *))L2CAP_CONNECTION_PARAMETER_UPDATE_REQ_ENTER)((a))

#define has_avdtp_pkg_lost()                                ((bool(*)(void))HAS_AVDTP_PKG_LOST_ENTRY)()

#if (SPP_INTERFACE_SUPPORT == 1)
#define BTSRC_List_RemoveAll(a)                             ((void(*)(struct ListStru*))LIST_REMOVEALL_ENTRY)((a))
#define SDAP_ModifyStringAttrib(a,b,c)                      ((int16(*)(unsigned long long, uint8 *, int16))SDAP_MODIFYSTRINGATTRIB_ENTRY)((a),(b),(c))
#define APP_Spp_Start()                                     ((void(*)(void))APP_SPP_START_ENTRY)()
#define App_Spp_Stop()                                      ((void(*)(void))APP_SPP_STOP_ENTRY)()
#define App_Spp_StartServerReq(a,b,c)                       ((void(*)(tApp_Spp_Callback , UINT8 , UINT8 ))APP_SPP_STARTSERVERREQ_ENTRY)((a),(b),(c))
#define App_Spp_StopServerReq(a)                            ((void(*)(UINT32))APP_SPP_STOPSERVERREQ_ENTRY)((a))
#define App_Spp_ConnectReq(a,b,c,d)                         ((void(*)(tApp_Spp_Callback , UINT8 *, UINT8 , UINT8 ))APP_SPP_CONNECTREQ_ENTRY)((a),(b),(c),(d))
#define App_Spp_DisconnectReq(a)                            ((void(*)(UINT32))APP_SPP_DISCONNECTREQ_ENTRY)((a))
#define App_Spp_DataReq(a,b,c,d)                            ((void(*)(tApp_Spp_Callback , UINT32 , UINT8 *, UINT32 ))APP_SPP_DATAREQ_ENTRY)((a),(b),(c),(d))
#define App_Spp_GetCFG(a,b,c)                               ((void(*)(tApp_Spp_Callback , UINT32 , struct Spp_RpnParStru *))APP_SPP_GETCFG_ENTRY)((a),(b),(c))
#define App_Spp_Get_Credits(a)                              ((uint16(*)(uint32))APP_SPP_GET_CREDITS_ENTRY)((a))
#endif

#if (BLE_INTERFACE_SUPPORT == 1)
#define SMP_PeerKeys_Remove(a)                              ((void(*)(struct SMP_KeyMissStru *))SMP_PEERKEYS_REMOVE_ENTRY)((a))
#define SMP_PeerKeys_Recover(a)                             ((void(*)(struct ListStru *))SMP_PEERKEYS_RECOVER_ENTRY)((a))
//SMP_CompareAddr return detail :
// return : match      1
//          no match   0
#define SMP_CompareAddr(a,b)                                ((uint8(*)(struct HCI_AddressStru*, struct HCI_AddressStru*))SMP_COMPAREADDR_ENTRY)((a),(b))
#define SMP_RegLocalCap(a)                                  ((void(*)(struct SMP_RegLocalCapStru *in))SMP_REGLOCALCAP_ENTRY)((a))

#define GATT_Server_PendIND_Response(a)                     ((void(*)(struct ATT_PendIndStru *))GATT_SERVER_PENDIND_RESPONSE_ENTRY)((a))
#define GATT_Exchange_MTU(a)                                ((void(*)(struct GATT_TL_ConnectStru *))GATT_EXCHANGE_MTU_ENTRY)((a))
#define GATT_Write_Value(a,b,c)                             ((void(*)(struct GATT_TL_ConnectStru*, struct ATT_HandleValueStru*, UINT8))GATT_WRITE_VALUE_ENTRY)((a),(b),(c))
#define GATT_Read_Value(a,b)                                ((void(*)(struct GATT_TL_ConnectStru *, UINT16))GATT_READ_VALUE_ENTRY)((a),(b))
/**
 * LE 断开连接
 * 输入参数：
 *       struct HCI_AddressStru* rmt_addr
 *       void* context   
 *       void* cbk
 * 返回值:无
 */
#define GATT_TL_Disconnect(a,b,c)                           ((void(*)(struct HCI_AddressStru* rmt_addr, void* context, void* cbk))GATT_TL_DISCONNECT_ENTRY)((a),(b),(c))
#define GATT_Discover_Characteristics(a,b,c,d)              ((void(*)(struct GATT_TL_ConnectStru*, UINT16,UINT16, struct ATT_UUIDStru*))GATT_DISCOVER_CHARACTERISTICS_ENTRY)((a),(b),(c),(d))
#define GATT_Discover_Services(a,b)                         ((void(*)(struct GATT_TL_ConnectStru *, struct ATT_UUIDStru*))GATT_DISCOVER_SERVICES_ENTRY)((a),(b))
#define GATT_Free_Service(a)                                ((void(*)(struct GATT_ServiceStru *))GATT_FREE_SERVICE_ENTRY)((a))
#define GATT_Server_UnRegTree(a)                            ((void(*)(UINT16))GATT_SERVER_UNREGTREE_ENTRY)((a))
#define GATT_Free_Character(a)                              ((void(*)(struct GATT_CharacteristicStru *))GATT_FREE_CHARACTER_ENTRY)((a))
#define GATT_Free_Descriptor(a)                             ((void(*)(struct GATT_CharacteristicStru *))GATT_FREE_DESCRIPTOR_ENTRY)((a))
#define GATT_Assemble_Value(a,b)                            ((struct ATT_ValueStru*(*)(struct ATT_HandleValueStru*, struct ATT_ValueStru*))GATT_ASSEMBLE_VALUE_ENTRY)((a),(b))
#define GATT_Server_NotifyIndicate(a)                       ((void(*)(struct GATT_Server_NotifyIndicateStru *))GATT_SERVER_NOTIFYINDICATE_ENTRY)((a))
#define GATT_Discover_AllCharacteristic_Descriptors(a,b,c)  ((void(*)(struct GATT_TL_ConnectStru*, UINT16, UINT16))GATT_DISCOVER_ALLCHARACTERISTIC_DESCRIPTORS_ENTRY)((a),(b),(c))
#define GATT_GetHandle_ByUUID(a,b)                          ((UINT16 (*)(UINT16 start_hdl, struct ATT_UUIDStru *uuid))GATT_GETHANDLE_BYUUID_ENTRY)((a),(b))
#define GATT_Server_ModifyValue(a)                          ((void (*)(struct ATT_ModifyValueStru *in))GATT_SERVER_MODIFYVALUE_ENTRY)((a))
#define GATT_AttChannelCbk_Reg(a)                           ((void (*)(GATTFunc *cbk))GATT_ATTCHANNELCBK_REG_ENTRY)((a))
#define GATT_NotificationCbk_Reg(a)                         ((void (*)(GATTFunc *cbk))GATT_NOTIFICATIONCBK_REG_ENTRY)((a))
#define GATT_Add_Characteristic(a,b,c,d,e)                  ((struct GATT_CharacteristicStru * (*)(struct GATT_ServiceStru *, UINT8 *, UINT16, struct ATT_UUIDStru *, UINT16))GATT_ADD_CHARACTERISTIC_ENTRY)((a),(b),(c),(d),(e))
#define GATT_Add_Descriptor_CCC(a,b,c)                      ((void (*)(struct GATT_CharacteristicStru *, UINT8 , UINT8))GATT_ADD_DESCRIPTOR_CCC_ENTRY)((a),(b),(c))
#define GATT_Add_Descriptor_ReportReference(a,b,c,d)        ((void (*)(struct GATT_CharacteristicStru *, UINT8 , UINT8, UINT8))GATT_ADD_DESCRIPTOR_REPORTREFERENCE_ENTRY)((a),(b),(c),(d))
#define GATT_Server_RegTree(a)                              ((void (*)(struct GATT_ServiceStru *))GATT_SERVER_REGTREE_ENTRY)((a))
#define GATT_Server_ServiceChanged(a,b)                     ((void (*)(UINT16,UINT16))GATT_SERVER_SERVICECHANGED_ENTRY)((a),(b))

#define LE_Set_Scan_Parmeters(a,b,c)                        ((void(*)(UINT8, UINT8, UINT8))LE_SET_SCAN_PARMETERS_ENTRY)((a),(b),(c))
#define LE_Set_Scan_Enable(a)                               ((void(*)(UINT8))LE_SET_SCAN_ENABLE_ENTRY)((a))
/**
 * 设置广播配置参数
 * 输入参数：
 *       UINT8 adv_type (enum _tHCI_Advertising_Type)
 *       UINT8 disc_mode ( Discovery mode )    
 *       UINT8 adv_speed (Advertising/Scanning Speed)
 *       UINT8 adv_filter_policy(tHCI_Advertising_Filter_Policy)
 * 返回值:无
 */
#define LE_Set_Advertising_Parameters(a,b,c,d)              ((void(*)(UINT8 adv_type, UINT8 disc_mode, UINT8 adv_speed, UINT8 adv_filter_policy))LE_SET_ADVERTISING_PARAMETERS_ENTRY)((a),(b),(c),(d))
/**
 * 开启/关闭广播
 * 输入参数：
 *       UINT8 adv_enable (typedef enum _tHCI_Advertising_Enable)
 * 返回值:无
 */
#define LE_Set_Advertise_Enable(a)                          ((void(*)(UINT8 adv_enable))LE_SET_ADVERTISE_ENABLE_ENTRY)((a))
/**
 * 注册GAP和GATT服务
 * 输入参数：
 *       UINT8 cbk  service回调函数
 * 返回值:无
 */
#define LE_GAP_ServiceReg(a)                                ((void(*)(void *))LE_GAP_SERVICEREG_ENTRY)((a))

#define ATT_UUID_GetU2(a)                                   ((uint16(*)(struct ATT_UUIDStru *))ATT_UUID_GETU2_ENTRY)((a))
#define ATT_UUID_Get(a,b,c)                                 ((void(*)(struct ATT_UUIDStru *out, UINT8 *p, UINT8 len))ATT_UUID_GET_ENTRY)((a),(b),(c))
#define ATT_UUID_SetU16(a,b)                                ((void(*)(struct ATT_UUIDStru *out, UINT8 *str))ATT_UUID_SETU16_ENTRY)((a),(b))
#define ATT_UUID_SET_U2(a,b)                                ((void(*)(struct ATT_UUIDStru *out, UINT16 u2))ATT_UUID_SETU2_ENTRY)((a),(b))
#define List_RemoveAt(a,b)                                  ((void(*)(struct ListStru*, void*))LIST_REMOVEAT_ENTRY)((a),(b))
/**
 * HCI命令发送
 * 输入参数：
 *       void * arg 
 *       UINT32 hci_cmd_ops
 * 返回值:无
 */
#define HCI_Command_SendInter(a,b)                          ((void(*)(void *arg, UINT32 hci_cmd_ops))HCI_COMMAND_SENDINTER_ENTRY)((a),(b))
#define HCI_LE_Capacity_Get()                               ((uint32(*)(void))HCI_LE_CAPACITY_GET_ENTRY)()
#endif

#endif
