#ifndef __SHARE_MEM_ID_H__
#define __SHARE_MEM_ID_H__

//最多“同时使用” 14 条记录

#define SHARE_MEM_ID_DAECFG          0 //DAE配置结构体，由 config 申请空间并共享
#define SHARE_MEM_ID_BTPLAYPIPE      1 //蓝牙推歌Input Buffer管理结构体，由 bt engine 共享
#define SHARE_MEM_ID_SCO_INPIPE      2 //蓝牙免提Input Buffer管理结构体，由 bt engine 共享
#define SHARE_MEM_ID_SCO_OUTPIPE     3 //蓝牙免提Output Buffer管理结构体，由 bt engine 共享
#define SHARE_MEM_ID_USBPCMPIPE      4 //USB音箱/声卡Input Buffer管理结构体，由 中间件 共享；即会先创建中间件并加载算法库，然后才过来数据

#define SHARE_MEM_ID_TWS_MMM_M2S     5 //TWS主箱发送命令给从箱，或者确认接收到从箱的事件
#define SHARE_MEM_ID_TWS_MMM_S2M     6 //TWS从箱上报事件给主箱，或者确认响应了主箱的命令

#define SHARE_MEM_ID_TWS_EG_M2S      7 //TWS主箱发送命令给从箱，或者确认接收到从箱的事件
#define SHARE_MEM_ID_TWS_EG_S2M      8 //TWS从箱上报事件给主箱，或者确认响应了主箱的命令

#define SHARE_MEM_ID_TWS_UI_M2S      9 //TWS主箱发送命令给从箱，或者确认接收到从箱的事件
#define SHARE_MEM_ID_TWS_UI_S2M      10//TWS从箱上报事件给主箱，或者确认响应了主箱的命令

#define SHARE_MEM_ID_TWS_MMM_M2S_PLAY 11//TWS正在播放时，主箱频繁发送命令给副箱，只能带2字节cmd_data

#define SHARE_MEM_ID_SCO_INPIPE_ASQT  12//ASQT场景使用更大BUFFER确保不会断音，仅通话调试时使用

#define SHARE_MEM_ID_BLESPP_INPIPE1   13//BLE/SPP接收数据缓冲区1
#define SHARE_MEM_ID_BLESPP_INPIPE2   14//BLE/SPP接收数据缓冲区2

#endif
