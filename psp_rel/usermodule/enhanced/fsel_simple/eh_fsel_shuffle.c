/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fiona.yang  2012-12-31 15:37    1.0             build this file
 *******************************************************************************/

#include "eh_fsel_entry.h"

const uint8 byte_set_val[8] =
    { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
const uint8 byte_get_val[8] =
    { 0x01, 0x02, 0x4, 0x08, 0x10, 0x20, 0x40, 0x80 };

/******************************************************************************/
/*
 * \par  Description:������ų�ʼ�����ڳ�Ա��map״̬
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/

void init_member_map(void)
{
    uint8 i, j;
    for (i = 0; i < (member_avg / 8); i++)
    {
        member_map[i] = 0xff;
    }

    j = member_avg % 8;

    member_max = member_avg;

    if (cur_group < member_avg_group)
    {
        //�����ƽ��ֵ���ǰ��ʱ
        j++;
        member_max++;
    }

    if (j != 0)
    {
        member_map[i] = byte_set_val[j - 1];
    }

    return;

}

/******************************************************************************/
/*
 * \par  Description: ����ռ�����״̬
 *
 * \param[in]    bitmap-λͼ���ַ,num-�ڼ���bit(1~8)

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool set_bitmap_bit(uint8 *bitmap, uint16 num)
{
    uint8 i, j;

    num--; //��Ϊ��0��ʼ
    i = (uint8)(num / 8);
    j = (uint8)(num % 8);

    if ((bitmap[i] & byte_get_val[j]) > 0)
    {
        //�����bit,��ʾռ��
        bitmap[i] = (uint8)(bitmap[i] & (255 - byte_get_val[j]));
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************/
/*
 * \par  Description:����δ���Ź��ĳ�Ա
 *
 * \param[in]

 * \param[out]

 * \return        1~n

 * \note
 *******************************************************************************/

uint16 get_bitmap_bit(uint8 *bitmap, uint8 count_max, uint16 rand_num)
{
    uint16 i;
    //�����������
    if ((rand_num & 0x01) > 0)
    {
        //��ͷ��ǰ��
        for (i = 1; i < (count_max + 1); i++)
        {
            if (FALSE != set_bitmap_bit(bitmap, i))
            {
                return i;
            }
        }
    }
    else
    {
        //��ĩ������
        for (i = count_max; i > 0; i--)
        {
            if (FALSE != set_bitmap_bit(bitmap, i))
            {
                return i;
            }
        }

    }

    return 0;
}

/******************************************************************************/
/*
 * \par  Description:��ȡshuffle�����
 *
 * \param[in]

 * \param[out]

 * \return      group_num(1~n)

 * \note
 *******************************************************************************/

uint16 get_group_num(void)
{
    uint16 rand_num;
    uint8 i = 3;
    uint8 tmp_group;
    //���ֻ��һ�飿����
    if (group_max == 0)
    {
        return 1;
    }
    tmp_group = group_max;
    while (i != 0)
    {
        i--;
        //try 3��,ȡ�������
        rand_num = (uint16)((sys_random() % tmp_group) + 1);
        //�������bitmap
        if (FALSE != set_bitmap_bit(group_map, rand_num))
        {
            return rand_num;
        }
    }
    //��ȡbitmap
    return get_bitmap_bit(group_map, group_max, rand_num);
}

/******************************************************************************/
/*
 * \par  Description:��ȡ���ڳ�Ա��shuffle��
 *
 * \param[in]

 * \param[out]

 * \return        member_num(1~n)

 * \note
 *******************************************************************************/

static uint16 get_member_num(void)
{
    uint16 rand_num;
    uint16 temp8;
    uint8 i = 3;

    if (0 == member_max)
    {
        return 1;
    }

    temp8 = member_max;

    if (cur_group < member_avg_group)
    {
        //�����ƽ��ֵ���ǰ��ʱ
        temp8++;
    }

    while (i != 0)
    {
        i--;
        //try 3��
        rand_num = (uint8)((sys_random() % temp8) + 1);
        if (FALSE != set_bitmap_bit(member_map, rand_num))
        {
            return rand_num;
        }
    }

    return get_bitmap_bit(member_map, member_max, rand_num);

}

/******************************************************************************/
/*
 * \par  Description:��ȡ���ݶ����е���һ��
 *
 * \param[in]

 * \param[out]

 * \return       file_no (1~file_total)

 * \note
 *******************************************************************************/

uint16 get_nextlist_num(void)
{
    if (cur_save_pt >= (SET_PREV_MAX - 1))
    {
        //ѭ��buffer�ķ�ʽ���
        cur_save_pt = 0;
    }
    else
    {
        cur_save_pt++;
    }

    if (cur_save_prev_count < SET_PREV_MAX)
    {
        cur_save_prev_count++;
    }

    cur_save_next_count--;

    return play_num_list[cur_save_pt];

}

/******************************************************************************/
/*
 * \par  Description:��ȡ���ݶ����е���һ��
 *
 * \param[in]

 * \param[out]

 * \return        file_no (1~file_total)

 * \note
 *******************************************************************************/

static uint16 get_prevlist_num(void)
{
    if (cur_save_pt == 0)
    {
        //ѭ��buffer�ķ�ʽ���
        cur_save_pt = SET_PREV_MAX - 1;
    }
    else
    {
        cur_save_pt--;
    }

    if (cur_save_next_count < SET_PREV_MAX)
    {
        cur_save_next_count++;
    }

    cur_save_prev_count--;

    return play_num_list[cur_save_pt];
}

/******************************************************************************/
/*
 * \par  Description:��ȡ���ŵ�shuffle��number
 *
 * \param[in]

 * \param[out]

 * \return        file_no (1~file_total)

 * \note
 *******************************************************************************/

uint16 get_shuffle_num(void)
{
    uint16 temp_num;

    if (FALSE == use_member_flag)
    {
        //ֻʹ�����shuffle
        cur_group = (uint8) get_group_num();
        return (uint16) cur_group;
    }

    //ʹ������shuffle
    if (cur_group == 0)
    {
        //�״ν���
        cur_group = (uint8) get_group_num();
        if (0 == cur_group)
        {
            //�������Ѿ�������
            return 0;
        }
        init_member_map();
    }

    cur_member = get_member_num();
    if (0 == cur_member)
    {
        //��ǰ���Ա��ȫ��������
        cur_group = (uint8) get_group_num();
        if (0 == cur_group)
        {
            //�������Ѿ�������
            return 0;
        }
        init_member_map();
        cur_member = get_member_num();
        if (0 == cur_member)
        {
            fsel_error = FSEL_ERR_SHUFFLE;
            return 0;
        }
    }

    temp_num = SET_GROUP_MAX * (cur_member - 1) + cur_group;
    return temp_num;
}

/******************************************************************************/
/*
 * \par  Description:��ʼshuffle����
 * \bool fsel_init_shuffle(uint16 num_total)
 * \param[in] num_total param1 ��Ҫshuffle���ļ�����

 * \param[out] none

 * \return    bool
 TRUE   succese
 FALSE  fail
 * \ingroup   eh_fsel_shuffle.c
 * \note
 *******************************************************************************/
bool fsel_init_shuffle(uint16 num_total)
{
    uint8 i, j;
    //�ڴ˴���ֵΪ0����ʼ���������ļ�
    filtrate_shuffle_no = 0; //

    if (num_total == 0)
    {
        return FALSE;
    }
    //���ӳ���
    libc_memset(group_map, 0x00, sizeof(group_map));
    //���ڳ�Աӳ���
    libc_memset(member_map, 0x00, sizeof(member_map));
    //ʹ������shuffle���ܱ�־
    use_member_flag = FALSE;
    //��ǰ���
    cur_group = 0;
    //��ǰ��Ա��
    cur_member = 0;
    //��ǰ���ָ��(0~)
    cur_save_pt = SET_PREV_MAX;
    //ǰһ����
    cur_save_prev_count = 1;
    //��һ����
    cur_save_next_count = 1;
    //��ת���,1-yes,0-no
    cur_overturn_flag = 0;
    //shuffle �ļ�����
    shuffle_total = num_total;

    if (num_total > SET_GROUP_MAX)
    {
        //������������,�������ڳ�Աshuffle
        use_member_flag = TRUE;
        libc_memset(group_map, 0xff, sizeof(group_map));
        group_max = SET_GROUP_MAX;
    }
    else
    {
        //������������,ֻ����shuffle�Ϳ�����
        group_max = (uint8) num_total;
        for (i = 0; i < (num_total / 8); i++)
        {
            group_map[i] = 0xff;
        }

        if ((num_total % 8) != 0)
        {
            j = (uint8)((num_total % 8) - 1);
            group_map[i % (SET_GROUP_MAX / 8)] = byte_set_val[j];
        }
    }

    if (FALSE != use_member_flag)
    {
        //������������,���ڳ�Աshuffle
        member_avg = (uint8)(num_total / SET_GROUP_MAX);
        member_avg_group = (uint8)((num_total % SET_GROUP_MAX) + 1);
    }

    return TRUE;

}

/******************************************************************************/
/*
 * \par  Description:��ȡshuffle���Ƶ���һ���ļ�
 * \uint16 fsel_shuffle_next_file(void)
 * \param[in] none
 * \param[out]none

 * \return        file_no (1~file_total)
 * \ingroup       eh_fsel_shuffle.c
 * \note
 *******************************************************************************/
//shuffle mode  get next
uint16 fsel_shuffle_next_file(void)
{
    uint16 temp_num;

    if (shuffle_total < eh_file_total_all)
    {
        //�ļ����������(�����ļ�)�����³�ʼ��
        fsel_init_shuffle (eh_file_total_all);
    }
    else
    {
        //���¸�ֵ��ֹ file_total<shuffle_total�����(�ļ�ɾ��)
        shuffle_total = eh_file_total_all;
    }
    //��Ч
    if (shuffle_total == 0)
    {
        return 0;
    }
    //�������,��ֱ�Ӵӱ����16���ļ��в���,����ֱ��shuffle
    while (cur_save_next_count > 1)
    {
        temp_num = get_nextlist_num();
        if (temp_num <= shuffle_total)
        {
            if (temp_num != 0)
            {
                return temp_num;
            }
            else
            {
                break;
            }
        }

        cur_save_next_count--;
    }

    //list can't find,use shuffle al
    //��֤���������ֵ��shuffle_total��Χ��,����file_no����ͬ
    do
    {
        temp_num = get_shuffle_num();
        if (temp_num == 0)
        {
            break;
        }

    }
    while ((filtrate_shuffle_no == temp_num) || (temp_num > shuffle_total));

    if (0 != temp_num)
    {
        //SAVE TO PLAY_NUM_LIST
        if (cur_save_pt >= (SET_PREV_MAX - 1))
        {
            //ѭ��buffer�ķ�ʽ���
            cur_save_pt = 0;
        }
        else
        {
            cur_save_pt++;
        }
        //��ǰ�л�����ʱ�Ļ���
        if (cur_save_prev_count < SET_PREV_MAX)
        {
            cur_save_prev_count++;
        }
        //���沥�Ź���16�����,�Ա����
        play_num_list[cur_save_pt] = temp_num;
    }

    return temp_num;
}

/******************************************************************************/
/*
 * \par  Description:��ȡshuffle���Ƶ���һ���ļ�
 * \uint16 fsel_shuffle_prev_file(void)
 * \param[in] none
 * \param[out]none

 * \return        file_no (1~file_total)
 * \ingroup       eh_fsel_shuffle.c
 * \note
 *******************************************************************************/
//shuffle mode  get prev
uint16 fsel_shuffle_prev_file(void)
{
    uint16 temp_num;

    if (shuffle_total < eh_file_total_all)
    {
        //�ļ����������(�����ļ�)�����³�ʼ��
        fsel_init_shuffle (eh_file_total_all);
    }
    else
    {
        //���¸�ֵ��ֹ file_total<shuffle_total�����(�ļ�ɾ��)
        shuffle_total = eh_file_total_all;
    }

    if (shuffle_total == 0)
    {
        return 0;
    }
    //����Ѿ�����˿ɻ����ļ�,��ӱ����б��в���
    while (cur_save_prev_count > 1)
    {
        temp_num = get_prevlist_num();

        if (temp_num <= shuffle_total)
        {
            if (temp_num != 0)
            {
                return temp_num;
            }
            else
            {
                break;
            }
        }

        cur_save_prev_count--;
    }

    //list can't find, use shuffle al
    //��֤���������ֵ��shuffle_total��Χ��,����file_no����ͬ
    do
    {
        temp_num = get_shuffle_num();
        if (temp_num == 0)
        {
            break;
        }

    }
    while ((filtrate_shuffle_no == temp_num) || (temp_num > shuffle_total));

    if (0 != temp_num)
    {
        //SAVE TO PLAY_NUM_LIST
        if (cur_save_pt == 0)
        {
            //ѭ��buffer�ķ�ʽ���
            cur_save_pt = SET_PREV_MAX - 1;
        }
        else
        {
            cur_save_pt--;
        }

        if (cur_save_next_count < SET_PREV_MAX)
        {
            cur_save_next_count++;
        }
        //���沥�Ź���16���ļ�
        play_num_list[cur_save_pt] = temp_num;

    }

    return temp_num;
}

