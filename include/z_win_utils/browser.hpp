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
 * @brief ��������
 */


#pragma once
#include "win_utils_header.h"
#include "system_path.hpp"
#include "file_version.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief ��������
     */
    class ZLBrowser
    {
    public:
        /**
         * @brief �ж��Ƿ����û��������
         * @param[in] lpParentProcess ������·��
         * @return �ɹ�����TRUE,ʧ��FALSE
         */
        static BOOL IsUserLaunchBrowser(LPCTSTR lpParentProcess)
        {
            if (!lpParentProcess)
            {
                return FALSE;
            }
            CString sExplorer = ZLSystemPath::GetWindowsDir()  + L"explorer.exe";
            return (sExplorer.CollateNoCase(lpParentProcess) == 0);
        }

        /**
         * @brief ��ȡĬ�������
         * @param[out] strDefaultBrowser ���ص�Ĭ�������·��
         * @return �ɹ�����TRUE,ʧ��FALSE
         */
        static BOOL GetDefaultBrowser(CString& strDefaultBrowser)
        {
            BOOL bResult = FALSE;
            LPWSTR pDefaultBrowser = NULL;
            ULONG ulSize = 0;
            HRESULT hResult = E_FAIL;
            CComPtr<IQueryAssociations> pQa;

            hResult = ::AssocCreate(CLSID_QueryAssociations, IID_IQueryAssociations, (LPVOID*)&pQa);
            if (FAILED(hResult) || !pQa)
            {goto _abort;}

            hResult = pQa->Init(0, L"http", NULL, NULL);
            if (FAILED(hResult))
            {goto _abort;}

            hResult = pQa->GetString(0, ASSOCSTR_COMMAND, NULL, NULL, &ulSize);
            if (S_FALSE != hResult || 0 == ulSize)
            {goto _abort;}

            pDefaultBrowser = new WCHAR[ulSize + 1];
            if (!pDefaultBrowser)
            {goto _abort;}

            RtlZeroMemory(pDefaultBrowser, (ulSize + 1) * sizeof(WCHAR));

            hResult = pQa->GetString(0, ASSOCSTR_COMMAND, NULL, pDefaultBrowser, &ulSize);
            if (FAILED(hResult) || 1 > ulSize)
            {goto _abort;}

            ::PathRemoveArgsW(pDefaultBrowser);
            ::PathUnquoteSpacesW(pDefaultBrowser);

            if (!::PathFileExistsW(pDefaultBrowser))
            {goto _abort;}

            strDefaultBrowser = pDefaultBrowser;
            bResult = TRUE;
_abort:
            if (pDefaultBrowser)
            {
                delete[] pDefaultBrowser;
                pDefaultBrowser = NULL;
            }
            return bResult;
        }

        /**
         * @brief ��ȡIE�汾
         * @return ���ذ汾��, ʧ��Ϊ0
         */
        static unsigned int GetIEMajorVersion()
        {
            static unsigned int s_version = UINT_MAX;
            if (s_version == UINT_MAX)
            {
                CString sIe = ZLSystemPath::GetProgramFileDir() + L"internet explorer\\iexplore.exe";
                ZLFileVersion fv;
                if (fv.Create(sIe))
                {
                    s_version = fv.GetFileVersion(3);
                }
                else
                {
                    s_version = 0;
                }
            }
            return s_version;
        }
    };

}
}
