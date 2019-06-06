/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib_init.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

#define NUM_KEYS    (0x4)

// 简化HID描述符
#define INPUT_REPORT_ID         3
#define INPUT_REPORT_BYTES      1

#define OUTPUT_REPORT_ID        3
#define OUTPUT_REPORT_BYTES     2

#define HID_HDR_SIZE            2

const uint8 hid_descriptor[] =
{
#if 0
        0x05, 0x01, //USAGE_PAGE (Generic Desktop Controls)
        0x09, 0x06,//USAGE (Keyboard)
        0xa1, 0x01,//COLLECTION (Application (mouse, keyboard))
        0x85, 0x01,//REPORT_ID (1)
        0x75, 0x01,//report size(1)
        0x95, 0x08,//report count(8)
        0x05, 0x07,//usage page(Keyboard/Keypad )
        0x19, 0xe0,//Usage Minimum
        0x29, 0xe7,//Usage Maximum
        0x15, 0x00,//Logical Minimum
        0x25, 0x01,//Logical Maxiimum
        0x81, 0x02,//Input()

        0x75, 0x01,//report size(1)
        0x95, 0x08,//report count()
        0x81, 0x01,//input()

        0x75, 0x08,//report size()
        0x95, 0x01,//report count()
        0x91, 0x03,//Output

        0x95, NUM_KEYS,//Report Count
        0x75, 0x08,//report size
        0x15, 0x00,//Logical Minimum
        0x26, 0xff, 0x00,//Logical Maxiimum
        0x05, 0x07,//usage page(Keyboard/Keypad )
        0x19, 0x00,//Usage Minimum
        0x29, 0xff,//usage Maximum
        0x81, 0x00,//input()
        0xc0,//END_COLLECTION

        0x05, 0x01,// USAGE_PAGE (Generic Desktop Controls)
        0x09, 0x02,//USAGE (Mouse)
        0xa1, 0x01,//COLLECTION (Application (mouse, keyboard))
        0x85, 0x02,//REPORT_ID (2)
        0x09, 0x01,//USAGE (Pointer)
        0xa1, 0x00,//COLLECTION (Physical (group of axes))
        0x05, 0x09,//usage page(Button)
        0x19, 0x01,//Usage Minimum
        0x29, 0x08,//Usage Maximum
        0x15, 0x00,//Logical Minimum
        0x25, 0x01,//Logical Maximum
        0x95, 0x08,//Report Count
        0x75, 0x01,//Report size
        0x81, 0x02,//input()
        0x05, 0x01,//usage page()
        0x09, 0x30,//usage()
        0x09, 0x31,//usage()
        0x09, 0x38,//usage()
        0x15, 0x81,//logical minimum
        0x25, 0x7f,//logical maximum
        0x75, 0x08,//report size
        0x95, 0x03,//report count
        0x81, 0x06,//input
        0x05, 0x0c,//usage page(Consumer)
        0x0a, 0x38, 0x02,//usage()
        0x75, 0x08,//report size
        0x95, 0x01,//report count
        0x15, 0x81,//logical minimum
        0x25, 0x7f,//logical maximum
        0x81, 0x06,//input()

        0x75, 0x08,//report size()
        0x95, 0x02,//report count()
        0x91, 0x03,//Output
        0xc0,//END_COLLECTION
        0xc0,// END_COLLECTION
#endif

        0x05, 0x0c, // USAGE_PAGE (Consumer)
        0x09, 0x01, //USAGE (Consumer control)
        0xa1, 0x01, //COLLECTION (Application (mouse, keyboard))
        0x85, INPUT_REPORT_ID, //REPORT_ID (3)
        0x15, 0x00, //Logical Minimum
        0x25, 0x01, //Logical Maximum
        0x75, 0x01, //Report size(1)
        0x95, 0x08,   //Report Count(8)
        //        0x0a,0x24,0x02, //USAGE (AC back)
        //        0x0a,0x25,0x02, //USAGE (AC forward)
        //        0x0a,0x27,0x02, //USAGE (AC refresh)
        //        0x0a,0x21,0x02, //USAGE (AC search)
        //        0x0a,0x2a,0x02, //USAGE (AC bookmarks)
        //        0x0a,0x23,0x02, //USAGE (AC home)
        //        0x0a,0x8a,0x01, //USAGE (AC email reader)
        //        0x09,0xe2,  //USAGE (mute)
        0x09, 0xea, //USAGE (volume down)
        0x09, 0xe9, //USAGE (volume up)
        0x09, 0xe2, //USAGE (mute)
        0x09, 0xcd, //USAGE (play/pause)
        0x09, 0xb6, //USAGE (scan previous track)
        0x09, 0xb5, //USAGE (scan next track)
        0x09, 0x83, //USAGE (fast forward)
        0x09, 0xb4, //USAGE (rewind)
        0x81, 0x02, //input(data, variable, absolute)
        //        0x75,0x01,  //Report size(1)
        //        0x95,0x1e,    //Report Count(30)
        //        0x81,0x03,  //input(constant, variable, absolute)

        0x75, 0x08, //report size()
        0x95, OUTPUT_REPORT_BYTES, //report count()
        0x91, OUTPUT_REPORT_ID, //Output

        0xc0,       //END_COLLECTION
//    0xc0        // END_COLLECTION
        };

