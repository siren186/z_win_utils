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
 * @brief �ļ�·������
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ṩ��windows�����ļ�·���Ĳ���
     */
    class ZLPath
    {
    public:
        /**
         * @brief ��ȡģ��ȫ·��
         * @param[in] hModule ģ����
         * @return �ɹ�����ģ��ȫ·��, ʧ�ܷ��ؿմ�
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
         * @brief ��ȡ�ļ���
         * @param[in] szFullPath �ļ�·��
         * @return �ɹ������ļ���, ʧ�ܷ���·��
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
         * @brief ��ȡ���ļ���·��
         * @param[in] szPath �ļ�ȫ·��
         * @return �ɹ������ļ���·����, ʧ�ܷ��ؿմ�
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
         * @brief ��ȡģ���ļ���·��
         * @param[in] hModule ģ����
         * @return �ɹ������ļ���·����, ʧ�ܷ��ؿմ�
         */
        static CString GetModuleDir(HMODULE hModule)
        {
            return PathToParentDir(GetModuleFullPath(hModule));
        }

        /**
         * @brief ȥ���ļ���׺��
         * @param[in] szPath �ļ���
         * @return �ɹ������ļ���, ʧ�ܷ��ش���׺���ļ���
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
         * @brief ��ȡ·���µ��ļ���׺
         * @param[in] szPath ȫ·��
         * @return �ɹ������ļ���׺
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
         * @brief ��ȡ��·��
         * @param[in] szFullPathȫ·��
         * @return �ɹ����ظ�·��, ʧ�ܷ���ȫ·��
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
         * @brief ·��ĩβ�ӷ�б��
         * @param[in] szPath ·��
         * @return ����·��
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
         * @brief ȥ��·��ĩβ�����б�߻�б��
         * @param[in] szPath ·��
         * @param[in] removeAll �Ƿ��Ƴ�ĩβ���е�б�߻�б�ߣ���ĩβ���ظ������ʱ����Ч
         * @return ����ȥ��б�߻�б�ߺ��·��
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
         * @brief ��·��ת���ɺϷ���windows·������·������".."�����·��ƴ�ӣ��ᱻת������
         * @param[in] szPath Ҫת����·��
         * @param[in] bUniqueBackslash �Ƿ�ȥ��б�ܻ�б��
         * @param[in] bFormatBackslash �Ƿ�б��ת���ɷ�б��
         * @return ת�����·��
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
         * @brief ����2��·�������·��
         * @param[in] pszFrom Դ·�������ɵ����·���Ļ�·��
         * @param[in] pszTo Ŀ��·�������ɵ����·��������Դ·��Ϊ��·�����㣬�õ�Ŀ��·�������λ��
         * @param[in] dwAttrFrom ·�����ԣ�������ļ��У���ʹ��FILE_ATTRIBUTE_DIRECTORY
         * @param[in] dwAttrTo ·�����ԣ�������ļ��У���ʹ��FILE_ATTRIBUTE_DIRECTORY
         * @return ���ؼ���õ������·��
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
