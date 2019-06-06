debug=0;

tab=fw_info;
DISP_EN=firmware information;
DISP_CH=固件信息;
DISPLAY=1;

key=FACTORY_FW_INFO_SOLUTION_PROVIDER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=solution provider:max 90 bytes;
DISP_CH=方案商:最长不能超过30个汉字，或90个英文字符;
MENU_INDEX=1;
DISPLAY=1;

key=FACTORY_FW_INFO_MANUFACTURER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=manufacturer:max 90 bytes;
DISP_CH=制造商:最长不能超过30个汉字，或90个英文字符;
MENU_INDEX=2;
DISPLAY=1;

key=FACTORY_FW_INFO_VERSION_NUMBER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=version number:max 90 bytes;
DISP_CH=版本号:最长不能超过90个英文字符;
MENU_INDEX=3;
DISPLAY=1;

key=FACTORY_FW_INFO_MODIFY_DATE;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=modify date:max 90 bytes;
DISP_CH=修改日期:最长不能超过90个英文字符;
MENU_INDEX=4;
DISPLAY=1;

key=FACTORY_FW_INFO_MODIFY_DESC;
type=STR;
operate=EDIT;
len=600;
RANGE=;
tabname=fw_info;
DISP_EN=modify description:max 600 bytes;
DISP_CH=修改说明:最长不能超过200个汉字，或600个英文字符，各修改点以;分割，工具会以此分行显示;
MENU_INDEX=5;
DISPLAY=1;









tab=setting;
DISP_EN=app setting;
DISP_CH=系统配置;
DISPLAY=1;

tab_layer1=ap_onff; 
tabname =setting;
DISP_EN=ap switch select;
DISP_CH=应用开关选择;
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_APP_SUPPORT_CARDPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support card playing:0-not support, 1-support;
DISP_CH=是否支持卡播放功能:0-不支持,1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support linein:0-not support, 1-support;
DISP_CH=是否支持linein播放功能:0-不支持,1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_FMRADIO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support fm:0-not support, 1-support;
DISP_CH=是否支持FM收音机:0表示不支持FM，1表示支持FM;
DISPLAY=1;

key=SETTING_APP_SUPPORT_BT_INBACK;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support bluetooth run in back:0-not supported,1-supported;
DISP_CH=是否支持后台蓝牙:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_BTPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support bluetooth a2dp play:0-not supported,1-supported;
DISP_CH=是否支持蓝牙播歌:0-不支持, 1-支持;
DISPLAY=0;

key=SETTING_APP_SUPPORT_BTCALL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support bluetooth hfp call:0-not supported,1-supported;
DISP_CH=是否支持蓝牙免提:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_UHOSTPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support uhost play:0-not supported,1-supported;
DISP_CH=是否支持U盘播放:0表示不支持，1表示支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_MDISKPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support main disk play:0-not supported,1-supported;
DISP_CH=是否支持内置磁盘播放:0表示不支持，1表示支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_USBBOX;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support usb box:0-not supported,1-supported;
DISP_CH=是否支持USB音箱:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_USBMSC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support usb msc:0-not supported,1-supported;
DISP_CH=是否支持大容量磁盘功能:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_RECORD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support recorder:0-not supported,1-supported;
DISP_CH=是否支持录音功能:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SUPPORT_TAKEPHOTO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support take photo:0-not supported,1-supported;
DISP_CH=是否支持拍照模式:0-不支持, 1-支持;
DISPLAY=1;

key=SETTING_APP_SWITCH_SEQUENCE;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=setting;
DISP_EN=app switch sequence:0 btplay,1 card play,2 linein;
DISP_CH=应用切换顺序:0蓝牙播歌,1播卡,2 linein,3 fm radio,4 usb audio,5插U; 默认值表示开机默认进入的应用，数值意义参见case_type.h中的宏定义 APP_FUNC_XXX;
MENU_INDEX=10;
DISPLAY=1;

key=SETTING_HARDWARE_SUPPORT_CARD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname =setting;
RANGE_DISP_EN=0:no support card,1:support card;
RANGE_DISP_CH=0:不支持卡,1:支持卡;
DISP_EN=hardware support card:0-not support, 1-support;
DISP_CH=硬件是否支持卡:0 不支持 1支持;
MENU_INDEX=11;
DISPLAY=1;

key=SETTING_SYS_CAPACITOR_SELECT;
type=DATA;
operate=SINGLE;
len=;
RANGE=6-10;
tabname =setting;
RANGE_DISP_EN=6:1.2pF,7:1.4pF,8:1.6pF,9:1.8pF,10:2.0pF;
RANGE_DISP_CH=6:1.2pF,7:1.4pF,8:1.6pF,9:1.8pF,10:2.0pF;
DISP_EN= capacitor select: 6:1.2pF,7:1.4pF,8:1.6pF,9:1.8pF,10:2.0pF;
DISP_CH= 内部电容选择: 6:1.2pF,7:1.4pF,8:1.6pF,9:1.8pF,10:2.0pF;
MENU_INDEX=12;
DISPLAY=1;

key=SETTING_APP_SUPPORT_CHARGE_LOOP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname =setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN= support charge loop: 0-not support, 1-support;
DISP_CH= 是否支持空闲充电模式:0表示不支持，1表示支持;
MENU_INDEX=13;
DISPLAY=1;

tab_layer1=power_option; 
tabname =setting;
DISP_EN=power manager option;
DISP_CH=电源管理选项;
MENU_INDEX=20;
DISPLAY=1;

key=SETTING_ONOFF_POWEROFF_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
step=1;
tab_layer1_name=power_option; 
tabname =setting;
RANGE_DISP_EN=0:disable power save,1:enable power save;
RANGE_DISP_CH=0:关闭省电关机,1:打开省电关机;
DISP_EN=power save enable:0 for disable, 1 for enable
DISP_CH=是否打开省电关机:0表示关闭省电关机，1表示打开省电关机
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_ONOFF_POWEROFF_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
step=1;
tab_layer1_name=power_option; 
tabname =setting;
RANGE_DISP_EN=0:power off,1:low power;
RANGE_DISP_CH=0:软关机,1:省电模式;
DISP_EN=power save mode:0 power off, 1 for low power
DISP_CH=省电关机模式:0表示进入软关机，1表示进入低功耗模式
MENU_INDEX=2;
DISPLAY=0;

key=SETTING_ONOFF_POWEROFF_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-120;
step=1;
tab_layer1_name=power_option; 
tabname =setting;
DISP_EN=power save:1 means 1 minute.
DISP_CH=省电关机时间:单位为1分钟；如果支持低功耗模式，那么从进入低功耗后开始计时
MENU_INDEX=3;
DISPLAY=1;

key=SETTING_ONOFF_POWEROFF_BT_WAITPAIR_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-120;
tab_layer1_name=power_option; 
tabname =setting;
DISP_EN=bluetooth power save:1 means 1 minute.
DISP_CH=蓝牙省电关机时间:如果支持低功耗模式，那么该配置项就无意义了
MENU_INDEX=4;
DISPLAY=1;

key=SETTING_ONOFF_POWEROFF_TWS_PR_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=power_option; 
tabname =setting;
RANGE_DISP_EN=0:master & slave,1:only self;
RANGE_DISP_CH=0:对箱关机,1:单箱关机;
DISP_EN=tws power off mode:0, tws poweroff; 1, single poweroff
DISP_CH=TWS按键关机模式:0表示对箱关机，1表示单箱关机，哪个音箱按键关机就关哪个音箱
MENU_INDEX=5;
DISPLAY=1;

tab_layer1=keyinput_option; 
tabname =setting;
DISP_EN=key input option;
DISP_CH=按键输入选项;
MENU_INDEX=21;
DISPLAY=1;

key=SETTING_INPUT_KEY_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=logic key array:logic key array, default value is onoff pin key value, key_msg_value.h;
DISP_CH=逻辑按键定义:逻辑按键定义，默认值表示ONOFF PIN的键值，详见 key_msg_value.h 中的定义;
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_INPUT_KEY_ADC_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=physical key adc value array:physical key adc value array;
DISP_CH=物理按键ADC值列表:物理按键ADC值列表，由硬件决定;
MENU_INDEX=2;
DISPLAY=1;

key=SETTING_IR_KEY_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
step=1;
tab_layer1_name=keyinput_option; 
tabname =setting;
RANGE_DISP_EN=0:not support,1:support;
RANGE_DISP_CH=0:不支持,1:支持;
DISP_EN=support ir key:0 not support, 1 support
DISP_CH=是否支持IR遥控:0表示不支持，1表示支持
MENU_INDEX=3;
DISPLAY=1;

key=SETTING_IR_KEY_PROTOCOL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
step=1;
tab_layer1_name=keyinput_option; 
tabname =setting;
RANGE_DISP_EN=0:Toshiba 9012,1:NEC,2:RC5,3:RC6;
RANGE_DISP_CH=0:Toshiba 9012,1:NEC,2:RC5,3:RC6;
DISP_EN=ir key protocol:0--Toshiba 9012, 1--NEC, 2--RC5, 3--RC6;
DISP_CH=IR遥控协议:0表示Toshiba 9012，1表示NEC，2表示RC5，3表示RC6;
MENU_INDEX=4;
DISPLAY=1;

key=SETTING_IR_KEY_USERCODE;
type=DATA;
operate=EDIT;
len=;
RANGE=0-16777215;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=ir key usercode: ir key usercode
DISP_CH=IR遥控用户码: IR遥控用户码, 比如0xff00
DISPLAY=1;

key=SETTING_IR_KEY_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=IR key array:IR key array, default value is 0,KEY_NULL, key_msg_value.h;
DISP_CH=红外遥控按键定义:红外遥控按键定义，默认值为0，无意义，详见 key_msg_value.h 中的定义;
MENU_INDEX=6;
DISPLAY=1;

key=SETTING_IR_KEY_KEY_CODE_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=IR key code array:IR key code array;
DISP_CH=红外遥控按键键值列表:红外遥控按键键值列表，由硬件决定;
MENU_INDEX=7;
DISPLAY=1;

