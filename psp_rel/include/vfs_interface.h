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
 * \file     vfs_interface.h
 * \brief    �����ļ�ϵͳ�ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __VFS_INTERFACE_H__
#define __VFS_INTERFACE_H__

#include <storage_device.h>
#include <vfs.h>
#include <attr.h>
#include <typeext.h>
#include <act_stdio.h>

//#define VC_6
//#define pc
#define   SUPPORT_FASR_SEEK 0  //�Ƿ�֧�ֿ���seek��򿪴��ļ�

#define FORMAT_FAT32                0
#define FORMAT_FAT16                1
#define FORMAT_EXFAT                2

//����CD����Ĳ�������
#define  CD_UP			'\\'    //�ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��Ŀ¼��ʼλ�ã�
#define  CD_BACK		'|'     //�ı䵱ǰĿ¼����һ����Ŀ¼��Ŀ¼��ָ��ָ��֮ǰCD��ȥ����Ŀ¼
#define  CD_ROOT		':'     //�ı䵱ǰĿ¼����Ŀ¼     
#define  CD_SUB			'/'     //�ı䵱ǰĿ¼����ǰĿ¼���Ӧ����Ŀ¼����ʱ����c������
#define  CD_HEAD		'.'     //�ı䵱ǰλ�õ�Ŀ¼��ʼλ��

//����DIR����Ĳ�������
#define  DIR_NEXT			0x80    //������
#define  DIR_PREV			0x81    //��ǰ����
#define  DIR_HEAD			0x82    //��Ŀ¼��������
#define  DIR_TAIL			0x83    //��Ŀ¼β��ǰ����

//����DIR ��չ����ӳ��
#define  EXT_BITMAP_BUFF            (0x80000000)
#define  EXT_NAME_MP3               (0x80000000>>1)
#define  EXT_NAME_WMA               (0x80000000>>2)
#define  EXT_NAME_WAV               (0x80000000>>3)
#define  EXT_NAME_AAC               (0x80000000>>4)
#define  EXT_NAME_FLAC              (0x80000000>>5)
#define  EXT_NAME_APE               (0x80000000>>6)
#define  EXT_NAME_AVI               (0x80000000>>7)
#define  EXT_NAME_TXT               (0x80000000>>8)
#define  EXT_NAME_JPG               (0x80000000>>9)
#define  EXT_NAME_JPEG              (0x80000000>>10)
#define  EXT_NAME_BMP               (0x80000000>>11)
#define  EXT_NAME_GIF               (0x80000000>>12)
#define  EXT_NAME_LRC               (0x80000000>>13)
#define  EXT_NAME_ASF               (0x80000000>>14)
#define  EXT_NAME_OGG               (0x80000000>>15)
#define  EXT_NAME_WMV               (0x80000000>>16)
#define  EXT_NAME_AMV               (0x80000000>>17)
#define  EXT_NAME_AAX               (0x80000000>>18)
#define  EXT_NAME_AA                (0x80000000>>19)
#define  EXT_NAME_MP1               (0x80000000>>20)
#define  EXT_NAME_MP2               (0x80000000>>21)
#define  EXT_NAME_M4A               (0x80000000>>22)
#define  EXT_NAME_POS               (0x80000000>>23)
#define  EXT_NAME_ACT               (0x80000000>>24)   /* ACT is added by fix FS_ROM rom_ext_name.  2013/3/13 14:42:24. */
#define  EXT_NAME_BMK               (0x80000000>>25)
#define  EXT_NAME_BIN               (0x80000000>>26)
#define  EXT_NAME_APK               (0x80000000>>27)
#define  EXT_NAME_PCM               (0x80000000>>28)
#define  EXT_NAME_ALL               (0x80000000>>29)
#define  EXT_NAME_ALL_FILE          (0x80000000>>30)
#define  EXT_NAME_ALL_DIR           (0x80000000>>31)



