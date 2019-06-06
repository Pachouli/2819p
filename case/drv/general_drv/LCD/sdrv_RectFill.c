/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : Sdrv_RectFill.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

/*
*******************************************************************************
*
** FUNCTION:     CheckDisplaySpace
**
** Description:  �����Ļ�ռ��Ƿ�������ʾҪ���糬����Χ�򱨴�
**
**  input
**     codemod: �����ģʽ���ַ������ͼƬ����
**
**  output
**     True: ��Ļ�ռ���������ʾҪ��
**     False: ��Ļ�ռ䲻��������ʾҪ��
********************************************************************************
*/
bool CheckDisplaySpace_Rect(uint8 codemod,uint8 fontwidth)
{
    uint8 justwidth;
    
    switch(codemod)
    {
    case UnicodeMode:
        justwidth = fontwidth;
        if( (StrEndX != 0) && ( (region.x+justwidth) > StrEndX))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X�ᳬ��
            return FALSE;
        }
        goto NextStep;
    case PicDisMode:
        justwidth = region.width;
        NextStep:
        //�ж��Ƿ�X���y�ᳬ��
        if ((region.x+justwidth) > (region_win.x+region_win.width))
        {
//          DUMP(RESULT_PUTCHAR_X_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_X_ERR;//X�ᳬ��
            return FALSE;
        }
        if ((region.y+region.height) > (region_win.y+region_win.height))
        {
//          DUMP(RESULT_PUTCHAR_Y_ERR);
            //PutCharErrFlag = RESULT_PUTCHAR_Y_ERR;//Y�ᳬ��
            return FALSE;
        }
        break;
    default:
//      DUMP(RESULT_PUTCHAR_OTHER_ERR);
        //PutCharErrFlag = RESULT_PUTCHAR_OTHER_ERR;//��֪���͵�Ҳ����ʾ
        return FALSE;
    }
    
    return TRUE;
}

////*******************************************************************************/
///* ��ָ���������ɻ�����ɫ
//** FUNCTION:     Sdrv_FillRect
//**
//** Description:  
//**
//**  input
//**        region_t * Fillregion:Ҫ�������� 
//**        
//**  output
//**     none
//********************************************************************************/

void Sdrv_FillRect (region_t * Fillregion)
{

    uint16 pixelcnt;
    libc_memcpy(&region, Fillregion, sizeof(region_t));

    if ( (region.x > (Display_Length-1))
      || (region.y > (Display_Height-1)) )
    {
        return; //���곬�����˳�
    }     

    if (CheckDisplaySpace_Rect(PicDisMode,0) == TRUE) //�����ʾ�����Ƿ񳬳�
    {
        Hdrv_SetWindow(&region);//�趨���� //�趨 Ҫ�������ݵ� ��ʼ��ַ �� ������ַ
        pixelcnt = region.width * region.height;
        //RectBuffFillColor(DisplayBufLen/2);
        while(pixelcnt > (DisplayBufLen/2))
        {
            //�ҳ�����PAGE���ͳ�
//            RectBuffFillColor(DisplayBufLen/2);
            Hdrv_BuffDataTrans(DisplayBuffer, DisplayBufLen);//����ʱ�Դ���DisplayBuffer����������͵�LCM
            //����ֻ�� �������ݣ����ܵط�����Ϊ����֮ǰ Hdrv_SetWindow �Ѿ��趨������ʼλ�õص�
            
            pixelcnt = pixelcnt - (DisplayBufLen/2);
        }
        //��ʣ��������ͳ�
//        RectBuffFillColor(pixelcnt);
        Hdrv_BuffDataTrans(DisplayBuffer, pixelcnt*2);//����ʱ�Դ����������͵�LCM                
    }    
    
    return;
}

#endif
