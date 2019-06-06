/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_Arabic.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00
* Description: support unicode font
********************************************************************************
*/
#include "lcd_inner.h"

#if (SUPPORT_SPI_LCD_DISPLAY == 1)

#define GUI_SUPPORT_BIDI 1
extern bool GUI_BIDI_IsNSM(uint16 c);

#ifdef GUI_SUPPORT_BIDI

typedef struct
{
    uint16 Isolated;
    uint16 Final;
    uint16 Initial;
    uint16 Medial;
}KEY_INFO;

const KEY_INFO _aKeyInfo[] =
{
    /*        Base      Isol.   Final   Initial Medial */
    { /* 0  0x0621 */ 0xFE80, 0x0000, 0x0000, 0x0000 /* Arabic letter Hamza                 */ },
    { /* 1  0x0622 */ 0xFE81, 0xFE82, 0x0000, 0x0000 /* Arabic letter Alef with Madda above */ },
    { /* 2  0x0623 */ 0xFE83, 0xFE84, 0x0000, 0x0000 /* Arabic letter Alef with Hamza above */ },
    { /* 3  0x0624 */ 0xFE85, 0xFE86, 0x0000, 0x0000 /* Arabic letter Waw with Hamza above  */ },
    { /* 4  0x0625 */ 0xFE87, 0xFE88, 0x0000, 0x0000 /* Arabic letter Alef with Hamza below */ },
    { /* 5  0x0626 */ 0xFE89, 0xFE8A, 0xFE8B, 0xFE8C /* Arabic letter Yeh with Hamza above  */ },
    { /* 6  0x0627 */ 0xFE8D, 0xFE8E, 0x0000, 0x0000 /* Arabic letter Alef                  */ },
    { /* 7  0x0628 */ 0xFE8F, 0xFE90, 0xFE91, 0xFE92 /* Arabic letter Beh                   */ },
    { /* 8  0x0629 */ 0xFE93, 0xFE94, 0x0000, 0x0000 /* Arabic letter Teh Marbuta           */ },
    { /* 9  0x062A */ 0xFE95, 0xFE96, 0xFE97, 0xFE98 /* Arabic letter Teh                   */ },
    { /* 10 0x062B */ 0xFE99, 0xFE9A, 0xFE9B, 0xFE9C /* Arabic letter Theh                  */ },
    { /* 11 0x062C */ 0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0 /* Arabic letter Jeem                  */ },
    { /* 12 0x062D */ 0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4 /* Arabic letter Hah                   */ },
    { /* 13 0x062E */ 0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8 /* Arabic letter Khah                  */ },
    { /* 14 0x062F */ 0xFEA9, 0xFEAA, 0x0000, 0x0000 /* Arabic letter Dal                   */ },
    { /* 15 0x0630 */ 0xFEAB, 0xFEAC, 0x0000, 0x0000 /* Arabic letter Thal                  */ },
    { /* 16 0x0631 */ 0xFEAD, 0xFEAE, 0x0000, 0x0000 /* Arabic letter Reh                   */ },
    { /* 17 0x0632 */ 0xFEAF, 0xFEB0, 0x0000, 0x0000 /* Arabic letter Zain                  */ },
    { /* 18 0x0633 */ 0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4 /* Arabic letter Seen                  */ },
    { /* 19 0x0634 */ 0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8 /* Arabic letter Sheen                 */ },
    { /* 20 0x0635 */ 0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC /* Arabic letter Sad                   */ },
    { /* 21 0x0636 */ 0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0 /* Arabic letter Dad                   */ },
    { /* 22 0x0637 */ 0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4 /* Arabic letter Tah                   */ },
    { /* 23 0x0638 */ 0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8 /* Arabic letter Zah                   */ },
    { /* 24 0x0639 */ 0xFEC9, 0xFECA, 0xFECB, 0xFECC /* Arabic letter Ain                   */ },
    { /* 25 0x063A */ 0xFECD, 0xFECE, 0xFECF, 0xFED0 /* Arabic letter Ghain                 */ },
    { /* 26 0x0641 */ 0xFED1, 0xFED2, 0xFED3, 0xFED4 /* Arabic letter Feh                   */ },
    { /* 27 0x0642 */ 0xFED5, 0xFED6, 0xFED7, 0xFED8 /* Arabic letter Qaf                   */ },
    { /* 28 0x0643 */ 0xFED9, 0xFEDA, 0xFEDB, 0xFEDC /* Arabic letter Kaf                   */ },
    { /* 29 0x0644 */ 0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0 /* Arabic letter Lam                   */ },
    { /* 30 0x0645 */ 0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4 /* Arabic letter Meem                  */ },
    { /* 31 0x0646 */ 0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8 /* Arabic letter Noon                  */ },
    { /* 32 0x0647 */ 0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC /* Arabic letter Heh                   */ },
    { /* 33 0x0648 */ 0xFEED, 0xFEEE, 0x0000, 0x0000 /* Arabic letter Waw                   */ },
    { /* 34 0x0649 */ 0xFEEF, 0xFEF0, 0x0000, 0x0000 /* Arabic letter Alef Maksura          */ },
    { /* 35 0x064A */ 0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4 /* Arabic letter Yeh                   */ },
    { /* 36 0x067E */ 0xFB56, 0xFB57, 0xFB58, 0xFB59 /* Eastern arabic letter Peh           */ },
    { /* 37 0x0686 */ 0xFB7A, 0xFB7B, 0xFB7C, 0xFB7D /* Eastern arabic letter Tcheh         */ },
    { /* 38 0x0698 */ 0xFB8A, 0xFB8B, 0x0000, 0x0000 /* Eastern arabic letter Jeh           */ },
    { /* 39 0x06A9 */ 0xFB8E, 0xFB8F, 0xFB90, 0xFB91 /* Eastern arabic letter Keheh         */ },
    { /* 40 0x06AF */ 0xFB92, 0xFB93, 0xFB94, 0xFB95 /* Eastern arabic letter Gaf           */ },
    { /* 41 0x06CC */ 0xFBFC, 0xFBFD, 0xFBFE, 0xFBFF /* Eastern arabic letter Farsi Yeh     */ },
};

