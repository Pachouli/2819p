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
 * \brief    虚拟文件系统接口
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
#define   SUPPORT_FASR_SEEK 0  //是否支持快速seek或打开大文件

#define FORMAT_FAT32                0
#define FORMAT_FAT16                1
#define FORMAT_EXFAT                2

//定义CD命令的操作类型
#define  CD_UP			'\\'    //改变当前目录到上一级父目录，目录项指针指向目录起始位置；
#define  CD_BACK		'|'     //改变当前目录到上一级父目录，目录项指针指向之前CD进去的子目录
#define  CD_ROOT		':'     //改变当前目录到根目录     
#define  CD_SUB			'/'     //改变当前目录到当前目录项对应的子目录，此时参数c起作用
#define  CD_HEAD		'.'     //改变当前位置到目录起始位置

//定义DIR命令的操作类型
#define  DIR_NEXT			0x80    //向后检索
#define  DIR_PREV			0x81    //向前检索
#define  DIR_HEAD			0x82    //从目录首向后检索
#define  DIR_TAIL			0x83    //从目录尾向前检索

//定义DIR 扩展名的映射
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



//定义OPEN文件的方式
#define  R_NORMAL_SEEK		0x00    //普通seek模式
#define  R_FAST_SEEK		0x01    //快速seek模式(R_FAST_SEEK每次打开后需要先seek到文件末尾)
#define  OPEN_MODIFY		0x02    //修改模式（不允许跨越文件大小写）
#define  LARGE_FILE_SEEK	0x03    //大文件seek模式（大文件打开可能较慢）
#define  OPEN_RECOVER		0x04    //恢复模式（允许恢复写后没有正常close的文件,主要用于录音掉电恢复）
#define  REC_RECOVER        0x05    //WAV录音恢复模式(仅用于录音应用，与OPEN_RECOVER相比，此模式除了用于录音恢复，还会从特定LBA获取录音文件长度，其他应用请勿使用)

//定义文件及目录操作的类型
#define  FS_DIR                     (0x00)
#define  FS_FILE                    (0x01)

//定义时间操作的类型
#define  FS_TIME_CREATE             (0X00)
#define  FS_TIME_MODIFY             (0x01)

//定义目录项查找的方向类型
#define   NEXT_ITEM      'n'  //下一个目录项
#define   PREV_ITEM      'p'  //上一个目录项
#define   INVALIB_ITEM   'i'  //无效目录项
#define   SEC_ITEM       's'  //第二个目录项，只对exfat


//定义获取分区空间的类型
#define  GET_TOTAL_SPACE            (0x00)
#define  GET_SPARE_SPACE            (0x01)

//定义文件属性
#define         ATTR_NORMAL         (0x00)      // normal file
#define         ATTR_READONLY       (0x01)      // file is readonly
#define         ATTR_HIDDEN         (0x02)      // file is hidden
#define         ATTR_SYSTEM         (0x04)      // file is a system file
#define         ATTR_VOLUME         (0x08)      // entry is a volume label
#define         ATTR_LONG_FILENAME  (0x0f)      // this is a long filename entry
#define         ATTR_DIRECTORY      (0x10)      // entry is a directory name
#define         ATTR_ARCHIVE        (0x20)      // file is new or modified
//定义最大支持目录层级
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

//定义目录层级结构的内存占用情况
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
     //当前目录的起始曲目号
    uint32 begin_index;    
    //当前目录的父目录的簇号
    uint32 far_cluster;
    //当前目录的父目录的偏移
    uint32 far_direntry;
    //当前文件的目录项所在簇号
    uint32 cur_cluster;
    //当前文件的偏移
    uint32 cur_direntry;    
    //当前结构体在一个扇区内的偏移量
    uint8 store_index;
    //当前
    uint8 cur_sector;
    //当前目录的层级
    uint8 cur_layer;
    uint8 reserve[9];
} list_dir_info_t;


