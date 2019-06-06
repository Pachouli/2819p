
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    utf8�ַ���תunicode�ַ���
 * \param[in]    src��Դ�ַ���ָ��
 * \param[out]   dest��Ŀ���ַ���ָ��
 * \param[out]   size��ת����unicode���ֽ���
 * \return       none
 * \ingroup      string
 * \note
 *******************************************************************************/
void utf8_to_unicode(uint8* src, uint16*dest, uint16* size)
{
    uint8* pin = src;
    uint8 in_val;
    uint16 resultsize = 0;
    uint8 t1, t2, t3;
    uint8 tmpval[2] =
    { 0, 0 };
    while (*pin != 0)
    {
        in_val = *pin;
        if ((in_val > 0) && (in_val <= 0x7f))
        {
            tmpval[0] = in_val;
            tmpval[1] = 0;
        }
        else if ((in_val & (0xff << 5)) == 0xc0) //��2bit �Ƿ�Ϊ1
        {
            t1 = (uint8)(in_val & (0xff >> 3));//ȡ��5bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit
            //(t1�ĵ�2bit ���������bit,����t2�ĵ�6bit)
            tmpval[0] = (uint8)(t2 | ((t1 & (0xff >> 6)) << 6));
            tmpval[1] = t1 >> 2;//ȡ���м�3bit
        }
        else if ((in_val & (0xff << 4)) == 0xe0)
        {
            t1 = (uint8)(in_val & (0xff >> 3));//ȡ��5bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t3 = (uint8)(in_val & (0xff >> 2));//ȡ��6bit

            //Little Endian
            //(t2��2bit �������2bit,����t3�ĵ�6bit);
            tmpval[0] = (uint8)(((t2 & (0xff >> 6)) << 6) | t3);
            //(t1 �ĵ�4bit�����ֽڵĸ�4bit,����t2���м�4bit)
            tmpval[1] = (t1 << 4) | (t2 >> 2);
        }
        else
        {
            //do nothing for QAC
        }
        pin++;
        resultsize += 2;
        *dest = tmpval[1];
        *dest = *dest << 8;
        *dest |= tmpval[0];
        dest++;
    }
    resultsize += 2;
    *size = resultsize;
    *dest = 0;
}
/*! \endcond */

#endif
