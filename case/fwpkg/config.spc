debug=0;

tab=fw_info;
DISP_EN=firmware information;
DISP_CH=�̼���Ϣ;
DISPLAY=1;

key=FACTORY_FW_INFO_SOLUTION_PROVIDER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=solution provider:max 90 bytes;
DISP_CH=������:����ܳ���30�����֣���90��Ӣ���ַ�;
MENU_INDEX=1;
DISPLAY=1;

key=FACTORY_FW_INFO_MANUFACTURER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=manufacturer:max 90 bytes;
DISP_CH=������:����ܳ���30�����֣���90��Ӣ���ַ�;
MENU_INDEX=2;
DISPLAY=1;

key=FACTORY_FW_INFO_VERSION_NUMBER;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=version number:max 90 bytes;
DISP_CH=�汾��:����ܳ���90��Ӣ���ַ�;
MENU_INDEX=3;
DISPLAY=1;

key=FACTORY_FW_INFO_MODIFY_DATE;
type=STR;
operate=EDIT;
len=90;
RANGE=;
tabname=fw_info;
DISP_EN=modify date:max 90 bytes;
DISP_CH=�޸�����:����ܳ���90��Ӣ���ַ�;
MENU_INDEX=4;
DISPLAY=1;

key=FACTORY_FW_INFO_MODIFY_DESC;
type=STR;
operate=EDIT;
len=600;
RANGE=;
tabname=fw_info;
DISP_EN=modify description:max 600 bytes;
DISP_CH=�޸�˵��:����ܳ���200�����֣���600��Ӣ���ַ������޸ĵ���;�ָ���߻��Դ˷�����ʾ;
MENU_INDEX=5;
DISPLAY=1;









tab=setting;
DISP_EN=app setting;
DISP_CH=ϵͳ����;
DISPLAY=1;

tab_layer1=ap_onff; 
tabname =setting;
DISP_EN=ap switch select;
DISP_CH=Ӧ�ÿ���ѡ��;
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
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support card playing:0-not support, 1-support;
DISP_CH=�Ƿ�֧�ֿ����Ź���:0-��֧��,1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_LINEIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support linein:0-not support, 1-support;
DISP_CH=�Ƿ�֧��linein���Ź���:0-��֧��,1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_FMRADIO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support fm:0-not support, 1-support;
DISP_CH=�Ƿ�֧��FM������:0��ʾ��֧��FM��1��ʾ֧��FM;
DISPLAY=1;

key=SETTING_APP_SUPPORT_BT_INBACK;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support bluetooth run in back:0-not supported,1-supported;
DISP_CH=�Ƿ�֧�ֺ�̨����:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_BTPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support bluetooth a2dp play:0-not supported,1-supported;
DISP_CH=�Ƿ�֧����������:0-��֧��, 1-֧��;
DISPLAY=0;

key=SETTING_APP_SUPPORT_BTCALL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support bluetooth hfp call:0-not supported,1-supported;
DISP_CH=�Ƿ�֧����������:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_UHOSTPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support uhost play:0-not supported,1-supported;
DISP_CH=�Ƿ�֧��U�̲���:0��ʾ��֧�֣�1��ʾ֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_MDISKPLAY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support main disk play:0-not supported,1-supported;
DISP_CH=�Ƿ�֧�����ô��̲���:0��ʾ��֧�֣�1��ʾ֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_USBBOX;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support usb box:0-not supported,1-supported;
DISP_CH=�Ƿ�֧��USB����:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_USBMSC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support usb msc:0-not supported,1-supported;
DISP_CH=�Ƿ�֧�ִ��������̹���:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_RECORD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support recorder:0-not supported,1-supported;
DISP_CH=�Ƿ�֧��¼������:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SUPPORT_TAKEPHOTO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=ap_onff;    
tabname=setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support take photo:0-not supported,1-supported;
DISP_CH=�Ƿ�֧������ģʽ:0-��֧��, 1-֧��;
DISPLAY=1;

key=SETTING_APP_SWITCH_SEQUENCE;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=setting;
DISP_EN=app switch sequence:0 btplay,1 card play,2 linein;
DISP_CH=Ӧ���л�˳��:0��������,1����,2 linein,3 fm radio,4 usb audio,5��U; Ĭ��ֵ��ʾ����Ĭ�Ͻ����Ӧ�ã���ֵ����μ�case_type.h�еĺ궨�� APP_FUNC_XXX;
MENU_INDEX=10;
DISPLAY=1;

key=SETTING_HARDWARE_SUPPORT_CARD;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname =setting;
RANGE_DISP_EN=0:no support card,1:support card;
RANGE_DISP_CH=0:��֧�ֿ�,1:֧�ֿ�;
DISP_EN=hardware support card:0-not support, 1-support;
DISP_CH=Ӳ���Ƿ�֧�ֿ�:0 ��֧�� 1֧��;
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
DISP_CH= �ڲ�����ѡ��: 6:1.2pF,7:1.4pF,8:1.6pF,9:1.8pF,10:2.0pF;
MENU_INDEX=12;
DISPLAY=1;

key=SETTING_APP_SUPPORT_CHARGE_LOOP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname =setting;
RANGE_DISP_EN=0:not support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN= support charge loop: 0-not support, 1-support;
DISP_CH= �Ƿ�֧�ֿ��г��ģʽ:0��ʾ��֧�֣�1��ʾ֧��;
MENU_INDEX=13;
DISPLAY=1;

tab_layer1=power_option; 
tabname =setting;
DISP_EN=power manager option;
DISP_CH=��Դ����ѡ��;
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
RANGE_DISP_CH=0:�ر�ʡ��ػ�,1:��ʡ��ػ�;
DISP_EN=power save enable:0 for disable, 1 for enable
DISP_CH=�Ƿ��ʡ��ػ�:0��ʾ�ر�ʡ��ػ���1��ʾ��ʡ��ػ�
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
RANGE_DISP_CH=0:��ػ�,1:ʡ��ģʽ;
DISP_EN=power save mode:0 power off, 1 for low power
DISP_CH=ʡ��ػ�ģʽ:0��ʾ������ػ���1��ʾ����͹���ģʽ
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
DISP_CH=ʡ��ػ�ʱ��:��λΪ1���ӣ����֧�ֵ͹���ģʽ����ô�ӽ���͹��ĺ�ʼ��ʱ
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
DISP_CH=����ʡ��ػ�ʱ��:���֧�ֵ͹���ģʽ����ô�����������������
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
RANGE_DISP_CH=0:����ػ�,1:����ػ�;
DISP_EN=tws power off mode:0, tws poweroff; 1, single poweroff
DISP_CH=TWS�����ػ�ģʽ:0��ʾ����ػ���1��ʾ����ػ����ĸ����䰴���ػ��͹��ĸ�����
MENU_INDEX=5;
DISPLAY=1;

tab_layer1=keyinput_option; 
tabname =setting;
DISP_EN=key input option;
DISP_CH=��������ѡ��;
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
DISP_CH=�߼���������:�߼��������壬Ĭ��ֵ��ʾONOFF PIN�ļ�ֵ����� key_msg_value.h �еĶ���;
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
DISP_CH=������ADCֵ�б�:������ADCֵ�б���Ӳ������;
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
RANGE_DISP_CH=0:��֧��,1:֧��;
DISP_EN=support ir key:0 not support, 1 support
DISP_CH=�Ƿ�֧��IRң��:0��ʾ��֧�֣�1��ʾ֧��
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
DISP_CH=IRң��Э��:0��ʾToshiba 9012��1��ʾNEC��2��ʾRC5��3��ʾRC6;
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
DISP_CH=IRң���û���: IRң���û���, ����0xff00
DISPLAY=1;

key=SETTING_IR_KEY_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=keyinput_option; 
tabname =setting;
DISP_EN=IR key array:IR key array, default value is 0,KEY_NULL, key_msg_value.h;
DISP_CH=����ң�ذ�������:����ң�ذ������壬Ĭ��ֵΪ0�������壬��� key_msg_value.h �еĶ���;
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
DISP_CH=����ң�ذ�����ֵ�б�:����ң�ذ�����ֵ�б���Ӳ������;
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
RANGE_DISP_CH=0:ʹ���ⲿ��������,1:ʹ��LRADC1������������;
DISP_EN=keyboard resistance selection: 0, use extern pullup resistance, 1,use lradc1 inner pullup resistance;
DISP_CH=������������ѡ��: 0��ʹ���ⲿ��������,1��ʹ��LRADC1����100Kŷķ��������;
MENU_INDEX=8;
DISPLAY=1;