//����OPEN�ļ��ķ�ʽ
#define  R_NORMAL_SEEK		0x00    //��ͨseekģʽ
#define  R_FAST_SEEK		0x01    //����seekģʽ(R_FAST_SEEKÿ�δ򿪺���Ҫ��seek���ļ�ĩβ)
#define  OPEN_MODIFY		0x02    //�޸�ģʽ���������Խ�ļ���Сд��
#define  LARGE_FILE_SEEK	0x03    //���ļ�seekģʽ�����ļ��򿪿��ܽ�����
#define  OPEN_RECOVER		0x04    //�ָ�ģʽ������ָ�д��û������close���ļ�,��Ҫ����¼������ָ���
#define  REC_RECOVER        0x05    //WAV¼���ָ�ģʽ(������¼��Ӧ�ã���OPEN_RECOVER��ȣ���ģʽ��������¼���ָ���������ض�LBA��ȡ¼���ļ����ȣ�����Ӧ������ʹ��)

//�����ļ���Ŀ¼����������
#define  FS_DIR                     (0x00)
#define  FS_FILE                    (0x01)

//����ʱ�����������
#define  FS_TIME_CREATE             (0X00)
#define  FS_TIME_MODIFY             (0x01)

//����Ŀ¼����ҵķ�������
#define   NEXT_ITEM      'n'  //��һ��Ŀ¼��
#define   PREV_ITEM      'p'  //��һ��Ŀ¼��
#define   INVALIB_ITEM   'i'  //��ЧĿ¼��
#define   SEC_ITEM       's'  //�ڶ���Ŀ¼�ֻ��exfat


//�����ȡ�����ռ������
#define  GET_TOTAL_SPACE            (0x00)
#define  GET_SPARE_SPACE            (0x01)

//�����ļ�����
#define         ATTR_NORMAL         (0x00)      // normal file
#define         ATTR_READONLY       (0x01)      // file is readonly
#define         ATTR_HIDDEN         (0x02)      // file is hidden
#define         ATTR_SYSTEM         (0x04)      // file is a system file
#define         ATTR_VOLUME         (0x08)      // entry is a volume label
#define         ATTR_LONG_FILENAME  (0x0f)      // this is a long filename entry
#define         ATTR_DIRECTORY      (0x10)      // entry is a directory name
#define         ATTR_ARCHIVE        (0x20)      // file is new or modified
//�������֧��Ŀ¼�㼶
#define  MAX_DIR_LAYER              (9)

#define  LONG_NAME_TAG              (0xfeff)
#define  MOUNTS_USED_MASTER   0X7F

typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
    uint8 hour;
    uint8 minute;
    uint8 second;
} file_time_t;

//����Ŀ¼�㼶�ṹ���ڴ�ռ�����
typedef struct
{
    uint8 buffer[MAX_DIR_LAYER * 8 + 1];
} pdir_layer_t;

typedef struct
{
    uint8 buffer[MAX_DIR_LAYER * 8 + 2];
} pdir_layer_ext_t;
typedef struct
{
    uint8 buffer[8];
} pfile_offset_t;

typedef struct
{
     //��ǰĿ¼����ʼ��Ŀ��
    uint32 begin_index;    
    //��ǰĿ¼�ĸ�Ŀ¼�Ĵغ�
    uint32 far_cluster;
    //��ǰĿ¼�ĸ�Ŀ¼��ƫ��
    uint32 far_direntry;
    //��ǰ�ļ���Ŀ¼�����ڴغ�
    uint32 cur_cluster;
    //��ǰ�ļ���ƫ��
    uint32 cur_direntry;    
    //��ǰ�ṹ����һ�������ڵ�ƫ����
    uint8 store_index;
    //��ǰ
    uint8 cur_sector;
    //��ǰĿ¼�Ĳ㼶
    uint8 cur_layer;
    uint8 reserve[9];
} list_dir_info_t;


