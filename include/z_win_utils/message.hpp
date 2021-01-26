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
 * @brief ������Ϣ���
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief ������Ϣ���
     */
    class ZLMessage
    {
    public:
        /**
         * @brief �μ�MSDN��RegisterWindowMessage����
         */
        static UINT RegisterMessage(LPCTSTR lpMsgName)
        {
            return ::RegisterWindowMessage(lpMsgName);
        }

        /**
         * @brief ��UIPI�У���ӻ�ɾ��һ����Ϣ
         * @param[in] uMsg   Ҫ���The message to add to or remove from the filter.
         * @param[in] dwOper ָ����������
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see ChangeWindowMessageFilter
         */
        static BOOL ChangeMessageFilter(UINT uMsg, DWORD dwOper = 1 /* MSGFLT_ADD */ )
        {
            typedef BOOL (__stdcall *ChangeWindowMessageFilterType)(UINT, DWORD);

            BOOL bRet = FALSE;
            static HMODULE hModule = NULL;
            static ChangeWindowMessageFilterType pChangeWindowMessageFilterType = NULL;
            if (pChangeWindowMessageFilterType == NULL)
            {
                hModule = ::LoadLibrary(TEXT("user32.dll"));
                if (hModule == NULL)
                    goto Exit0;
                pChangeWindowMessageFilterType = (ChangeWindowMessageFilterType)::GetProcAddress(hModule, "ChangeWindowMessageFilter");
                if (pChangeWindowMessageFilterType == NULL)
                    goto Exit0;
            }
            bRet = pChangeWindowMessageFilterType(uMsg, dwOper);
    Exit0:
            return bRet;
        }
    };


}
}
