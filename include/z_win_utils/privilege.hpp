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
 * @brief 进程权限相关
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 调整进程权限
     */
    class ZLPrivilege
    {
    public:
        /**
        * @brief 调整当前进程权限
        * @param[in] szPrivileges 权限名
        * @return 成功返回TRUE，失败返回FALSE
        * @see OpenProcessToken LookupPrivilegeValue AdjustTokenPrivileges
        */
        static BOOL GetPrivileges(LPCTSTR szPrivileges = SE_DEBUG_NAME)
        {
            BOOL bRet = FALSE;
            HANDLE hToken = NULL;
            LUID CurrentLUID = { 0 };
            TOKEN_PRIVILEGES TokenPrivileges = { 0 };
            __try
            {
                bRet = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
                if (!bRet) __leave;

                bRet = ::LookupPrivilegeValue(NULL, szPrivileges, &CurrentLUID);
                if (!bRet) __leave;

                TokenPrivileges.PrivilegeCount = 1;
                TokenPrivileges.Privileges[0].Luid = CurrentLUID;
                TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                bRet = ::AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);
                if (!bRet) __leave;

                bRet = TRUE;
            }
            __finally
            {
                if (hToken)
                {
                    ::CloseHandle(hToken);
                    hToken = NULL;
                }
            }
            return bRet;
        }

        /**
         * @brief 判断指定进程是否具有管理员或者管理员以上的权限
         * @param[in] dwProcessId 进程ID
         */
        static BOOL IsProcessRunAsAdmin(DWORD dwProcessId)
        {
            HANDLE hProcess = NULL;
            HANDLE hToken = NULL;
            BOOL bElevated = FALSE;

            do
            {
                // 打开进程, 获取进程句柄
                hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
                if (NULL == hProcess)
                {
                    break;
                }

                // 打开进程令牌, 获取进程令牌句柄
                if (FALSE == ::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
                {
                    break;
                }

                // 获取进程令牌特权提升信息
                DWORD dwRet = 0;
                TOKEN_ELEVATION tokenEle = { 0 };
                if (FALSE == ::GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRet))
                {
                    break;
                }

                // 获取进程是否提升的结果
                bElevated = tokenEle.TokenIsElevated;
            } while (false);

            if (hToken)
            {
                ::CloseHandle(hToken);
                hToken = NULL;
            }

            if (hProcess)
            {
                ::CloseHandle(hProcess);
                hProcess = NULL;
            }

            return bElevated;
        }
    };

}
}