tab_layer1=aout_option; 
tabname =setting;
DISP_EN=audio output option;
DISP_CH=��Ƶ���ѡ��;
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
RANGE_DISP_CH=0:�߰ڷ�,1:�Ͱڷ�;
DISP_EN=audio output pa swing select:0 - 2p8 pa swing, 1 - 1p6 pa swing;
DISP_CH=��Ƶ����ڷ�ѡ��:0��ʾ�߰ڷ������1��ʾ�Ͱڷ����;
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
RANGE_DISP_CH=0:˫�������,1:��Ϻ�����������,2:��Ϻ�����������,3:��Ϻ�˫�������,4:������������˫�������;
DISP_EN=audio output channel select:0 - dual channel output, 1 - mix left channel output, 2 - mix right channel output, 3 - mix dual channel output, 4 - switch dual channel output;
DISP_CH=��Ƶ���ͨ��ѡ��:0��ʾ˫���������1��ʾ��Ϻ�������������2��ʾ��Ϻ�������������3��ʾ��Ϻ�˫���������4��ʾ������������˫�������;
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
RANGE_DISP_CH=0:��ʹ�ܹ��ž���,1:ʹ�ܹ��ž���;
DISP_EN=extern pa mute when pause:0-disable, 1-enable;
DISP_CH=��ͣ����ʱ���ž���ʹ��:0��ʾ��ʹ�ܣ�1��ʾʹ��;
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
RANGE_DISP_CH=0:��ʹ�ö���,1:ʹ�ö���;
DISP_EN=use earphone: 0 for no use, 1 for use;
DISP_CH=�Ƿ�ʹ�ö������:ʹ�ö����������PAʱ��Ҫ��PA����ʱ�ϳ����ر�PAʱ��Ҫ��PA�ŵ��ʱҲ�ϳ���0��ʾ��ʹ�ã�1��ʾʹ��;
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
RANGE_DISP_CH=0:��ֱ��,1:ֱ��;
DISP_EN=enable direct drive: 0 indirect dirve, 1 direct drive;
DISP_CH=�Ƿ�ʹ�ö���ֱ����0��ʾ��ֱ����1��ʾֱ��;
MENU_INDEX=5;
DISPLAY=1;

tab_layer1=tts_option; 
tabname =setting;
DISP_EN=tts option;
DISP_CH=״̬��������ѡ��;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=tts option:0-disable,1-enable;
DISP_CH=״̬��������ѡ��:0-�ر�,1-����;
DISPLAY=1;

key=SETTING_SOUND_ENABLE_CONTROL_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=tts_option; 
tabname =setting;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=control tts option:0-disable,1-enable;
DISP_CH=״̬��������������TTSѡ��:������һ��/��һ��/��ͣ/���ŵȣ�0��ʾ�رգ�1��ʾ��;
DISPLAY=1;

key=SETTING_SOUND_TTS_LANGUAGE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=tts_option; 
tabname =setting;
RANGE_DISP_EN=0:chinese,1:english;  
RANGE_DISP_CH=0:����,1:Ӣ��;
DISP_EN=tts language:0 for chinese, 1 for english;
DISP_CH=TTS������������:0��ʾ���ģ�1��ʾӢ��;
DISPLAY=0;

tab_layer1=kt_option; 
tabname =setting;
DISP_EN=keytone option;
DISP_CH=������ѡ��;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=keytone:0-disable,1-enable;
DISP_CH=����������:0-�ر�,1-����;
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
DISP_CH=�������������:��λ0.1dB;
MENU_INDEX=2;
DISPLAY=1;

tab_layer1=led_segment_option; 
tabname =setting;
DISP_EN=led segment option;
DISP_CH=�����ѡ��;
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
RANGE_DISP_CH=0:��֧��,1:֧��;
DISP_EN=support LED segment:0-not support, 1-support;
DISP_CH=�Ƿ�֧��LED�������ʾ:0 ��֧�� 1֧��;
DISPLAY=1;

key=SETTING_DISPLAY_LED_SEGMENT_LIGHT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=led_segment_option; 
tabname =setting;
RANGE_DISP_EN=0:Dont support,1:support;  
RANGE_DISP_CH=0:��֧��,1:֧��;
DISP_EN=support LED backlight:0-not support, 1-support;
DISP_CH=�Ƿ�֧�ֿ���ʱ�ر������:0 ��֧�� 1֧��;
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
DISP_CH=����ʱ��:��λ1��;
DISPLAY=1;

tab_layer1=led_option; 
tabname =setting;
DISP_EN=led light option;
DISP_CH=LED��ѡ��;
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
RANGE_DISP_CH=0:��֧��,1:֧��;
DISP_EN=support LED:0-not support, 1-support;
DISP_CH=�Ƿ�֧��LED����ʾ:0 ��֧�� 1֧��;
DISPLAY=1;

tab_layer1=volume_control; 
tabname =setting;
DISP_EN=volume control parameters;
DISP_CH=�����������;
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
DISP_CH=Ĭ��������:�������״ο���Ĭ��������������Ҳ������Ϊÿ�ο���������������;
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
DISP_CH=Ĭ�ϴ�绰����:�������״ο���Ĭ�ϴ�绰����������Ҳ������Ϊÿ�ο��������ô�绰����;
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
RANGE_DISP_CH=0:������,1:����; 
DISP_EN=power on reset volume default:0-not reset,1-reset;
DISP_CH=�����������õ�Ĭ��ֵѡ��:0��ʾ�����ã�1��ʾҪ����;
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
DISP_CH=������С����:���⿪���������κ�����;
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
DISP_CH=�����������:���⿪������̫��;
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
DISP_CH=����������:��λΪ0.1db��Ϊ����ʾ��Ҫ��������ʵ��ͣ�Ϊ����ʾ��Ҫ��������ʵ��ߣ�����ʧ��Ȼ�Ӵ�
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
DISP_CH=line-in��������:��λΪ0.1db��Ϊ����ʾ��Ҫ��������ʵ��ͣ�Ϊ����ʾ��Ҫ��������ʵ��ߣ�����ʧ��Ȼ�Ӵ�
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
DISP_CH=fm��������������:��λΪ0.1db��Ϊ����ʾ��Ҫ��������ʵ��ͣ�Ϊ����ʾ��Ҫ��������ʵ��ߣ�����ʧ��Ȼ�Ӵ�
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
DISP_CH=��绰��������:��λΪ0.1db����绰����������Сһ����������СһЩ
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
RANGE_DISP_CH=0:������,1:�������,2:�ʶ�����,3:�ض�����,4:�û��Զ���;
DISP_EN=volume limit grade:0-disable limit,1-light limit,2-moderate limit,3-heavy limit,4-user defined;
DISP_CH=�����ϵ�ʱ�������Ƽ���:0��ʾ������; �������Ʊ�������λ 0.1db, �ֱ��Ӧ����0 ~ ����9; 1��ʾ������ƣ��ʺϵ�������ϴ�����DCDC�ķ������������Ʊ� [-50,-50,-30,-20,-10,0,0,0,0,0];2��ʾ�ʶ����ƣ��ʺϵ��������������DCDC�ķ������������Ʊ� [-80,-80,-60,-40,-25,-10,0,0,0,0];3��ʾ�ض����ƣ��ʺϵ��������С����DCDC�ķ������������Ʊ� [-100,-100,-80,-60,-40,-20,0,0,0,0];4��ʾ�Զ��壬�� SETTING_SOUND_VOLUME_PHY_LIMIT_TBL ����
MENU_INDEX=9;
DISPLAY=1;

tab_layer1=alarm_control; 
tabname =setting;
DISP_EN=alarm control parameters;
DISP_CH=�����������;
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
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=support snooze:0-not support, 1-support;
DISP_CH=�Ƿ�֧��̰˯����:0-��֧��,1-֧��;
DISPLAY=1;

key=SETTING_TIMEALARM_SNOOZE_TIMER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=alarm_control;
tabname =setting;
DISP_EN=snooze timer:1 ~ 30 minutes;
DISP_CH=̰˯ʱ��:��λΪ����;
DISPLAY=1;

key=SETTING_TIMEALARM_SNOOZE_MAX_TIMES;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=snooze times:1 ~ 10 times;
DISP_CH=̰˯������:1 ~ 10 ��;
DISPLAY=1;

