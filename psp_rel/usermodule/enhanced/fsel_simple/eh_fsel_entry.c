/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-28 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

#include "dir_list.h"

#define MAX_SEL_MODE   0x01

//��������
static const fsel_get_next_p fsel_get_next_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_next, //Ŀ¼ѡ����һ���ļ�
    };

static const fsel_get_prev_p fsel_get_prev_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_prev, //Ŀ¼ѡ����һ���ļ�
    };

static const fsel_get_byno_p fsel_get_byno_array[MAX_SEL_MODE] =
    {
        fsel_dir_get_byno_all, //Ŀ¼�������ѡ���ļ�
    };

static const fsel_set_location_p fsel_set_location_array[MAX_SEL_MODE] =
    {
        (void *) fsel_dir_set_location, //Ŀ¼����·����Ϣ
    };

static const fsel_get_location_p fsel_get_location_array[MAX_SEL_MODE] =
    {
        (void *) fsel_dir_get_location, //Ŀ¼��ȡ·����Ϣ
    };

static const fsel_get_nextdir_p fsel_get_nextdir_array[MAX_SEL_MODE] =
    {
        fsel_dir_next_dir, //Ŀ¼�л���һ��Ŀ¼
    };
static const fsel_get_prevdir_p fsel_get_prevdir_array[MAX_SEL_MODE] =
    {
        fsel_dir_prev_dir, //Ŀ¼�л���һ��Ŀ¼
    };

static const fsel_get_total_p fsel_get_total_array[MAX_SEL_MODE] =
    {
        fsel_total_file_all, //Ŀ¼��ȡ�ļ�����
    };

/******************************************************************************/
/*
 * \par  Description:��ȡ��һ���ļ�
 *
 * \param[in]    none

 * \param[out]      strfile ����ļ���չ��

 * \return        true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == 0)
    {
        return 0;
    }
    return fsel_get_next_array[eh_fsel_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:��ȡ��һ���ļ�
 *
 * \param[in]    none

 * \param[out]   strfile ����ļ���չ��

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevfile(char *strfile)
{
    fsel_error = FSEL_NO_ERR;
    if (eh_vfs_mount == 0)
    {
        return 0;
    }
    return fsel_get_prev_array[eh_fsel_type](strfile);
}

/******************************************************************************/
/*
 * \par  Description:��ָ����Ż�ȡ�ļ�
 *
 * \param[in]    num �ļ��������(1~...)

 * \param[out]      strfile ����ļ���չ��

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_byno(char *strfile, uint16 num)
{
    fsel_error = FSEL_NO_ERR;

    if (num == 0)
    {
        return FALSE;
    }
    return fsel_get_byno_array[eh_fsel_type](strfile, num);
}

/******************************************************************************/
/*
 * \par  Description:��ǰĿ¼���б���ļ��ܸ���
 *
 * \param[in]    none

 * \param[out]      none

 * \return       �ļ��ܸ���

 * \note
 *******************************************************************************/
uint16 fsel_get_total(void)
{
    fsel_error = FSEL_NO_ERR;

    if (eh_file_total_all != 0)
    {
        return eh_file_total_all;
    }
    else
    {
        return fsel_get_total_array[eh_fsel_type]();
    }
}

/******************************************************************************/
/*
 * \par  Description:��ǰ�����ļ�����
 *
 * \param[in]    none

 * \param[out]      none

 * \return       �ļ��ܸ���

 * \note
 *******************************************************************************/
uint16 fsel_get_disk_total(void)
{
    return fsel_get_total_array[eh_fsel_type]();
}


/******************************************************************************/
/*
 * \par  Description:��ȡ��ǰѡ���ļ������
 *
 * \param[in]    none

 * \param[out]      none

 * \return        ���(1~...)

 * \note
 *******************************************************************************/
uint16 fsel_get_fileno(void)
{
    //fsel_error = FSEL_NO_ERR;
    //return eh_file_no;
    uint16 file_no = eh_file_no_all;

    return file_no;
}

/******************************************************************************/
/*
 * \par  Description:������Ʋ���ģʽ
 *
 * \param[in]    mode ����ģʽ

 * \param[out]      none

 * \return        ture or false

 * \note
 *******************************************************************************/
bool fsel_set_mode(uint8 mode)
{
    fsel_error = FSEL_NO_ERR;

#ifdef SUPPORT_SHUFFLE_MODE
    //check shuffle
    if (((uint8) mode & 0xf0) > 0)
    {
        if (eh_shuffle_sw == FALSE)
        {
            //֮ǰ����shuffle,Ҫ��ʼ��
            eh_shuffle_sw = TRUE;
            //if(eh_file_total_all == 0)
            //{
            eh_file_total_all = fsel_get_total();
            //}
            fsel_init_shuffle (eh_file_total_all);
            filtrate_shuffle_no = eh_file_no_all;
        }
    }
    else
    {
        eh_shuffle_sw = FALSE;
    }
#else
    eh_shuffle_sw = FALSE;
#endif

    fsel_init_val.fsel_mode = (uint8)(mode & 0x0f);

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description:�����ļ�location
 *
 * \param[in]    location
 type

 * \param[out]

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_set_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if (location == NULL)
    {
        return FALSE;
    }

    return fsel_set_location_array[eh_fsel_type](location);
}

/******************************************************************************/
/*
 * \par  Description:��ȡ��ǰѡ���ļ���location
 *
 * \param[in]    type

 * \param[out]      location

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_location(void *location)
{
    fsel_error = FSEL_NO_ERR;

    if ((eh_file_no == 0) || (location == NULL))
    {
        return FALSE;
    }

    return fsel_get_location_array[eh_fsel_type](location);
}

/******************************************************************************/
/*
 * \par  Description:��ȡ��һ��Ŀ¼
 *
 * \param[in]        none

 * \param[out]      strfile-����Ŀ¼��ʶ

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_nextdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_nextdir_array[eh_fsel_type](strdir);
}

/******************************************************************************/
/*
 * \par  Description:��ȡ��һ��Ŀ¼
 *
 * \param[in]    none

 * \param[out]          strfile-����Ŀ¼��ʶ

 * \return         true or false

 * \note
 *******************************************************************************/
bool fsel_get_prevdir(uint8 *strdir)
{
    fsel_error = FSEL_NO_ERR;

    return fsel_get_prevdir_array[eh_fsel_type](strdir);
}
