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
 * @brief ���̵�ģ��ö��
 */

#pragma once
#include "win_utils_header.h"
#include "privilege.hpp"
#include <vector>
#include <tlhelp32.h>

namespace zl
{
namespace WinUtils
{
    typedef struct _ZLModuleEnumInfo
    {
        _ZLModuleEnumInfo()
        {
            dwTh32ModuleID = 0;
            dwGlblcntUsage = 0;
            dwProccntUsage = 0;
            byModBaseAddr = 0;
            dwModBaseSize = 0;
            hModule = NULL;
        }
        DWORD dwTh32ModuleID;               ///> ���������Ľ���������ģ���ʶ����
        DWORD dwGlblcntUsage;               ///> ȫ��ģ���ʹ�ü�����
        DWORD dwProccntUsage;               ///> �������̵ķ�Χ��ģ���ʹ�ü�����
        BYTE* byModBaseAddr;                ///> ��ַ��ģ�飬���������Ľ��̷�Χ�ڡ�
        DWORD dwModBaseSize;                ///> ��С�����ֽڣ�ģ�顣
        CString cstrModule;                 ///> ģ������
        CString cstrExePath;                ///> ģ���ȫ·����
        HMODULE hModule;                    ///> ���
    }ZLModuleEnumInfo;

    typedef std::vector<ZLModuleEnumInfo> ZLModuleEnumInfoVec;
    /**
     * @brief ����ģ��ö��
     */
    class ZLProcessModule
    {
    public:
        /**
         * @brief ö��ָ�����̵�ģ��
         * @param[in] dwPid   ����ID
         * @param[in] InfoVec ��Ž�����Ϣ������
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         */
        static BOOL Enum(DWORD dwPid, ZLModuleEnumInfoVec& InfoVec)
        {
            InfoVec.clear();
            BOOL bRet = FALSE;
            HANDLE hSnapMod = INVALID_HANDLE_VALUE;
            do 
            {
                hSnapMod = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
                if (INVALID_HANDLE_VALUE == hSnapMod)
                {
                    break;
                }

                MODULEENTRY32W me32 = {0};
                me32.dwSize = sizeof(me32);
                if (!::Module32FirstW(hSnapMod, &me32))
                {
                    break;
                }

                do
                {
                    ZLModuleEnumInfo modeInfo;
                    modeInfo.cstrExePath = me32.szExePath;
                    modeInfo.cstrModule = me32.szModule;
                    modeInfo.dwGlblcntUsage = me32.GlblcntUsage;
                    modeInfo.dwProccntUsage = me32.ProccntUsage;
                    modeInfo.dwModBaseSize = me32.modBaseSize;
                    modeInfo.byModBaseAddr = me32.modBaseAddr;
                    modeInfo.dwTh32ModuleID = me32.th32ModuleID;
                    modeInfo.hModule = me32.hModule;
                    InfoVec.push_back(modeInfo);
                    ::ZeroMemory(&me32, sizeof(me32));
                    me32.dwSize = sizeof(me32);
                } while (::Module32NextW(hSnapMod, &me32));
                bRet = TRUE;
            } while (FALSE);
            if (hSnapMod != INVALID_HANDLE_VALUE)
            {
                ::CloseHandle(hSnapMod);
            }
            return bRet;
        }
    };
}
}