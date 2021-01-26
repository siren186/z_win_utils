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
 * @brief �ļ���Ϣ���
 * 
 *      �ļ���Ϣ������С,�޸�ʱ���.
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ļ���Ϣ��غ�������
     */
    class ZLFileInfo
    {
    public:

        /**
         * @brief ��ȡ�ļ���С
         * @param[in] lpFilePath �ļ�·��
         * @param[out] lSize     ���ڴ洢�ļ��Ĵ�С��Ϣ
         * @return �ɹ�����TRUE, ����lSize�д�ŵ����ļ��Ĵ�С��Ϣ, ʧ�ܷ���FALSE.
         */
        static BOOL GetFileSize(LPCTSTR lpFilePath, LONGLONG &lSize)
        {
            BOOL bReturn = FALSE;

            HANDLE hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE |FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

            if (INVALID_HANDLE_VALUE != hFile)
            {
                LARGE_INTEGER lFileSize = {0};
                if (::GetFileSizeEx(hFile, &lFileSize))
                {
                    lSize = lFileSize.QuadPart;
                    bReturn = TRUE;
                }

                ::CloseHandle(hFile);
            }

            return bReturn;
        }

        /**
         * @brief ��ȡ�ļ��Ĵ���ʱ��,����ʱ�估�޸�ʱ��
         * @param[in]  lpFilePath   �ļ�·��
         * @param[out] pftCreate    ���ڴ洢�õ��Ĵ���ʱ��,����Ҫ����NULL
         * @param[out] pftAccess    ���ڴ洢�õ��ķ���ʱ��,����Ҫ����NULL
         * @param[out] pftWrite     ���ڴ洢�õ����޸�ʱ��,����Ҫ����NULL
         * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
         */
        static BOOL GetFileTimeInfo(
            LPCTSTR   lpFilePath,
            PFILETIME pftCreate,
            PFILETIME pftAccess,
            PFILETIME pftWrite)
        {
            BOOL bReturn = FALSE;

            HANDLE hFile = ::CreateFile(
                lpFilePath,
                GENERIC_READ, 
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
                NULL, 
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL, 
                NULL);

            if (hFile != INVALID_HANDLE_VALUE)
            {
                bReturn = ::GetFileTime(hFile, pftCreate, pftAccess, pftWrite);

                ::CloseHandle(hFile);
            }

            return bReturn;
        }
    };
}
}
