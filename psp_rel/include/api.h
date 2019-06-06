/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     api.h
 * \brief    各个驱动入口函数指针起始地址
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _API_H
#define _API_H


#define KERNEL_ENTRY_VA_START           (0x10000000)
#define LIBC_ENTRY_VA_START             (KERNEL_ENTRY_VA_START + 4 * 72)
#define BASE_ENTRY_VA_START             (LIBC_ENTRY_VA_START + 4 * 40)
#define STUB_ENTRY_VA_START             (BASE_ENTRY_VA_START + 4 * 15)
#define HAL_ENTRY_VA_START              (STUB_ENTRY_VA_START + 4 * 6)

#define STG_DRV_ENTRY_VA_START          (0x11000000)
#define VFS_ENTRY_VA_START              (0x12000000)
#define HAL_USB_ENTRY_VA_START          (0x12100000)
#define HAL_USOUND_ENTRY_VA_START       (0x3c000000)
#define COMMON_ENTRY_VA_START           (0x15000000)

/*key && fm*/
#define KEY_ENTRY_VA_START              (0x13000000)
#define LED_ENTRY_OFFSET                (0x40) /*FIX to backward compatibility*/
#define LED_ENTRY_VA_START              (KEY_ENTRY_VA_START + LED_ENTRY_OFFSET)
#define FM_ENTRY_OFFSET                 (0x80) /*FIX to backward compatibility*/
#define FM_ENTRY_VA_START               (KEY_ENTRY_VA_START + FM_ENTRY_OFFSET)
#define FM_TX_ENTRY_OFFSET              (0xA8) /*FIX to backward compatibility*/
#define FM_TX_ENTRY_VA_START            (KEY_ENTRY_VA_START + FM_TX_ENTRY_OFFSET)
#define LCD_ENTRY_OFFSET                (0xC0) /*FIX to backward compatibility*/
#define LCD_ENTRY_VA_START              (KEY_ENTRY_VA_START + LCD_ENTRY_OFFSET)
#define RTC_ENTRY_OFFSET                (0x110) /*FIX to backward compatibility*/
#define RTC_ENTRY_VA_START              (KEY_ENTRY_VA_START + RTC_ENTRY_OFFSET)

#define HDMI_ENTRY_OFFSET               (0x150)
#define HDMI_ENTRY_VA_START             (KEY_ENTRY_VA_START + HDMI_ENTRY_OFFSET)

/*bt controller && host*/
#define BT_CON_ROM_ZERO_ADDR            (0x9FC9AFFC)
#define BT_CON_ENTRY_VA_START           (0x21000000)
#define BT_CON_OP_ENTRY_VA_START        (0x21000000)
#define BT_CON_ENTRY_VA_SIZE            (0x200)
#define BT_HOST_ENTRY_VA_START          (BT_CON_ENTRY_VA_START + BT_CON_ENTRY_VA_SIZE)
#define BT_HOST_OP_ENTRY_VA_START       (BT_HOST_ENTRY_VA_START)
#define BT_HOST_ENTRY_VA_SIZE           (0x400)
#define BT_HOST_OP_ENTRY_FOR_CON        (0xbfc44F00)

#define MMM_ENTRY_VA_START              (0x30000000)
#define AL_DECODER_ENTRY_VA_START       (0x31000000)
#define AL_AEC_ENTRY_VA_START           (AL_DECODER_ENTRY_VA_START)
#define AL_PEQ_ENTRY_VA_START           (0x32000000)

#define AUDIO_CHANNEL_ENTRY_VA_START    (0x14000000) /* ADC CNANNEL、WAV编码库、录音模块合并一个entry */
#define AL_ENCODER_WAV_ENTRY_VA_START   (AUDIO_CHANNEL_ENTRY_VA_START + 0x8) /* ADC CNANNEL、WAV编码库、录音模块合并一个entry */
#define RECORD_MODULE_ENTRY_VA_START    (AUDIO_CHANNEL_ENTRY_VA_START + 0x10) /* ADC CNANNEL、WAV编码库、录音模块合并一个entry */

