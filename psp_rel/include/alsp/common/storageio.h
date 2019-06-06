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
* \brief    ����Դ洢�豸����������ӿ�
* \author   kkli
* \version 1.0
* \date  2008/09/02
*******************************************************************************/
#ifndef __STORAGEIO_H__
#define __STORAGEIO_H__

/*!
 * \brief
 *      �ṩ�ͱ�׼�ļ�ϵͳ�����ӿ�һ�µĴ洢�豸��������ӿ�
 */
typedef struct storage_io_s {
    /*! ������ */
    int (*read)(void *buf, int size, int count, struct storage_io_s *io);
    /*! д���� */
    int (*write)(void *buf, int size, int count, struct storage_io_s *io);
    /*! ��λ */
    int (*seek)(struct storage_io_s *io, int offset, int whence);
    /*! �ļ�λ�� */
    int (*tell)(struct storage_io_s *io);
	/* �ļ����� 0: VFS�ļ���1��SD�ļ� */
	int file_type;
	/* �ļ���� */
	void *handel;
} storage_io_t;

#endif // __STORAGEIO_H__
