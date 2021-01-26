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
 * @brief 文件路径操作
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供对windows常见文件路径的操作
     */
    class ZLPath
    {
    public:
        /**
         * @brief 获取模块全路径
         * @param[in] hModule 模块句柄
         * @return 成功返回模块全路径, 失败返回空串
         */
        static CString GetModuleFullPath(HMODULE hModule)
        {
            TCHAR szFileName[MAX_PATH + 1] = { 0 };
            DWORD dwRet = ::GetModuleFileName(hModule, szFileName, MAX_PATH);

            CString fullpath;
            if (dwRet)
            {
                fullpath = szFileName;
            }
            return fullpath;
        }

        /**
         * @brief 获取文件名
         * @param[in] szFullPath 文件路径
         * @return 成功返回文件名, 失败返回路径
         */
        static CString PathToFileName(LPCTSTR szPath)
        {
            int len = lstrlen(szPath);
            int i = len - 1;
            for (; i >= 0; --i)
            {
                if (_T('\\') == szPath[i] || _T('/') == szPath[i])
                {
                    break;
                }
            }

            CString filename;
            if (i > 0)
            {
                filename.Append(&szPath[i + 1]);
            }
            else
            {
                filename = szPath;
            }
            return filename;
        }

        /**
         * @brief 获取父文件夹路径
         * @param[in] szPath 文件全路径
         * @return 成功返回文件夹路径名, 失败返回空串
         */
        static CString PathToParentDir(LPCTSTR szPath)
        {
            int len = lstrlen(szPath);
            int i = len - 2;
            for (; i >= 0; --i)
            {
                if (_T('\\') == szPath[i] || _T('/') == szPath[i])
                {
                    break;
                }
            }

            CString parent;
            if (i > 0)
            {
                parent.Append(szPath, i + 1);
            }
            return parent;
        }

        /**
         * @brief 获取模块文件夹路径
         * @param[in] hModule 模块句柄
         * @return 成功返回文件夹路径名, 失败返回空串
         */
        static CString GetModuleDir(HMODULE hModule)
        {
            return PathToParentDir(GetModuleFullPath(hModule));
        }

        /**
         * @brief 去除文件后缀名
         * @param[in] szPath 文件名
         * @return 成功返回文件名, 失败返回带后缀的文件名
         */
        static CString PathRemoveExtension(LPCTSTR szPath)
        {
            CString strTemp(szPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos)
            {
                return strTemp;
            }
            return strTemp.Left(nPos);
        }

        /**
         * @brief 获取路径下的文件后缀
         * @param[in] szPath 全路径
         * @return 成功返回文件后缀
         */
        static CString PathToExtension(LPCTSTR szPath)
        {
            CString strTemp(szPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos)
            {
                return _T("");
            }
            return strTemp.Mid(nPos + 1);
        }

        /**
         * @brief 获取根路径
         * @param[in] szFullPath全路径
         * @return 成功返回根路径, 失败返回全路径
         */
        static CString GetRootDir(LPCTSTR szFullPath)
        {
            int len = lstrlen(szFullPath);
            int i = 0;
            for (; i < len; ++i)
            {
                if (_T('\\') == szFullPath[i] || _T('/') == szFullPath[i])
                {
                    break;
                }
            }

            CString strRoot;
            if (i > 0)
            {
                strRoot.Append(szFullPath, i + 1);
            }
            return strRoot;
        }

        /**
         * @brief 路径末尾加反斜线
         * @param[in] szPath 路径
         * @return 返回路径
         */
        static CString PathAddBackslash(LPCTSTR szPath)
        {
            CString strPath = szPath;
            if (strPath.IsEmpty())
            {
                return _T("");
            }

            TCHAR ch = strPath[strPath.GetLength() - 1];
            if (_T('\\') != ch && _T('/') != ch)
            {
                strPath.Append(_T("\\"));
            }
            return strPath;
        }

        /**
         * @brief 去除路径末尾多余的斜线或反斜线
         * @param[in] szPath 路径
         * @param[in] removeAll 是否移除末尾所有的斜线或反斜线，在末尾有重复多个的时候有效
         * @return 返回去掉斜线或反斜线后的路径
         */
        static CString PathRemoveBackslash(LPCTSTR szPath, BOOL removeAll = TRUE)
        {
            CString strPath;
            if (removeAll)
            {
                int len = lstrlen(szPath);
                int i = len - 1;
                for (; i >= 0; --i)
                {
                    if (_T('\\') != szPath[i] && _T('/') != szPath[i])
                    {
                        break;
                    }
                }

                if (i > 0)
                {
                    strPath.Append(szPath, i + 1);
                }
            }
            else
            {
                strPath = szPath;
                LPTSTR pszBuffer = strPath.GetBuffer(strPath.GetLength());
                ::PathRemoveBackslash(pszBuffer);
                strPath.ReleaseBuffer();
            }

            return strPath;
        }

        /**
         * @brief 将路径转换成合法的windows路径，如路径中有".."等相对路径拼接，会被转换出来
         * @param[in] szPath 要转换的路径
         * @param[in] bUniqueBackslash 是否去重斜杠或反斜杠
         * @param[in] bFormatBackslash 是否将斜杠转换成反斜杠
         * @return 转换后的路径
         */
        static CString PathCanonicalize(LPCTSTR szPath, BOOL bUniqueBackslash = TRUE, BOOL bFormatBackslash = TRUE)
        {
            CString strPath = szPath;
            if (bFormatBackslash)
            {
                strPath.Replace(_T('/'), _T('\\'));
            }

            TCHAR buffer[MAX_PATH + 1] = { 0 };
            if (::PathCanonicalize(buffer, strPath))
            {
                strPath = buffer;
            }

            if (bUniqueBackslash)
            {
                int len = strPath.GetLength();
                for (int i = 0; i < len; i++)
                {
                    if (0 == strPath.Replace(_T("\\\\"), _T("\\")))
                    {
                        break;
                    }
                }
            }

            return strPath;
        }

        /**
         * @brief 计算2个路径的相对路径
         * @param[in] pszFrom 源路径，生成的相对路径的基路径
         * @param[in] pszTo 目标路径，生成的相对路径，会以源路径为基路径计算，得到目标路径的相对位置
         * @param[in] dwAttrFrom 路径属性，如果是文件夹，请使用FILE_ATTRIBUTE_DIRECTORY
         * @param[in] dwAttrTo 路径属性，如果是文件夹，请使用FILE_ATTRIBUTE_DIRECTORY
         * @return 返回计算得到的相对路径
         */
        static CString PathRelativePathTo(LPCTSTR pszFrom, LPCTSTR pszTo, DWORD dwAttrFrom = FILE_ATTRIBUTE_NORMAL, DWORD dwAttrTo = FILE_ATTRIBUTE_NORMAL)
        {
            CString strOut;
            TCHAR szBuffer[MAX_PATH + 1] = { 0 };
            if (::PathRelativePathTo(szBuffer, pszFrom, dwAttrFrom, pszTo, dwAttrTo))
            {
                strOut = szBuffer;
            }

            return strOut;
        }
    };
}
}