key=SETTING_SYS_GPIO8_ADCKEY_RES_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=keyinput_option; 
tabname =setting;
RANGE_DISP_EN=0:use extern pullup resistance,1:use lradc1 inner pullup resistance;
RANGE_DISP_CH=0:使用外部上拉电阻,1:使用LRADC1内置上拉电阻;
DISP_EN=keyboard resistance selection: 0, use extern pullup resistance, 1,use lradc1 inner pullup resistance;
DISP_CH=按键上拉电阻选择: 0，使用外部上拉电阻,1，使用LRADC1内置100K欧姆上拉电阻;
MENU_INDEX=8;
DISPLAY=1;

tab_layer1=aout_option; 
tabname =setting;
DISP_EN=audio output option;
DISP_CH=音频输出选项;
MENU_INDEX=22;
DISPLAY=1;

key=SETTING_HARDWARE_AUDIO_OUTPUT_PA_SWING;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aout_option; 
tabname =setting;
RANGE_DISP_EN=0:2p8 pa swing,1:1p6 pa swing;
RANGE_DISP_CH=0:高摆幅,1:低摆幅;
DISP_EN=audio output pa swing select:0 - 2p8 pa swing, 1 - 1p6 pa swing;
DISP_CH=音频输出摆幅选择:0表示高摆幅输出，1表示低摆幅输出;
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tab_layer1_name=aout_option; 
tabname =setting;
RANGE_DISP_EN=0:dual channel output,1:mix left channel output,2:mix right channel output,3:mix dual channel output,4:switch dual channel output;
RANGE_DISP_CH=0:双声道输出,1:混合后仅左声道输出,2:混合后仅右声道输出,3:混合后双声道输出,4:左右声道互换双声道输出;
DISP_EN=audio output channel select:0 - dual channel output, 1 - mix left channel output, 2 - mix right channel output, 3 - mix dual channel output, 4 - switch dual channel output;
DISP_CH=音频输出通道选择:0表示双声道输出，1表示混合后仅左声道输出，2表示混合后仅右声道输出，3表示混合后双声道输出，4表示左右声道互换双声道输出;
MENU_INDEX=2;
DISPLAY=1;

key=SETTING_HARDWARE_EXTERN_PA_MUTE_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aout_option; 
tabname =setting;
RANGE_DISP_EN=0:disable pa mute,1:enable pa mute;
RANGE_DISP_CH=0:不使能功放静音,1:使能功放静音;
DISP_EN=extern pa mute when pause:0-disable, 1-enable;
DISP_CH=暂停播放时功放静音使能:0表示不使能，1表示使能;
MENU_INDEX=3;
DISPLAY=1;

key=SETTING_HARDWARE_EARPHONE_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aout_option; 
tabname =setting;
RANGE_DISP_EN=0:no use earphone,1:use earphone;
RANGE_DISP_CH=0:不使用耳机,1:使用耳机;
DISP_EN=use earphone: 0 for no use, 1 for use;
DISP_CH=是否使用耳机输出:使用耳机输出开启PA时需要给PA充电耗时较长，关闭PA时需要给PA放电耗时也较长，0表示不使用，1表示使用;
MENU_INDEX=4;
DISPLAY=1;

key=SETTING_HARDWARE_EARPHONE_DDV_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aout_option; 
tabname =setting;
RANGE_DISP_EN=0:indirect drive,1:direct drive;
RANGE_DISP_CH=0:非直驱,1:直驱;
DISP_EN=enable direct drive: 0 indirect dirve, 1 direct drive;
DISP_CH=是否使用耳机直驱，0表示非直驱，1表示直驱;
MENU_INDEX=5;
DISPLAY=1;

tab_layer1=tts_option; 
tabname =setting;
DISP_EN=tts option;
DISP_CH=状态语音播报选项;
MENU_INDEX=23;
DISPLAY=1;

key=SETTING_SOUND_ENABLE_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=tts_option; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=tts option:0-disable,1-enable;
DISP_CH=状态语音播报选项:0-关闭,1-开启;
DISPLAY=1;

key=SETTING_SOUND_ENABLE_CONTROL_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=tts_option; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=control tts option:0-disable,1-enable;
DISP_CH=状态语音播报控制类TTS选项:包括上一曲/下一曲/暂停/播放等，0表示关闭，1表示打开;
DISPLAY=1;

key=SETTING_SOUND_TTS_LANGUAGE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=tts_option; 
tabname =setting;
RANGE_DISP_EN=0:chinese,1:english;  
RANGE_DISP_CH=0:中文,1:英文;
DISP_EN=tts language:0 for chinese, 1 for english;
DISP_CH=TTS播报语音类型:0表示中文，1表示英文;
DISPLAY=0;

tab_layer1=kt_option; 
tabname =setting;
DISP_EN=keytone option;
DISP_CH=按键音选项;
MENU_INDEX=24;
DISPLAY=1;

key=SETTING_SOUND_ENABLE_KEYTONE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=kt_option; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=keytone:0-disable,1-enable;
DISP_CH=按键音开关:0-关闭,1-开启;
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_SOUND_SOFT_KEYTONE_MAX_VOLUME;
type=DATA;
operate=SINGLE;
len=;
RANGE=-120-0;
step=10;
tab_layer1_name=kt_option; 
tabname =setting;
DISP_EN=soft keytone max volume:-120 ~ 0, unit 0.1dB, step 1dB;
DISP_CH=软按键音最大音量:单位0.1dB;
MENU_INDEX=2;
DISPLAY=1;

tab_layer1=led_segment_option; 
tabname =setting;
DISP_EN=led segment option;
DISP_CH=数码管选项;
MENU_INDEX=25;
DISPLAY=1;

key=SETTING_DISPLAY_SUPPORT_LED_SEGMENT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=led_segment_option; 
tabname =setting;
RANGE_DISP_EN=0:Dont support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持;
DISP_EN=support LED segment:0-not support, 1-support;
DISP_CH=是否支持LED数码管显示:0 不支持 1支持;
DISPLAY=1;

key=SETTING_DISPLAY_LED_SEGMENT_LIGHT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=led_segment_option; 
tabname =setting;
RANGE_DISP_EN=0:Dont support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持;
DISP_EN=support LED backlight:0-not support, 1-support;
DISP_CH=是否支持空闲时关闭数码管:0 不支持 1支持;
DISPLAY=1;

key=SETTING_DISPLAY_LED_SEGMENT_LIGHT_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=10-120;
step=10;
tab_layer1_name=led_segment_option; 
tabname =setting;
DISP_EN=backlight time:1 means 1s;
DISP_CH=背光时间:单位1秒;
DISPLAY=1;

tab_layer1=led_option; 
tabname =setting;
DISP_EN=led light option;
DISP_CH=LED灯选项;
MENU_INDEX=26;
DISPLAY=1;

key=SETTING_DISPLAY_SUPPORT_LED;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=led_option; 
tabname =setting;
RANGE_DISP_EN=0:Dont support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持;
DISP_EN=support LED:0-not support, 1-support;
DISP_CH=是否支持LED灯显示:0 不支持 1支持;
DISPLAY=1;

tab_layer1=volume_control; 
tabname =setting;
DISP_EN=volume control parameters;
DISP_CH=声音相关配置;
MENU_INDEX=27;
DISPLAY=1;

key=SETTING_SOUND_DEFAULT_MAIN_VOLUME;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tab_layer1_name=volume_control; 
tabname =setting;
DISP_EN=default main volume:0~31;
DISP_CH=默认主音量:升级后首次开机默认主音量，或者也可以作为每次开机的重置主音量;
MENU_INDEX=1;
DISPLAY=1;

key=SETTING_SOUND_DEFAULT_CALL_VOLUME;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=default call volume:0~15;
DISP_CH=默认打电话音量:升级后首次开机默认打电话音量，或者也可以作为每次开机的重置打电话音量;
MENU_INDEX=2;
DISPLAY=1;

key=SETTING_SOUND_VOLUME_RESET_POWERON;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=volume_control;
tabname =setting;
RANGE_DISP_EN=0:not reset,1:reset;  
RANGE_DISP_CH=0:不重置,1:重置; 
DISP_EN=power on reset volume default:0-not reset,1-reset;
DISP_CH=开机音量重置到默认值选项:0表示不重置，1表示要重置;
MENU_INDEX=3;
DISPLAY=1;

key=SETTING_SOUND_MIN_VOLUME_POWERON;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
step=1;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=power on mix volume:0~31;
DISP_CH=开机最小音量:避免开机听不到任何声音;
MENU_INDEX=4;
DISPLAY=1;

key=SETTING_SOUND_MAX_VOLUME_POWERON;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-31;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=power on max volume:0~31;
DISP_CH=开机最大音量:避免开机声音太大;
MENU_INDEX=5;
DISPLAY=1;

key=SETTING_SOUND_MAIN_VOLUME_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=-90-30;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=main volume gain:unit is 0.1db
DISP_CH=主音量增益:单位为0.1db，为负表示需要将输出功率调低，为正表示需要将输出功率调高，但是失真度会加大
MENU_INDEX=6;
DISPLAY=1;

key=LINEIN_VOLUME_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=-90-30;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=line-in volume gain:unit is 0.1db
DISP_CH=line-in音量增益:单位为0.1db，为负表示需要将输出功率调低，为正表示需要将输出功率调高，但是失真度会加大
MENU_INDEX=7;
DISPLAY=1;

key=RADIO_VOLUME_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=-90-30;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=fm radio volume gain:unit is 0.1db
DISP_CH=fm收音机音量增益:单位为0.1db，为负表示需要将输出功率调低，为正表示需要将输出功率调高，但是失真度会加大
MENU_INDEX=7;
DISPLAY=1;

key=BTCALL_SPEAKER_VOLUME_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=-90-0;
tab_layer1_name=volume_control;
tabname =setting;
DISP_EN=call volume gain:unit is 0.1db
DISP_CH=打电话音量增益:单位为0.1db，打电话场景声音大小一般会比主音量小一些
MENU_INDEX=8;
DISPLAY=1;