key=SETTING_TIMEALARM_ALARM_OVERTIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=alarm overtime:1 ~ 30 minutes;
DISP_CH=���ӳ�ʱʱ��:��λΪ����;
DISPLAY=1;

key=SETTING_TIMEALARM_ALARM_OVERTIME_DEAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=alarm_control;
tabname =setting;
RANGE_DISP_EN=0:close,1:auto snooze;  
RANGE_DISP_CH=0:�ر�����,1:�Զ�ѡ��̰˯; 
DISP_EN=alarm overtime deal:0-close alarm, 1-auto snooze;
DISP_CH=���ӳ�ʱδ��������ѡ��:0��ʾ�ر����ӣ�1��ʾ�Զ�ѡ��̰˯;
DISPLAY=1;

key=SETTING_TIMEALARM_DEFAULT_VOLUME;
type=DATA;
operate=SINGLE;
len=;
RANGE=5-31;
tab_layer1_name=alarm_control; 
tabname =setting;
DISP_EN=alarm default volume:5 ~ 31;
DISP_CH=����Ĭ������:5 ~ 31;
DISPLAY=1;

key=SETTING_TIMEALARM_TIME_AMPM_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=alarm_control;
tabname =setting;
RANGE_DISP_EN=0:24 hour,1:ampm;  
RANGE_DISP_CH=0:24Сʱ��,1:12Сʱ��; 
DISP_EN=ampm enable:0-24 hour, 1-ampm;
DISP_CH=ʱ����ʾ��ʽ:0��ʾ24Сʱ�ƣ�1��ʾ12Сʱ��;
DISPLAY=1;

tab_layer1=card_config; 
tabname =setting;
DISP_EN=card configuration;
DISP_CH=���������;
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
RANGE_DISP_CH=0:0��,1:1��,2:2��,3:3��,4:4��,5:5��,6:6��,7:7��;
DISP_EN= card cmd pin pad level:0 ~ 7 level,0 is lowest;
DISP_CH= ��cmd pin������������:0 ~ 7����0Ϊ��ͼ�;
DISPLAY=0;

key=SETTING_CARD_CLK_PAD_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tab_layer1_name=card_config; 
tabname =setting;
RANGE_DISP_EN=0:level 0,1:level 1,2:level 2,3:level 3,4:level 4,5:level 5,6:level 6,7:level 7;
RANGE_DISP_CH=0:0��,1:1��,2:2��,3:3��,4:4��,5:5��,6:6��,7:7��;
DISP_EN= card clk  pinpad level;
DISP_CH= ��clk pin������������:0 ~ 7����0Ϊ��ͼ�;
DISPLAY=0;

key=SETTING_CARD_DAT_PAD_LEVEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2,3,4,5,6,7;
tab_layer1_name=card_config; 
tabname =setting;
RANGE_DISP_EN=0:level 0,1:level 1,2:level 2,3:level 3,4:level 4,5:level 5,6:level 6,7:level 7;
RANGE_DISP_CH=0:0��,1:1��,2:2��,3:3��,4:4��,5:5��,6:6��,7:7��;
DISP_EN= card data pin pad level;
DISP_CH= ��data pin������������:0 ~ 7����0Ϊ��ͼ�;
DISPLAY=0;

tab_layer1=test_debug; 
tabname =setting;
DISP_EN=test & debug configuration;
DISP_CH=���Ե���ѡ��;
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
RANGE_DISP_CH=0:�������Ź�,1:�رտ��Ź�;
DISP_EN=watchdog switch:0-watchdog open,1-watchdog close;
DISP_CH=���Ź�����:0-�������Ź���1-�رտ��Ź�;
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
DISP_CH=������������ʱ��:0:not reset,1:352ms,2:703ms,3:1.4s,4:2.8s,5:5.6s,6:11.2s;
DISPLAY=1;

key=SETTING_SYS_EMI_REDUCTION_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:don't care EMI,1:EMI Reduction;
RANGE_DISP_CH=0:������EMI,1:����EMI;
DISP_EN=EMI Reduction Enable:0-don't care EMI, 1-EMI Reduction Enable;
DISP_CH=����EMIʹ��:0��ʾ������EMI��1��ʾ����EMIʹ��;
DISPLAY=1;

key=SETTING_SYS_ADCKEY_ADFU_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-3;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:disable adckey adfu,1:enable adckey adfu,2:force enable udisk,3:enable both;
RANGE_DISP_CH=0:��ֹADFU,1:ʹ��ADFU,2:ǿ��ʹ�ܶ�����,3:���߾�ʹ��;
DISP_EN=enter adfu when boot detected adc key:0:,disable adckey adfu, 1,enable adckey adfu, 2,force enable udisk, 3,enable both;
DISP_CH=����ʱ��⵽ADC��������ADFU:0,��ֹ��1,ʹ��ADFU, 2,ǿ��ʹ�ܶ�����, 3,���߾�ʹ��;
DISPLAY=1;

key=SETTING_SYS_ADCKEY_BOOT_FUNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:NONE,1:ADFU,2:CardProduct;
RANGE_DISP_CH=0:������,1:ADFU,2:������;
DISP_EN=lradc1 detect 0 option: 0, none, 1, enter ADFU, 2, enter Card Product;
DISP_CH=lradc1 ��⵽0��ѡ��: 0����������������,1������ADFU������2�����뿨����;
DISPLAY=1;

key=SETTING_UART_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no support uart print,1:support uart print;
RANGE_DISP_CH=0:��֧��Uart��ӡ,1:֧��Uart��ӡ;
DISP_EN=uart print support:0-no support uart print, 1-support uart print;
DISP_CH=�Ƿ�֧��Uart��ӡ:0 ��֧��Uart��ӡ 1֧��Uart��ӡ;
DISPLAY=0;

key=SETTING_BT_CONTROLLER_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no support bt controller print,1:support bt controller print;
RANGE_DISP_CH=0:��֧��������������ӡ,1:֧��������������ӡ;
DISP_EN=bt controller print support:0-not support, 1-support;
DISP_CH=�Ƿ�֧��������������ӡ:0 ��֧�� 1֧��;
DISPLAY=0;

key=SETTING_BT_ENGINE_PRINT_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tab_layer1_name=test_debug; 
tabname =setting;
RANGE_DISP_EN=0:no print,1:print;
RANGE_DISP_CH=0:����ӡ,1:��ӡ;
DISP_EN=bt engine print support:0-no print,1-print;
DISP_CH=����Э��ջ��ӡʹ��:0��ʾ����ӡ��1��ʾ����ӡ;
DISPLAY=0;

tab_layer1=dae_config; 
tabname =setting;
DISP_EN=digital audio effect configuration;
DISP_CH=������Ч����;
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
RANGE_DISP_CH=0:����������Ч,1:�ر�������Ч;
DISP_EN=bypass mode:0-off,1-on;
DISP_CH=�Ƿ����DAE������Ч:0��ʾ�����ã�������DAE������Ч��1��ʾ���ã����ر�������Ч
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
DISP_CH=Ԥ˥��:��λ0.1db
MENU_INDEX=2;
DISPLAY=1;

tab_layer2=parameter_eq; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=parameter eq configuration;
DISP_CH=����������;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=parameter eq option:0-disable,1-enable;
DISP_CH=PEQ����:0-�ر�,1-����;
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
DISP_CH=PEQ BAND1 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND2 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND3 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND4 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ band5 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ band6 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND7 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ band8 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND9 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND10 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND11 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND12 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND13 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
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
DISP_CH=PEQ BAND14 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

tab_layer2=limiter_config; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=limiter configuration;
DISP_CH=�޷�������;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=limiter option:0-disable,1-enable;
DISP_CH=�޷�������:0-�ر�,1-����;
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
DISP_CH=�޷�������:[�޷�����ֵ����λΪ0.1DB����Χ -60DB ~ 0DB; �޷�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 10 ms; �޷����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 1000 ms]
DISPLAY=1;

