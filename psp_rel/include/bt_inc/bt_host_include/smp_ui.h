/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
Module Name:
    smp_ui.h
Abstract:
    This module defines the security manager protocol UI interface functions
    & macros.
---------------------------------------------------------------------------*/

#ifndef G_SMP_UI_H
#define G_SMP_UI_H

#include "global.h"
#include "hci_ui.h"

/*---------------------------------------------------------------------------*/
/*                              SMP Macros & Structures                      */
/*---------------------------------------------------------------------------*/
typedef void (SMP_CbkFunc)(UINT8 event, void *arg);/* callback to ATT/APP UI */

enum
{
    SMP_MASTER =    0,/* Master is Initiator */
    SMP_SLAVE =     1/* Slave is Responder */
};

enum  /* mask in SMP_LELink_Check */
{
    SMP_LINKCHECK_MITM =                0x01,/* Link, Not By just work */
    SMP_LINKCHECK_ENCRYPTED =           0x02,/* Link, Encrypted */
    SMP_LINKCHECK_ENCRYPT_KEYSIZE =     0x04,/* Link, Encryption keysize is good enough */
    SMP_LINKCHECK_BOND =                0x08,/* Key, Bonding flag */
    SMP_LINKCHECK_LTK =                 0x10,/* Remote, LTK, eDIV, Rand? */
    SMP_LINKCHECK_CSRK =                0x20,/* Local, Write Sign */
    SMP_LINKCHECK_IRK =                 0x40/* Remote, IRK, Public/Static Address */
};

/*-----------------------------------------------------------------------------
LE Key Distribution Format:
    EncKey-(LTK,EDIV,Rand)(1bit), IdKey-(IRK)(1bit), Sign-(CSRK)(1bit), Reserved(5bits)
-----------------------------------------------------------------------------*/
enum
{
    /* EncKey is a 1-bit field that is set to one to indicate that the device shall distribute LTK
    using the Encryption Information command followed by EDIV and Rand using the Master
    Identification command. */
    SMP_KEYDMASK_LTK_EDIV_RAND =                (1U),

    /* IdKey is a 1-bit field that is set to one to indicate that the device shall distribute IRK
    using the Identity Information command followed by its public device or static random
    address using Identity Address Information */
    SMP_KEYDMASK_IRK =                          (1U << 1),

    /* Sign is a 1-bit field that is set to one to indicate that the device shall distribute CSRK
    using the Signing Information command */
    SMP_KEYDMASK_CSRK =                         (1U << 2),

    SMP_KEYDMASK_ALL =                          (SMP_KEYDMASK_LTK_EDIV_RAND | SMP_KEYDMASK_IRK | SMP_KEYDMASK_CSRK)
};

enum  /* IO Capability */
{
    SMP_IO_DISPLAY_ONLY =                       0x00,
    SMP_IO_DISPLAY_YESNO =                      0x01,
    SMP_IO_KEYBOARD_ONLY =                      0x02,
    SMP_IO_NOINPUT_NOOUTPUT =                   0x03,
    SMP_IO_KEYBOARD_DISPLAY =                   0x04/* 0x05-0xFF Reserved */
};

enum  /* OOB Data Present Values */
{
    SMP_OOB_AUTHENTICATION_DATA_NOTPRESENT =    0x00,
    SMP_OOB_AUTHENTICATION_DATA_PRESENT =       0x01/* From link device */
};

/*-----------------------------------------------------------------------------
Authentication Requirements Flags Format:
    Bonding_Flags(2bits), MITM(1bit), Reserved(5bits)
-----------------------------------------------------------------------------*/
enum  /* AuthReq(Authentication Requirements Flags) in Pairing Request/Response */
{
    SMP_AUTHREQ_BONDING_FLAG =                  (1U),       /* 2bits, 00=NoBonding, 01=Bonding, others reserved */
    SMP_AUTHREQ_BONDING_FLAG_RESERVED =         (1U << 1),  /* Reserved, but defination is not clear */
    /* 1bit, when authentcation use no just work, then MITM authentated */
    SMP_AUTHREQ_MITM =                          (1U << 2),

    SMP_AUTHREQ_SECURE_CONNECTIONS =            (1U << 3),  /* 1bit, is the device capable of LE Secure Connections? */
    /* used to ignore reserved bits in authreq */
    SMP_AUTHREQ_VAILD_RANGE =                   (SMP_AUTHREQ_BONDING_FLAG | SMP_AUTHREQ_MITM)
};

enum  /* event in SMP_Cbk */
{
    /* For requester */
    SMP_EV_SECURITY_OK,
    SMP_EV_SECURITY_FAILED,

    /* For application UI */
    SMP_EV_TK_OOB,
    SMP_EV_TK_DISPLAY,                      /* 6 digits, display as %c into char */
    SMP_EV_TK_PASSKEY_ENTRY,                /* application input 6 digits */
    SMP_EV_TK_KILL_WINDOW,                  /* TK_Display and TK_Passkey_Entry window need to be closed */
    SMP_EV_KEY_STORE,                       /* application need to store these keys indexed by addr */
    SMP_EV_KEY_MISS,                        /* Remote key missing, controller tell us PIN or Key Missing */
    SMP_EV_ECDH_VA_CONFIRM,                 /* ECDH, Va Confirm for Just Work, struct SMP_TKCbkStru */
};

