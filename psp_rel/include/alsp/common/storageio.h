/********************************************************************************
*                              5003
*                            Module: storage io
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*       kkli     2008-09-02 10:00     1.0             build this file
********************************************************************************/
/*!
* \file     storageio.h
* \brief    定义对存储设备的输入输出接口
* \author   kkli
* \version 1.0
* \date  2008/09/02
*******************************************************************************/
#ifndef __STORAGEIO_H__
#define __STORAGEIO_H__

/*!
 * \brief
 *      提供和标准文件系统操作接口一致的存储设备输入输出接口
 */
typedef struct storage_io_s {
    /*! 读数据 */
    int (*read)(void *buf, int size, int count, struct storage_io_s *io);
    /*! 写数据 */
    int (*write)(void *buf, int size, int count, struct storage_io_s *io);
    /*! 定位 */
    int (*seek)(struct storage_io_s *io, int offset, int whence);
    /*! 文件位置 */
    int (*tell)(struct storage_io_s *io);
	/* 文件类型 0: VFS文件，1：SD文件 */
	int file_type;
	/* 文件句柄 */
	void *handel;
} storage_io_t;

#endif // __STORAGEIO_H__