tab_layer2=drc_config; 
tab_layer1_name=dae_config; 
tabname =setting;
DISP_EN=drc configuration;
DISP_CH=��̬��Χ��������;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=drc option:0-disable,1-enable;
DISP_CH=��̬��Χ���ƿ���:0-�ر�,1-����;
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
DISP_CH=��Ƶ��:����5Hz;
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
DISP_CH=��Ƶ��DRC����:[��ֵ����λΪ0.1DB����Χ -60DB ~ 0DB;�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;ѹ������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;ѹ���ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;��ֹƵ�ʣ���λΪ1Hz��ȡֵ��ΧΪ 100Hz ~ 400Hz;Qֵ����λΪ0.1��ȡֵ��ΧΪ 0.3 ~ 1.0; 1]
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
DISP_CH=��Ƶ��DRC����:[��ֵ����λΪ0.1DB����Χ -60DB ~ 0DB;�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;ѹ������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;ѹ���ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;��ֹƵ�ʣ���λΪ1Hz��ȡֵ��ΧΪ 100Hz ~ 400Hz;Qֵ����λΪ0.1��ȡֵ��ΧΪ 0.3 ~ 1.0; 1]
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
DISP_CH=DRC������:step 0.1dB;
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
DISP_CH=DRC��������:step 0.1dB;
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
DISP_CH=aux ��Ƶ��:����5Hz;
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
DISP_CH=aux ��Ƶ��DRC����:[��ֵ����λΪ0.1DB����Χ -60DB ~ 0DB;�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;ѹ������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;ѹ���ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;��ֹƵ�ʣ���λΪ1Hz��ȡֵ��ΧΪ 100Hz ~ 400Hz;Qֵ����λΪ0.1��ȡֵ��ΧΪ 0.3 ~ 1.0; 1]
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
DISP_CH=aux ��Ƶ��DRC����:[��ֵ����λΪ0.1DB����Χ -60DB ~ 0DB;�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;ѹ������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 100 ms;ѹ���ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 5000 ms;��ֹƵ�ʣ���λΪ1Hz��ȡֵ��ΧΪ 100Hz ~ 400Hz;Qֵ����λΪ0.1��ȡֵ��ΧΪ 0.3 ~ 1.0; 1]
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
DISP_CH=aux DRC������:step 0.1dB;
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
DISP_CH=aux DRC��������:step 0.1dB;
DISPLAY=1;

tab_layer1=okm_config; 
tabname =setting;
DISP_EN=karaoke configuration;
DISP_CH=����OK����;
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
DISP_CH=MIC����Դѡ��:0��ʾMIC0��1��ʾMIC1
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
DISP_CH=mic-in OP����:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db,8:0db,9:1.5db,10:3db,11:4.5db,12:6db,13:7.5db,14:8db,15:9.5db;
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
DISP_CH=mic-in ADC ����:��λ3dB����Χ0 ~ 15����ʾ0dB ~ 45dB;
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
RANGE_DISP_CH=0:MIC����,1:MIC��ϵ�����,2:MIC�滻������;
DISP_EN=mic channel mix play left channel option:0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
DISP_CH=MICͨ����ϵ�����ͨ��������ѡ��:0��ʾ��Ӱ�첥��ͨ����������1��ʾ��ϵ�����ͨ����������2��ʾ�滻����ͨ��������;
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
RANGE_DISP_CH=0:MIC����,1:MIC��ϵ�����,2:MIC�滻������;
DISP_EN=mic channel mix play right channel option:0:MIC disable,1:MIX mix PLAY,2:MIC replace PLAY;
DISP_CH=MICͨ����ϵ�����ͨ��������ѡ��:0��ʾ��Ӱ�첥��ͨ����������1��ʾ��ϵ�����ͨ����������2��ʾ�滻����ͨ��������;
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
DISP_CH=MICͨ������������:-24.0dB ~ 0dB, unit is 0.1dB;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=mic channel peq enable:0-disable,1-enable;
DISP_CH=MICͨ���Ƿ�ʹ��PEQ:0-�ر�,1-����;
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
DISP_CH=MICͨ��PEQ����:��DAE_PEQ_BANK13_SETTING������ȥ;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=limiter option:0-disable,1-enable;
DISP_CH=�޷�������:0-�ر�,1-����;
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
DISP_CH=�޷�������:[�޷�����ֵ����λΪ0.1DB����Χ -60DB ~ 0DB; �޷�������ʱ�䣬��λΪ0.01ms��ȡֵ��ΧΪ 0.02 ~ 10 ms; �޷����ͷ�ʱ�䣬��λΪ1ms��ȡֵ��ΧΪ 1 ~ 1000 ms]
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=MIC echo effect:0-disable,1-enable;
DISP_CH=MIC����Ч��:�Ƿ�ʹ��MIC����Ч��,0-�ر�,1-����;
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
DISP_CH=MIC������ʱ:��λ1ms;
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
DISP_CH=echo˥��ϵ��:MIC������ȵ����б������������20��;
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
DISP_CH=������һ����ͨ�˲�����Ƶ��:20 ~ 20000;
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
DISP_CH=�����ڶ�����ͨ�˲�����Ƶ��:20 ~ 20000;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=inhibition howling:0-disable,1-enable;
DISP_CH=�Ƿ�������Х���㷨:0-�ر�,1-����;
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=duching effect:0-disable,1-enable;
DISP_CH=�Ƿ�������Ч��:0-�ر�,1-����;
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
DISP_CH=����Ч������:[���ʱ��ms������ʱ��ms���ͷ�ʱ��ms��������ֵ/��ֵ0.1dB�����ܷ���0.1dB]
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=voice removal:0-disable,1-enable;
DISP_CH=�Ƿ������������㷨:0-�ر�,1-����;
MENU_INDEX=19;
DISPLAY=1;










tab=usb;
DISP_EN=usb property;
DISP_CH=USB����;
DISPLAY=1;

key=USB_VID;
type=STR;
operate=EDIT;
len=6;
RANGE=;
tabname=usb;
DISP_EN=USB VID;
DISP_CH=USB VID:�豸����ID;
MENU_INDEX=1;
DISPLAY=1;

key=INF_USB_VENDOR;
type=STR;
operate=EDIT;
len=8;
RANGE=;
tabname=usb;
DISP_EN=usb vendor;
DISP_CH=USB�豸����(<8�ַ�);
MENU_INDEX=2;
DISPLAY=1;

tab_layer1=usb_sound; 
tabname =usb;
DISP_EN=usb sound configuration;
DISP_CH=USB��������;
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
DISP_CH=USB SOUND PID:ͬһVID�²�ͬusb�豸ID������ͬ;
DISPLAY=1;

key=INF_SOUND_PRODUCT_NAME;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tab_layer1_name=usb_sound; 
tabname =usb;
DISP_EN=usb sound product name;
DISP_CH=USB SOUND����:���ż�¼������USB�豸��;
DISPLAY=1;

key=INF_VOLUME_FLAG;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-3;
tab_layer1_name=usb_sound; 
tabname =usb;
RANGE_DISP_EN=1:only player,2:only pc,3:player and pc but pc first;
RANGE_DISP_CH=1:С���ɵ�,2:PC�ɵ�,3:С��PC���ɵ���PC����;
DISP_EN=hid volume adjust option:1 for only player, 2 for only pc, 3 for player and pc but pc first;
DISP_CH=HID��������ѡ��:1С���ɵ���2PC�ɵ���3С��PC���ɵ���PC����
DISPLAY=1;

key=USB_SOUND_AUTO_PLAY_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=usb_sound; 
tabname =usb;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=enable auto play:0 disable auto play, 1 enable auto play;
DISP_CH=�Ƿ����Զ����Ź���:0��ʾ�رգ�1��ʾ����;
DISPLAY=1;

tab_layer1=usb_card_reader; 
tabname =usb;
DISP_EN=usb card reader configuration;
DISP_CH=USB����������;
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
DISP_CH=USB CARDREADER PID:ͬһVID�²�ͬusb�豸ID������ͬ;
DISPLAY=1;

key=INF_CARDREADER_PRODUCT_NAME;
type=STR;
operate=EDIT;
len=16;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=USB cardreader product name;
DISP_CH=USB CARDREADER����:���ٶ������豸��;
DISPLAY=1;

key=INF_INQUIRY_INFORMATION_FOB;
type=STR;
operate=EDIT;
len=58;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=infomation of Udisk;
DISP_CH=Udisk ��Ϣ;
DISPLAY=1;

key=INF_UDISK_LABEL_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=usb_card_reader;    
tabname =usb;
RANGE_DISP_EN=0:udisk label disable,1:udisk label enable;
RANGE_DISP_CH=0:��֧��,1:֧��; 
DISP_EN=udisk label enable:0-disable, 1-enable;
DISP_CH=�Ƿ�ʹ�ܽ���U��ģʽ���þ��:0-��֧��,1-֧��;
DISPLAY=1;

