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
 * @brief Ŀ¼/�ļ������
 */


#pragma once
#include "win_utils_header.h"
#include "path.hpp"

namespace zl
{
namespace WinUtils
{

    /**
     * @brief �ṩ��Ŀ¼�Ĵ������޸ĺ�ɾ������
     */
    class ZLDirectory
    {
    public:
        /**
         * @brief ����Ŀ¼
         * @param[in] szPath ·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL CreateDeepDirectory(LPCTSTR lpPath)
        {
            if (!lpPath)
            {
                return FALSE;
            }

            BOOL bRetCode = FALSE;
            CString strPath(lpPath);
            if (::GetFileAttributes(lpPath) != INVALID_FILE_ATTRIBUTES)
                return TRUE;
            bRetCode = ::CreateDirectory(lpPath, NULL);
            if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
            {
                strPath = ZLPath::PathToParentDir(strPath);
                if (strPath.IsEmpty()) return FALSE;

                bRetCode = CreateDeepDirectory(strPath);
                if (!bRetCode) return FALSE;

                bRetCode = ::CreateDirectory(lpPath, NULL);
                if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
                    return FALSE;
            }
            return TRUE;
        }

        /**
         * @brief ɾ��Ŀ¼
         * @param[in] szDir             ·��
         * @param[in] bContinueWhenFail ɾ��ĳ���ļ�ʧ��ʱ,�Ƿ����
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL DeleteDirectory(LPCTSTR lpDir, BOOL bContinueWhenFail = TRUE)
        {
            if (!lpDir)
            {
                return FALSE;
            }

            BOOL bReturn = FALSE;
            CString strDir = ZLPath::PathAddBackslash(lpDir);
            CString sFindPath;
            sFindPath.Format(_T("%s*.*"), strDir);
            WIN32_FIND_DATA fData;
            HANDLE hFind = ::FindFirstFile(sFindPath, &fData);
            if (hFind == INVALID_HANDLE_VALUE)
            {
                goto Exit0;
            }

            do
            {
                if (0 == _tcscmp(fData.cFileName, _T(".")) || 0 == _tcscmp(fData.cFileName, _T("..")))
                {
                    continue;
                }

                if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    CString sSubfolder;
                    sSubfolder.Format(_T("%s%s\\"), strDir, fData.cFileName);
                    if ((FALSE == DeleteDirectory(sSubfolder, bContinueWhenFail)) && (!bContinueWhenFail))
                    {
                        goto Exit0;
                    }
                }
                else
                {
                    CString sFileName = fData.cFileName;
                    sFileName.MakeLower();
                    if ((FALSE == ::DeleteFile(strDir + sFileName)) && (!bContinueWhenFail))
                    {
                        goto Exit0;
                    }
                }
            } while (::FindNextFile(hFind, &fData) != 0);

            bReturn = TRUE;

Exit0:
            if (hFind != INVALID_HANDLE_VALUE)
            {
                ::FindClose(hFind);
            }

            ::RemoveDirectory(strDir);
            strDir = ZLPath::PathRemoveBackslash(strDir);
            ::RemoveDirectory(strDir);
            return bReturn;
        }

        /**
         * @brief ����Ŀ¼
         * @param[in] szSrcDir   ԴĿ¼
         * @param[in] szDstDir   Ŀ��Ŀ¼
         * @param[in] bCoverFile �Ƿ񸲸��Ѵ��ڵ��ļ�
         * @return ���ؿ����ļ�����Ŀ
         * @see 
         */
        static int CopyDirectory(LPCTSTR lpSrcDir, LPCTSTR lpDstDir, BOOL bCoverFile = TRUE)
        {
            if (!lpSrcDir || !lpDstDir)
            {
                return 0;
            }

            int nReturn = 0;
            CString strSubFile;
            CString strSubDstFile;
            CString strSrcDir = ZLPath::PathAddBackslash(lpSrcDir);
            CString strDstDir = ZLPath::PathAddBackslash(lpDstDir);
            CreateDeepDirectory(strDstDir);

            CString strFind;
            strFind.Format(_T("%s*.*"), strSrcDir);
            WIN32_FIND_DATA fData;
            HANDLE hFind = ::FindFirstFile(strFind, &fData);
            if (hFind == INVALID_HANDLE_VALUE)
            {
                goto Exit0;
            }

            do 
            {
                if (_tcscmp(fData.cFileName, _T(".")) == 0 || _tcscmp(fData.cFileName, _T("..")) == 0)
                {
                    continue;
                }

                if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    strSubFile.Format(_T("%s%s\\"), strSrcDir, fData.cFileName);
                    strSubDstFile.Format(_T("%s%s\\"), strDstDir, fData.cFileName);

                    CreateDeepDirectory(strSubDstFile);
                    nReturn += CopyDirectory(strSubFile, strSubDstFile, !bCoverFile);
                }
                else
                {
                    strSubFile.Format(_T("%s%s"), strSrcDir, fData.cFileName);
                    strSubDstFile.Format(_T("%s%s"), strDstDir, fData.cFileName);

                    if (!::CopyFile(strSubFile, strSubDstFile, !bCoverFile))
                    {
                        nReturn++;
                    }
                }
            } while (::FindNextFile(hFind, &fData) != 0);

Exit0:
            if (hFind != INVALID_HANDLE_VALUE)
            {
                ::FindClose(hFind);
            }
            return nReturn;
        }
    };

}
}