//�����ļ�ϵͳ��������
#define  ERR_PHY_R                  (0x02)      //���̶�����
#define  ERR_PHY_W                  (0x03)      //����д����
#define  ERR_NO_FORMAT              (0x04)      //����δ��ʽ��
#define  ERR_WRITE_PROTECTED        (0x05)      //����д����
#define  ERR_FILE_OVER_OFFSET       (0x06)      //�ļ����������ļ��߽�
#define  ERR_DIR_OVER_OFFSET        (0x07)      //Ŀ¼��������Ŀ¼�߽�
#define  ERR_FILE_NO_EXIST          (0x08)      //�ļ�������
#define  ERR_DIR_NO_EXIST           (0x09)      //Ŀ¼������
#define  ERR_FILE_OP_SPACE          (0x10)      //��ʾ�ļ�����ʱû�д��̿ռ�,����д����
#define  ERR_DIR_OP_SPACE           (0x11)      //Ŀ¼����ʱû�д��̿ռ�,������չĿ¼,�½���Ŀ¼
#define  ERR_FILE_OP_ROOT_FULL      (0x12)      //Ŀ¼������
#define  ERR_DEL_DIR_NO_EMPTY       (0x13)      //ɾ��Ŀ¼ʱ����,��ʾɾ����Ŀ¼�ǿ�
#define  ERR_DIRENTRY_ATTR          (0x14)      //Ŀ¼�����Գ���
#define  ERR_DIR_TYPE               (0x15)
#define  ERR_FHANDLE_FULL           (0x16)
#define  ERR_DIR_EXIST              (0x17)      //Ŀ¼�Ѵ���(����Ŀ¼ʱ)
#define  ERR_FILE_EXIST             (0x18)      //�ļ��Ѵ��ڣ������ļ�ʱ��
#define  ERR_VAILD_PARA             (0x19)      //�Ƿ��Ĳ���

/*�����ļ�����ļ�ʱ�Ĵ�������*/
#define  ERR_HANDLE_USEUP           (0x1a)      //�������
#define  ERR_NO_EMPTY_DIRENTRY      (0X1B)      //û�п�Ŀ¼��
#define  ERR_FILENAME_USED_BADCHARS (0X1c)      //�ļ������в�֧�ֵ��ַ�
#define  ERR_CREAT_SHORT_NAME       (0X1D)      //��������ʧ��
#define  ERR_BUILD_CLUSTER_CHAIN    (0X1E)      //��ȡ�մأ�����fat��ʧ��
#define  ERR_SET_DIRENTRY           (0X1F)      //����Ŀ¼�����
#define  ERR_GET_DIRENTRY           (0X20)      //��ȡĿ¼�����
#define  ERR_NO_EMPTY_CLUSTER       (0X21)      //�Ҳ����մ�
#define  ERR_INVADLE_EMPTY_CLUSTER  (0X21)    //�ҵ��Ŀմ���Ч

#define  ERR_LBA_OVERFLOW            0X30      //LBA��ַ���
#define  ERR_PHY_UPDATA              0x31       //���̸���ʧ��
#define  ERR_PARAM_IN                0X32      //�������Ĳ�������
typedef enum
{
    VFS_CD = 0,
    VFS_DIR,
    VFS_GET_NAME,
    VFS_FILE_DIR_OFFSET,
    VFS_MAKE_DIR,

    VFS_FILE_OPEN,
    VFS_FILE_CREATE,
    VFS_FILE_CLOSE,
    VFS_FILE_DIR_REMOVE,
    VFS_FILE_GET_SIZE,

    VFS_GET_TIME,

    VFS_FILE_SEEK,
    VFS_FILE_TELL,
    VFS_FILE_READ,
    VFS_FILE_WRITE,

    VFS_GET_SPACE,
    VFS_GET_ERR_INFO,

    VFS_FILE_DIR_EXIST,
    VFS_FILE_ATTRALTER,
    VFS_FILE_RENAME,

    VFS_CREATEVOLUME,
    VFS_SET_TIME,

    VFS_CLEAR_PRIVATE_DATA,

    VFS_FILE_MOVE,
    VFS_REC_FILE_UPDATE,
    VFS_REC_FILE_RECOVER,
} vfs_cmd_e;



#ifdef pc
extern uint32 vfs_entry[];
#define FS_ENTRY_ADDR(x)  (uint32)(vfs_entry+ x)
#else
#define FS_ENTRY_ADDR(x)  (VFS_ENTRY_VA_START + (4 * x))
#endif