key=SETTING_SOUND_VOLUME_PHY_LIMIT_GRADE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tab_layer1_name=volume_control;
tabname =setting;
RANGE_DISP_EN=0:disable limit,1:light limit,2:moderate limit,3:heavy limit,4:user defined;
RANGE_DISP_CH=0:不限制,1:轻度限制,2:适度限制,3:重度限制,4:用户自定义;
DISP_EN=volume limit grade:0-disable limit,1-light limit,2-moderate limit,3-heavy limit,4-user defined;
DISP_CH=电量较低时音量限制级别:0表示不限制; 音量限制表音量单位 0.1db, 分别对应电量0 ~ 电量9; 1表示轻度限制，适合电池容量较大且有DCDC的方案，音量限制表 [-50,-50,-30,-20,-10,0,0,0,0,0];2表示适度限制，适合电池容量适中且有DCDC的方案，音量限制表 [-80,-80,-60,-40,-25,-10,0,0,0,0];3表示重度限制，适合电池容量较小且无DCDC的方案，音量限制表 [-100,-100,-80,-60,-40,-20,0,0,0,0];4表示自定义，由 SETTING_SOUND_VOLUME_PHY_LIMIT_TBL 定义
MENU_INDEX=9;
DISPLAY=1;

tab_layer1=alarm_control; 
tabname =setting;
DISP_EN=alarm control parameters;
DISP_CH=闹钟相关配置;
MENU_INDEX=28;
DISPLAY=1;

key=SETTING_TIMEALARM_SNOOZE_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=alarm_control;
tabname =setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=support snooze:0-not support, 1-support;
DISP_CH=是否支持贪睡功能:0-不支持,1-支持;
DISPLAY=1;

key=SETTING_TIMEALARM_SNOOZE_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=alarm_control;
tabname =setting;
DISP_EN=snooze timer:1 ~ 30 minutes;
DISP_CH=贪睡时间:单位为分钟;
DISPLAY=1;

key=SETTING_TIMEALARM_SNOOZE_MAX_TIMES;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=snooze times:1 ~ 10 times;
DISP_CH=贪睡最多次数:1 ~ 10 次;
DISPLAY=1;

key=SETTING_TIMEALARM_ALARM_OVERTIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=alarm overtime:1 ~ 30 minutes;
DISP_CH=闹钟超时时间:单位为分钟;
DISPLAY=1;

key=SETTING_TIMEALARM_ALARM_OVERTIME_DEAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=alarm_control;
tabname =setting;
RANGE_DISP_EN=0:close,1:auto snooze;  
RANGE_DISP_CH=0:关闭闹钟,1:自动选择贪睡; 
DISP_EN=alarm overtime deal:0-close alarm, 1-auto snooze;
DISP_CH=闹钟超时未操作处理选项:0表示关闭闹钟，1表示自动选择贪睡;
DISPLAY=1;

key=SETTING_TIMEALARM_DEFAULT_VOLUME;
type=DATA;
operate=SINGLE;
len=;
RANGE=5-31;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=alarm default volume:5 ~ 31;
DISP_CH=闹钟默认音量:5 ~ 31;
DISPLAY=1;

key=SETTING_TIMEALARM_TIME_AMPM_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=alarm_control;
tabname =setting;
RANGE_DISP_EN=0:24 hour,1:ampm;  
RANGE_DISP_CH=0:24小时制,1:12小时制; 
DISP_EN=ampm enable:0-24 hour, 1-ampm;
DISP_CH=时间显示方式:0表示24小时制，1表示12小时制;
DISPLAY=1;

tab_layer1=card_config; 
tabname =setting;
DISP_EN=card configuration;
DISP_CH=卡相关配置;
MENU_INDEX=29;
DISPLAY=0;

key=SETTING_CARD_CMD_PAD_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tab_layer1_name=card_config; 
tabname =setting;
RANGE_DISP_EN=0:level 0,1:level 1,2:level 2,3:level 3,4:level 4,5:level 5,6:level 6,7:level 7;
RANGE_DISP_CH=0:0级,1:1级,2:2级,3:3级,4:4级,5:5级,6:6级,7:7级;
DISP_EN= card cmd pin pad level:0 ~ 7 level,0 is lowest;
DISP_CH= 卡cmd pin驱动能力配置:0 ~ 7级，0为最低级;
DISPLAY=0;

key=SETTING_CARD_CLK_PAD_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tab_layer1_name=card_config; 
tabname =setting;
RANGE_DISP_EN=0:level 0,1:level 1,2:level 2,3:level 3,4:level 4,5:level 5,6:level 6,7:level 7;
RANGE_DISP_CH=0:0级,1:1级,2:2级,3:3级,4:4级,5:5级,6:6级,7:7级;
DISP_EN= card clk  pinpad level;
DISP_CH= 卡clk pin驱动能力配置:0 ~ 7级，0为最低级;
DISPLAY=0;

key=SETTING_CARD_DAT_PAD_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tab_layer1_name=card_config; 
tabname =setting;
RANGE_DISP_EN=0:level 0,1:level 1,2:level 2,3:level 3,4:level 4,5:level 5,6:level 6,7:level 7;
RANGE_DISP_CH=0:0级,1:1级,2:2级,3:3级,4:4级,5:5级,6:6级,7:7级;
DISP_EN= card data pin pad level;
DISP_CH= 卡data pin驱动能力配置:0 ~ 7级，0为最低级;
DISPLAY=0;

tab_layer1=test_debug; 
tabname =setting;
DISP_EN=test & debug configuration;
DISP_CH=测试调试选项;
MENU_INDEX=30;
DISPLAY=1;

key=SETTING_SYS_DEBUG_MODE_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:watchdog open,1:watchdog close;
RANGE_DISP_CH=0:开启看门狗,1:关闭看门狗;
DISP_EN=watchdog switch:0-watchdog open,1-watchdog close;
DISP_CH=看门狗开关:0-开启看门狗，1-关闭看门狗;
DISPLAY=1;

key=SETTING_SYS_RESET_AFTER_PRODUCT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-6;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:not reset,1:352ms,2:703ms,3:1.4s,4:2.8s,5:5.6s,6:11.2s;
RANGE_DISP_CH=0:not reset,1:352ms,2:703ms,3:1.4s,4:2.8s,5:5.6s,6:11.2s;
DISP_EN=when MP over, hardware reset time:0:not reset,1:352ms,2:703ms,3:1.4s,4:2.8s,5:5.6s,6:11.2s;
DISP_CH=量产结束重启时间:0:not reset,1:352ms,2:703ms,3:1.4s,4:2.8s,5:5.6s,6:11.2s;
DISPLAY=1;

key=SETTING_SYS_EMI_REDUCTION_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:don't care EMI,1:EMI Reduction;
RANGE_DISP_CH=0:不关心EMI,1:降低EMI;
DISP_EN=EMI Reduction Enable:0-don't care EMI, 1-EMI Reduction Enable;
DISP_CH=降低EMI使能:0表示不关心EMI，1表示降低EMI使能;
DISPLAY=1;

key=SETTING_SYS_ADCKEY_ADFU_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:disable adckey adfu,1:enable adckey adfu,2:force enable udisk,3:enable both;
RANGE_DISP_CH=0:禁止ADFU,1:使能ADFU,2:强制使能读卡器,3:两者均使能;
DISP_EN=enter adfu when boot detected adc key:0:,disable adckey adfu, 1,enable adckey adfu, 2,force enable udisk, 3,enable both;
DISP_CH=开机时检测到ADC按键进入ADFU:0,禁止，1,使能ADFU, 2,强制使能读卡器, 3,两者均使能;
DISPLAY=1;

key=SETTING_SYS_ADCKEY_BOOT_FUNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:NONE,1:ADFU,2:CardProduct;
RANGE_DISP_CH=0:无作用,1:ADFU,2:卡量产;
DISP_EN=lradc1 detect 0 option: 0, none, 1, enter ADFU, 2, enter Card Product;
DISP_CH=lradc1 检测到0的选项: 0，无作用正常启动,1，进入ADFU升级，2，进入卡量产;
DISPLAY=1;

key=SETTING_UART_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no support uart print,1:support uart print;
RANGE_DISP_CH=0:不支持Uart打印,1:支持Uart打印;
DISP_EN=uart print support:0-no support uart print, 1-support uart print;
DISP_CH=是否支持Uart打印:0 不支持Uart打印 1支持Uart打印;
DISPLAY=0;

key=SETTING_BT_CONTROLLER_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no support bt controller print,1:support bt controller print;
RANGE_DISP_CH=0:不支持蓝牙控制器打印,1:支持蓝牙控制器打印;
DISP_EN=bt controller print support:0-not support, 1-support;
DISP_CH=是否支持蓝牙控制器打印:0 不支持 1支持;
DISPLAY=0;

key=SETTING_BT_ENGINE_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no print,1:print;
RANGE_DISP_CH=0:不打印,1:打印;
DISP_EN=bt engine print support:0-no print,1-print;
DISP_CH=蓝牙协议栈打印使能:0表示不打印，1表示仅打印;
DISPLAY=0;

tab_layer1=dae_config; 
tabname =setting;
DISP_EN=digital audio effect configuration;
DISP_CH=数字音效配置;
MENU_INDEX=40;
DISPLAY=1;

key=DAE_BYPASS_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=dae_config;    
tabname=setting;
RANGE_DISP_EN=0:bypass mode disable,1:bypass mode enable;
RANGE_DISP_CH=0:开启数字音效,1:关闭数字音效;
DISP_EN=bypass mode:0-off,1-on;
DISP_CH=是否禁用DAE数字音效:0表示不禁用，即开启DAE数字音效，1表示禁用，即关闭数字音效
MENU_INDEX=1;
DISPLAY=1;

key=DAE_PRECUT_RATIO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-600;
tab_layer1_name=dae_config;    
tabname=setting;
DISP_EN=precut:unit is 0.1db
DISP_CH=预衰减:单位0.1db
MENU_INDEX=2;
DISPLAY=1;