//定义文件系统错误类型
#define  ERR_PHY_R                  (0x02)      //磁盘读错误
#define  ERR_PHY_W                  (0x03)      //磁盘写错误
#define  ERR_NO_FORMAT              (0x04)      //磁盘未格式化
#define  ERR_WRITE_PROTECTED        (0x05)      //磁盘写保护
#define  ERR_FILE_OVER_OFFSET       (0x06)      //文件操作超出文件边界
#define  ERR_DIR_OVER_OFFSET        (0x07)      //目录操作超出目录边界
#define  ERR_FILE_NO_EXIST          (0x08)      //文件不存在
#define  ERR_DIR_NO_EXIST           (0x09)      //目录不存在
#define  ERR_FILE_OP_SPACE          (0x10)      //表示文件操作时没有磁盘空间,不能写数据
#define  ERR_DIR_OP_SPACE           (0x11)      //目录操作时没有磁盘空间,不能扩展目录,新建子目录
#define  ERR_FILE_OP_ROOT_FULL      (0x12)      //目录不存在
#define  ERR_DEL_DIR_NO_EMPTY       (0x13)      //删除目录时返回,表示删除的目录非空
#define  ERR_DIRENTRY_ATTR          (0x14)      //目录项属性出错
#define  ERR_DIR_TYPE               (0x15)
#define  ERR_FHANDLE_FULL           (0x16)
#define  ERR_DIR_EXIST              (0x17)      //目录已存在(建立目录时)
#define  ERR_FILE_EXIST             (0x18)      //文件已存在（建立文件时）
#define  ERR_VAILD_PARA             (0x19)      //非法的参数

/*创建文件或打开文件时的错误类型*/
#define  ERR_HANDLE_USEUP           (0x1a)      //句柄用完
#define  ERR_NO_EMPTY_DIRENTRY      (0X1B)      //没有空目录项
#define  ERR_FILENAME_USED_BADCHARS (0X1c)      //文件名带有不支持的字符
#define  ERR_CREAT_SHORT_NAME       (0X1D)      //创建短名失败
#define  ERR_BUILD_CLUSTER_CHAIN    (0X1E)      //获取空簇，更新fat表失败
#define  ERR_SET_DIRENTRY           (0X1F)      //更新目录项出错
#define  ERR_GET_DIRENTRY           (0X20)      //获取目录项出错
#define  ERR_NO_EMPTY_CLUSTER       (0X21)      //找不到空簇
#define  ERR_INVADLE_EMPTY_CLUSTER  (0X21)    //找到的空簇无效

#define  ERR_LBA_OVERFLOW            0X30      //LBA地址溢出
#define  ERR_PHY_UPDATA              0x31       //磁盘更新失败
#define  ERR_PARAM_IN                0X32      //传进来的参数错误
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
 libc提供的函数以指针形式存放在数组里面
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
 libc函数调用接口,通过函数指针方式调用
*/

/*
bool vfs_cd(uint32 vfs_mount, uint8 mode, uint8* input_name)
功能描述：根据用户输入将当前目录指向当前目录的子目录，父目录或直接返回根目录
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
mode：cd的操作命令类型，具体命令请参照vfs_interface.h  中//定义CD命令的操作类型
input_name：子目录名的buffer指针；为NULL的时候默认为进入当前目录。前两个字符为"0xfffe"则表示通过长名切换目录
返回值：0-失败；1-成功
*/
#define vfs_cd(a,b,c)                       ((bool(*)(uint32 , uint8 , uint8* ))VFS_CD_ENTRY)((a), (b), (c))

/*
uint32 vfs_dir(uint32 vfs_mount, uint8 mode, uint8* input_name,uint32 ext_name_bitmap)
功能描述：在当前目录按各种方式检索目录或文件
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
mode: 检索的命令方式，具体命令请参照vfs_interface.h 中//定义DIR命令的操作类型
input_name：用来存放输入文件名的buffer指针。如指针为NULL，则直接从当前目录项开始dir或不起作用
ext_name_bitmap：如是有效内存地址，则表示输入扩展名字符串的地址；如不是则表示要dir的bitmap。此参数如最高位为1表示传入的内存地址，否则则是bitmap值
		 具体的bitmap对应的意义请参照vfs_interface.h 中//定义DIR 扩展名的映射
返回值：0-失败；非0-成功
*/
#define vfs_dir(a,b,c,d)                    ((uint32(*)(uint32 , uint8 , uint8* , uint32 ))VFS_DIR_ENTRY)((a), (b), (c), (d))

/*
uint16 vfs_get_name(uint32 vfs_mount, uint8* output_name, uint16 longname_length)
功能描述：取当前的文件名（优先返回长名，没有长名则返回短名）或后缀名
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
output_name：用来存放输出文件名的buffer指针
longname_length：输入为要获取的长名字符数（包括长名标记和结束符），如为0表示获取后缀名
返回值：0-失败；非0-实际返回的获取到的文件名字符数。为取长名时返回实际函数输出的长名字符的个数（包括长名标记和结束符0x0000），为短名时返回11；获取后缀名时返回3
*/
#define vfs_get_name(a,b,c)                 ((uint16(*)(uint32 , uint8* , uint16 ))VFS_GET_NAME_ENTRY)((a), (b), (c))