#define AL_DECODER_SBC_ENTRY_VA_START   (0x33000000)
#define AL_ENCODER_SBC_ENTRY_VA_START   (0x33000010)

#define AL_SP_PRE_ENTRY_VA_START        (0x34000000)
#define AL_SP_POST_ENTRY_VA_START       (0x34000010)

#define AL_RESERVD_PLC_VA_START         (0x35000000)
#define AL_RESERVD_HFP_VA_START         (AL_RESERVD_PLC_VA_START + 0x08)

#define AL_USER1_ENTRY_VA_START         (0x3d000000)
#define AL_USER2_ENTRY_VA_START         (0x3e000000)
#define AL_USER3_ENTRY_VA_START         (0x3f000000)

#define HAL_DRV_RAM_OP_ENTRY            (0xbfc22200)

//MEMORY SIZE DEFINE
#define A2DP_SBC_INPUT_BUFFER_LEN          (0x4A00)
//#define A2DP_SBC_INPUT_BUFFER_LEN        (0x3600)//OK_MIC场景缓冲能力调低
#define A2DP_SBC_DIRECT_INPUT_BUFFER_LEN   (0x400)
#define A2DP_M2S_AVDTP_PACKET_LEN          (0x400)
#define A2DP_M2S_AVDTP_CACHE_LEN           (0x3800)
//LINEIN　TWS
#define LINEIN_SBC_INPUT_BUFFER_LEN        (0x3600)

#define HFP_SCO_ERR_REPORT_BUF_LEN         (0x40)
#define HFP_SCO_OUTPUT_CACHE_LEN           (0x180)
#define HFP_SCO_RECEIVE_CACHE_LEN          (0x140)

//USOUND INPUT CACHE
#define USOUND_STREAM_FIFO_LEN             (0x400)
#define USOUND_STREAM_BUFFER_LEN           (0x1800)

//RECORD ENCODE OUTPUT CACHE
#define RECORD_ENCODE_OUTPUT_CACHE_LEN     (0x2000)

#define ASQT_DUMP_BUFFER_SIZE              (0x1400)
#define ASQT_SCO_IN_CACHE_SIZE             (0x800)
#define STUB_RW_DATA_SIZE                  (0x400)

#define BT_ENGINE_HEAP_SIZE                (0x6600)
#define BT_ENGINE_ASQT_HEAP_SIZE           (0x4600) //分出8KB给ASQT DUMP BUFFER
#define TEST_POOL_SIZE                     (0x4600) //分出8KB给ATT

#define CONTROLLER_UART_DMA_BUF_LEN        (0x400)
#define CONTROLLER_HCI_BUF_LEN             (0x2000)

#define RANDOM_LAP_BUF_SISE                (16*2*375)

#define PRINT_DMA_CACHE_LEN                (0x300) //max length 0x1000, cache must move to another address

#define BLESPP_RECV_CACHE1_LEN             (0x280)
#define BLESPP_RECV_CACHE2_LEN             (0x280)

#define VRAM_MERGE_TEMP_BUFFER_LEN         (0x8000)

#define MUSIC_FSEL_DISK_SCAN_DATA_STACK_ADDR  (0x9fc28000)
#define MUSIC_FSEL_DISK_SCAN_DATA_STACK_LEN   (0x2000)

//ESD断点实时备份缓冲区
#define ESD_BK_INFOR_ADDR                  (0x9fc23F80)
#define ESD_BK_INFOR_LEN                   (0x50)
//应用层全局变量区
#define GLOBAL_VAR_ADDR                    (ESD_BK_INFOR_ADDR + ESD_BK_INFOR_LEN)
#define GLOBAL_VAR_LEN                     (0x30) //ESD_BK_INFOR_LEN + GLOBAL_VAR_LEN = 0x80

#endif