tab_layer2=parameter_eq; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=parameter eq configuration;
DISP_CH=均衡器配置;
MENU_INDEX=3;
DISPLAY=1;

key=DAE_PEQ_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=parameter eq option:0-disable,1-enable;
DISP_CH=PEQ开关:0-关闭,1-开启;
DISPLAY=1;

key=DAE_PEQ_BANK0_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band1 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND1 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK1_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band2 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND2 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK2_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band3 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND3 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK3_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band4 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND4 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK4_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band5 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band5 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK5_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band6 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band6 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK6_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band7 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND7 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK7_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band8 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band8 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK8_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band9 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND9 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK9_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band10 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND10 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK10_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band11 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND11 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK11_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band12 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND12 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK12_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band13 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND13 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=DAE_PEQ_BANK13_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=parameter_eq;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=peq band14 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND14 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

tab_layer2=limiter_config; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=limiter configuration;
DISP_CH=限幅器配置;
MENU_INDEX=4;
DISPLAY=1;

key=DAE_LIMITER_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=limiter_config;
tab_layer1_name=dae_config; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=limiter option:0-disable,1-enable;
DISP_CH=限幅器开关:0-关闭,1-开启;
DISPLAY=1;

key=DAE_LIMITER_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=limiter_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=limiter setting:[threshold, attack_time, release_time, reserve]
DISP_CH=限幅器配置:[限幅器阈值，单位为0.1DB，范围 -60DB ~ 0DB; 限幅器启动时间，单位为0.01ms，取值范围为 0.02 ~ 10 ms; 限幅器释放时间，单位为1ms，取值范围为 1 ~ 1000 ms]
DISPLAY=1;

tab_layer2=drc_config; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc configuration;
DISP_CH=动态范围控制配置;
MENU_INDEX=5;
DISPLAY=1;

key=DAE_DRC_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=drc option:0-disable,1-enable;
DISP_CH=动态范围控制开关:0-关闭,1-开启;
DISPLAY=1;

key=DAE_DRC_CROSSOVER;
type=DATA;
operate=SINGLE;
len=;
RANGE=100-400;
step=5;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc crossover:step 5Hz;
DISP_CH=分频点:步进5Hz;
DISPLAY=1;

key=DAE_DRC_BAND1_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc low band setting:[threshold, detect_attack_time, detect_release_time, attack_time, release_time, cutoff_freq, q_value, 1]
DISP_CH=低频段DRC配置:[阈值，单位为0.1DB，范围 -60DB ~ 0DB;检测启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;检测释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;压缩启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;压缩释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;截止频率，单位为1Hz，取值范围为 100Hz ~ 400Hz;Q值，单位为0.1，取值范围为 0.3 ~ 1.0; 1]
DISPLAY=1;

key=DAE_DRC_BAND2_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc high band setting:[threshold, detect_attack_time, detect_release_time, attack_time, release_time, cutoff_freq, q_value, 1]
DISP_CH=高频段DRC配置:[阈值，单位为0.1DB，范围 -60DB ~ 0DB;检测启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;检测释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;压缩启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;压缩释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;截止频率，单位为1Hz，取值范围为 100Hz ~ 400Hz;Q值，单位为0.1，取值范围为 0.3 ~ 1.0; 1]
DISPLAY=1;

key=DAE_DRC_PRE_SOFTVOL_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-200;
step=1;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc sensitivity:step 0.1dB;
DISP_CH=DRC灵敏度:step 0.1dB;
DISPLAY=1;

key=DAE_DRC_POST_SOFTVOL_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-200;
step=1;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc power gain:step 0.1dB;
DISP_CH=DRC功率增益:step 0.1dB;
DISPLAY=1;

key=DAE_AUX_DRC_CROSSOVER;
type=DATA;
operate=SINGLE;
len=;
RANGE=100-400;
step=5;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=aux drc crossover:step 5Hz;
DISP_CH=aux 分频点:步进5Hz;
DISPLAY=1;

key=DAE_AUX_DRC_BAND1_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=aux drc low band setting:[threshold, detect_attack_time, detect_release_time, attack_time, release_time, cutoff_freq, q_value, 1]
DISP_CH=aux 低频段DRC配置:[阈值，单位为0.1DB，范围 -60DB ~ 0DB;检测启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;检测释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;压缩启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;压缩释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;截止频率，单位为1Hz，取值范围为 100Hz ~ 400Hz;Q值，单位为0.1，取值范围为 0.3 ~ 1.0; 1]
DISPLAY=1;

key=DAE_AUX_DRC_BAND2_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=aux drc high band setting:[threshold, detect_attack_time, detect_release_time, attack_time, release_time, cutoff_freq, q_value, 1]
DISP_CH=aux 高频段DRC配置:[阈值，单位为0.1DB，范围 -60DB ~ 0DB;检测启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;检测释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;压缩启动时间，单位为0.01ms，取值范围为 0.02 ~ 100 ms;压缩释放时间，单位为1ms，取值范围为 1 ~ 5000 ms;截止频率，单位为1Hz，取值范围为 100Hz ~ 400Hz;Q值，单位为0.1，取值范围为 0.3 ~ 1.0; 1]
DISPLAY=1;

key=DAE_AUX_DRC_PRE_SOFTVOL_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-200;
step=1;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=aux drc sensitivity:step 0.1dB;
DISP_CH=aux DRC灵敏度:step 0.1dB;
DISPLAY=1;

key=DAE_AUX_DRC_POST_SOFTVOL_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-200;
step=1;
tab_layer2_name=drc_config;
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=aux drc power gain:step 0.1dB;
DISP_CH=aux DRC功率增益:step 0.1dB;
DISPLAY=1;

tab_layer1=okm_config; 
tabname =setting;
DISP_EN=karaoke configuration;
DISP_CH=卡拉OK配置;
MENU_INDEX=50;
DISPLAY=1;

key=OK_MIC_MICIN_SOURCE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:MIC0,1:MIC1;
RANGE_DISP_CH=0:MIC0,1:MIC1;
DISP_EN=mic in source:0-MIC0,1-MIC1;
DISP_CH=MIC输入源选择:0表示MIC0，1表示MIC1
MENU_INDEX=1;
DISPLAY=1;

key=OK_MIC_MICIN_OP_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db,8:0db,9:1.5db,10:3db,11:4.5db,12:6db,13:7.5db,14:8db,15:9.5db;
RANGE_DISP_CH=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db,8:0db,9:1.5db,10:3db,11:4.5db,12:6db,13:7.5db,14:8db,15:9.5db;
DISP_EN=mic-in OP gain:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db,8:0db,9:1.5db,10:3db,11:4.5db,12:6db,13:7.5db,14:8db,15:9.5db;
DISP_CH=mic-in OP增益:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db,8:0db,9:1.5db,10:3db,11:4.5db,12:6db,13:7.5db,14:8db,15:9.5db;
MENU_INDEX=2;
DISPLAY=1;

key=OK_MIC_MICIN_ADC_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=mic-in ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=mic-in ADC 增益:单位3dB，范围0 ~ 15，表示0dB ~ 45dB;
MENU_INDEX=3;
DISPLAY=1;

key=OK_MIC_MIX_MODE_L_CHAN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
RANGE_DISP_CH=0:MIC忽略,1:MIC混合到播放,2:MIC替换掉播放;
DISP_EN=mic channel mix play left channel option:0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
DISP_CH=MIC通道混合到播放通道左声道选项:0表示不影响播放通道左声道，1表示混合到播放通道左声道，2表示替换播放通道左声道;
MENU_INDEX=4;
DISPLAY=1;

key=OK_MIC_MIX_MODE_R_CHAN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
RANGE_DISP_CH=0:MIC忽略,1:MIC混合到播放,2:MIC替换掉播放;
DISP_EN=mic channel mix play right channel option:0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
DISP_CH=MIC通道混合到播放通道右声道选项:0表示不影响播放通道左声道，1表示混合到播放通道左声道，2表示替换播放通道左声道;
MENU_INDEX=5;
DISPLAY=1;

key=OK_MIC_SOFT_VOLUME_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=-240-0;
step=1;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=mic soft volume gain:-24.0dB ~ 0dB, unit is 0.1dB;
DISP_CH=MIC通道软音量增益:-24.0dB ~ 0dB, unit is 0.1dB;
MENU_INDEX=6;
DISPLAY=1;

key=OK_MIC_PEQ_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=mic channel peq enable:0-disable,1-enable;
DISP_CH=MIC通道是否使能PEQ:0-关闭,1-开启;
MENU_INDEX=7;
DISPLAY=1;

key=OK_MIC_PEQ_BAND_NUM;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-14;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=mic channel peq point number:count begin DAE_PEQ_BANK13_SETTING;
DISP_CH=MIC通道PEQ点数:从DAE_PEQ_BANK13_SETTING倒数上去;
MENU_INDEX=8;
DISPLAY=1;

key=OK_MIC_LIMITER_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=limiter option:0-disable,1-enable;
DISP_CH=限幅器开关:0-关闭,1-开启;
MENU_INDEX=9;
DISPLAY=1;

key=OK_MIC_LIMITER_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=limiter setting:[threshold, attack_time, release_time, reserve]
DISP_CH=限幅器配置:[限幅器阈值，单位为0.1DB，范围 -60DB ~ 0DB; 限幅器启动时间，单位为0.01ms，取值范围为 0.02 ~ 10 ms; 限幅器释放时间，单位为1ms，取值范围为 1 ~ 1000 ms]
MENU_INDEX=10;
DISPLAY=1;

key=OK_MIC_ECHO_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=MIC echo effect:0-disable,1-enable;
DISP_CH=MIC回声效果:是否使能MIC回声效果,0-关闭,1-开启;
MENU_INDEX=11;
DISPLAY=1;