/*
 libc�ṩ�ĺ�����ָ����ʽ�������������
*/
#define VFS_CD_ENTRY                        (*((uint32*)(FS_ENTRY_ADDR(VFS_CD))))
#define VFS_DIR_ENTRY                       (*((uint32*)(FS_ENTRY_ADDR(VFS_DIR))))
#define VFS_GET_NAME_ENTRY                  (*((uint32*)(FS_ENTRY_ADDR(VFS_GET_NAME))))
#define VFS_FILE_DIR_OFFSET_ENTRY           (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_DIR_OFFSET))))
#define VFS_MAKE_DIR_ENTRY                  (*((uint32*)(FS_ENTRY_ADDR(VFS_MAKE_DIR))))

#define VFS_FILE_OPEN_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_OPEN))))
#define VFS_FILE_CREATE_ENTRY               (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_CREATE))))
#define VFS_FILE_CLOSE_ENTRY                (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_CLOSE))))
#define VFS_FILE_DIR_REMOVE_ENTRY           (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_DIR_REMOVE))))
#define VFS_FILE_GET_SIZE_ENTRY             (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_GET_SIZE))))

#define VFS_GET_TIME_ENTRY                  (*((uint32*)(FS_ENTRY_ADDR(VFS_GET_TIME))))

#define VFS_FILE_SEEK_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_SEEK))))
#define VFS_FILE_TELL_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_TELL))))
#define VFS_FILE_READ_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_READ))))
#define VFS_FILE_WRITE_ENTRY                (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_WRITE))))

#define VFS_GET_SPACE_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_GET_SPACE))))
#define VFS_GET_ERR_INFO_ENTRY              (*((uint32*)(FS_ENTRY_ADDR(VFS_GET_ERR_INFO))))

#define VFS_FILE_DIR_EXIST_ENTRY            (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_DIR_EXIST))))
#define VFS_FILE_ATTRALTER_ENTRY            (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_ATTRALTER))))
#define VFS_FILE_RENAME_ENTRY               (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_RENAME))))

#define VFS_CREATE_VOLUME_ENTRY             (*((uint32*)(FS_ENTRY_ADDR(VFS_CREATEVOLUME))))
#define VFS_SET_TIME_ENTRY                  (*((uint32*)(FS_ENTRY_ADDR(VFS_SET_TIME))))

#define VFS_CLEAR_PRIVATE_DATA_ENTRY        (*((uint32*)(FS_ENTRY_ADDR(VFS_CLEAR_PRIVATE_DATA))))

#define VFS_FILE_MOVE_ENTRY                 (*((uint32*)(FS_ENTRY_ADDR(VFS_FILE_MOVE))))
#define VFS_REC_FILE_UPDATE_ENTRY           (*((uint32*)(FS_ENTRY_ADDR(VFS_REC_FILE_UPDATE))))
#define VFS_REC_FILE_RECOVER_ENTRY           (*((uint32*)(FS_ENTRY_ADDR(VFS_REC_FILE_RECOVER))))







/*
 libc�������ýӿ�,ͨ������ָ�뷽ʽ����
*/

/*
bool vfs_cd(uint32 vfs_mount, uint8 mode, uint8* input_name)
���������������û����뽫��ǰĿ¼ָ��ǰĿ¼����Ŀ¼����Ŀ¼��ֱ�ӷ��ظ�Ŀ¼
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
mode��cd�Ĳ����������ͣ��������������vfs_interface.h  ��//����CD����Ĳ�������
input_name����Ŀ¼����bufferָ�룻ΪNULL��ʱ��Ĭ��Ϊ���뵱ǰĿ¼��ǰ�����ַ�Ϊ"0xfffe"���ʾͨ�������л�Ŀ¼
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_cd(a,b,c)                       ((bool(*)(uint32 , uint8 , uint8* ))VFS_CD_ENTRY)((a), (b), (c))

/*
uint32 vfs_dir(uint32 vfs_mount, uint8 mode, uint8* input_name,uint32 ext_name_bitmap)
�����������ڵ�ǰĿ¼�����ַ�ʽ����Ŀ¼���ļ�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
mode: ���������ʽ���������������vfs_interface.h ��//����DIR����Ĳ�������
input_name��������������ļ�����bufferָ�롣��ָ��ΪNULL����ֱ�Ӵӵ�ǰĿ¼�ʼdir��������
ext_name_bitmap��������Ч�ڴ��ַ�����ʾ������չ���ַ����ĵ�ַ���粻�����ʾҪdir��bitmap���˲��������λΪ1��ʾ������ڴ��ַ����������bitmapֵ
		 �����bitmap��Ӧ�����������vfs_interface.h ��//����DIR ��չ����ӳ��
����ֵ��0-ʧ�ܣ���0-�ɹ�
*/
#define vfs_dir(a,b,c,d)                    ((uint32(*)(uint32 , uint8 , uint8* , uint32 ))VFS_DIR_ENTRY)((a), (b), (c), (d))