const uint8 hid_keyboard_val[] =
{ 0x28, 0x29, 0x2a, 0x2b };

void APP_HID_REG(void)
{
    struct HID_RegInfoStru *reg_info;

    reg_info = NEW(sizeof(struct HID_RegInfoStru));
    libc_memset(reg_info, 0, sizeof(struct HID_RegInfoStru));
    reg_info->mask |= HID_MASK_VCABLE_TRUE | HID_MASK_RECONNINIT_TRUE;
    reg_info->parser_ver = 0x0111;
    reg_info->sub_cls = 0x40;       // For BQB
    reg_info->country_code = 0x21;
    reg_info->desc_list_size = sizeof(hid_descriptor);
    reg_info->desc_ptr = (uint8 *) hid_descriptor;

    App_HID_StartServerReq(HidAppCbk, reg_info);
}

bool HID_connect(bt_dev_t *dev)
{
    struct HID_ConnectReqStru *in;

    in = NEW(sizeof(struct HID_ConnectReqStru));
    libc_memset(in, 0, sizeof(struct HID_ConnectReqStru));
    libc_memcpy(in->bd_addr, dev->dev_info.bd_addr.bytes, BD_ADDR_LEN);
    HID_ConnectReq(in);
    return TRUE;
}

void HID_disconnect(void)
{
    struct HID_ConnectReqStru *in;

    in = NEW(sizeof(struct HID_ConnectReqStru));
    libc_memset(in, 0, sizeof(struct HID_ConnectReqStru));
    libc_memcpy(in->bd_addr, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes, BD_ADDR_LEN);
    HID_DisconnectReq(in);
}

void HidAppCbk(UINT32 inst_handle, tHid_Event event, void *param)
{
    bt_dev_t *dev;
    HID_DBG_PRINTF("hid cbk ev %d\n", event);
    switch (event)
    {
        case APP_HID_EV_CONNECT_COMPLETE:
        dev = RmtFindDevByBD(((struct HidConnectionInforStru *) param)->bd);
        if (dev == NULL)
        {
            return;
        }
        if (((struct HidConnectionInforStru *) param)->result == APP_HID_ER_SUCCESS)
        {
            dev->p_dev_status->service_connect |= HID_CONNECTED;
            dev->p_dev_status->need_auto_connect &= HID_DISCONNECTED;
            dev->profile |= HID_CONNECTED;

            save_rmtdev_info();
            HID_PRINTF("connect complet\n");
            if (get_cur_device_status(dev) == STATUS_WAIT_PAIRED)
            {
                bt_linking_flag = FALSE;
                set_cur_device_status(dev, STATUS_LINKED);
            }
        }
        else
        {
            HID_PRINTF_WARNNING("connect fail\n");
        }
        break;

        case APP_HID_EV_DISCONNECT_COMPLETE:
        dev = RmtFindDevByBD(((struct HidConnectionInforStru *) param)->bd);
        if (dev == NULL)
        {
            return;
        }
        dev->p_dev_status->service_connect &= (uint8) HID_DISCONNECTED;
        HID_PRINTF("dis connect\n")
        ;
        break;

        case APP_HID_EV_DATA_IND:
        HID_DataInd((struct HidDataIndStru *) param);
        break;

        default:
        break;
    }
    FREE(param);
}

void HID_deal_keyboard_msg(uint8 msg_type)
{
    uint8 tmp_data[8];
    // Photograph function
    HID_DBG_PRINTF("keyboard msg %d\n", msg_type);
    libc_memset(tmp_data, 0, 8);
    tmp_data[0] = 0xa1; //msg type=data(0x0a,bit4-7) and report type=input(0x01,bit0-3)
    tmp_data[1] = hid_keyboard; // report id = 1
    tmp_data[2] = 0x00;
    tmp_data[3] = 0x00;
    //msg type 参照HID Usage Tables 1.12.pdf中的10 Keyboard/Keypad Page (0x07)
    //Table 12: Keyboard/Keypad Page表
    if (msg_type >= sizeof(hid_keyboard_val))
    {
        tmp_data[4] = hid_keyboard_val[0]; //enter for android
    }
    else
    {
        tmp_data[4] = hid_keyboard_val[msg_type]; //enter for android
    }

    tmp_data[5] = 0x00;
    tmp_data[6] = 0x00;
    tmp_data[7] = 0x00;

    HID_Packet_Send_Interrupt(tmp_data, 8);
    tmp_data[4] = 0x00; //release
    HID_Packet_Send_Interrupt(tmp_data, 8);
}