key=OK_MIC_ECHO_DELAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-210;
step=1;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=echo delay:unit is 1ms;
DISP_CH=MIC回声延时:单位1ms;
MENU_INDEX=12;
DISPLAY=1;

key=OK_MIC_ECHO_RATIO_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=echo decay:echo decay array, max is 20 numbers;
DISP_CH=echo衰减系数:MIC回声深度调节列表，参数个数最多20个;
MENU_INDEX=13;
DISPLAY=1;

key=OK_MIC_ECHO_LP_BAND0;
type=DATA;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=echo lowpass 1 frequency:20 ~ 20000;
DISP_CH=回声第一个低通滤波截至频率:20 ~ 20000;
MENU_INDEX=14;
DISPLAY=1;

key=OK_MIC_ECHO_LP_BAND1;
type=DATA;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=echo lowpass 2 frequency:20 ~ 20000;
DISP_CH=回声第二个低通滤波截至频率:20 ~ 20000;
MENU_INDEX=15;
DISPLAY=1;

key=OK_MIC_FREQ_SHIFT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=inhibition howling:0-disable,1-enable;
DISP_CH=是否开启消除啸叫算法:0-关闭,1-开启;
MENU_INDEX=16;
DISPLAY=0;

key=OK_MIC_DUCKING_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=duching effect:0-disable,1-enable;
DISP_CH=是否开启闪避效果:0-关闭,1-开启;
MENU_INDEX=17;
DISPLAY=1;

key=OK_MIC_DUCKING_PARA;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=okm_config;    
tabname=setting;
DISP_EN=ducking effect setting:[detect_attack_time ms, attack_time ms, release_time ms, mic threshold 0.1dB, play decay 0.1dB]
DISP_CH=闪避效果配置:[检测时间ms，启动时间ms，释放时间ms，闪避阈值/峰值0.1dB，闪避幅度0.1dB]
MENU_INDEX=18;
DISPLAY=1;

key=PLAY_VOICE_REMOVAL_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=okm_config;    
tabname=setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=voice removal:0-disable,1-enable;
DISP_CH=是否开启人声消除算法:0-关闭,1-开启;
MENU_INDEX=19;
DISPLAY=1;










tab=usb;
DISP_EN=usb property;
DISP_CH=USB属性;
DISPLAY=1;

key=USB_VID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB VID;
DISP_CH=USB VID:设备厂商ID;
MENU_INDEX=1;
DISPLAY=1;

key=INF_USB_VENDOR;
type=STR;
operate=EDIT;
len=8;
RANGE=;
tabname=usb;
DISP_EN=usb vendor;
DISP_CH=USB设备厂商(<8字符);
MENU_INDEX=2;
DISPLAY=1;

tab_layer1=usb_sound; 
tabname =usb;
DISP_EN=usb sound configuration;
DISP_CH=USB音箱配置;
MENU_INDEX=10;
DISPLAY=1;

key=USB_SOUND_PID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tab_layer1_name=usb_sound; 
tabname =usb;
DISP_EN=USB SOUND PID;
DISP_CH=USB SOUND PID:同一VID下不同usb设备ID不能相同;
DISPLAY=1;

key=INF_SOUND_PRODUCT_NAME;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tab_layer1_name=usb_sound; 
tabname =usb;
DISP_EN=usb sound product name;
DISP_CH=USB SOUND名称:播放加录音功能USB设备名;
DISPLAY=1;

key=INF_VOLUME_FLAG;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-3;
tab_layer1_name=usb_sound; 
tabname =usb;
RANGE_DISP_EN=1:only player,2:only pc,3:player and pc but pc first;
RANGE_DISP_CH=1:小机可调,2:PC可调,3:小机PC均可调且PC优先;
DISP_EN=hid volume adjust option:1 for only player, 2 for only pc, 3 for player and pc but pc first;
DISP_CH=HID音量调节选项:1小机可调，2PC可调，3小机PC均可调且PC优先
DISPLAY=1;

key=USB_SOUND_AUTO_PLAY_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=usb_sound; 
tabname =usb;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=enable auto play:0 disable auto play, 1 enable auto play;
DISP_CH=是否开启自动播放功能:0表示关闭，1表示开启;
DISPLAY=1;

tab_layer1=usb_card_reader; 
tabname =usb;
DISP_EN=usb card reader configuration;
DISP_CH=USB读卡器配置;
MENU_INDEX=20;
DISPLAY=1;

key=USB_CARDREADER_PID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=USB CARDREADER PID;
DISP_CH=USB CARDREADER PID:同一VID下不同usb设备ID不能相同;
DISPLAY=1;

key=INF_CARDREADER_PRODUCT_NAME;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=USB cardreader product name;
DISP_CH=USB CARDREADER名称:高速读卡器设备名;
DISPLAY=1;

key=INF_INQUIRY_INFORMATION_FOB;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=infomation of Udisk;
DISP_CH=Udisk 信息;
DISPLAY=1;

key=INF_UDISK_LABEL_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=usb_card_reader;    
tabname =usb;
RANGE_DISP_EN=0:udisk label disable,1:udisk label enable;
RANGE_DISP_CH=0:不支持,1:支持; 
DISP_EN=udisk label enable:0-disable, 1-enable;
DISP_CH=是否使能进入U盘模式配置卷标:0-不支持,1-支持;
DISPLAY=1;

key=INF_UDISK_LABEL;
type=STR;
operate=EDIT;
len=11;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=show udisk label;
DISP_CH=磁盘卷标名;
DISPLAY=1;









tab=radio;
DISP_EN=radio property;
DISP_CH=收音机属性;
DISPLAY=1;

key=RADIO_AP_BAND_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname =radio;
RANGE_DISP_EN=0:chinese band,1:japanese band,2:europe band;
RANGE_DISP_CH=0:中国频段,1:日本频段,2:欧洲频段;
DISP_EN=radio band select:0-normal，1-japan，2-europe;
DISP_CH=电台波段选择:0-中国频段，1-日本频段，2-欧洲频段;
MENU_INDEX=1;
DISPLAY=1;

key=RADIO_SEEK_THRESHOLD;
type=ARRAY;
operate=EDIT;
len=;
RANGE=0-65535;
tabname=radio;
DISP_EN=search threshold: search threshold.
DISP_CH=搜台门限值: 各FM模组的门限值定义不一致，请根据datasheet设置
MENU_INDEX=2;
DISPLAY=1;

key=RADIO_SEEK_AUDITION_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:diable,1:enable;
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=auto search audition:0 disable, 1 enable;
DISP_CH=自动搜台试听开关:0表示关闭，1表示开启;
MENU_INDEX=3;
DISPLAY=1;

key=RADIO_SEEK_AUDITION_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tabname=radio;
DISP_EN=auto search audition time:unit is 1S;
DISP_CH=自动搜台试听时长:单位1S;
MENU_INDEX=4;
DISPLAY=1;

key=RADIO_GAIN_INPUT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=radio;
RANGE_DISP_EN=0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
RANGE_DISP_CH=0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISP_EN=FM OP gain:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISP_CH=FM OP 增益:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
MENU_INDEX=5;
DISPLAY=1;

key=RADIO_GAIN_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=radio;
DISP_EN=FM ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=FM ADC 增益:单位3dB，范围0 ~ 15，表示0dB ~ 45dB;
MENU_INDEX=6;
DISPLAY=1;

key=RADIO_CLK_SRC_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:inner clock,1:extern clock;
RANGE_DISP_CH=0:片内时钟源,1:外部时钟源;
DISP_EN=FM clock select:0 for inner clock source, 1 for extern clock source
DISP_CH=Fm 时钟源选择:0表示片内时钟源，1表示外部时钟源
MENU_INDEX=7;
DISPLAY=1;

key=RADIO_CLK_VAL_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tabname=radio;
RANGE_DISP_EN=0:32768Hz,1:6.5MHz(NO REALIZE),2:13MHz(NO REALIZE),3:24MHz,4:26MHz;
RANGE_DISP_CH=0:32768Hz,1:6.5M（暂不可用）,2:13M（暂不可用）,3:24M,4:26M;
DISP_EN=FM clock frequency select:0 for 32768Hz, 1 for 6.5MHz(NO REALIZE), 2 for 13MHz(NO REALIZE), 3 for 24MHz, 4 for 26MHz
DISP_CH=Fm 时钟频率选择:0表示32K，1表示6.5M（暂不可用），2表示13M（暂不可用），3表示24M，4表示26M
MENU_INDEX=8;
DISPLAY=1;

key=RADIO_ENABLE_FREQ_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=frequency tts option:0-disable,1-enable;
DISP_CH=是否打开频点TTS:0表示关闭，1表示打开;
MENU_INDEX=9;
DISPLAY=1;

key=RADIO_INPUT_PIN_USE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=radio;
RANGE_DISP_EN=0:AUXL0 and AUXR0 DUAL,1:AUXL1 and AUXR1 DUAL,2:AUXL0 and AUXL1 MIX,3:AUXR0 and AUXR1 MIX,4:AUXL0 and AUXR0 MIX,5:ONLY AUXL0,6:ONLY AUXR0,7:ONLY AUXL1,8:ONLY AUXR1;
RANGE_DISP_CH=0:AUXL0和AUXR0立体声,1:AUXL1和AUXR1立体声,2:AUXL0和AUXL1混合,3:AUXR0和AUXR1混合,4:AUXL0和AUXR0混合,5:仅AUXL0输入,6:仅AUXR0输入,7:仅AUXL1输入,8:仅AUXR1输入;
DISP_EN=fm radio input pin use:0 for AUXL0 and AUXR0 DUAL,1 for AUXL1 and AUXR1 DUAL,2 for AUXL0 and AUXL1 MIX,3 for AUXR0 and AUXR1 MIX,4 for AUXL0 and AUXR0 MIX,5 for ONLY AUXL0,6 for ONLY AUXR0,7 for ONLY AUXL1,8 for ONLY AUXR1;
DISP_CH=fm radio 输入引脚使用:0表示AUXL0和AUXR0立体声输入，1表示AUXL1和AUXR1立体声输入，2表示AUXL0和AUXL1混合输入，3表示AUXR0和AUXR1混合输入，4表示AUXL0和AUXR0混合输入，5表示IC外混合后AUXL0输入，6表示IC外混合后AUXR0输入，7表示IC外混合后AUXL1输入，8表示IC外混合后AUXR1输入;
DISPLAY=1;









