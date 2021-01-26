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
 * @brief cpu��Ϣ
 */


#pragma once
#include "win_utils_header.h"
#include "register.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief cpu��Ϣ���
     */
    class ZLCpu
    {
    public:
        /**
         * @brief ��ȡ����������
         * @return ���ش���������
         */
        static int GetProcessorsCount()
        {
            SYSTEM_INFO info = {0};
            ::GetSystemInfo(&info);
            return info.dwNumberOfProcessors;
        }

        /**
         * @brief ��ȡ��������Ƶ
         * @return ���ش�������Ƶ����MHzΪ��λ
         */
        static DWORD GetCpuMHz()
        {
            DWORD dwMHz = 0;
            ZLRegister reg;
            if (reg.Open(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"))
            {
                reg.GetDwordValue(L"~MHz", dwMHz);
            }
            return dwMHz;
        }
    };

}
}
