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
 * @brief ���а�������
 */


#pragma once
#include "win_utils_header.h"
#include <string.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ṩ�Լ��а����ݵ����á���ȡ����
     */
    class ZLClipboard
    {
    public:
        /**
         * @brief ���ü��а�����
         * @param[in] pszData  ָ�򻺳�����ָ��
         * @param[in] nDataLen ����������
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         */
        static BOOL SetClipboard(const char* pszData, const int nDataLen)
        {
            if (::OpenClipboard(NULL))
            {
                ::EmptyClipboard();
                HGLOBAL hMem = ::GlobalAlloc(GMEM_DDESHARE, nDataLen + 1);
                if (hMem)
                {
                    char *buffer = (char *)::GlobalLock(hMem);
                    strcpy_s(buffer, nDataLen + 1, pszData);
                    ::GlobalUnlock(hMem);
                    ::SetClipboardData(CF_TEXT, hMem);
                }
                ::CloseClipboard();
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief ��ȡ���а�����
         * @return ���а�����,ʧ��Ϊ��
         */
        static CStringA GetClipboard()
        {
            CStringA sText;
            if (::IsClipboardFormatAvailable(CF_TEXT) && ::OpenClipboard(NULL))
            {
                HGLOBAL hMem = ::GetClipboardData(CF_TEXT);
                if (hMem)
                {
                    LPSTR lpStr = (LPSTR)::GlobalLock(hMem);
                    if (lpStr)
                    {
                        sText = lpStr;
                        ::GlobalUnlock(hMem);
                    }
                }
                ::CloseClipboard();
            }
            return sText;
        }
    };

}
}
