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
#include <stack>

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
         * @brief �����ļ���ʱ�Ļص�����
         * @param[in] filepath ���������ļ�·��
         * @param[in] param �ص������ĸ��Ӳ���
         * @return ����falseʱ��ֹͣ����
         * @see ���ж��ǲ����ļ��У���ͨ��wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY�ж�
         */
        typedef bool(*WalkDirectoryCallback)(const WIN32_FIND_DATA* wfd, LPCTSTR parentDir, LPVOID param);

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

        /**
         * @brief ���·����Ӧ��Դ�Ƿ����ļ���
         * @param[in] szPath ���·��
         * @return ���ļ��з���TRUE������FALSE
         */
        static BOOL IsDirectory(LPCTSTR szPath)
        {
            DWORD dwAttr = ::GetFileAttributes(szPath);
            if( dwAttr != (DWORD)-1 && 
                (dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
            {
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief �Էǵݹ�ķ�ʽ�����ļ��м������е����ļ���
         * @param[in] lpDir Ҫ�������ļ���·��
         * @param[in] callback �ص��ӿ�
         * @param[in] param �ص��ӿڵĸ��Ӳ���
         * @return �ɹ�����TRUE
         */
        static BOOL WalkDirectory(LPCTSTR lpDir, WalkDirectoryCallback callback, LPVOID param = NULL)
        {
            if (!lpDir || !callback)
            {
                return FALSE;
            }

            CString szRootDir = ZLPath::PathAddBackslash(lpDir);
            std::stack<CString> dirStack;
            dirStack.push(szRootDir);

            while (!dirStack.empty())
            {
                CString curDir = dirStack.top();
                dirStack.pop();

                WIN32_FIND_DATA wfd = { 0 };
                HANDLE hFind = FindFirstFile(curDir + TEXT("*"), &wfd);
                if (INVALID_HANDLE_VALUE == hFind)
                {
                    continue;
                }

                if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (0 != StrCmp(wfd.cFileName, TEXT(".")) &&
                        0 != StrCmp(wfd.cFileName, TEXT("..")))
                    {
                        CString childDir = curDir + wfd.cFileName + TEXT("\\");
                        dirStack.push(childDir);
                    }

                    if (!callback(&wfd, curDir, param)) // �ļ����а�������"."��".."·��
                    {
                        return TRUE;
                    }
                }
                else
                {
                    if (!callback(&wfd, curDir, param))
                    {
                        return TRUE;
                    }
                }

                while (FindNextFile(hFind, &wfd))
                {
                    if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (0 != StrCmp(wfd.cFileName, TEXT(".")) &&
                            0 != StrCmp(wfd.cFileName, TEXT("..")))
                        {
                            CString childDir = curDir + wfd.cFileName + TEXT("\\");
                            dirStack.push(childDir);
                        }

                        if (!callback(&wfd, curDir, param))
                        {
                            return TRUE;
                        }
                    }
                    else
                    {
                        if (!callback(&wfd, curDir, param))
                        {
                            return TRUE;
                        }
                    }
                }

                if (INVALID_HANDLE_VALUE != hFind)
                {
                    ::FindClose(hFind);
                }
            }
            return TRUE;
        }
    }; // end of class ZLDirectory
}
}