/*
uint16 vfs_get_name(uint32 vfs_mount, uint8* output_name, uint16 longname_length)
����������ȡ��ǰ���ļ��������ȷ��س�����û�г����򷵻ض��������׺��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
output_name�������������ļ�����bufferָ��
longname_length������ΪҪ��ȡ�ĳ����ַ���������������Ǻͽ�����������Ϊ0��ʾ��ȡ��׺��
����ֵ��0-ʧ�ܣ���0-ʵ�ʷ��صĻ�ȡ�����ļ����ַ�����Ϊȡ����ʱ����ʵ�ʺ�������ĳ����ַ��ĸ���������������Ǻͽ�����0x0000����Ϊ����ʱ����11����ȡ��׺��ʱ����3
*/
#define vfs_get_name(a,b,c)                 ((uint16(*)(uint32 , uint8* , uint16 ))VFS_GET_NAME_ENTRY)((a), (b), (c))

/*
bool vfs_file_dir_offset(uint32 vfs_mount, pdir_layer_t* ptr_layer, pfile_offset_t* ptr_file, uint8 mode)
������������ȡ�����õ�ǰĿ¼���λ����Ϣ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_layer: �洢Ҫ��ȡ�����õ�Ŀ¼�㼶��Ϣbufferָ�룻ΪNULL��ʾ��ǰĿ¼�����Ϣ
ptr_file ���洢Ҫ��ȡ�����õ��ļ�λ����Ϣbufferָ��
mode     ��������ʽ��0-��ʾ��ȡ��1-��ʾ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_dir_offset(a,b,c,d)        ((bool(*)(uint32 , pdir_layer_t* , pfile_offset_t* , uint8 ))VFS_FILE_DIR_OFFSET_ENTRY)((a), (b), (c), (d))

/*
bool vfs_make_dir(uint32 vfs_mount, uint8* input_name)
�����������ڵ�ǰĿ¼������һ���û�����ָ��Ŀ¼������Ŀ¼��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: Ҫ������Ŀ¼��ָ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_make_dir(a,b)                   ((bool(*)(uint32 , uint8* ))VFS_MAKE_DIR_ENTRY)((a), (b))

/*
uint32 vfs_file_open(uint32 vfs_mount, uint8* input_name,uint8 mode)
���������������û�������ļ����ڵ�ǰĿ¼�д�һ���Ѵ��ڵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: �����ļ����ļ���������ָ�룻Ϊnull��ʾֱ�Ӵ򿪵�ǰĿ¼��ָ����ļ�
mode ��    �򿪷�ʽ���������vfs_interface.h ��//����OPEN�ļ��ķ�ʽ
����ֵ��0-ʧ�ܣ���0-�򿪵��ļ����
*/
#define vfs_file_open(a,b,c)                ((uint32(*)(uint32 , uint8* , uint8 ))VFS_FILE_OPEN_ENTRY)((a), (b), (c))

/*
uint32 vfs_file_create(uint32 vfs_mount, uint8* input_name,uint32 lentgh)
���������������û�������ļ�������һ���ļ�,�û��ɻ�õ�ǰ�����ļ��ľ����
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: Ϊ��Ҫ�������ļ����ļ����ַ���ָ��
lentgh ��   �������ļ��ĳ��ȣ���λΪbyte,����512byte��512���㣩��Ϊ0��ʾ�������ļ�
����ֵ��0-ʧ�ܣ���0-�������ļ����
*/
#define vfs_file_create(a,b,c)              ((uint32(*)(uint32 , uint8* , uint32 ))VFS_FILE_CREATE_ENTRY)((a), (b), (c))

