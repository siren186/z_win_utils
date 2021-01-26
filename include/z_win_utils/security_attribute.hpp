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
 * @brief ��ȫ����
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ṩ�԰�ȫ���ԵĴ�����ɾ������
     */
    class ZLSecurityAttrabute
    {
    public:
         /**
         * @brief ������ȫ����
         * @return �ɹ����ش����İ�ȫ����ָ�룬ʧ�ܷ���NULL
         * @see InitializeSecurityDescriptor SetSecurityDescriptorDacl
         */
        static PSECURITY_ATTRIBUTES CreateSecurityAttribute()
        {
            BOOL bRet = FALSE;
            PSECURITY_ATTRIBUTES psa = (PSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
            PSECURITY_DESCRIPTOR psd = (PSECURITY_DESCRIPTOR)malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);

            if (!psa || !psd)
                goto cleanup;

            bRet = ::InitializeSecurityDescriptor(psd, SECURITY_DESCRIPTOR_REVISION);
            if (!bRet)
                goto cleanup;

            bRet = ::SetSecurityDescriptorDacl(psd, TRUE, NULL, FALSE);
            if (!bRet)
                goto cleanup;

            psa->nLength = sizeof(SECURITY_ATTRIBUTES);
            psa->bInheritHandle = TRUE;
            psa->lpSecurityDescriptor = psd;
cleanup:
            if (!bRet)
            {
                if (psd)
                {
                    free(psd);
                    psd = NULL;
                }
                if (psa)
                {
                    free(psa);
                    psa = NULL;
                }
            }
            return psa;
        }
        /**
         * @brief �ͷŴ����İ�ȫ����
         * @param[in] psa ��ȫ������ָ��
         */
        static void FreeSecurityAttribute(PSECURITY_ATTRIBUTES psa)
        {
            if (psa)
            {
                if (psa->lpSecurityDescriptor)
                {
                    free(psa->lpSecurityDescriptor);
                }
                free(psa);
            }
        }
    };

}
}
