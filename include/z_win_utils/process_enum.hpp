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

namespace zl
{
namespace WinUtils
{
    typedef struct _ZLProcessEnumInfo
    {
        _ZLProcessEnumInfo()
        {
            dwTh32ProcessID = 0;
            dwCntThreads = 0;
            dwTh32ParentProcessID = 0;
            lpcPriClassBase = 0;
        }
        DWORD dwTh32ProcessID;                  ///> ����ID
        DWORD dwCntThreads;                     ///> �˽��̿������̼߳���
        DWORD dwTh32ParentProcessID;            ///> �����̵�ID
        LONG lpcPriClassBase;                   ///> �߳�����Ȩ
        CString cstrExeFile;                    ///> ����ȫ·��
    }ZLProcessEnumInfo;

    typedef std::vector<ZLProcessEnumInfo> ZLProcessEnumInfoVec;
    /**
     * @brief ����ö��
     */
    class ZLProcessEnum
    {
    public:
        /**
         * @brief ö�����н���
         * @param[in,out] infoVec ������н�����Ϣ������
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         */
        static BOOL Enum(ZLProcessEnumInfoVec& infoVec)
        {
            infoVec.clear();
            BOOL bRet = FALSE;
            HANDLE hSnapProc = INVALID_HANDLE_VALUE;
            do
            {
                hSnapProc = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                if (INVALID_HANDLE_VALUE == hSnapProc)
                {
                    break;
                }

                PROCESSENTRY32 pe32 = {0};
                pe32.dwSize = sizeof(pe32);
                if (!::Process32FirstW(hSnapProc, &pe32))
                {
                    break;
                }

                do
                {
                    ZLProcessEnumInfo proInfo;
                    proInfo.dwTh32ProcessID = pe32.th32ProcessID;
                    proInfo.dwCntThreads = pe32.cntThreads;
                    proInfo.dwTh32ParentProcessID = pe32.th32ParentProcessID;
                    proInfo.lpcPriClassBase = pe32.pcPriClassBase;
                    proInfo.cstrExeFile = pe32.szExeFile;
                    infoVec.push_back(proInfo);
                } while (::Process32NextW(hSnapProc, &pe32));
                bRet = TRUE;
            } while (FALSE);
            if (hSnapProc != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hSnapProc);
            }
            return bRet;
        }

        /**
         * @brief �ж�ָ�������Ƿ����
         * @param[in] lpFileName �����ļ���
         * @return ���ڷ���TRUE�������ڷ���FALSE��ö�ٽ���ʧ�ܷ���FALSE
         */
        static BOOL IsProcExist(LPCWSTR lpFileName)
        {
            BOOL bExist = FALSE;
            ZLProcessEnumInfoVec vecProc;
            ZLProcessEnum::Enum(vecProc);
            for (ZLProcessEnumInfoVec::const_iterator it = vecProc.begin();
                it != vecProc.end();
                ++it)
            {
                if (ZLPath::PathToFileName(it->cstrExeFile).CompareNoCase(lpFileName) == 0)
                {
                    bExist = TRUE;
                    break;
                }
            }
            return bExist;
        }
    };
}
}