void HID_deal_consumer_ctl_msg(uint8 msg_type)
{
    uint8 tmp_data[3];
    HID_DBG_PRINTF("consumer control %d\n", msg_type);
    libc_memset(tmp_data, 0, sizeof(tmp_data));
    tmp_data[0] = 0xa1; //msg type=data(0x0a,bit4-7) and report type=input(0x01,bit0-3)
    tmp_data[1] = hid_consumer_control; // report id=3
    //每一个bit代表一种控制功能，对应到下面的信息
    // 参照HID Usage Tables 1.12.pdf中的15 Consumer Page (0x0C)
    //Table 17: Consumer Usage Page
    /*
     0x09, 0xea, //USAGE (volume down)
     0x09, 0xe9, //USAGE (volume up)
     0x09, 0xe2, //USAGE (mute)
     0x09, 0xcd, //USAGE (play/pause)
     0x09, 0xb6, //USAGE (scan previous track)
     0x09, 0xb5, //USAGE (scan next track)
     0x09, 0x83, //USAGE (fast forward)
     0x09, 0xb4, //USAGE (rewind)

     */
    tmp_data[2] = 0x01 << (msg_type & 0x07);
    HID_Packet_Send_Interrupt(tmp_data, 3);
    tmp_data[2] = 0x00; //release
    HID_Packet_Send_Interrupt(tmp_data, 3);
}

void HID_DataInd(struct HidDataIndStru * dataInd)
{
    uint8 hid_hdr, report_type, report_id, *buf = dataInd->data;
    uint32 data_len = dataInd->data_size;
    uint8 report_buf[4];

    HID_DBG_PRINTF("HID_DataInd\n");
    //libc_printhex(dataInd->data, dataInd->data_size, 0);

    hid_hdr = (*buf) >> 4;
    report_id = *(buf + 1);
    switch (hid_hdr)
    {
        case HID_CONTROL:
        HID_Handle_Control_Message((*buf) & 0x0f);
        break;

        case HID_SET_REPORT:
        report_type = (*buf) & 0x03;
        if (report_type == 3)
        {
            HID_Send_Handshake_Packet(Hid_invalid_param);
        }
        else if (report_id != 3)
        {
            HID_Send_Handshake_Packet(Hid_invalid_id);
        }
        else if (data_len != OUTPUT_REPORT_BYTES + HID_HDR_SIZE)
        {
            HID_Send_Handshake_Packet(Hid_invalid_param);
        }
        else
        {
            HID_Send_Handshake_Packet(Hid_success);
        }
        break;

        case HID_GET_REPORT:
        report_type = (*buf) & 0x03;
        if (report_id != 3)
        {
            HID_Send_Handshake_Packet(Hid_invalid_id);
        }
        else if ((*buf) & 0x04)     // This bit MUST be 0
        {
            HID_Send_Handshake_Packet(Hid_invalid_param);
        }
        else
        {
            uint8 len = 0;

            report_buf[0] = 0xa0 + report_type;
            report_buf[1] = 3;      // report id
            report_buf[2] = 0;
            report_buf[3] = 0;

            if (report_type == 1)           // input
            {
                len = INPUT_REPORT_BYTES + HID_HDR_SIZE;
            }
            else if (report_type == 2)      // output
            {
                len = OUTPUT_REPORT_BYTES + HID_HDR_SIZE;
            }
            HID_Packet_Send_Control(report_buf, len);
            //HID_Send_Handshake_Packet(Hid_invalid_param);
        }
        break;

        case HID_SET_PROTOCAL:
        HID_Send_Handshake_Packet(Hid_unsupported);
        break;

        case HID_GET_PROTOCAL:
        HID_Send_Handshake_Packet(Hid_unsupported);
        break;

        case HID_DATC:
        break;

        case HID_DATA:
        report_type = (*buf) & 0x03;
        if ((dataInd->data_size <= 2) && (report_type == 2))
        {
            break;
        }

        if (report_type != 2)
        {
            break;
        }

        HID_Send_Handshake_Packet(Hid_unsupported);
        break;

        case HID_HANDSHAKE:
        HID_Send_Handshake_Packet(Hid_unsupported);
        break;

        default:
        HID_Send_Handshake_Packet(Hid_unsupported);
        break;
    }
}

void HID_Send_Handshake_Packet(HID_HANDSHAKE_MESSAGE message)
{
    uint8 hid_buf[1];
    hid_buf[0] = (HID_HANDSHAKE << 4) | message;
    HID_Packet_Send_Control(hid_buf, 1);
}

void HID_Handle_Control_Message(HID_CONTROL_OP operation)
{
    switch (operation)
    {
        case Hid_control_op_nop:
        break;

        case Hid_control_op_hard_reset:
        break;

        case Hid_control_op_soft_reset:
        break;

        case Hid_control_op_suspend:
        HID_Send_Handshake_Packet(Hid_success);
        break;

        case Hid_control_op_exit_suspend:
        break;

        case Hid_control_op_unplug:
        break;

        default:
        HID_Send_Handshake_Packet(Hid_invalid_param);
        break;
    }
}