/*
bool vfs_file_close(uint32 vfs_mount, uint32 fhandle)
�����������ر��ļ�,�û�������Ҫ�رյ��ļ������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
fhandle: ��Ҫ�رյ��ļ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
˵����fhandle��vfs_file_open��vfs_file_create���صľ��
*/
#define vfs_file_close(a,b)                 ((bool(*)(uint32 , uint32 ))VFS_FILE_CLOSE_ENTRY)((a), (b))

/*
bool vfs_file_dir_remove(uint32 vfs_mount, uint8* input_name, uint8 type)
�����������ڵ�ǰĿ¼��ɾ��һ���û�����ָ��Ŀ¼���ļ���ɾ��Ŀ¼ʱҪ��Ŀ¼Ϊ�ա�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: Ҫɾ����Ŀ¼���ļ�������Ϊnull��ֱ��ɾ����ǰĿ¼��ָ���Ŀ¼���ļ���
type  ��Ҫ���������ͣ�0-ɾ��Ŀ¼��1-ɾ���ļ�
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_dir_remove(a,b,c)          ((bool(*)(uint32 , uint8*,uint8))VFS_FILE_DIR_REMOVE_ENTRY)((a), (b), (c))

/*
bool vfs_file_get_size(uint32 vfs_mount, uint32* output_length, uint8* ptr_file, uint8 mode)
����������ȡ�ļ��ĳ��ȣ��ֽ�Ϊ��λ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
output_length: ���ص��ļ����ȣ����ֽ�Ϊ��λ��
ptr_file�� ����ȡ���ȵ��ļ�������ļ�������ָ��
mode  ����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ�
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_get_size(a,b,c,d)          ((bool(*)(uint32 , uint32* , uint8* , uint8 ))VFS_FILE_GET_SIZE_ENTRY)((a), (b), (c), (d))


/*
bool vfs_get_time(uint32 vfs_mount, file_time_t* output_time, uint8* input_name, uint8 type)
������������ȡ�ļ��Ĵ���ʱ����޸�ʱ��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
output_time:  ʱ�����bufָ��
input_name�� ����ȡʱ����ļ����ļ�������ָ�룻Ϊnull��ʾ��ǰĿ¼��ָ����ļ�
type  ��FS_TIME_CREATE-��ȡ����ʱ�䣻FS_TIME_MODIFY-��ȡ�޸�ʱ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_get_time(a,b,c,d)               ((bool(*)(uint32 , file_time_t* , uint8* , uint8 ))VFS_GET_TIME_ENTRY)((a), (b), (c), (d))


/*
bool vfs_file_seek(uint32 vfs_mount, int32 offset, uint8 type, uint32 fhandle)
������������λ�ļ����ֽ�ƫ�ƣ�ʵ�֣�
	a.��������ļ��׵�ƫ������ʵ���û������˳������������
	b.ʵ�ִӵ�ǰλ�����ļ��׻��ļ�βƫ�ƣ�ʵ���û������˳������������
	c.��������ļ��׵�ƫ������ʵ���û�����Ķ�ָ��λ�����ݵ��޸ġ�
	d.���read֧���û�����˳������ݣ���������Լ������λ�����ݣ�������Է���ʵ�ֻ�д�޸��Ѿ����ɵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
offset:  ��ӦSEEKƫ��������Χ��2GB
type�� ��ӦSEEK���ͣ��������vfs_interface.h ��//�����ļ�SEEK�ķ�ʽ
fhandle  ���Ѵ򿪵��ļ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_seek(a,b,c,d)              ((bool(*)(uint32 , int32 , uint8 , uint32 ))VFS_FILE_SEEK_ENTRY)((a), (b), (c), (d))

/*
bool vfs_file_tell(uint32 vfs_mount, uint32* ptr_offset ,uint32 fhandle)
����������ȡ��ǰ��д������ָ�룬ָ����ָ����ļ�ͷ���ֽ�ƫ������������ʱ�û����øú�����¼AB�㣬���seek��readʵ�����ݵ�AB��ȡ��
	  д����ʱ��֧���û������޸������ɵ��ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_offset:  �ļ���ǰ��дλ������ļ�ͷ��ƫ����
fhandle  ���Ѵ򿪵��ļ����
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_tell(a,b,c)                ((bool(*)(uint32 , uint32* , uint32 ))VFS_FILE_TELL_ENTRY)((a), (b), (c))

/*
uint32 vfs_file_read(uint32 vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
�������������ļ��ĵ�ǰλ�ö�ȡһ����С������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_data_buffer:  �������������buffer��ָ�룬buffer�Ĵ�Сһ������С�ں���ָ����byte_count
byte_count�������ֽ���
fhandle  ���Ѵ򿪵��ļ����
����ֵ��0-ʧ�ܣ���0-����ʵ�ʶ������ֽ���
*/
#define vfs_file_read(a,b,c,d)              ((uint32(*)(uint32 , uint8* , uint32 , uint32 ))VFS_FILE_READ_ENTRY)((a), (b), (c), (d))