/*
bool vfs_file_dir_offset(uint32 vfs_mount, pdir_layer_t* ptr_layer, pfile_offset_t* ptr_file, uint8 mode)
功能描述：获取或设置当前目录项的位置信息
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_layer: 存储要获取或设置的目录层级信息buffer指针；为NULL表示当前目录层的信息
ptr_file ：存储要获取或设置的文件位置信息buffer指针
mode     ：操作方式：0-表示获取，1-表示设置
返回值：0-失败；1-成功
*/
#define vfs_file_dir_offset(a,b,c,d)        ((bool(*)(uint32 , pdir_layer_t* , pfile_offset_t* , uint8 ))VFS_FILE_DIR_OFFSET_ENTRY)((a), (b), (c), (d))

/*
bool vfs_make_dir(uint32 vfs_mount, uint8* input_name)
功能描述：在当前目录下生成一个用户程序指定目录名的子目录。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 要创建的目录名指针
返回值：0-失败；1-成功
*/
#define vfs_make_dir(a,b)                   ((bool(*)(uint32 , uint8* ))VFS_MAKE_DIR_ENTRY)((a), (b))

/*
uint32 vfs_file_open(uint32 vfs_mount, uint8* input_name,uint8 mode)
功能描述：根据用户输入的文件名在当前目录中打开一个已存在的文件。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 待打开文件的文件名的输入指针；为null表示直接打开当前目录项指向的文件
mode ：    打开方式，具体参照vfs_interface.h 中//定义OPEN文件的方式
返回值：0-失败；非0-打开的文件句柄
*/
#define vfs_file_open(a,b,c)                ((uint32(*)(uint32 , uint8* , uint8 ))VFS_FILE_OPEN_ENTRY)((a), (b), (c))

/*
uint32 vfs_file_create(uint32 vfs_mount, uint8* input_name,uint32 lentgh)
功能描述：根据用户输入的文件名创建一个文件,用户可获得当前操作文件的句柄。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 为将要创建的文件的文件名字符串指针
lentgh ：   创建空文件的长度（单位为byte,不足512byte以512计算），为0表示创建空文件
返回值：0-失败；非0-创建的文件句柄
*/
#define vfs_file_create(a,b,c)              ((uint32(*)(uint32 , uint8* , uint32 ))VFS_FILE_CREATE_ENTRY)((a), (b), (c))

/*
bool vfs_file_close(uint32 vfs_mount, uint32 fhandle)
功能描述：关闭文件,用户输入需要关闭的文件句柄。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
fhandle: 需要关闭的文件句柄
返回值：0-失败；1-成功
说明：fhandle是vfs_file_open或vfs_file_create返回的句柄
*/
#define vfs_file_close(a,b)                 ((bool(*)(uint32 , uint32 ))VFS_FILE_CLOSE_ENTRY)((a), (b))

/*
bool vfs_file_dir_remove(uint32 vfs_mount, uint8* input_name, uint8 type)
功能描述：在当前目录下删除一个用户程序指定目录或文件，删除目录时要求目录为空。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 要删除的目录或文件名；如为null则直接删除当前目录项指向的目录或文件；
type  ：要操作的类型：0-删除目录；1-删除文件
返回值：0-失败；1-成功
*/
#define vfs_file_dir_remove(a,b,c)          ((bool(*)(uint32 , uint8*,uint8))VFS_FILE_DIR_REMOVE_ENTRY)((a), (b), (c))

/*
bool vfs_file_get_size(uint32 vfs_mount, uint32* output_length, uint8* ptr_file, uint8 mode)
功能描述：取文件的长度，字节为单位
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
output_length: 返回的文件长度（以字节为单位）
ptr_file： 待获取长度的文件句柄或文件名输入指针
mode  ：表示参数ptr_file的意义。0-表示ptr_file为文件句柄；1-表示ptr_file为文件名指针;ptr_file为null表示当前目录项指向的文件
返回值：0-失败；1-成功
*/
#define vfs_file_get_size(a,b,c,d)          ((bool(*)(uint32 , uint32* , uint8* , uint8 ))VFS_FILE_GET_SIZE_ENTRY)((a), (b), (c), (d))


/*
bool vfs_get_time(uint32 vfs_mount, file_time_t* output_time, uint8* input_name, uint8 type)
功能描述：获取文件的创建时间或修改时间
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
output_time:  时间输出buf指针
input_name： 待获取时间的文件的文件名输入指针；为null表示当前目录项指向的文件
type  ：FS_TIME_CREATE-获取创建时间；FS_TIME_MODIFY-获取修改时间
返回值：0-失败；1-成功
*/
#define vfs_get_time(a,b,c,d)               ((bool(*)(uint32 , file_time_t* , uint8* , uint8 ))VFS_GET_TIME_ENTRY)((a), (b), (c), (d))