enum  /* mask of struct SMP_RequireNodeStru */
{
    /* Not initiated by slave security_request or master pairing_request */
    SMP_REQUIRE_MASK_INCOMING =                 0x01,
    SMP_REQUIRE_MASK_SECURE_CONNECTIONS_ONLY =  0x02,/* Will the device ONLY use Secure Connections (e.g. FIPS) mode? */
    /* Already callbacked to upper layer for the require result of SMP */
    SMP_REQUIRE_MASK_RESULT_CBK =               0x04
};

struct SMP_RequireNodeStru
{
    void *cbk;
    void *context;
    UINT8 mode;                         /* etc: LE_GAP_SECURITY_MODE1_LEVEL1, used to detect what to do */
    /* etc: SMP_AUTHREQ_MITM, this is requirement, from incoming PDU or upper set, must fullfill */
    UINT8 authreq;
    UINT8 mask;                         /* etc: SMP_REQUIRE_MASK_INCOMING */
    UINT8 key_distrib[2];               /* x: SMP_MASTER etc: SMP_KEYDMASK_LTK_EDIV_RAND */
};

struct SMP_PairStartStru                /* in of SMP_Pairing_Start */
{
    struct HCI_AddressStru addr;        /* Remote addr to find LELink */
    void *cbk;
    void *context;
    UINT8 mode;                         /* etc: LE_GAP_SECURITY_MODE1_LEVEL1 */
};

struct SMP_RegLocalCapStru              /* in of SMP_RegLocalCap */
{
    /* Each device shall have maximum and minimum encryption key length parameters which defines
    the maximum and minimum size of the encryption key allowed in octets. The maximum and minimum
    encryption key length parameters shall be between 7 octets (56 bits) and 16 octets (128 bits),
    in 1 octet (8 bit) steps. This is defined by a profile or device application */
    UINT8 max_encry_keysize;            /* 7~16 */
    UINT8 min_encry_keysize;            /* 7~16 */
    void *cbk;                          /* Application UI cbk, for etc: SMP_EV_TK_OOB */
};

struct SMP_SetTKStru                    /* in of SMP_SetTK */
{
    struct HCI_AddressStru addr;        /* SMP_ADDR_REMOTE_LINK of peer */
    UINT8 len;                          /* tk=NULL, len=0 means cancelled, 6 for Passkey Entry */
    UINT8 tk[16];                       /* Max len 16 */
};

struct SMP_VaConfirmStru                /* in of SMPC_VaConfirm_Got */
{
    struct HCI_AddressStru addr;        /* SMP_ADDR_REMOTE_LINK of peer */
    UINT8 is_ok;                        /* OK, not OK */
};

enum  /* mask in struct SMP_KeyStru */
{
    SMP_KEYMASK_LTK =       0x01,/* Peer Encryption_Information */
    SMP_KEYMASK_EDIV_RAND = 0x02,/* Peer Master_Identification, LTK+eDIV,Rand = Bond? */
    SMP_KEYMASK_IRK =       0x04,/* Peer Identity_Information */
    SMP_KEYMASK_ADDR =      0x08,/* Identity_Address_Information */
    SMP_KEYMASK_CSRK =      0x10,/* Signing_Information */
    /* By not JustWork, MITM protected, The security properties of the distributed keys
    shall be set to the security properties of the STK that was used to distribute them */
    SMP_KEYMASK_MITM =      0x20
};

struct SMP_KeyStru  /* Master role security database */
{
    UINT8 give;                     /* SMP_KEYMASK_LTK, local give remote these keys */
    UINT8 div[2];                   /* 16bits, 2octets, Peripherial(slave) distributed DIV for remote(Master) */
    /* SMP_KEYMASK_LTK, remote give local these keys, include SMP_KEYMASK_MITM for both direction */
    UINT8 mask;
    UINT8 ltk_len;                  /* as Security Level */
    UINT8 ltk[16];
    UINT8 ediv[2];
    UINT8 rand[8];                  /* Encryption keys */
    UINT8 irk[16];                  /* Resolveable address */
    UINT8 csrk[16];                 /* Signature */
};

struct SMP_KeyStoreStru             /* Keys that indexes by addr */
{
    struct HCI_AddressStru addr;    /* SMP_ADDR_REMOTE_PUBLIC_STATIC of peer */
    struct SMP_KeyStru key;         /* Bonding information */
};

struct SMP_TKCbkStru
{
    struct HCI_AddressStru addr;/* SMP_ADDR_REMOTE_LINK of peer */
    UINT8 c[6];/* 6 digit char %c */
};

struct SMP_KeyMissStru
{
    struct HCI_AddressStru addr;/* SMP_ADDR_REMOTE_LINK of peer */
};


#endif/* G_SMP_UI_H */

