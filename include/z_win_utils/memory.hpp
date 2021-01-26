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
 * @brief �ڴ����
 */


#pragma once
#include "win_utils_header.h"
#include "register.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief ��ȡ�ڴ��С
     */
    class ZLMemory
    {
    public:
        /**
         * @brief ��ȡ�ڴ��С����MBΪ��λ
         * @return �ɹ������ڴ��С(MB)
         */
        static ULONG GetMemorySize()
        {
            MEMORYSTATUSEX statex = {0};
            statex.dwLength = sizeof(statex);
            ::GlobalMemoryStatusEx(&statex);
            return (ULONG)((statex.ullTotalPhys) / (1024 * 1024));
        }

        /**
         * @brief ��ȡ��ʹ���ڴ��С����MBΪ��λ
         * @return �ɹ������ڴ��С(MB)
         */
        static ULONGLONG GetUsedMem()
        {
            MEMORYSTATUSEX statex = {0};
            statex.dwLength = sizeof(statex);
            ::GlobalMemoryStatusEx(&statex);
            return (ULONG)((statex.ullTotalPhys - statex.ullAvailPhys) / (1024 * 1024));
        }
    };

}
}