key=INF_UDISK_LABEL;
type=STR;
operate=EDIT;
len=11;
RANGE=;
tab_layer1_name=usb_card_reader; 
tabname =usb;
DISP_EN=show udisk label;
DISP_CH=���̾����;
DISPLAY=1;









tab=radio;
DISP_EN=radio property;
DISP_CH=����������;
DISPLAY=1;

key=RADIO_AP_BAND_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname =radio;
RANGE_DISP_EN=0:chinese band,1:japanese band,2:europe band;
RANGE_DISP_CH=0:�й�Ƶ��,1:�ձ�Ƶ��,2:ŷ��Ƶ��;
DISP_EN=radio band select:0-normal��1-japan��2-europe;
DISP_CH=��̨����ѡ��:0-�й�Ƶ�Σ�1-�ձ�Ƶ�Σ�2-ŷ��Ƶ��;
MENU_INDEX=1;
DISPLAY=1;

key=RADIO_SEEK_THRESHOLD;
type=ARRAY;
operate=EDIT;
len=;
RANGE=0-65535;
tabname=radio;
DISP_EN=search threshold: search threshold.
DISP_CH=��̨����ֵ: ��FMģ�������ֵ���岻һ�£������datasheet����
MENU_INDEX=2;
DISPLAY=1;

key=RADIO_SEEK_AUDITION_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:diable,1:enable;
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=auto search audition:0 disable, 1 enable;
DISP_CH=�Զ���̨��������:0��ʾ�رգ�1��ʾ����;
MENU_INDEX=3;
DISPLAY=1;

key=RADIO_SEEK_AUDITION_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-10;
tabname=radio;
DISP_EN=auto search audition time:unit is 1S;
DISP_CH=�Զ���̨����ʱ��:��λ1S;
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
DISP_CH=FM OP ����:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
MENU_INDEX=5;
DISPLAY=1;

key=RADIO_GAIN_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=radio;
DISP_EN=FM ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=FM ADC ����:��λ3dB����Χ0 ~ 15����ʾ0dB ~ 45dB;
MENU_INDEX=6;
DISPLAY=1;

key=RADIO_CLK_SRC_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:inner clock,1:extern clock;
RANGE_DISP_CH=0:Ƭ��ʱ��Դ,1:�ⲿʱ��Դ;
DISP_EN=FM clock select:0 for inner clock source, 1 for extern clock source
DISP_CH=Fm ʱ��Դѡ��:0��ʾƬ��ʱ��Դ��1��ʾ�ⲿʱ��Դ
MENU_INDEX=7;
DISPLAY=1;

key=RADIO_CLK_VAL_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-4;
tabname=radio;
RANGE_DISP_EN=0:32768Hz,1:6.5MHz(NO REALIZE),2:13MHz(NO REALIZE),3:24MHz,4:26MHz;
RANGE_DISP_CH=0:32768Hz,1:6.5M���ݲ����ã�,2:13M���ݲ����ã�,3:24M,4:26M;
DISP_EN=FM clock frequency select:0 for 32768Hz, 1 for 6.5MHz(NO REALIZE), 2 for 13MHz(NO REALIZE), 3 for 24MHz, 4 for 26MHz
DISP_CH=Fm ʱ��Ƶ��ѡ��:0��ʾ32K��1��ʾ6.5M���ݲ����ã���2��ʾ13M���ݲ����ã���3��ʾ24M��4��ʾ26M
MENU_INDEX=8;
DISPLAY=1;

key=RADIO_ENABLE_FREQ_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=radio;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=frequency tts option:0-disable,1-enable;
DISP_CH=�Ƿ��Ƶ��TTS:0��ʾ�رգ�1��ʾ��;
MENU_INDEX=9;
DISPLAY=1;

key=RADIO_INPUT_PIN_USE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=radio;
RANGE_DISP_EN=0:AUXL0 and AUXR0 DUAL,1:AUXL1 and AUXR1 DUAL,2:AUXL0 and AUXL1 MIX,3:AUXR0 and AUXR1 MIX,4:AUXL0 and AUXR0 MIX,5:ONLY AUXL0,6:ONLY AUXR0,7:ONLY AUXL1,8:ONLY AUXR1;
RANGE_DISP_CH=0:AUXL0��AUXR0������,1:AUXL1��AUXR1������,2:AUXL0��AUXL1���,3:AUXR0��AUXR1���,4:AUXL0��AUXR0���,5:��AUXL0����,6:��AUXR0����,7:��AUXL1����,8:��AUXR1����;
DISP_EN=fm radio input pin use:0 for AUXL0 and AUXR0 DUAL,1 for AUXL1 and AUXR1 DUAL,2 for AUXL0 and AUXL1 MIX,3 for AUXR0 and AUXR1 MIX,4 for AUXL0 and AUXR0 MIX,5 for ONLY AUXL0,6 for ONLY AUXR0,7 for ONLY AUXL1,8 for ONLY AUXR1;
DISP_CH=fm radio ��������ʹ��:0��ʾAUXL0��AUXR0���������룬1��ʾAUXL1��AUXR1���������룬2��ʾAUXL0��AUXL1������룬3��ʾAUXR0��AUXR1������룬4��ʾAUXL0��AUXR0������룬5��ʾIC���Ϻ�AUXL0���룬6��ʾIC���Ϻ�AUXR0���룬7��ʾIC���Ϻ�AUXL1���룬8��ʾIC���Ϻ�AUXR1����;
DISPLAY=1;









tab=music;
DISP_EN=music property;
DISP_CH=���ز�������;
DISPLAY=1;

key=MUSIC_REPLAY_BY_PREV_KEY;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=music;
RANGE_DISP_EN=0:switch prev song,1:replay current song;
RANGE_DISP_CH=0:�л���һ��,1:��ͷ��ʼ����;
DISP_EN=replay by prev key:0,no support, 1,support
DISP_CH=��PREV���ص���ͷ�ز�:���������ų���5���Ӻ󣬰�PREV���Ƿ�ص���ͷ�ز���0��ʾ�л�����һ����1��ʾ��ͷ��ʼ����
DISPLAY=1;

key=MUSIC_ENABLE_SEQ_NUM_TTS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=music;
RANGE_DISP_EN=0:disable,1:enable;  
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=seq num tts option:0-disable,1-enable;
DISP_CH=�Ƿ����Ŀ��TTS:0��ʾ�رգ�1��ʾ��;
DISPLAY=1;








tab=linein;
DISP_EN=linein property;
DISP_CH=��Ƶ��������;
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
DISP_CH=line-in����:0:-12db,1:-3db,2:0db,3:1.5db,4:3db,5:4.5db,6:6db,7:7.5db;
DISPLAY=1;

key=LINEIN_GAIN_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=linein;
DISP_EN=linein ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=linein ADC ����:��λ3dB����Χ0 ~ 15����ʾ0dB ~ 45dB;
DISPLAY=1;

key=LINEIN_CHANNEL_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:digital channel,1:anolog channel;
RANGE_DISP_CH=0:����ͨ��,1:ģ��ͨ��;
DISP_EN=linein channel selection:0 for digital channel,1 for anolog channel;
DISP_CH=line-in ͨ��ѡ��:0��ʾ����ͨ��, 1��ʾģ��ͨ��;
DISPLAY=1;

key=LINEIN_INPUT_PIN_USE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=linein;
RANGE_DISP_EN=0:AUXL0 and AUXR0 DUAL,1:AUXL1 and AUXR1 DUAL,2:AUXL0 and AUXL1 MIX,3:AUXR0 and AUXR1 MIX,4:AUXL0 and AUXR0 MIX,5:ONLY AUXL0,6:ONLY AUXR0,7:ONLY AUXL1,8:ONLY AUXR1;
RANGE_DISP_CH=0:AUXL0��AUXR0������,1:AUXL1��AUXR1������,2:AUXL0��AUXL1���,3:AUXR0��AUXR1���,4:AUXL0��AUXR0���,5:��AUXL0����,6:��AUXR0����,7:��AUXL1����,8:��AUXR1����;
DISP_EN=linein input pin use:0 for AUXL0 and AUXR0 DUAL,1 for AUXL1 and AUXR1 DUAL,2 for AUXL0 and AUXL1 MIX,3 for AUXR0 and AUXR1 MIX,4 for AUXL0 and AUXR0 MIX,5 for ONLY AUXL0,6 for ONLY AUXR0,7 for ONLY AUXL1,8 for ONLY AUXR1;
DISP_CH=line-in ��������ʹ��:0��ʾAUXL0��AUXR0���������룬1��ʾAUXL1��AUXR1���������룬2��ʾAUXL0��AUXL1������룬3��ʾAUXR0��AUXR1������룬4��ʾAUXL0��AUXR0������룬5��ʾIC���Ϻ�AUXL0���룬6��ʾIC���Ϻ�AUXR0���룬7��ʾIC���Ϻ�AUXL1���룬8��ʾIC���Ϻ�AUXR1����;
DISPLAY=1;

