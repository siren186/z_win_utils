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
 * @brief �ļ�ö�ٲ���
 */


#pragma once
#include "win_utils_header.h"
#include "path.hpp"
#include <vector>

namespace zl
{
namespace WinUtils
{

    typedef struct _ZLFileEnumInfo
    {
        CString m_strFileName;
        DWORD	m_nSize;
    } ZLFileEnumInfo;

    /**
     * @brief �ļ�ö����ز���
     */
    class ZLFileEnum
    {
    public:
        /**
         * @brief ��ʼ��
         * @param[in] lpDirName ·��
         * @param[in] lpPost    �����ļ����ͣ�Ĭ��Ϊ�����ļ�
         */
        ZLFileEnum(LPCTSTR lpDirName, LPCTSTR lpPost=_T("*.*"))
        {
            if (!lpDirName || !lpPost)
                return;

            CString strSearch(lpDirName);
            ZLPath::PathAddBackslash(strSearch);
            m_strDir = strSearch;
            strSearch.Append(lpPost);

            WIN32_FIND_DATA wfd = {0};
            HANDLE hFind = ::FindFirstFile(strSearch, &wfd);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                do 
                {
                    CString strFileName = wfd.cFileName;
                    if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (strFileName.Compare(_T("."))==0||
                            strFileName.Compare(_T(".."))==0)
                            continue;
                        m_listDirs.push_back(strFileName);
                    }
                    else
                    {
                        ZLFileEnumInfo fi;
                        fi.m_strFileName = strFileName;
                        fi.m_nSize  = wfd.nFileSizeLow;
                        m_listFiles.push_back(fi);
                    }
                }while (::FindNextFile(hFind, &wfd));
                ::FindClose( hFind );
            }
        }

        /**
         * @brief ��ȡ�ļ����µ��ļ���Ŀ
         * @return �����ļ���Ŀ
         */
        int GetFileCount()
        {
            return (int)m_listFiles.size();
        }

        /**
         * @brief ��ȡ�ļ����µ��ļ�����Ŀ
         * @return �����ļ�����Ŀ
         */
        int GetDirCount()
        {
            return (int)m_listDirs.size();
        }

        /**
         * @brief ��ȡָ���±괦���ļ���
         * @param[in] i �±�
         * @return �ļ���
         */
        CString& GetFileName(int i)
        {
            return m_listFiles[i].m_strFileName;
        }

        /**
         * @brief ��ȡָ���±괦�ļ��Ĵ�С
         * @param[in] i �±�
         * @return �ļ���С
         */
        DWORD GetFilesize(int i)
        {
            return m_listFiles[i].m_nSize;
        }

        /**
         * @brief ��ȡָ���±괦���ļ�����
         * @param[in] i �±�
         * @return �ļ�����
         */
        CString& GetDirName(int i)
        {
            return m_listDirs[i];
        }

        /**
         * @brief ��ȡָ���±괦���ļ���ȫ·��
         * @param[in] i �±�
         * @param[out] strFull �ļ���ȫ·��
         */
        void GetFileFullPath(int i, CString& strFull)
        {
            strFull = m_strDir;
            strFull.Append(m_listFiles[i].m_strFileName);
        }

        /**
         * @brief ��ȡָ���±괦���ļ��е�ȫ·��
         * @param[in] i �±�
         * @param[out] strFull �ļ��е�ȫ·��
         */
        void GetDirFullPath(int i, CString& strFull)
        {
            strFull = m_strDir;
            strFull.Append(m_listDirs[i]);
        }

    protected:
        std::vector<ZLFileEnumInfo>     m_listFiles;
        std::vector<CString>            m_listDirs;
        CString                         m_strDir;
    };

}
}