/*********************************************************************/
uint16 _GetTableIndex(uint16 c)
{
    if (c < 0x621)
    {
        return 0;
    }
    if (c > 0x6cc)
    {
        return 0;
    }
    if (c <= 0x63a)
    {
        return c - 0x621;
    }
    if ((c >= 0x641) && (c <= 0x64a))
    {
        return c - 0x641 + 26;
    }
    if (c == 0x67e)
    {
        return 36;
    }
    if (c == 0x686)
    {
        return 37;
    }
    if (c == 0x698)
    {
        return 38;
    }
    if (c == 0x6a9)
    {
        return 39;
    }
    if (c == 0x6af)
    {
        return 40;
    }
    if (c == 0x6cc)
    {
        return 41;
    }
    return 0;
}

/*********************************************************************/
uint16  _GetLigature(uint16 Char, uint16 Next, uint16 PrevAffectsJoining)
{
    if (((Next != 0x622) && (Next != 0x623) && (Next != 0x625) && (Next != 0x627)) || (Char != 0x644))
    {
        return 0;
    }
    if (PrevAffectsJoining != 0)
    {
        switch (Next)
        {
        case 0x622:
            return 0xfef6;
        case 0x623:
            return 0xfef8;
        case 0x625:
            return 0xfefa;
        case 0x627:
            return 0xfefc;
        }
    }
    else
    {
        switch (Next)
        {
        case 0x622:
            return 0xfef5;
        case 0x623:
            return 0xfef7;
        case 0x625:
            return 0xfef9;
        case 0x627:
            return 0xfefb;
        }
    }
    return 0;
}

/*********************************************************************/
uint16 GUI__IsArabicCharacter(uint16 c)
{
    return ((c >= 0x600) && (c <= 0x6ff)) ? 1 : 0;
}