key=LINEIN_LOWENERGY_POWEROFF_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=low energy poweroff enable:0-disable,1-enable;
DISP_CH=������ʡ��ػ�����:0-�ر�,1-����;
DISPLAY=1;

key=LINEIN_ENABLE_TWS_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=linein;
RANGE_DISP_EN=0:disable,1:enable;
RANGE_DISP_CH=0:��ֹ,1:ʹ��;
DISP_EN=tws mode enable:0-disable,1-enable;
DISP_CH=TWSģʽʹ��:0-��ֹ,1-ʹ��;
DISPLAY=1;











tab=record;
DISP_EN=record property;
DISP_CH=¼����������;
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
DISP_CH=¼��mic-in OP����:0:26db,1:30db,2:31.5db,3:33db,4:34.5db,5:36db,6:37.5db,7:39db;
DISPLAY=1;

key=RECORD_GAIN_MIC_ADC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=record;
DISP_EN=record mic-in ADC gain:unit 3dB, 0 ~ 15;
DISP_CH=¼�� mic-in ADC ����:��λ3dB����Χ0 ~ 15����ʾ0dB ~ 45dB;
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
DISP_CH=MIC¼��������ѡ��:0:8KHz,1:11KHz,2:12KHz,3:16KHz,4:22KHz,5:24KHz,6:32KHz,7:44KHz,8:48KHz;
DISPLAY=1;

key=RECORD_SCAN_MAX_SPACE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-8;
tabname=record;
RANGE_DISP_EN=0:all disk,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
RANGE_DISP_CH=0:ȫ��ɨ��,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISP_EN=record disk scan max space:0:all disk,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISP_CH=¼��ɨ����̿��пռ�:0-ȫ��ɨ��,1:32M,2:64M,3:128M,4:256M,5:512M,6:1024M(1G),7:1536M(1.5G),8:2048M(2G);
DISPLAY=1;













tab=btstack;
DISP_EN=btstack property;
DISP_CH=����Э��ջ����;
DISPLAY=1;

key=BTSTACK_DEVICE_NAME;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth device name:max 29 bytes;
DISP_CH=�����豸����:���29�ַ�;
DISPLAY=1;

key=BTSTACK_DEVICE_NAME_HID;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth hid device name:max 29 bytes;
DISP_CH=����HID�豸����:���29�ַ�;
DISPLAY=1;

key=BTSTACK_FIX_PIN_CODE;
type=STR;
operate=EDIT;
len=15;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth fix pin code:max 15 bytes;
DISP_CH=�����������:���15�ַ�;
DISPLAY=0;

key=BTSTACK_CLASS_OF_DEVICE;
type=DATA;
operate=EDIT;
len=;
RANGE=0-16777215;
tabname=btstack;
DISP_EN=class of device: detail information on www.bluetooth.org/zh-cn/specification/assigned-numbers/baseband; 0x240540 FOR HID DEVICE
DISP_CH=�����豸����:���ֽڱ�ʾmajor service�����ֽڱ�ʾmajor device�����ֽڱ�ʾminor device������������ο� www.bluetooth.org/zh-cn/specification/assigned-numbers/baseband; �����HID�豸��Ҫ����Ϊ 0x240540
DISPLAY=1;

key=BTSTACK_BDADD_HIGH3BYTES;
type=DATA;
operate=EDIT;
len=;
RANGE=0-16777215;
tabname=btstack;
DISP_EN=bt addr[byte5, byte4, byte3];
DISP_CH=������ַ��3�ֽ�;
DISPLAY=1;

key=BTSTACK_CAPACITANCE_VALUE_ARRAY;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tabname=btstack;
DISP_EN=;
DISP_CH=��ǰ�¶���Գ����貹������ֵ��:Ĭ��ֵΪ1��ʾ�¶Ȳ�����,Ϊ0��ʾ�¶Ȳ����رգ�[]����ֵ��λΪ0.1pF���ֱ��Ӧ-20�棬0�棬20�棬40�棬60�棬75��
DISPLAY=0;

key=BTSTACK_SNIFF_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btstack;
RANGE_DISP_EN=0:sniff disable,1:sniff enable;
RANGE_DISP_CH=0:��ֹ����sniff,1:ʹ�ܽ���sniff;
DISP_EN=sniff enable:0,sniff disable, 1,sniff enable
DISP_CH=�Ƿ�ʹ�ܽ���sniff:0��ʾ��ֹ����sniff,1��ʾʹ�ܽ���sniff
DISPLAY=0;

key=BTSTACK_SNIFF_INTERVAL_VALUE;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-2;
tabname=btstack;
DISP_EN=sniff interval:1:500ms;2:1.28s
DISP_CH=sniff ʱ����:1:500ms;2:1.28s
DISPLAY=0;

key=BTSTACK_BLE_LOCAL_NAME;
type=STR;
operate=EDIT;
len=29;
RANGE=;
tabname=btstack;
DISP_EN=bluetooth low Energy device name:max 29 bytes;
DISP_CH=�͹��������豸����:���29�ַ�;
DISPLAY=1;

key=BTSTACK_BLE_MANUFACTURE_ID;
type=DATA;
operate=EDIT;
len=;
RANGE=0~65535;
tabname=btstack;
DISP_EN=bluetooth low Energy Manufactur id:max 29 bytes;
DISP_CH=�͹�����������ID:0~0xFFFF;
DISPLAY=1;











tab=btmanager;
DISP_EN=btmanager property;
DISP_CH=��������������;
DISPLAY=1;

key=BTMANAGER_ENABLE_A2DP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:disable a2dp profile,1:enable a2dp_profile;
RANGE_DISP_CH=0:��֧��A2DP�߼���Ƶ�ַ�����,1:֧��A2DP�߼���Ƶ�ַ�����;
DISP_EN=CP:0-disbale,1-enable;
DISP_CH=CP:0-��֧��,1-֧��;
DISPLAY=0;

key=BTMANAGER_ENABLE_A2DP_CP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1,2;
tabname=btmanager;
RANGE_DISP_EN=0:not support a2dp,1:DTCP,2:SCMS-T;
RANGE_DISP_CH=0:��֧��A2DP,1:֧��DTCP,2:֧��SCMS-T;
DISP_EN=CP:0-not support,1-DTCP,2-SCMS-T;
DISP_CH=CP:0-��֧��,1-֧��DTCP,2-֧��SCMS-T;
DISPLAY=0;

key=BTMANAGER_ENABLE_AVDTP_AAC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:not support AAC,1:support AAC;
RANGE_DISP_CH=0:��֧��AAC��ʽ,1:֧��AAC��ʽ;
DISP_EN=support aac:0-disable,1-enable;
DISP_CH=֧��AAC��ʽ:0-�ر�,1-����;
DISPLAY=0;

key=BTMANAGER_ENABLE_AVRCP_VOLUME_SYNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support volume SYNC,1:support SYNC;
RANGE_DISP_CH=0:��֧������ͬ��,1:֧������ͬ��;
DISP_EN=enable avrcp1.4 absolute volume control:0 - disable,1 - enable;
DISP_CH=�Ƿ�ʹ��AVRCP1.4����ͬ������:0-�ر�,1-����;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support hfp profile,1:support hfp profile;
RANGE_DISP_CH=0:��֧��HFP,1:֧��HFP;
DISP_EN=support hfp profile:0 - disable,1 - enable;
DISP_CH=�Ƿ�ʹ��HFP:0-�ر�,1-����;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP_VOLUME_SYNC;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support HFP volume SYNC,1:support HFP volume SYNC;
RANGE_DISP_CH=0:��֧��ͨ������ͬ��,1:֧��ͨ������ͬ��;
DISP_EN=enable hfp absolute volume control:0 - disable,1 - enable;
DISP_CH=�Ƿ�ʹ��HFP����ͬ������:0-�ر�,1-����;
DISPLAY=1;