tab=music;
DISP_EN=music property;
DISP_CH=本地播歌属性;
DISPLAY=1;

key=MUSIC_REPLAY_BY_PREV_KEY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=music;
RANGE_DISP_EN=0:switch prev song,1:replay current song;
RANGE_DISP_CH=0:切换上一曲,1:重头开始播放;
DISP_EN=replay by prev key:0,no support, 1,support
DISP_CH=按PREV键回到开头重播:当歌曲播放超过5秒钟后，按PREV键是否回到开头重播，0表示切换到上一曲，1表示从头开始播放
DISPLAY=1;

key=MUSIC_ENABLE_SEQ_NUM_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=seq num tts option:0-disable,1-enable;
DISP_CH=是否打开曲目号TTS:0表示关闭，1表示打开;
DISPLAY=1;








tab=linein;
DISP_EN=linein property;
DISP_CH=音频输入属性;
DISPLAY=1;

key=LINEIN_GAIN_INPUT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=linein;
RANGE_DISP_EN=0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
RANGE_DISP_CH=0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISP_EN=linein gain:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISP_CH=line-in增益:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISPLAY=1;

key=LINEIN_GAIN_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=linein;
DISP_EN=linein ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=linein ADC 增益:单位3dB，范围0 ~ 15，表示0dB ~ 45dB;
DISPLAY=1;

key=LINEIN_CHANNEL_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:digital channel,1:anolog channel;
RANGE_DISP_CH=0:数字通道,1:模拟通道;
DISP_EN=linein channel selection:0 for digital channel,1 for anolog channel;
DISP_CH=line-in 通道选择:0表示数字通道, 1表示模拟通道;
DISPLAY=1;

key=LINEIN_INPUT_PIN_USE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=linein;
RANGE_DISP_EN=0:AUXL0 and AUXR0 DUAL,1:AUXL1 and AUXR1 DUAL,2:AUXL0 and AUXL1 MIX,3:AUXR0 and AUXR1 MIX,4:AUXL0 and AUXR0 MIX,5:ONLY AUXL0,6:ONLY AUXR0,7:ONLY AUXL1,8:ONLY AUXR1;
RANGE_DISP_CH=0:AUXL0和AUXR0立体声,1:AUXL1和AUXR1立体声,2:AUXL0和AUXL1混合,3:AUXR0和AUXR1混合,4:AUXL0和AUXR0混合,5:仅AUXL0输入,6:仅AUXR0输入,7:仅AUXL1输入,8:仅AUXR1输入;
DISP_EN=linein input pin use:0 for AUXL0 and AUXR0 DUAL,1 for AUXL1 and AUXR1 DUAL,2 for AUXL0 and AUXL1 MIX,3 for AUXR0 and AUXR1 MIX,4 for AUXL0 and AUXR0 MIX,5 for ONLY AUXL0,6 for ONLY AUXR0,7 for ONLY AUXL1,8 for ONLY AUXR1;
DISP_CH=line-in 输入引脚使用:0表示AUXL0和AUXR0立体声输入，1表示AUXL1和AUXR1立体声输入，2表示AUXL0和AUXL1混合输入，3表示AUXR0和AUXR1混合输入，4表示AUXL0和AUXR0混合输入，5表示IC外混合后AUXL0输入，6表示IC外混合后AUXR0输入，7表示IC外混合后AUXL1输入，8表示IC外混合后AUXR1输入;
DISPLAY=1;

key=LINEIN_LOWENERGY_POWEROFF_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=low energy poweroff enable:0-disable,1-enable;
DISP_CH=低能量省电关机开关:0-关闭,1-开启;
DISPLAY=1;

key=LINEIN_ENABLE_TWS_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:禁止,1:使能;
DISP_EN=tws mode enable:0-disable,1-enable;
DISP_CH=TWS模式使能:0-禁止,1-使能;
DISPLAY=1;











tab=record;
DISP_EN=record property;
DISP_CH=录音输入属性;
DISPLAY=1;

key=RECORD_GAIN_MIC_INPUT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tabname=record;
RANGE_DISP_EN=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db;
RANGE_DISP_CH=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db;
DISP_EN=record mic-in OP gain:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db;
DISP_CH=录音mic-in OP增益:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db;
DISPLAY=1;

key=RECORD_GAIN_MIC_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=record;
DISP_EN=record mic-in ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=录音 mic-in ADC 增益:单位3dB，范围0 ~ 15，表示0dB ~ 45dB;
DISPLAY=1;

key=RECORD_SAMPLE_RATE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=record;
RANGE_DISP_EN=0:8KHz,1:11KHz,2:12KHz,3:16KHz,4:22KHz,5:24KHz,6:32KHz,7:44KHz,8:48KHz;
RANGE_DISP_CH=0:8KHz,1:11KHz,2:12KHz,3:16KHz,4:22KHz,5:24KHz,6:32KHz,7:44KHz,8:48KHz;
DISP_EN=MIC record sample rate:0:8KHz,1:11KHz,2:12KHz,3:16KHz,4:22KHz,5:24KHz,6:32KHz,7:44KHz,8:48KHz;
DISP_CH=MIC录音采样率选择:0:8KHz,1:11KHz,2:12KHz,3:16KHz,4:22KHz,5:24KHz,6:32KHz,7:44KHz,8:48KHz;
DISPLAY=1;

key=RECORD_SCAN_MAX_SPACE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=record;
RANGE_DISP_EN=0:all disk,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
RANGE_DISP_CH=0:全盘扫描,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISP_EN=record disk scan max space:0:all disk,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISP_CH=录音扫描磁盘空闲空间:0-全盘扫描,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISPLAY=1;













tab=btstack;
DISP_EN=btstack property;
DISP_CH=蓝牙协议栈属性;
DISPLAY=1;

key=BTSTACK_DEVICE_NAME;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth device name:max 29 bytes;
DISP_CH=蓝牙设备名称:最大29字符;
DISPLAY=1;

key=BTSTACK_DEVICE_NAME_HID;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth hid device name:max 29 bytes;
DISP_CH=蓝牙HID设备名称:最大29字符;
DISPLAY=1;

key=BTSTACK_FIX_PIN_CODE;
type=STR;
operate=EDIT;
len=15;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth fix pin code:max 15 bytes;
DISP_CH=蓝牙配对密码:最大15字符;
DISPLAY=0;

key=BTSTACK_CLASS_OF_DEVICE;
type=DATA;
operate=EDIT;
len=;
RANGE=0-16777215;
tabname=btstack;
DISP_EN=class of device: detail information on www.bluetooth.org/zh-cn/specification/assigned-numbers/baseband; 0x240540 FOR HID DEVICE
DISP_CH=蓝牙设备类型:高字节表示major service，中字节表示major device，低字节表示minor device，各比特意义参考 www.bluetooth.org/zh-cn/specification/assigned-numbers/baseband; 如果是HID设备则要设置为 0x240540
DISPLAY=1;

key=BTSTACK_BDADD_HIGH3BYTES;
type=DATA;
operate=EDIT;
len=;
RANGE=0-16777215;
tabname=btstack;
DISP_EN=bt addr[byte5, byte4, byte3];
DISP_CH=蓝牙地址高3字节;
DISPLAY=1;

key=BTSTACK_CAPACITANCE_VALUE_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=btstack;
DISP_EN=;
DISP_CH=当前温度相对常温需补偿电容值表:默认值为1表示温度补偿打开,为0表示温度补偿关闭，[]内数值单位为0.1pF，分别对应-20℃，0℃，20℃，40℃，60℃，75℃
DISPLAY=0;

key=BTSTACK_SNIFF_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btstack;
RANGE_DISP_EN=0:sniff disable,1:sniff enable;
RANGE_DISP_CH=0:禁止进入sniff,1:使能进入sniff;
DISP_EN=sniff enable:0,sniff disable, 1,sniff enable
DISP_CH=是否使能进入sniff:0表示禁止进入sniff,1表示使能进入sniff
DISPLAY=0;

key=BTSTACK_SNIFF_INTERVAL_VALUE;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-2;
tabname=btstack;
DISP_EN=sniff interval:1:500ms;2:1.28s
DISP_CH=sniff 时间间隔:1:500ms;2:1.28s
DISPLAY=0;

key=BTSTACK_BLE_LOCAL_NAME;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth low Energy device name:max 29 bytes;
DISP_CH=低功耗蓝牙设备名称:最大29字符;
DISPLAY=1;

key=BTSTACK_BLE_MANUFACTURE_ID;
type=DATA;
operate=EDIT;
len=;
RANGE=0~65535;
tabname=btstack;
DISP_EN=bluetooth low Energy Manufactur id:max 29 bytes;
DISP_CH=低功耗蓝牙厂商ID:0~0xFFFF;
DISPLAY=1;











tab=btmanager;
DISP_EN=btmanager property;
DISP_CH=蓝牙管理器属性;
DISPLAY=1;

key=BTMANAGER_ENABLE_A2DP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:disable a2dp profile,1:enable a2dp_profile;
RANGE_DISP_CH=0:不支持A2DP高级音频分发服务,1:支持A2DP高级音频分发服务;
DISP_EN=CP:0-disbale,1-enable;
DISP_CH=CP:0-不支持,1-支持;
DISPLAY=0;

key=BTMANAGER_ENABLE_A2DP_CP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=btmanager;
RANGE_DISP_EN=0:not support a2dp,1:DTCP,2:SCMS-T;
RANGE_DISP_CH=0:不支持A2DP,1:支持DTCP,2:支持SCMS-T;
DISP_EN=CP:0-not support,1-DTCP,2-SCMS-T;
DISP_CH=CP:0-不支持,1-支持DTCP,2-支持SCMS-T;
DISPLAY=0;

