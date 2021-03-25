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
 * @brief 进程枚举
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
     * @brief 进程枚举
     */
    class ZLProcessEnum
    {
    public:

        /**
         * @brief 枚举所有进程
         * @param[out] vec 枚举到的所有进程列表
         * @return 成功返回TRUE，失败返回FALSE
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
         * @brief 判断指定进程是否存在
         * @param[in] lpFileName 进程文件名(不包含路径)，不区分大小写
         * @param[out] pInfo 被匹配中的进程基本信息
         * @return 存在返回TRUE，不存在返回FALSE
         * @attention 如果有多个进程与之匹配时，仅返回第一个与之匹配的进程信息
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