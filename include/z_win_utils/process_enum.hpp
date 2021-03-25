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
 * @brief ����ö��
 */


#pragma once
#include "win_utils_header.h"
#include "privilege.hpp"
#include "process.hpp"
#include "path.hpp"
#include <vector>
#include <TlHelp32.h>

namespace zl
{
namespace WinUtils
{
    /**
     * @brief ����ö��
     */
    class ZLProcessEnum
    {
    public:

        /**
         * @brief ö�����н���
         * @param[out] vec ö�ٵ������н����б�
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         */
        static HRESULT Enum(std::vector<PROCESSENTRY32>& vec)
        {
            HRESULT hr = S_OK;
            HANDLE hSnapshot = INVALID_HANDLE_VALUE;

            vec.clear();

            do
            {
                hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (INVALID_HANDLE_VALUE == hSnapshot)
                {
                    DWORD dwErr = ::GetLastError();
                    hr = HRESULT_FROM_WIN32(dwErr);
                    break;
                }

                PROCESSENTRY32 pe32 = {0};
                pe32.dwSize = sizeof(pe32);
                if (!::Process32First(hSnapshot, &pe32))
                {
                    break;
                }

                do
                {
                    vec.push_back(pe32);
                } while (::Process32Next(hSnapshot, &pe32));
            } while (FALSE);

            if (hSnapshot != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hSnapshot);
            }

            return hr;
        }

        /**
         * @brief �ж�ָ�������Ƿ����
         * @param[in] lpFileName �����ļ���(������·��)�������ִ�Сд
         * @param[out] pInfo ��ƥ���еĽ��̻�����Ϣ
         * @return ���ڷ���TRUE�������ڷ���FALSE
         * @attention ����ж��������֮ƥ��ʱ�������ص�һ����֮ƥ��Ľ�����Ϣ
         */
        static BOOL IsProcessExist(LPCTSTR lpFileName, PROCESSENTRY32* pInfo = NULL)
        {
            if (!lpFileName)
            {
                return FALSE;
            }

            std::vector<PROCESSENTRY32> vec;
            ZLProcessEnum::Enum(vec);
            for (std::vector<PROCESSENTRY32>::const_iterator it = vec.begin(); it != vec.end(); ++it)
            {
                if (0 == ::StrCmpI(lpFileName, it->szExeFile))
                {
                    if (pInfo)
                    {
                        *pInfo = *it;
                    }
                    return TRUE;
                }
            }

            return FALSE;
        }
    };
}
}