/*
bool vfs_file_seek(uint32 vfs_mount, int32 offset, uint8 type, uint32 fhandle)
功能描述：定位文件的字节偏移，实现：
	a.根据相对文件首的偏移量，实现用户程序的顺序读和随机读。
	b.实现从当前位置往文件首或文件尾偏移，实现用户程序的顺序读和随机读。
	c.根据相对文件首的偏移量，实现用户程序的对指定位置数据的修改。
	d.配合read支持用户程序顺序读数据，快进快退以及随机定位读数据，另外可以方便实现回写修改已经生成的文件。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
offset:  对应SEEK偏移量，范围是2GB
type： 对应SEEK类型，具体参照vfs_interface.h 中//定义文件SEEK的方式
fhandle  ：已打开的文件句柄
返回值：0-失败；1-成功
*/
#define vfs_file_seek(a,b,c,d)              ((bool(*)(uint32 , int32 , uint8 , uint32 ))VFS_FILE_SEEK_ENTRY)((a), (b), (c), (d))

/*
bool vfs_file_tell(uint32 vfs_mount, uint32* ptr_offset ,uint32 fhandle)
功能描述：取当前读写操作的指针，指针是指相对文件头的字节偏移量。读数据时用户调用该函数记录AB点，配合seek和read实现数据的AB读取。
	  写数据时，支持用户程序修改已生成的文件。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_offset:  文件当前读写位置相对文件头的偏移量
fhandle  ：已打开的文件句柄
返回值：0-失败；1-成功
*/
#define vfs_file_tell(a,b,c)                ((bool(*)(uint32 , uint32* , uint32 ))VFS_FILE_TELL_ENTRY)((a), (b), (c))

/*
uint32 vfs_file_read(uint32 vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
功能描述：从文件的当前位置读取一定大小的数据
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_data_buffer:  读操作数据输出buffer的指针，buffer的大小一定不能小于后面指定的byte_count
byte_count：读的字节数
fhandle  ：已打开的文件句柄
返回值：0-失败；非0-返回实际读到的字节数
*/
#define vfs_file_read(a,b,c,d)              ((uint32(*)(uint32 , uint8* , uint32 , uint32 ))VFS_FILE_READ_ENTRY)((a), (b), (c), (d))


/*
uint32 vfs_file_write(uint32 vfs_mount, uint8* ptr_data_buffer, uint32 byte_count, uint32 fhandle)
功能描述：从文件的当前位置读取一定大小的数据
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_data_buffer:  写数据buffer的指针
byte_count：写的字节数
fhandle  ：已打开的文件句柄
返回值：0-失败；非0-返回实际写的字节数
*/
#define vfs_file_write(a,b,c,d)             ((uint32(*)(uint32 , uint8* , uint32 , uint32 ))VFS_FILE_WRITE_ENTRY)((a), (b), (c), (d))

/*
bool vfs_get_space(uint32 vfs_mount, uint32* ptr_sector_count, uint8 type)
功能描述：获取磁盘分区空间，根据输入参数不同选择要求返回磁盘分区总的扇区数还是剩余扇区数
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_sector_count:  返回分区的扇区数
type：0-表示调用将返回表示当前磁盘分区总空间的扇区数；1-表示返回当前磁盘分区剩余空间的扇区数
返回值：0-失败；1-成功
*/
#define vfs_get_space(a,b,c)                ((bool(*)(uint32 , uint32* , uint8 ))VFS_GET_SPACE_ENTRY)((a), (b), (c))

/*
uint8 vfs_get_err_info(uint32 vfs_mount)
功能描述：获取文件系统出错信息
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
返回值：0-没有错误；非0-错误信息，具体参照vfs_interface.h 中//定义文件系统错误类型
*/
#define vfs_get_err_info(a)                 ((uint8(*)(uint32))VFS_GET_ERR_INFO_ENTRY)(a)

/*
uint32 vfs_file_dir_exist(uint32 vfs_mount, uint8* input_name, uint8 type)
功能描述：判断当前目录是否有指定的子目录或文件
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name:要判断的文件或目录名指针
type    :要判断的类型：0-目录；1-文件
返回值：0-不存在；非0-存在，前返回该文件或目录所在的目录的首簇号
*/
#define vfs_file_dir_exist(a,b,c)           ((uint32(*)(uint32 , uint8* , uint8 ))VFS_FILE_DIR_EXIST_ENTRY)((a), (b), (c))


