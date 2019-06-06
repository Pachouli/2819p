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
 * \file     vfs.h
 * \brief    �����ļ�ϵͳ�ӿ�
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __VFS_DEFINE_H__
#define __VFS_DEFINE_H__

//�������֧��Ŀ¼�㼶
#define  MAX_DIR_LAYER              (9)
#define DISK_BASE_MAX 7
#define DISK_SD 0                   /*ϵͳ��*/
#define DISK_VM 1                   /*VM��*/
#define DISK_HID 2                  /*������*/
#define DISK_AUTORUN 3              /*autorun��*/
#define DISK_ENCRYPT 4
#define DISK_UNKNOWN 5
#define DISK_C 6                    /*base�洢�û���*/
#define DISK_H 0x10                 /*card��*/
#define DISK_U 0x20                 /*uhost��*/

#define STG_CARD 1
#define STG_UHOST 2

/*C����������������U��Ӧ����PC��A&B������A&C��*/
#define DISK_BASE_MAX 7

#define   DIR_ENTRY_OFFSET    0x20  //һ��Ŀ¼��Ĵ�С

#define   FAR_CLUSTER_MASTER   0x00FFFFFF

//ָfat16/32���ļ���Ŀ¼�����ֵ���Чλ��
#define   FAT_LONG_NAME_BITMAP  0x7fe3ffcf    //B - 01111111 11100011 11111111 11001111
#define   FILTER_LONGNAME_ITEM      0
#define   INCLUDE_LONGNAME_ITEM     1

#define   ADD_FHANDLE_COUNT   2     // used 2*512 buffer
#define   FS_MOUNT_MAX  2
#define   DEF_BYTES_PER_SECTOR  512
#define   BYTES_PER_DIRENTRY    32

#define   GET_ONLY      0
#define   GET_AND_FILL_BACK 1
#define   GET_AND_INC_BACK  2

#define   FHANDLE_COUNT   4
#define   FHANDLE_FREE    0xff
#define   FHANDLE_WRITE   0x80

#define   FAT_DIRENTRIES_PER_SECTOR (512/32)

#define CLUST_FIRST     2               // first legal cluster number
#define CLUST_RSRVD     0xfffffff6      // reserved cluster range
#define CLUST_BAD       0xfffffff7      // a cluster with a defect
#define CLUST_EOFS      0xfffffff8      // start of eof cluster range
#define CLUST_EOFE      0xffffffff      // end of eof cluster range
#define FAT16_MASK      0x0000ffff      // mask for 16 bit cluster numbers
#define FAT32_MASK      0x0fffffff      // mask for FAT32 cluster numbers
#define CONT_CLUST_MASK      0x7f       // mask for EXFAT continue cluster
#define CONT_CLUST_FLAG      0x80       // check for EXFAT continue cluster


#define   GET_NORMAL_DE          0     // may for root head and when over tail last operation 
#define   GET_CURRENT_DE          (-32)    // may for root head and when over tail last operation 

#define         SLOT_EMPTY          0x00    // slot has never been used
#define         SLOT_E5             0x05    // the real value is 0xe5
#define         SLOT_DELETED        0xe5    // file in this slot deleted
#define         LCASE_BASE          0x08    // filename base in lower case
#define         LCASE_EXT           0x10    // filename extension in lower case

//����Ŀ¼�㼶�ṹ
typedef struct
{
    uint32 cluster;
    uint32 offset;
} dir_layer_info_t;

typedef struct
{
    dir_layer_info_t dir_layer_info[MAX_DIR_LAYER];
    uint8 cur_layer; // dir layer
} fdir_layer_t;


//����Ŀ¼�㼶�ṹ
typedef struct
{
    uint32 current_cluster;
    uint32 offset_of_cluster;
} file_offset_t;

//����ȫ�ֱ����ṹ
typedef struct
{
    uint8 fs_type;  //�ļ�ϵͳ���:0-fat32,1-fat16,2-exfat
    //used_tag:bit7(ֻ��exfat��Ч):���ļ���Ŀ¼����ռ�Ĵ����Ƿ�������ǣ�bit7=1��ʾ������bit7=0��ʾ������
    //bit0-6:��ʾ�þ����ʹ�����;0x7f-��ʹ�ã�0:δʹ��
    uint8 used_tag; 
    uint8 fs_status;//ϵͳ��ǰ��״̬��0-����״̬����0-����״̬
    int8  dir_flag; //��ǰdir�ļ���Ŀ¼�ķ���
    //dbr info
    uint32 dbr_offset; //DBR ��ƫ����
    uint32 bpb_total_sectors; //ϵͳ��������
    uint32 bpb_fat_sectors;   //fat��ռ�õ�������
    uint16 bpb_sectors_per_cluster; //ÿ����ռ�õ�������    
    uint16 root_dir_sectors;  // ��������
    uint32 fat1_start_lbra;   //fat�����ʼ��ַ
    uint32 exfat_dir_end_cluster; //�����������ļ���Ŀ¼�Ľ����غ�
    uint32 data_start_lbra;   //ϵͳ���ݵ���ʼ��ַ
    uint32 total_data_sectors; //����������������
    uint32 total_data_clusters; // ���������ܴ���

    // dir info   
    uint32 de_current_cluster; //��ǰ�����Ĵغ�
    uint32 de_cluster_offset; //��ǰ�����Ĵ����ڵ�ƫ����
    fdir_layer_t g_dir_layer;  //��ǰ������Ŀ¼�����Ϣ

    int8 once_get_de_ctl;     //��ȡĿ¼����Ϣ�ķ���;0-�ӵ�ǰλ�ã�-32-��ǰ��32-����
    uint8 handle_index;      //�þ��ռ�õ�������


} fs_data_t;



