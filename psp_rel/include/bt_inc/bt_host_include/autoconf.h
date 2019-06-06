/*
 * Automatically generated C config: don't edit
 */
#define AUTOCONF_INCLUDED
/*
 * System dependent Selection
 */
#define CONFIG_GNUMAKE 1
#define CONFIG_CHANGE_VERSION 1
#define CONFIG_MAJOR_VERSION 0x8
#define CONFIG_MINOR_VERSION 0x5
#define CONFIG_PATCH_LEVEL 0x0
#define CONFIG_BUILD_RELEASE 0x20121018
#define CONFIG_DEBUG_SYMBOL 1
//#define CONFIG_DEBUG 1
#undef  CONFIG_GNU
#define CONFIG_MICROSOFT 1
#undef  CONFIG_BORLAND
#undef  CONFIG_ARM
#undef  CONFIG_ARMTCC
#undef  CONFIG_DIABDCC
#undef  CONFIG_HITACHI
#define CONFIG_OS_WIN32 1
#undef  CONFIG_OS_UC
//#undef  CONFIG_APP_CREATE_THREADS
#define  CONFIG_APP_CREATE_THREADS
#define CONFIG_LITTLE_ENDIAN 1
#undef  CONFIG_BIG_ENDIAN
#undef  CONFIG_ALIGN_ONEBYTE
#undef  CONFIG_ALIGN_TWOBYTES
#define CONFIG_ALIGN_FOURBYTES 1
#undef  CONFIG_TYPE_NO_LIMIT
#define CONFIG_TYPE_BIT32_ONLY 1
#undef  CONFIG_TYPE_BIT16_ONLY

//#ifndef CONFIG_MEMORY_STATIC
#define  CONFIG_MEMORY_STATIC
//#endif/* CONFIG_MEMORY_STATIC */

#undef  CONFIG_MEMORY_LEAK_DETECT
#undef  CONFIG_CHECK_STACK
#define CONFIG_USER_POOL_SIZE (32768)
#define CONFIG_CPU_BIT32 1
#undef  CONFIG_CPU_BIT16
#undef  CONFIG_FUNC_LIST
#define CONFIG_QUEUE_MAX (64)
#define CONFIG_GAP 1
/*
 * HCI Configuration
 */
#undef  CONFIG_HCI_10A
#undef  CONFIG_HCI_10B
#undef  CONFIG_HCI_11
#undef  CONFIG_HCI_12
#undef  CONFIG_HCI_21
#define CONFIG_HCI_30 1
#define CONFIG_HCI_DFLT_LKSUPER_TM (20000)
#define CONFIG_HCI_CONN_ACCEPT_TIMEOUT (29000)
#define CONFIG_HCI_ACL_BUFF_SIZE (1024000)
#define CONFIG_HCI_SCO_BUFF_SIZE (5120)
#undef  CONFIG_HC2HOST_FC_ENABLE
#undef  CONFIG_HCI_NO_SCO_DATA
/*
 * L2cap Configuration
 */
#define CONFIG_BLUELET 1
#define CONFIG_L2CAP_RTX_TIMEOUT (3000)
#define CONFIG_L2CAP_ERTX_TIMEOUT (60000)
#define CONFIG_L2CAP_HCI_DISCONN_TO (1000)
#define CONFIG_L2CAP_TIMEOUT (60000)
#define CONFIG_L2CAP_MAX_CHANNEL_NUM (100)
/*
 * SDP Configuration
 */
#undef  CONFIG_SDP_FIXED_DATABASE
/*
 * Rfcomm Configuration
 */
#define CONFIG_RFCOMM 1
#define CONFIG_CREDIT (4)
#define CONFIG_RFCOMM_DEFAULT_MTU (1600)
#define CONFIG_RFCOMM_SYS_N1 (1012)
#define CONFIG_RFCOMM_SYS_T1 (10000)
#define CONFIG_RFCOMM_SYS_T2 (10000)
/*
 * BLE Configuration
 */
//#define CONFIG_LE 1	//ble add
/*
 * ATT Configuration // ble add
 */
//#define CONFIG_ATT 1
/*
 * AVCTP Configuration
 */
#define CONFIG_AVCTP 1
/*
 * AVRCP Configuration
 */
#define CONFIG_AVRCP 1
#undef  CONFIG_AVRCP_10
#define CONFIG_AVRCP_14 1
/*
 * AVDTP Configuration
 */
#define CONFIG_AVDTP 1
#define CONFIG_AVDTP_TRANSACTION_RTX_TIMEOUT (5000)
/*
 * A2DP Configuration
 */
#define CONFIG_A2DP 1
#define CONFIG_A2DP_SNK 1
#define CONFIG_A2DP_DEFAULT_MTU (672)
#define CONFIG_A2DPAPP_WAVEHEADER_NUM (20)
/*
 * SDAP and Service Database Management
 */
#define CONFIG_SDAP 1
#define CONFIG_SDAP_SERVER 1
#define CONFIG_SDAP_CLIENT 1
#define CONFIG_SDAP_COMPLETE_VERSION 1
#define CONFIG_DI_SERVER 1
#define CONFIG_DI_CLIENT 1
/*
 * Serial port profile
 */
//#define CONFIG_SPP 1
#undef  CONFIG_SPP_VSERIAL
#define CONFIG_SPP_CLIENT 1
#define CONFIG_SPP_SERVER 1
/*
 * Hands Free profile
 */
#define CONFIG_HFP 1
#define CONFIG_HFP_HF 1
#define CONFIG_HFP_ATCMD_TIMEOUT_SHORT (3000)
#define CONFIG_HFP_ATCMD_TIMEOUT_LONG (6000)
#define CONFIG_HFP_HF_RING_TIMEOUT (180000)
/*
 * Generic OBEX
 */
//#define CONFIG_GOEP 1
//#define CONFIG_OBEX 1
#define CONFIG_OBEX_ADDITIONAL 1
#define CONFIG_OBEX_UTF8 1
#define CONFIG_GOEP_MAX_RECV_LEN (4248)
#define CONFIG_OBEX_MAX_SEND_LEN (4248)
#define CONFIG_GOEP_CLIENT 1
#undef  CONFIG_GOEP_SERVER
#define CONFIG_GOEP_AUTH 1
/*
 * OBEX Phone Book Access
 */
//#define CONFIG_PBAP 1
#define CONFIG_PBAP_CLIENT 1