/*
uint32 vfs_file_write(uint32 vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
�������������ļ��ĵ�ǰλ�ö�ȡһ����С������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_data_buffer:  д����buffer��ָ��
byte_count��д���ֽ���
fhandle  ���Ѵ򿪵��ļ����
����ֵ��0-ʧ�ܣ���0-����ʵ��д���ֽ���
*/
#define vfs_file_write(a,b,c,d)             ((uint32(*)(uint32 , uint8* , uint32 , uint32 ))VFS_FILE_WRITE_ENTRY)((a), (b), (c), (d))

/*
bool vfs_get_space(uint32 vfs_mount, uint32* ptr_sector_count, uint8 type)
������������ȡ���̷����ռ䣬�������������ͬѡ��Ҫ�󷵻ش��̷����ܵ�����������ʣ��������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_sector_count:  ���ط�����������
type��0-��ʾ���ý����ر�ʾ��ǰ���̷����ܿռ����������1-��ʾ���ص�ǰ���̷���ʣ��ռ��������
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_get_space(a,b,c)                ((bool(*)(uint32 , uint32* , uint8 ))VFS_GET_SPACE_ENTRY)((a), (b), (c))

/*
uint8 vfs_get_err_info(uint32 vfs_mount)
������������ȡ�ļ�ϵͳ������Ϣ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
����ֵ��0-û�д��󣻷�0-������Ϣ���������vfs_interface.h ��//�����ļ�ϵͳ��������
*/
#define vfs_get_err_info(a)                 ((uint8(*)(uint32))VFS_GET_ERR_INFO_ENTRY)(a)

/*
uint32 vfs_file_dir_exist(uint32 vfs_mount, uint8* input_name, uint8 type)
�����������жϵ�ǰĿ¼�Ƿ���ָ������Ŀ¼���ļ�
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name:Ҫ�жϵ��ļ���Ŀ¼��ָ��
type    :Ҫ�жϵ����ͣ�0-Ŀ¼��1-�ļ�
����ֵ��0-�����ڣ���0-���ڣ�ǰ���ظ��ļ���Ŀ¼���ڵ�Ŀ¼���״غ�
*/
#define vfs_file_dir_exist(a,b,c)           ((uint32(*)(uint32 , uint8* , uint8 ))VFS_FILE_DIR_EXIST_ENTRY)((a), (b), (c))


/*
uint8 vfs_file_attralter(uint32 vfs_mount, uint8 attr, uint8* ptr_file, uint8 mode)
������������ȡ���޸��ļ�������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
attr:0-��ȡ�ļ������ԣ���0-���óɵ��ļ����ԣ��������vfs_interface.h ��105���µĶ���
ptr_file    :����ȡ���޸����Ե��ļ�������ļ�������ָ�룻
mode����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ���Ŀ¼��
����ֵ��0-ʧ�ܣ���0-����Ҫ��ȡ�����õ�����ֵ
*/
#define vfs_file_attralter(a,b,c,d)         ((uint8(*)(uint32 , uint8 , uint8* , uint8 ))VFS_FILE_ATTRALTER_ENTRY)((a), (b), (c), (d))