key=BTMANAGER_ENABLE_AVDTP_AAC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:not support AAC,1:support AAC;
RANGE_DISP_CH=0:不支持AAC格式,1:支持AAC格式;
DISP_EN=support aac:0-disable,1-enable;
DISP_CH=支持AAC格式:0-关闭,1-开启;
DISPLAY=0;

key=BTMANAGER_ENABLE_AVRCP_VOLUME_SYNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support volume SYNC,1:support SYNC;
RANGE_DISP_CH=0:不支持音量同步,1:支持音量同步;
DISP_EN=enable avrcp1.4 absolute volume control:0 - disable,1 - enable;
DISP_CH=是否使能AVRCP1.4音量同步功能:0-关闭,1-开启;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support hfp profile,1:support hfp profile;
RANGE_DISP_CH=0:不支持HFP,1:支持HFP;
DISP_EN=support hfp profile:0 - disable,1 - enable;
DISP_CH=是否使能HFP:0-关闭,1-开启;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP_VOLUME_SYNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support HFP volume SYNC,1:support HFP volume SYNC;
RANGE_DISP_CH=0:不支持通话音量同步,1:支持通话音量同步;
DISP_EN=enable hfp absolute volume control:0 - disable,1 - enable;
DISP_CH=是否使能HFP音量同步功能:0-关闭,1-开启;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP_BAT_REPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support battery report,1:support battery report;
RANGE_DISP_CH=0:不支持小机电量上报,1:支持小机电量上报;
DISP_EN=battery report enable: 0 - disable,1 - enable;
DISP_CH=是否使能电池电量更新:0-关闭,1-开启;
DISPLAY=1;

key=BTMANAGER_SUPPORT_THREE_WAY_CALL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support 3-way call,1:support 3-way call;
RANGE_DISP_CH=0:不支持三方通话,1:支持三方通话;
DISP_EN=support 3-way call: 0 - not support,1 - support;
DISP_CH=是否支持三方通话:是否支持HFP三方通话,0表示不支持，1表示支持
DISPLAY=1;

key=BTMANAGER_ENABLE_HID;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:no support HID,1:support HID;
RANGE_DISP_CH=0:不支持HID,1:支持HID;
DISP_EN=support HID:0-disable,1-enable;
DISP_CH=支持HID功能:是否支持HID功能，0-不支持,1-支持;
DISPLAY=1;

key=BTMANAGER_ENABLE_AUTO_CONNECT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:not support auto connect,1:support auto connect;
RANGE_DISP_CH=0:不支持自动回连,1:支持自动回连;
DISP_EN=enable auto connection:0-disable,1-enable;
DISP_CH=自动回连开启:0-关闭,1-开启;
DISPLAY=1;

key=BTMANAGER_RECONNECT_ANOTHER_TIMES;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-20;
tabname=btmanager;
DISP_EN=bt engine auto connect another profile count:0 for all;
DISP_CH=后台回连另外一个服务的次数限制:0表示无限制;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_TIMEOUT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-99;
tabname=btmanager;
DISP_EN=auto connect count:0-always;
DISP_CH=超时断开后自动回连次数:0表示一直回连，1~99表示有限次;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_STARTUP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=btmanager;
DISP_EN=bt engine auto connect count for startup reconnect:0 for all;
DISP_CH=开机回连次数:0表示无限制，1~15表示有限次;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=btmanager;
DISP_EN=bt engine auto connect count for mode switch reconnect:0 for all;
DISP_CH=切MODE键进入蓝牙回连次数:0表示无限制，1~15表示有限次;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_INTERVAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-60;
tabname=btmanager;
DISP_EN=bt engine auto connect timer:uint 1s;
DISP_CH=自动回连时间间隔:单位1s;
DISPLAY=0;

key=BTMANAGER_MAX_PAIRED_LIST_MEMBER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-8;
tabname=btmanager;
DISP_EN=local device max paired list size:1-8;
DISP_CH=本地配对列表的最大成员个数:目前支持最多保存8个设备信息;
DISPLAY=1;

key=BTMANAGER_ENABLE_TWS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support tws,1:support tws;
RANGE_DISP_CH=0:不支持,1:支持;
DISP_EN=support tws: 0 - not support tws,1 - support tws;
DISP_CH=是否支持TWS对箱:0-不支持,1-支持;
DISPLAY=1;

key=BTMANAGER_VISUAL_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tabname=btmanager;
RANGE_DISP_EN=0:always visible,1:invisible after paried,2always invisible;
RANGE_DISP_CH=0:永远可见,1:配对后不可见,2:永远不可见
DISP_EN=support visula mode config:0:always visible,1:invisible after paried,2always invisible;
DISP_CH=是否支持配置可见性:0:永远可见,1:配对后不可见,2:永远不可见
DISPLAY=1;

key=BTMANAGER_ENABLE_SPP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:不支持SPP,1:支持SPP;
DISP_EN=support SPP:0-disable,1-enable;
DISP_CH=支持SPP功能:0-不支持,1-支持;
DISPLAY=1;

key=BTMANAGER_ENABLE_BLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:不支持BLE,1:支持BLE;
DISP_EN=support BLE:0-disable,1-enable;
DISP_CH=支持BLE功能:0-不支持,1-支持;
DISPLAY=1;

key=BTMANAGER_BLE_PASSTH_PROFILE_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:不支持ACT 透传 Profile,1:支持ACT 透传 Profile;
DISP_EN=support ACT Passthrough Profile:0-disable,1-enable;
DISP_CH=支持ACT 透传 Profile:0-不支持,1-支持;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_SRV_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile server uuid:36 bytes;
DISP_CH=Actions profile 服务uuid:36个字符;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_SET_CHAR_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile set char uuid:36 bytes;
DISP_CH=Actions profile 设定特性uuid:36个字符;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_GET_CHAR_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile server uuid:36 bytes;
DISP_CH=Actions profile 获取特性uuid:36个字符;
DISPLAY=1;

tab=btplay;
DISP_EN=btplay property;
DISP_CH=蓝牙播歌属性;
DISPLAY=0;

key=BTPLAY_SBC_PLAY_DELAY_START;
type=DATA;
operate=SINGLE;
len=;
RANGE=50-400;
step=1;
tabname=btplay;
DISP_EN=sbc play start delay:50ms to 450ms, space 1ms
DISP_CH=sbc播放起始延时:单箱播歌时sbc播放起始延时，单位为ms，步进为1ms
DISPLAY=0;

key=BTPLAY_SBC_PLAY_DELAY_START_TWS;
type=DATA;
operate=SINGLE;
len=;
RANGE=50-400;
step=1;
tabname=btplay;
DISP_EN=tws sbc play start delay:50ms to 450ms, space 1ms
DISP_CH=tws sbc播放起始延时:TWS播歌时sbc播放起始延时，单位为ms，步进为1ms
DISPLAY=0;









tab=btcall;
DISP_EN=btcall property;
DISP_CH=蓝牙打电话属性;
DISPLAY=1;

key=BTCALL_FRONT_CHF_CAG_EXCH_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btcall;
RANGE_DISP_EN=0:not support CAG/CHF switch,1:support CAG/CHF switch;
RANGE_DISP_CH=0:不支持CAG/CHF切换,1:支持CAG/CHF切换;
DISP_EN=support chf & cag exchage:0-disable,1-enable;
DISP_CH=支持切换音箱和手机通话:0-不支持,1-支持,按NEXT/PREV键进行切换;
MENU_INDEX=1;
DISPLAY=1;

key=BTCALL_IGNORE_UNCALLSCO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btcall;
RANGE_DISP_EN=0:dont ignore uncall sco data,1:ignore uncall sco data;
RANGE_DISP_CH=0:不过滤非通话sco数据,1:过滤非通话sco数据;
DISP_EN=ignore the uncall sco data: 0, not ignore, 1,ignore;
DISP_CH=过滤非通话状态下的SCO数据:0：不过滤，1：过滤;
MENU_INDEX=2;
DISPLAY=0;

tab_layer1=btcall_ring_parameter; 
tabname=btcall;
DISP_EN=btcall ring parameter;
DISP_CH=来电铃音参数配置;
MENU_INDEX=10;
DISPLAY=1;

key=BTCALL_CALLIN_HINT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=btcall_ring_parameter;
tabname=btcall;
RANGE_DISP_EN=0:only ring,1:only phonebook,2:ring + phonebook;
RANGE_DISP_CH=0:仅铃声,1:仅电话本播报,2:铃声+电话本播报;
DISP_EN=callin hint mode:0-only ring,1-only phonebook,2-ring + phonebook;
DISP_CH=来电提示模式:0-仅铃声,1-仅电话本播报,2-铃声+电话本播报;
DISPLAY=1;

key=BTCALL_CALLIN_PB_SPACE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=btcall_ring_parameter;
tabname=btcall;
DISP_EN=phonebook ring space time:unit is 1S;
DISP_CH=电话本播报间隔时长:以1s为单位，播完电话本后等待该时长后再循环播放;
DISPLAY=1;

key=BTCALL_CALLIN_RING_SPACE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-60;
tab_layer1_name=btcall_ring_parameter;
tabname=btcall;
DISP_EN=call in buildin ring space time:unit is 100ms;
DISP_CH=内置来电铃声间隔时长:以100ms为单位，播完内置来电铃声后等待该时长后再循环播放;
DISPLAY=1;

tab_layer1=btcall_volume_control; 
tabname=btcall;
DISP_EN=btcall volume control;
DISP_CH=通话音量控制参数配置;
MENU_INDEX=20;
DISPLAY=1;

key=BTCALL_MICIN_OP_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-7;
tab_layer1_name=btcall_volume_control; 
tabname=btcall;
RANGE_DISP_EN=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db
RANGE_DISP_CH=0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db
DISP_EN=mic-in gain:0-26db,1-30db,2-31.5db,3-33db,4-34.5db,5-36db,6-37.5db,7-39db
DISP_CH=打电话mic模拟增益:0-26db,1-30db,2-31.5db,3-33db,4-34.5db,5-36db,6-37.5db,7-39db
DISPLAY=1;