/*
uint8 vfs_file_attralter(uint32 vfs_mount, uint8 attr, uint8* ptr_file, uint8 mode)
功能描述：获取或修改文件的属性
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
attr:0-获取文件的属性；非0-设置成的文件属性，具体参照vfs_interface.h 第105行下的定义
ptr_file    :待获取或修改属性的文件句柄或文件名输入指针；
mode：表示参数ptr_file的意义。0-表示ptr_file为文件句柄；1-表示ptr_file为文件名指针;ptr_file为null表示当前目录项指向的文件或目录。
返回值：0-失败；非0-返回要获取或设置的属性值
*/
#define vfs_file_attralter(a,b,c,d)         ((uint8(*)(uint32 , uint8 , uint8* , uint8 ))VFS_FILE_ATTRALTER_ENTRY)((a), (b), (c), (d))

/*
bool vfs_file_rename(uint32 vfs_mount, uint8* ptr_new_name, uint8* ptr_file, uint8 mode)
功能描述：获取或修改文件的属性
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_new_name:新文件名字符串指针
ptr_file    :待重命名的文件句柄或文件名指针；
mode：表示参数ptr_file的意义。0-表示ptr_file为文件句柄；1-表示ptr_file为文件名指针;ptr_file为null表示当前目录项指向的文件或目录
返回值：0-失败；1-成功
*/
#define vfs_file_rename(a,b,c,d)            ((bool(*)(uint32 , uint8* , uint8* , uint8 ))VFS_FILE_RENAME_ENTRY)((a), (b), (c), (d))

/*
bool vfs_create_volume(uint32 vfs_mount, uint8* input_name)
功能描述：创建卷标
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 卷标名字符串指针;input_name=NULL表示查询是否存在卷标，input_name=非NULL直接修改卷标
返回值：0-失败；1-成功
*/
#define vfs_create_volume(a,b)              ((bool(*)(uint32 , uint8* ))VFS_CREATE_VOLUME_ENTRY)((a), (b))

/*
bool vfs_set_time(uint32 vfs_mount, file_time_t* ptr_input_time, uint8* input_name, uint8 type)
功能描述：创建卷标
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
ptr_input_time：需要设置的时间输入buf指针
input_name: 待设置时间的文件的文件名输入指针；为null表示前目录项指向的文件
type：FS_TIME_CREATE-设置创建时间;FS_TIME_MODIFY-设置修改时间
返回值：0-失败；1-成功
*/
#define vfs_set_time(a,b,c,d)               ((bool(*)(uint32 , file_time_t* , uint8* , uint8 ))VFS_SET_TIME_ENTRY)((a), (b), (c), (d))

#define vfs_clear_private_data(a)           ((uint8(*)(uint32 p_vfs_mount))VFS_CLEAR_PRIVATE_DATA_ENTRY)(a)

/*
bool vfs_file_move(uint32 vfs_mount, void *fat_direntry, uint8 *ptr_file_name, uint8 mode)
功能描述：删除或增加目录项
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
fat_direntry：目录项信息指针
ptr_file_name: 待增加或删除的目录名字符串指针；当mode==0时，如果为null表示前目录项指向的文件;否则参数无效
mode：0-删除目录项；1-增加目录项
返回值：0-失败；1-成功
*/
#define vfs_file_move(a,b,c,d)              ((bool(*)(uint32 , void* , uint8* , uint8 ))VFS_FILE_MOVE_ENTRY)((a), (b), (c), (d))

/*
uint32 vfs_rec_file_update(uint32 vfs_mount, void *wav_head_buf, uint32 fhandle)
功能描述：用于录音中更新录音文件WAV头部信息
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
wav_head_buf：WAV文件头部数据
buf_len: 更新数据长度，固定为512
fhandle：需要操作的文件句柄
返回值：文件的首簇号
*/
#define vfs_rec_file_update(a,b,c)              ((uint32(*)(uint32 , void* , uint32 ))VFS_REC_FILE_UPDATE_ENTRY)((a), (b), (c))

/*
uint32 vfs_rec_file_recover(uint32 vfs_mount, uint8* input_name,uint32 start_cluster)
功能描述：根据用户输入的录音文件名恢复由于掉电而未保存的录音文件。
参数说明：
vfs_mount：挂载文件系统后返回的句柄,不能为NULL;
input_name: 待打开文件的文件名的输入指针；为null表示直接打开当前目录项指向的文件
start_cluster ：文件句柄中的首簇信息，关键参数
返回值：0-失败；非0-打开的文件句柄
*/
#define vfs_rec_file_recover(a,b,c)                ((uint32(*)(uint32 , uint8* , uint32 ))VFS_REC_FILE_RECOVER_ENTRY)((a), (b), (c))


#endif