key=BTMANAGER_ENABLE_HFP_BAT_REPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support battery report,1:support battery report;
RANGE_DISP_CH=0:��֧��С�������ϱ�,1:֧��С�������ϱ�;
DISP_EN=battery report enable: 0 - disable,1 - enable;
DISP_CH=�Ƿ�ʹ�ܵ�ص�������:0-�ر�,1-����;
DISPLAY=1;

key=BTMANAGER_SUPPORT_THREE_WAY_CALL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support 3-way call,1:support 3-way call;
RANGE_DISP_CH=0:��֧������ͨ��,1:֧������ͨ��;
DISP_EN=support 3-way call: 0 - not support,1 - support;
DISP_CH=�Ƿ�֧������ͨ��:�Ƿ�֧��HFP����ͨ��,0��ʾ��֧�֣�1��ʾ֧��
DISPLAY=1;

key=BTMANAGER_ENABLE_HID;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:no support HID,1:support HID;
RANGE_DISP_CH=0:��֧��HID,1:֧��HID;
DISP_EN=support HID:0-disable,1-enable;
DISP_CH=֧��HID����:�Ƿ�֧��HID���ܣ�0-��֧��,1-֧��;
DISPLAY=1;

key=BTMANAGER_ENABLE_AUTO_CONNECT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btmanager;
RANGE_DISP_EN=0:not support auto connect,1:support auto connect;
RANGE_DISP_CH=0:��֧���Զ�����,1:֧���Զ�����;
DISP_EN=enable auto connection:0-disable,1-enable;
DISP_CH=�Զ���������:0-�ر�,1-����;
DISPLAY=1;

key=BTMANAGER_RECONNECT_ANOTHER_TIMES;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-20;
tabname=btmanager;
DISP_EN=bt engine auto connect another profile count:0 for all;
DISP_CH=��̨��������һ������Ĵ�������:0��ʾ������;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_TIMEOUT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-99;
tabname=btmanager;
DISP_EN=auto connect count:0-always;
DISP_CH=��ʱ�Ͽ����Զ���������:0��ʾһֱ������1~99��ʾ���޴�;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_STARTUP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=btmanager;
DISP_EN=bt engine auto connect count for startup reconnect:0 for all;
DISP_CH=������������:0��ʾ�����ƣ�1~15��ʾ���޴�;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_COUNT_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tabname=btmanager;
DISP_EN=bt engine auto connect count for mode switch reconnect:0 for all;
DISP_CH=��MODE������������������:0��ʾ�����ƣ�1~15��ʾ���޴�;
DISPLAY=0;

key=BTMANAGER_AUTO_CONNECT_INTERVAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-60;
tabname=btmanager;
DISP_EN=bt engine auto connect timer:uint 1s;
DISP_CH=�Զ�����ʱ����:��λ1s;
DISPLAY=0;

key=BTMANAGER_MAX_PAIRED_LIST_MEMBER;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-8;
tabname=btmanager;
DISP_EN=local device max paired list size:1-8;
DISP_CH=��������б������Ա����:Ŀǰ֧����ౣ��8���豸��Ϣ;
DISPLAY=1;

key=BTMANAGER_ENABLE_TWS;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0:not support tws,1:support tws;
RANGE_DISP_CH=0:��֧��,1:֧��;
DISP_EN=support tws: 0 - not support tws,1 - support tws;
DISP_CH=�Ƿ�֧��TWS����:0-��֧��,1-֧��;
DISPLAY=1;

key=BTMANAGER_VISUAL_MODE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tabname=btmanager;
RANGE_DISP_EN=0:always visible,1:invisible after paried,2always invisible;
RANGE_DISP_CH=0:��Զ�ɼ�,1:��Ժ󲻿ɼ�,2:��Զ���ɼ�
DISP_EN=support visula mode config:0:always visible,1:invisible after paried,2always invisible;
DISP_CH=�Ƿ�֧�����ÿɼ���:0:��Զ�ɼ�,1:��Ժ󲻿ɼ�,2:��Զ���ɼ�
DISPLAY=1;

key=BTMANAGER_ENABLE_SPP;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:��֧��SPP,1:֧��SPP;
DISP_EN=support SPP:0-disable,1-enable;
DISP_CH=֧��SPP����:0-��֧��,1-֧��;
DISPLAY=1;

key=BTMANAGER_ENABLE_BLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:��֧��BLE,1:֧��BLE;
DISP_EN=support BLE:0-disable,1-enable;
DISP_CH=֧��BLE����:0-��֧��,1-֧��;
DISPLAY=1;

key=BTMANAGER_BLE_PASSTH_PROFILE_SUPPORT;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btmanager;
RANGE_DISP_EN=0-disable,1-enable;
RANGE_DISP_CH=0:��֧��ACT ͸�� Profile,1:֧��ACT ͸�� Profile;
DISP_EN=support ACT Passthrough Profile:0-disable,1-enable;
DISP_CH=֧��ACT ͸�� Profile:0-��֧��,1-֧��;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_SRV_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile server uuid:36 bytes;
DISP_CH=Actions profile ����uuid:36���ַ�;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_SET_CHAR_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile set char uuid:36 bytes;
DISP_CH=Actions profile �趨����uuid:36���ַ�;
DISPLAY=1;

key=BTMANAGER_ACTION_PROFILE_GET_CHAR_UUID;
type=STR;
operate=EDIT;
len=36;
RANGE=;
tabname=btmanager;
DISP_EN=Actions profile server uuid:36 bytes;
DISP_CH=Actions profile ��ȡ����uuid:36���ַ�;
DISPLAY=1;

tab=btplay;
DISP_EN=btplay property;
DISP_CH=������������;
DISPLAY=0;

key=BTPLAY_SBC_PLAY_DELAY_START;
type=DATA;
operate=SINGLE;
len=;
RANGE=50-400;
step=1;
tabname=btplay;
DISP_EN=sbc play start delay:50ms to 450ms, space 1ms
DISP_CH=sbc������ʼ��ʱ:���䲥��ʱsbc������ʼ��ʱ����λΪms������Ϊ1ms
DISPLAY=0;

key=BTPLAY_SBC_PLAY_DELAY_START_TWS;
type=DATA;
operate=SINGLE;
len=;
RANGE=50-400;
step=1;
tabname=btplay;
DISP_EN=tws sbc play start delay:50ms to 450ms, space 1ms
DISP_CH=tws sbc������ʼ��ʱ:TWS����ʱsbc������ʼ��ʱ����λΪms������Ϊ1ms
DISPLAY=0;









tab=btcall;
DISP_EN=btcall property;
DISP_CH=������绰����;
DISPLAY=1;

key=BTCALL_FRONT_CHF_CAG_EXCH_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tabname=btcall;
RANGE_DISP_EN=0:not support CAG/CHF switch,1:support CAG/CHF switch;
RANGE_DISP_CH=0:��֧��CAG/CHF�л�,1:֧��CAG/CHF�л�;
DISP_EN=support chf & cag exchage:0-disable,1-enable;
DISP_CH=֧���л�������ֻ�ͨ��:0-��֧��,1-֧��,��NEXT/PREV�������л�;
MENU_INDEX=1;
DISPLAY=1;

key=BTCALL_IGNORE_UNCALLSCO;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-1;
tabname=btcall;
RANGE_DISP_EN=0:dont ignore uncall sco data,1:ignore uncall sco data;
RANGE_DISP_CH=0:�����˷�ͨ��sco����,1:���˷�ͨ��sco����;
DISP_EN=ignore the uncall sco data: 0, not ignore, 1,ignore;
DISP_CH=���˷�ͨ��״̬�µ�SCO����:0�������ˣ�1������;
MENU_INDEX=2;
DISPLAY=0;

tab_layer1=btcall_ring_parameter; 
tabname=btcall;
DISP_EN=btcall ring parameter;
DISP_CH=����������������;
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
RANGE_DISP_CH=0:������,1:���绰������,2:����+�绰������;
DISP_EN=callin hint mode:0-only ring,1-only phonebook,2-ring + phonebook;
DISP_CH=������ʾģʽ:0-������,1-���绰������,2-����+�绰������;
DISPLAY=1;

key=BTCALL_CALLIN_PB_SPACE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-30;
tab_layer1_name=btcall_ring_parameter;
tabname=btcall;
DISP_EN=phonebook ring space time:unit is 1S;
DISP_CH=�绰���������ʱ��:��1sΪ��λ������绰����ȴ���ʱ������ѭ������;
DISPLAY=1;

