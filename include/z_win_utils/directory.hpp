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
 * @brief 目录/文件夹相关
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
     * @brief 提供对目录的创建、修改和删除操作
     */
    class ZLDirectory
    {
    public:

        /**
         * @brief 遍历文件夹时的回调函数
         * @param[in] filepath 遍历到的文件路径
         * @param[in] param 回调函数的附加参数
         * @return 返回false时将停止遍历
         * @see 想判断是不是文件夹，可通过wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY判断
         */
        typedef bool(*WalkDirectoryCallback)(const WIN32_FIND_DATA* wfd, LPCTSTR parentDir, LPVOID param);

        /**
         * @brief 创建目录
         * @param[in] szPath 路径
         * @return 成功返回TRUE，失败返回FALSE
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
         * @brief 删除目录
         * @param[in] szDir             路径
         * @param[in] bContinueWhenFail 删除某个文件失败时,是否继续
         * @return 成功返回TRUE，失败返回FALSE
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
         * @brief 拷贝目录
         * @param[in] szSrcDir   源目录
         * @param[in] szDstDir   目标目录
         * @param[in] bCoverFile 是否覆盖已存在的文件
         * @return 返回拷贝文件的数目
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
         * @brief 检查路径对应资源是否是文件夹
         * @param[in] szPath 检查路径
         * @return 是文件夹返回TRUE，否则FALSE
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
         * @brief 以非递归的方式遍历文件夹及其所有的子文件夹
         * @param[in] lpDir 要遍历的文件夹路径
         * @param[in] callback 回调接口
         * @param[in] param 回调接口的附加参数
         * @return 成功返回TRUE
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

                    if (!callback(&wfd, curDir, param)) // 文件夹中包包含了"."和".."路径
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