typedef struct
{
    uint8 deName[8]; // filename, blank filled
    uint8 extension[3]; // extension, blank filled
    uint8 attributes; // file attributes
    uint8 deLowerCase; // NT VFAT lower case flags
    uint8 deCHundredth; // hundredth of seconds in CTime
    uint16 de_creat_time; // create time
    uint16 de_creat_date; // create date
    uint16 de_access_date; // access date
    uint16 high_cluster; // high bytes of cluster number
    uint16 de_modify_time; // last update time
    uint16 de_modify_date; // last update date
    uint16 start_cluster; // starting cluster of file
    uint32 file_size; // size of file in bytes

} fat_direntry_t;

typedef struct
{
    uint8 ldir_ord; // long name item order
    uint16 ldir_name1[5]; // long name 1
    uint8 attributes; // file attributes, must
    uint8 ldir_type; // long name type
    uint8 ldir_check_sum; // short name type
    uint16 ldir_name2[6]; // long name 2
    uint16 ldir_first_cluster; // not used,must 0
    uint16 ldir_name3[2]; // long name 3
} fat_ldir_t;

typedef struct
{
    uint8 mode;
    uint32 start_cluster; // starting cluster of file
    uint32 file_size; // size of file in bytes
    uint32 de_current_cluster; // the cluster of the file direntry
    uint32 de_cluster_offset; // the offset of the file direntry in cluster
    uint32 ct_current_cluster; // current cluster of file op
    uint32 ct_cluster_offset; // the offset of the file op in cluster
    uint32 postion; // file op postion
    uint16 de_creat_date; // create date
    uint16 de_creat_time; // create time
    void *ehance_addr;
} fhandle_t;

typedef struct
{
    uint32 *fast_seek_buffer; // fast seek buffer
    uint32 buff_cluster_count;
    uint32 ct_cluster_pos;
    uint8 used_tag;
    uint8 seek_buffer_ptr; // file current cluster ptr
} add_fhandle_t;

typedef struct
{
    uint32 exist_cluster_count;
    uint32 ct_cluster_pos;
} add_fhandle_write_t;

typedef struct
{
    uint16 magic_val;
    uint16 scan_fat_sector_offset;
    uint32 empty_clusters;//��¼���̻�ʣ��Ŀմ���
    uint32 scan_fat_ptr;//��¼�ϴ�fat���ϴ�ɨ�赽�����λ��
    uint32 bitmap_cache_bitptr;
} fs_vm_head_t;


/*!
 *      primary dir dentry
 *
 */
typedef struct exfat_primary_entry
{
    unsigned char type; /* entry type, 0x85 */
    unsigned char secondary_count; /* count of secondary directory entry */
    unsigned short check_sum; /* all diretory check sum */
    unsigned char flags; /* the attribute of file  */
    unsigned char reserved1[3]; /*  */
    unsigned short creat_time;
    unsigned short creat_date;
    unsigned short modify_time;
    unsigned short modify_date;
    unsigned short access_time;
    unsigned short access_date;
    unsigned char reserverd2[12];
} exfat_primary_entry_t;

/*!
 *      secondary dir dentry
 *
 */
typedef struct exfat_secondary_entry
{
    unsigned char type; /* entry type, 0xc0 */
    unsigned char flag; /* 1: fat entry of the file isnot sequential access, 3:sequential access */
    unsigned char reserved1; /* */
    unsigned char fname_length; /* the length of the file name  */
    unsigned short check_sum; /* file name check sum */
    unsigned short reserved2;
    uint32 flength_dwlow;
    uint32 flength_dwhigh;
    int reserved3;
    uint32 first_cluster;
    uint32 dlength_dwlow;
    uint32 dlength_dwhigh;
} exfat_secondary_entry_t;

/*!
 *      file name dir dentry
 *
 */
typedef struct exfat_name_entry
{
    unsigned char type; /* entry type, 0xc1 */
    unsigned char reserved; /*  */
    unsigned short name[15]; /* unicode name */
} exfat_name_entry_t;



/*!
 *      current dir enviornment
 *
 */
typedef struct curdir_op_env
{
    uint32 tmp_de_current_cluster;
    uint32 tmp_de_cluster_offset;
    uint32 tmp_dir_offset;
} curdir_op_env_t;
typedef struct {
    void *fat_private_data;
} vfs_mount_t;


typedef struct {
    uint8 fs_type;
    uint8 disk_partion;
}vfs_info_t;


#endif