/*
bool vfs_file_rename(uint32 vfs_mount, uint8* ptr_new_name, uint8* ptr_file, uint8 mode)
������������ȡ���޸��ļ�������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_new_name:���ļ����ַ���ָ��
ptr_file    :�����������ļ�������ļ���ָ�룻
mode����ʾ����ptr_file�����塣0-��ʾptr_fileΪ�ļ������1-��ʾptr_fileΪ�ļ���ָ��;ptr_fileΪnull��ʾ��ǰĿ¼��ָ����ļ���Ŀ¼
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_rename(a,b,c,d)            ((bool(*)(uint32 , uint8* , uint8* , uint8 ))VFS_FILE_RENAME_ENTRY)((a), (b), (c), (d))

/*
bool vfs_create_volume(uint32 vfs_mount, uint8* input_name)
�����������������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: ������ַ���ָ��;input_name=NULL��ʾ��ѯ�Ƿ���ھ�꣬input_name=��NULLֱ���޸ľ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_create_volume(a,b)              ((bool(*)(uint32 , uint8* ))VFS_CREATE_VOLUME_ENTRY)((a), (b))

/*
bool vfs_set_time(uint32 vfs_mount, file_time_t* ptr_input_time, uint8* input_name, uint8 type)
�����������������
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
ptr_input_time����Ҫ���õ�ʱ������bufָ��
input_name: ������ʱ����ļ����ļ�������ָ�룻Ϊnull��ʾǰĿ¼��ָ����ļ�
type��FS_TIME_CREATE-���ô���ʱ��;FS_TIME_MODIFY-�����޸�ʱ��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_set_time(a,b,c,d)               ((bool(*)(uint32 , file_time_t* , uint8* , uint8 ))VFS_SET_TIME_ENTRY)((a), (b), (c), (d))

#define vfs_clear_private_data(a)           ((uint8(*)(uint32 p_vfs_mount))VFS_CLEAR_PRIVATE_DATA_ENTRY)(a)

/*
bool vfs_file_move(uint32 vfs_mount, void *fat_direntry, uint8 *ptr_file_name, uint8 mode)
����������ɾ��������Ŀ¼��
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
fat_direntry��Ŀ¼����Ϣָ��
ptr_file_name: �����ӻ�ɾ����Ŀ¼���ַ���ָ�룻��mode==0ʱ�����Ϊnull��ʾǰĿ¼��ָ����ļ�;���������Ч
mode��0-ɾ��Ŀ¼�1-����Ŀ¼��
����ֵ��0-ʧ�ܣ�1-�ɹ�
*/
#define vfs_file_move(a,b,c,d)              ((bool(*)(uint32 , void* , uint8* , uint8 ))VFS_FILE_MOVE_ENTRY)((a), (b), (c), (d))

/*
uint32 vfs_rec_file_update(uint32 vfs_mount, void *wav_head_buf, uint32 fhandle)
��������������¼���и���¼���ļ�WAVͷ����Ϣ
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
wav_head_buf��WAV�ļ�ͷ������
buf_len: �������ݳ��ȣ��̶�Ϊ512
fhandle����Ҫ�������ļ����
����ֵ���ļ����״غ�
*/
#define vfs_rec_file_update(a,b,c)              ((uint32(*)(uint32 , void* , uint32 ))VFS_REC_FILE_UPDATE_ENTRY)((a), (b), (c))

/*
uint32 vfs_rec_file_recover(uint32 vfs_mount, uint8* input_name,uint32 start_cluster)
���������������û������¼���ļ����ָ����ڵ����δ�����¼���ļ���
����˵����
vfs_mount�������ļ�ϵͳ�󷵻صľ��,����ΪNULL;
input_name: �����ļ����ļ���������ָ�룻Ϊnull��ʾֱ�Ӵ򿪵�ǰĿ¼��ָ����ļ�
start_cluster ���ļ�����е��״���Ϣ���ؼ�����
����ֵ��0-ʧ�ܣ���0-�򿪵��ļ����
*/
#define vfs_rec_file_recover(a,b,c)                ((uint32(*)(uint32 , uint8* , uint32 ))VFS_REC_FILE_RECOVER_ENTRY)((a), (b), (c))


#endif
