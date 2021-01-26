/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/

/**
 * @file
 * @brief ��������ɫ
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    typedef enum _emConsoleColor
    {
        emConsoleColourBlack            = 0,    ///>0= ��ɫ
        emConsoleColourBlue             = 1,    ///>1= ��ɫ
        emConsoleColourGreen            = 2,    ///>2= ��ɫ
        emConsoleColourLightBlue        = 3,    ///>3= ����ɫ
        emConsoleColourRed              = 4,    ///>4= ��ɫ
        emConsoleColourPurple           = 5,    ///>5= ��ɫ
        emConsoleColourYellow           = 6,    ///>6= ��ɫ
        emConsoleColourWhite            = 7,    ///>7= ��ɫ
        emConsoleColourGray             = 8,    ///>8= ��ɫ
        emConsoleColourBabyBlue         = 9,    ///>9= ����ɫ
        emConsoleColourPaleGreen        = 10,   ///>A= ����ɫ
        emConsoleColourPaleLightGreen   = 11,   ///>B= ��ǳɫ
        emConsoleColourPompadour        = 12,   ///>C= ����ɫ
        emConsoleColourLavender         = 13,   ///>D= ����ɫ
        emConsoleColourFaintYellow      = 14,   ///>E= ����ɫ
        emConsoleColourBrightWhite      = 15,   ///>F= ����ɫ
    }emConsoleColor;

    /**
     * @brief ��������ɫ����
     */
    class ZLConsoleColor
    {
    public:
        ZLConsoleColor() {}
        ~ZLConsoleColor() {}

        /**
         * @brief ����������ǰ��ɫ
         * @param[in] color emConsoleColorö��ֵ
         * @return �ɹ�����TRUE,ʧ��FALSE
         * @see emConsoleColor
         */
        static BOOL SetConsoleForegroundColor(emConsoleColor color)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                WORD wColor = GetConsoleColor() & 0xf0;
                return ::SetConsoleTextAttribute(hOutPutHandle, wColor | (BYTE)color);
            }
            return FALSE;
        }

        /**
         * @brief ���������б���ɫ
         * @param[in] color emConsoleColorö��ֵ
         * @return �ɹ�����TRUE,ʧ��FALSE
         * @see emConsoleColor
         */
        static BOOL SetConsoleBackgroundColor(emConsoleColor color)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                WORD wColor = GetConsoleColor() & 0xf;
                return ::SetConsoleTextAttribute(hOutPutHandle, wColor | ((WORD((BYTE)color) << 4)));
            }
            return FALSE;
        }

        /**
         * @brief ����Ĭ������������
         * @return �ɹ�����TRUE, ʧ�ܷ���FALSE
         */
        static BOOL SetColorFontDefault()
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                return ::SetConsoleTextAttribute(hOutPutHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
            }
            return FALSE;
        }

        /**
         * @brief ��ȡ��������ɫ
         * @return ��ɫֵ,���CONSOLE_SCREEN_BUFFER_INFO�ṹ�ĳ�ԱwAttributes
         */
        static WORD GetConsoleColor()
        {
            WORD wRet = 0;
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo = {0};
                if (::GetConsoleScreenBufferInfo(hOutPutHandle, &consoleInfo))
                {
                    wRet = consoleInfo.wAttributes;
                }
            }
            return wRet;
        }
    };

}
}