/*********************************************************************
*
*       GUI__GetPresentationForm
*
* Purpose:
*   Returns the Arabic presentation form of the given character(s).
*   Depending on their position in the text the presentation form of
*   Arabic characters differr from their character codes. So the function
*   needs the previous and the next character code to calculate the right
*   representation form.
*   Some sequences of 2 characters are formed to one ligature. In this
*   case the routine uses the pointer pIgnoreNextCharacter to inform the
*   calling routine that the next character should be ignored.
*
* Parameters:
*   Char                 - Current character
*   Next                 - Previous character
*   Prev                 - Next character following the current character
*   pIgnoreNextCharacter - Pointer to inform calling routine that the
*                          next character should be ignored.
*                        - Pointer for decoding text data. Need if 'Next' is a Non Spacing Mark.
*/
uint16 GUI__GetPresentationForm(uint16 Char, uint16 Next, uint16 Prev, uint16 * pIgnoreNext  , uint16 NNext)
{
    uint16 CharIndex, NextAffectsJoining, PrevAffectsJoining, Final, Initial, Medial, Ligature;
    //static int PrevAffectsNextJoining;
    CharIndex = _GetTableIndex(Char);
    if (!CharIndex)
    {

        if (!GUI_BIDI_IsNSM(Char))
        {
            PrevAffectsNextJoining = 0;
        }

        return Char;
    }
    PrevAffectsJoining = PrevAffectsNextJoining;
    Ligature = _GetLigature(Char, Next, PrevAffectsJoining);
    if (!Ligature)
    {
        Ligature = _GetLigature(Prev, Char, PrevAffectsJoining);
    }
    if (Ligature != 0)
    {
        if (pIgnoreNext != NULL)
        {
            *pIgnoreNext = 1;
        }
        PrevAffectsNextJoining = 0;
        return Ligature;
    }
    else
    {
        if (pIgnoreNext != NULL)
        {
            *pIgnoreNext = 0;
        }
    }


    if (GUI_BIDI_IsNSM(Next) == TRUE)
    {
        if (Char != 0)
        {
            Next = NNext;
        }
    }

    NextAffectsJoining = _GetTableIndex(Next) && _aKeyInfo[CharIndex].Medial;
    if ((!PrevAffectsJoining) && (!NextAffectsJoining))
    {
        Char = _aKeyInfo[CharIndex].Isolated;
        PrevAffectsNextJoining = 0;
    }
    else if ((!PrevAffectsJoining) && (NextAffectsJoining != 0))
    {
        Initial = _aKeyInfo[CharIndex].Initial;
        PrevAffectsNextJoining = 1;
        if (Initial)
        {
            Char = Initial;
        }
        else
        {
            Char = _aKeyInfo[CharIndex].Isolated;
        }
    }
    else if ((PrevAffectsJoining != 0) && (NextAffectsJoining != 0))
    {
        Medial = _aKeyInfo[CharIndex].Medial;
        PrevAffectsNextJoining = 1;
        if (Medial)
        {
            Char = Medial;
        }
        else
        {
            Char = _aKeyInfo[CharIndex].Isolated;
        }
    }
    else if ((PrevAffectsJoining != 0) && (!NextAffectsJoining))
    {
        Final = _aKeyInfo[CharIndex].Final;
        PrevAffectsNextJoining = 0;
        if (Final != 0)
        {
            Char = Final;
        }
        else
        {
            Char = _aKeyInfo[CharIndex].Isolated;
        }
    }
    else
    {
        //for QAC
    }
    return Char;
}

uint8 UnicodeTOArabic(uint8 *str,uint8 length)
{
    uint16 IgnoreNextCharacter = 0;
    uint16 CurChar, NextChar = 0, PrevChar = 0,NNextChar=0;
    uint16 CharIsArabic;
    uint16 Glyph;
    uint8 i=0, j=0;
    uint8 total_len = length + length;
    uint8 result_length = 0;
    uint8 link_length = 0;
    uint8 *ptr;

    //����ܹ����26����д����һ��
    if(total_len > (AuxiliaryBufferLen - 2))
    {
        total_len = AuxiliaryBufferLen - 2;
    }
    while (i<total_len)
    {
        ptr = str + i;
        CurChar = (uint16)( (ptr[1]<<8) | ptr[0] );

        if(CurChar == 0x00)
        {
            link_length = result_length;
            break;
        }

        ptr = str + i + 2;
        NextChar = (uint16)( (ptr[1]<<8) | ptr[0] );

        ptr = str + i + 4;
        NNextChar = (uint16)( (ptr[1]<<8) | ptr[0] );

        if (IgnoreNextCharacter != 0)
        {
            IgnoreNextCharacter = 0;
            i = i + 2;
        }
        else
        {
            CharIsArabic = GUI__IsArabicCharacter(CurChar);
            if (CharIsArabic != 0)
            {
                Glyph = GUI__GetPresentationForm(CurChar, NextChar, PrevChar, &IgnoreNextCharacter,NNextChar);
                PrevChar = CurChar;
            }
            else
            {
                Glyph = CurChar;
                PrevChar = 0 ;
                link_length = result_length + 1;
            }

            ptr = str + j;
            ptr[0] = (uint8)(Glyph & 0xff);
            ptr[1] = (uint8)((Glyph>>8) & 0xff);

            i = i + 2;
            j = j + 2;
            result_length ++;
        }
    }

    ptr = str + j;
    ptr[0] = 0;
    ptr[1] = 0;

    return link_length;
}

#else

void GUI__Arabic_C(void);
void GUI__Arabic_C(void)
{}

#endif

#endif