key=BTCALL_MICIN_ADC_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=btcall_volume_control; 
tabname=btcall;
DISP_EN=call adc gain:unit 3dB, 0 ~ 15;
DISP_CH=打电话ADC增益:单位3dB，范围0 ~ 15，表示0dB ~ 45dB;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-3;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
RANGE_DISP_EN=1:using right channel,2:using left channel,3:using dual channel;
RANGE_DISP_CH=1:右声道输出,2:左声道输出,3:双声道输出;
DISP_EN=speaker channel enable:1-right channel,2-left channel,3-both channel;
DISP_CH=打电话时喇叭声道使能:1：右声道，2：左声道，3：双声道；（双声道可能会导致回声消除效果变差）;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_WEAK_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
RANGE_DISP_EN=0:no reduction,1:right reduction,2:left reduction;
RANGE_DISP_CH=0:无衰减,1:右声道衰减,2:左声道衰减;
DISP_EN=speaker channel weak selection:0-no weak, 1-right channel, 2-left channel;
DISP_CH=打电话时喇叭声道衰减选择:0-不衰减，1-右声道衰减，2-左声道衰减;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_WEAK_VAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-18;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
DISP_EN=speaker channel weak value:0db ~ -18db;
DISP_CH=打电话时喇叭声道衰减幅度:0db ~ -18db;
DISPLAY=1;

tab_layer1=aec_parameters; 
tabname=btcall;
DISP_EN=aec parameters configuration;
DISP_CH=AEC参数配置;
MENU_INDEX=30;
DISPLAY=1;

key=BTCALL_AEC_MODULE_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable AEC,1:enable AEC;
RANGE_DISP_CH=0:关闭AEC模块,1:使能AEC模块;
DISP_EN=aec module enable:0-disable,1-enable;
DISP_CH=AEC模块开关:1：打开，0：关闭;
MENU_INDEX=1;
DISPLAY=1;

key=BTCALL_AEC_HALF_FULL_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:full mode,1:half mode;
RANGE_DISP_CH=0:全双工,1:半双工;
DISP_EN=aec half full mode:0 for full mode, 1 for half mode;
DISP_CH=回声全半双工模式:0表示全双工，1表示半双工;
MENU_INDEX=2;
DISPLAY=1;

key=BTCALL_AEC_TAIL_LENGTH;
type=DATA;
operate=SINGLE;
len=;
RANGE=32,64,96;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec tail length:32ms ~ 96ms;
DISP_CH=回声消除尾长:单位 ms, 32ms ~ 96ms;
MENU_INDEX=3;
DISPLAY=1;

tab_layer2=aec_nonlinear_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec nonlinear processing parameters;
DISP_CH=AEC非线性处理参数配置;
MENU_INDEX=10;
DISPLAY=1;

key=BTCALL_AEC_NLP_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=aec_nonlinear_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable AEC,1:enable AEC;
RANGE_DISP_CH=0:关闭非线性模块,1:使能非线性模块;
DISP_EN=aec nlp enable:0-disable,1-enable;
DISP_CH=非线性处理子模块开关:1：打开，0：关闭;
DISPLAY=1;

key=BTCALL_AEC_NLP_AGGRESSIVENESS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer2_name=aec_nonlinear_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec nlp aggressiveness:0~15;
DISP_CH=非线性处理程度:整数0~15,越大代表非线性处理越厉害;
DISPLAY=1;

key=BTCALL_AEC_DT_THRESHOLD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer2_name=aec_nonlinear_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec dt threshold:0~15;
DISP_CH=双讲近端语音保留程度:整数0~15，越大表示双讲是近端语音保留越多，但是可能会带来单讲的回声，需找个折中值;
DISPLAY=1;

tab_layer2=aec_noise_reduction_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec noise reduction parameters;
DISP_CH=AEC降噪处理参数配置;
MENU_INDEX=20;
DISPLAY=1;

key=BTCALL_AEC_ANS_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=aec_noise_reduction_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable aec anti noise,1:enable aec anti noise;
RANGE_DISP_CH=0:关闭降噪子模块,1:打开降噪子模块;
DISP_EN=aec ans enable:0-disable,1-enable;
DISP_CH=降噪开关:1：打开，0：关闭;
DISPLAY=1;

key=BTCALL_AEC_NOISE_SUPPRESS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-25;
tab_layer2_name=aec_noise_reduction_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec noise suppress:0~25db;
DISP_CH=噪声抑制比:单位dB，0~25;
DISPLAY=1;

tab_layer2=aec_echo_residual_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec echo residual parameters;
DISP_CH=AEC回声残留参数配置;
MENU_INDEX=30;
DISPLAY=1;

key=BTCALL_AEC_AES_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer2_name=aec_echo_residual_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable aec aes modular,1:enable aec aes modular;
RANGE_DISP_CH=0:关闭残留回声抑制子模块,1:打开残留回声抑制子模块;
DISP_EN=aec aes enable:0-disable,1-enable;
DISP_CH=残留回声抑制子模块开关:1：打开，0：关闭;
DISPLAY=1;

key=BTCALL_AEC_ECHO_SUPPRESS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-40;
tab_layer2_name=aec_echo_residual_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec echo suppress:0~40db;
DISP_CH=回声抑制子模块单讲回声抑制比:单位dB， 0~40;
DISPLAY=1;

key=BTCALL_AEC_ECHO_SUPPRESS_ACTIVE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-40;
tab_layer2_name=aec_echo_residual_parameters;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec echo suppress active:0~40db;
DISP_CH=回声抑制子模块双讲回声抑制比:单位dB， 0~40;
DISPLAY=1;


key=BTCALL_AEC_CNG_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable comfort noise,1:enable comfort noise;
RANGE_DISP_CH=0:关闭舒适噪声子模块,1:打开舒适噪声子模块;
DISP_EN=aec cng enable:0-disable,1-enable;
DISP_CH=舒适噪声开关:1：打开，0：关闭;
MENU_INDEX=40;
DISPLAY=1;

key=BTCALL_AEC_NOISE_FLOOR;
type=DATA;
operate=SINGLE;
len=;
RANGE=-60--40;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=aec noise floor value:-60db ~ -40db;
DISP_CH=舒适噪声水平:单位 dB Fs, 该值对应为负值，一般应小于-40 dB;
MENU_INDEX=41;
DISPLAY=1;

key=BTCALL_AGC_PRETREAT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:agc pretreat disable,1:agc pretreat enable;
RANGE_DISP_CH=0:关闭前置AGC子模块,1:打开前置AGC子模块;
DISP_EN=agc pretreat enable:0-disable,1-enable;
DISP_CH=预处理AGC使能:预处理自动增益幅度控制使能，用于放大对方的声音，1：打开，0：关闭;
MENU_INDEX=50;
DISPLAY=1;

key=BTCALL_AGC_PRETREAT_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=agc pretreat level:0level ~ 15level;
DISP_CH=预处理AGC目标值:预处理自动增益幅度控制目标值;
MENU_INDEX=51;
DISPLAY=1;

key=BTCALL_AGC_POSTREAT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:agc postreat disable,1:agc postreat enable;
RANGE_DISP_CH=0:关闭后置AGC子模块,1:打开后置AGC子模块;
DISP_EN=aec postreat enable:0-disable,1-enable;
DISP_CH=后端AGC使能:后端自动增益幅度控制使能，用于放大自己的声音，1：打开，0：关闭;
MENU_INDEX=52;
DISPLAY=1;

key=BTCALL_AGC_POSTREAT_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=aec_parameters;
tabname=btcall;
DISP_EN=agc postreat level:0Level ~ 15Level;
DISP_CH=后端AGC目标值:后端自动增益幅度控制目标值;
MENU_INDEX=53;
DISPLAY=1;

tab_layer1=btcall_dae_parameters; 
tabname=btcall;
DISP_EN=btcall dae parameters configuration;
DISP_CH=通话PEQ参数配置;
MENU_INDEX=40;
DISPLAY=1;

key=BTCALL_BYPASS_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=btcall_dae_parameters;    
tabname=setting;
RANGE_DISP_EN=0:bypass mode disable,1:bypass mode enable;
RANGE_DISP_CH=0:开启数字音效,1:关闭数字音效;
DISP_EN=bypass mode:0-off,1-on;
DISP_CH=是否禁用DAE数字音效:0表示不禁用，即开启DAE数字音效，1表示禁用，即关闭数字音效
MENU_INDEX=1;
DISPLAY=1;

key=BTCALL_PEQ_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:关闭,1:开启;
DISP_EN=parameter eq option:0-disable,1-enable;
DISP_CH=PEQ开关:0-关闭,1-开启;
MENU_INDEX=2;
DISPLAY=1;

key=BTCALL_PEQ_BAND_NUM_POSTREAT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-14;
tab_layer1_name=btcall_dae_parameters;
tabname=btcall;
DISP_EN=postdeal peq point number:0 for don't postdeal;
DISP_CH=后端PEQ点数:如果为0表示后端不进行PEQ处理，如果为14表示前端不做PEQ处理 ;
MENU_INDEX=3;
DISPLAY=1;

key=BTCALL_PEQ_BANK0_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band1 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND1 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK1_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band2 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND2 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK2_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band3 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND3 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK3_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band4 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND4 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK4_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band5 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band5 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK5_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band6 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band6 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK6_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band7 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND7 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK7_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band8 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band8 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK8_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band9 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND9 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK9_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band10 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND10 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK10_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band11 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND11 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK11_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band12 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND12 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK12_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band13 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND13 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;

key=BTCALL_PEQ_BANK13_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band14 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND14 配置:[中心频率或截止频率；Q值，单位为0.1；增益，单位为0.1DB，范围-120 ~ 120；类型 1表示peaking，2表示high pass，3表示low pass，4表示low shelf，5表示high shelf，其他无效]
DISPLAY=1;