key=BTCALL_CALLIN_RING_SPACE_TIME;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-60;
tab_layer1_name=btcall_ring_parameter;
tabname=btcall;
DISP_EN=call in buildin ring space time:unit is 100ms;
DISP_CH=���������������ʱ��:��100msΪ��λ��������������������ȴ���ʱ������ѭ������;
DISPLAY=1;

tab_layer1=btcall_volume_control; 
tabname=btcall;
DISP_EN=btcall volume control;
DISP_CH=ͨ���������Ʋ�������;
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
DISP_CH=��绰micģ������:0-26db,1-30db,2-31.5db,3-33db,4-34.5db,5-36db,6-37.5db,7-39db
DISPLAY=1;

key=BTCALL_MICIN_ADC_GAIN;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-15;
tab_layer1_name=btcall_volume_control; 
tabname=btcall;
DISP_EN=call adc gain:unit 3dB, 0 ~ 15;
DISP_CH=��绰ADC����:��λ3dB����Χ0 ~ 15����ʾ0dB ~ 45dB;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=1-3;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
RANGE_DISP_EN=1:using right channel,2:using left channel,3:using dual channel;
RANGE_DISP_CH=1:���������,2:���������,3:˫�������;
DISP_EN=speaker channel enable:1-right channel,2-left channel,3-both channel;
DISP_CH=��绰ʱ��������ʹ��:1����������2����������3��˫��������˫�������ܻᵼ�»�������Ч����;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_WEAK_SEL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-2;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
RANGE_DISP_EN=0:no reduction,1:right reduction,2:left reduction;
RANGE_DISP_CH=0:��˥��,1:������˥��,2:������˥��;
DISP_EN=speaker channel weak selection:0-no weak, 1-right channel, 2-left channel;
DISP_CH=��绰ʱ��������˥��ѡ��:0-��˥����1-������˥����2-������˥��;
DISPLAY=1;

key=BTCALL_SPEAKER_CHANNEL_WEAK_VAL;
type=DATA;
operate=SINGLE;
len=;
RANGE=0-18;
tab_layer1_name=btcall_volume_control;
tabname=btcall;
DISP_EN=speaker channel weak value:0db ~ -18db;
DISP_CH=��绰ʱ��������˥������:0db ~ -18db;
DISPLAY=1;

tab_layer1=aec_parameters; 
tabname=btcall;
DISP_EN=aec parameters configuration;
DISP_CH=AEC��������;
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
RANGE_DISP_CH=0:�ر�AECģ��,1:ʹ��AECģ��;
DISP_EN=aec module enable:0-disable,1-enable;
DISP_CH=AECģ�鿪��:1���򿪣�0���ر�;
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
RANGE_DISP_CH=0:ȫ˫��,1:��˫��;
DISP_EN=aec half full mode:0 for full mode, 1 for half mode;
DISP_CH=����ȫ��˫��ģʽ:0��ʾȫ˫����1��ʾ��˫��;
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
DISP_CH=��������β��:��λ ms, 32ms ~ 96ms;
MENU_INDEX=3;
DISPLAY=1;

tab_layer2=aec_nonlinear_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec nonlinear processing parameters;
DISP_CH=AEC�����Դ����������;
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
RANGE_DISP_CH=0:�رշ�����ģ��,1:ʹ�ܷ�����ģ��;
DISP_EN=aec nlp enable:0-disable,1-enable;
DISP_CH=�����Դ�����ģ�鿪��:1���򿪣�0���ر�;
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
DISP_CH=�����Դ���̶�:����0~15,Խ���������Դ���Խ����;
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
DISP_CH=˫���������������̶�:����0~15��Խ���ʾ˫���ǽ�����������Խ�࣬���ǿ��ܻ���������Ļ��������Ҹ�����ֵ;
DISPLAY=1;

tab_layer2=aec_noise_reduction_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec noise reduction parameters;
DISP_CH=AEC���봦���������;
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
RANGE_DISP_CH=0:�رս�����ģ��,1:�򿪽�����ģ��;
DISP_EN=aec ans enable:0-disable,1-enable;
DISP_CH=���뿪��:1���򿪣�0���ر�;
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
DISP_CH=�������Ʊ�:��λdB��0~25;
DISPLAY=1;

tab_layer2=aec_echo_residual_parameters; 
tab_layer1_name=aec_parameters; 
tabname=btcall;
DISP_EN=aec echo residual parameters;
DISP_CH=AEC����������������;
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
RANGE_DISP_CH=0:�رղ�������������ģ��,1:�򿪲�������������ģ��;
DISP_EN=aec aes enable:0-disable,1-enable;
DISP_CH=��������������ģ�鿪��:1���򿪣�0���ر�;
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
DISP_CH=����������ģ�鵥���������Ʊ�:��λdB�� 0~40;
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
DISP_CH=����������ģ��˫���������Ʊ�:��λdB�� 0~40;
DISPLAY=1;


key=BTCALL_AEC_CNG_ENABLE;
type=DATA;
operate=SINGLE;
len=;
RANGE=0,1;
tab_layer1_name=aec_parameters;
tabname=btcall;
RANGE_DISP_EN=0:disable comfort noise,1:enable comfort noise;
RANGE_DISP_CH=0:�ر�����������ģ��,1:������������ģ��;
DISP_EN=aec cng enable:0-disable,1-enable;
DISP_CH=������������:1���򿪣�0���ر�;
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
DISP_CH=��������ˮƽ:��λ dB Fs, ��ֵ��ӦΪ��ֵ��һ��ӦС��-40 dB;
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
RANGE_DISP_CH=0:�ر�ǰ��AGC��ģ��,1:��ǰ��AGC��ģ��;
DISP_EN=agc pretreat enable:0-disable,1-enable;
DISP_CH=Ԥ����AGCʹ��:Ԥ�����Զ�������ȿ���ʹ�ܣ����ڷŴ�Է���������1���򿪣�0���ر�;
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
DISP_CH=Ԥ����AGCĿ��ֵ:Ԥ�����Զ�������ȿ���Ŀ��ֵ;
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
RANGE_DISP_CH=0:�رպ���AGC��ģ��,1:�򿪺���AGC��ģ��;
DISP_EN=aec postreat enable:0-disable,1-enable;
DISP_CH=���AGCʹ��:����Զ�������ȿ���ʹ�ܣ����ڷŴ��Լ���������1���򿪣�0���ر�;
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
DISP_CH=���AGCĿ��ֵ:����Զ�������ȿ���Ŀ��ֵ;
MENU_INDEX=53;
DISPLAY=1;

tab_layer1=btcall_dae_parameters; 
tabname=btcall;
DISP_EN=btcall dae parameters configuration;
DISP_CH=ͨ��PEQ��������;
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
RANGE_DISP_CH=0:����������Ч,1:�ر�������Ч;
DISP_EN=bypass mode:0-off,1-on;
DISP_CH=�Ƿ����DAE������Ч:0��ʾ�����ã�������DAE������Ч��1��ʾ���ã����ر�������Ч
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
RANGE_DISP_CH=0:�ر�,1:����;
DISP_EN=parameter eq option:0-disable,1-enable;
DISP_CH=PEQ����:0-�ر�,1-����;
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
DISP_CH=���PEQ����:���Ϊ0��ʾ��˲�����PEQ�������Ϊ14��ʾǰ�˲���PEQ���� ;
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
DISP_CH=PEQ BAND1 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK1_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band2 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND2 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK2_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band3 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND3 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK3_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band4 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND4 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK4_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band5 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band5 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK5_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band6 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band6 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK6_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band7 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND7 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK7_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band8 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ band8 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK8_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band9 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND9 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK9_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band10 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND10 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK10_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band11 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND11 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK11_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band12 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND12 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK12_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band13 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND13 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;

key=BTCALL_PEQ_BANK13_SETTING;
type=ARRAY;
operate=EDIT;
len=;
RANGE=;
tab_layer1_name=btcall_dae_parameters; 
tabname =btcall;
DISP_EN=peq band14 setting:[cutoff, Q value, gain, type]
DISP_CH=PEQ BAND14 ����:[����Ƶ�ʻ��ֹƵ�ʣ�Qֵ����λΪ0.1�����棬��λΪ0.1DB����Χ-120 ~ 120������ 1��ʾpeaking��2��ʾhigh pass��3��ʾlow pass��4��ʾlow shelf��5��ʾhigh shelf��������Ч]
DISPLAY=1;
