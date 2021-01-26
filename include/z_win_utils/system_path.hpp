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
 * @brief ϵͳ���·������
 */


#pragma once
#include "win_utils_header.h"
#include "path.hpp"
#include "wow64.hpp"
#include <shlobj.h>

namespace zl
{
namespace WinUtils
{
     /**
     * @brief ��ȡϵͳ��ص�Ŀ¼
     * @note �ӿ��б�����:
     *      GetSystemDir                    windows\system32
     *      GetWindowsDir                   windows
     *      GetCommonAppDataDir             All Users\Application Data
     *      GetCommonTempDir                All Users\Templates
     *      GetAppDataDir                   <user name>\Application Data
     *      GetTempDir                      <user name>\Templates
     *      GetProgramFileDir               Program Files
     */
    /**
     * @brief ��ȡ������ϵͳ���·��
     */
    class ZLSystemPath
    {
    public:
        ZLSystemPath() {}
        ~ZLSystemPath() {}

    public:
        /**
         * @brief ��ȡϵͳĿ¼
         * @return �ɹ�����ϵͳĿ¼��ʧ�ܷ��ؿմ�
         * @see GetSystemDirectory
         */
        static CString GetSystemDir()
        {
            CString cstrRet;
            int nLen = ::GetSystemDirectory(NULL, 0);
            if (nLen > 0)
            {
                ::GetSystemDirectory(cstrRet.GetBuffer(nLen + 1), nLen);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }
        /**
         * @brief ��ȡWindowsĿ¼
         * @return �ɹ�����WindowsĿ¼��ʧ�ܷ��ؿմ�
         * @see GetWindowsDirectory
         */
        static CString GetWindowsDir()
        {
            CString cstrRet;
            int nLen = ::GetWindowsDirectory(NULL, 0);
            if (nLen > 0)
            {
                ::GetWindowsDirectory(cstrRet.GetBuffer(nLen + 1), nLen);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }
        /**
         * @brief ��ȡCommonAppDataĿ¼
         * @return �ɹ�����CommonAppDataĿ¼��ʧ�ܷ��ؿմ�
         * @see SHGetSpecialFolderPath
         */
        static CString GetCommonAppDataDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_COMMON_APPDATA,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }
        /**
         * @brief ��ȡAppDataĿ¼
         * @return �ɹ�����AppDataĿ¼��ʧ�ܷ��ؿմ�
         * @see SHGetFolderPath
         */
        static CString GetAppDataDir(HANDLE hToken = NULL)
        {
            CString cstrRet;
            if (hToken != NULL)
            {
                ::SHGetFolderPath(NULL,
                    CSIDL_APPDATA,
                    hToken,
                    SHGFP_TYPE_CURRENT,
                    cstrRet.GetBuffer(MAX_PATH + 1));
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            else
            {
                ::SHGetSpecialFolderPath(NULL,
                    cstrRet.GetBuffer(MAX_PATH + 1),
                    CSIDL_APPDATA,
                    FALSE);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }
        /**
         * @brief ��ȡCommonTempĿ¼
         * @return �ɹ�����CommonTempĿ¼��ʧ�ܷ��ؿմ�
         * @see SHGetSpecialFolderPath
         */
        static CString GetCommonTempDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_COMMON_TEMPLATES,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }
        /**
         * @brief ��ȡTempĿ¼
         * @return �ɹ�����TempĿ¼��ʧ�ܷ��ؿմ�
         * @see SHGetFolderPath
         */
        static CString GetTempDir(HANDLE hToken = NULL)
        {
            CString cstrRet;
            if (hToken != NULL)
            {
                ::SHGetFolderPath(NULL,
                    CSIDL_TEMPLATES,
                    hToken,
                    SHGFP_TYPE_CURRENT,
                    cstrRet.GetBuffer(MAX_PATH + 1));
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            else
            {
                ::SHGetSpecialFolderPath(NULL,
                    cstrRet.GetBuffer(MAX_PATH + 1),
                    CSIDL_TEMPLATES,
                    FALSE);
                cstrRet.ReleaseBuffer();
                ZLPath::PathAddBackslash(cstrRet);
            }
            return cstrRet;
        }
        /**
         * @brief ��ȡProgramFileĿ¼
         * @return �ɹ�����ProgramFileĿ¼��ʧ�ܷ��ؿմ�
         * @see SHGetFolderPath
         */
        static CString GetProgramFileDir()
        {
            CString cstrRet;
            ::SHGetSpecialFolderPath(NULL,
                cstrRet.GetBuffer(MAX_PATH + 1),
                CSIDL_PROGRAM_FILES,
                FALSE);
            cstrRet.ReleaseBuffer();
            ZLPath::PathAddBackslash(cstrRet);
            return cstrRet;
        }
    };

}
}