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
 * @brief �ļ��������
 * @todo
 */


#pragma once

#include "win_utils_header.h"
#include "directory.hpp"
#include "buffer.hpp"
#include "file_enum.hpp"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ṩ�ļ�������ز���
     */
    class ZLFileOperation
    {
    public:
        /**
         * @brief ȷ���ļ������ļ��д��ڣ��������ڳ��Խ���
         * @param[in] filePath �ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL EnsureFileDir(LPCTSTR filePath)
        {
            CString destDir = ZLPath::PathRemoveFileName(filePath);
            if (destDir.IsEmpty())
            {
                return FALSE;
            }

            if (!::PathFileExists(destDir))
            {
                if (!ZLDirectory::CreateDeepDirectory(destDir))
                {
                    return FALSE;
                }
            }

            return ZLDirectory::IsDirectory(destDir);
        }

        /**
         * @brief �����ļ�
         * @param[in] srcFilePath Դ�ļ�·��
         * @param[in] dstFilePath Ŀ���ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL CopyFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!::PathFileExists(srcFilePath) || !EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            return ::CopyFile(srcFilePath,dstFilePath,FALSE);
        }

        /**
         * @brief ͨ����д��ʽ�����ļ�
         * @param[in] srcFilePath Դ�ļ�·��
         * @param[in] dstFilePath Ŀ���ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL CopyFileByReadWrite(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            ATL::CHandle hSrcFile(::CreateFile(srcFilePath,GENERIC_READ,FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
            ATL::CHandle hDstFile(::CreateFile(dstFilePath,GENERIC_WRITE,0,
                NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
            if (hSrcFile==INVALID_HANDLE_VALUE || hDstFile==INVALID_HANDLE_VALUE)
            {
                return FALSE;
            }
            LARGE_INTEGER fileSizeLI = {0};
            if (!GetFileSizeEx(hSrcFile,&fileSizeLI))
            {
                return FALSE;
            }
            DWORD bufferSize = 4*1024;
            ZLBuffer buffer;
            buffer.Alloc(bufferSize);
            while(fileSizeLI.QuadPart > 0)
            {
                DWORD realReadedSize = 0;
                DWORD realWritedSize = 0;
                if (!ReadFile(hSrcFile, buffer.GetBuffer(), buffer.GetSize(), &realReadedSize, NULL))
                {
                    return FALSE;
                }
                if (!WriteFile(hDstFile, buffer.GetBuffer(), realReadedSize, &realWritedSize, NULL))
                {
                    return FALSE;
                }
                fileSizeLI.QuadPart -= realWritedSize;
            }
            return TRUE;
        }

        /**
         * @brief �ƶ��ļ�
         * @param[in] srcFilePath Դ�ļ�·��
         * @param[in] dstFilePath Ŀ���ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL MoveFile(LPCTSTR srcFilePath, LPCTSTR dstFilePath)
        {
            if (!::PathFileExists(srcFilePath) || !EnsureFileDir(dstFilePath))
            {
                return FALSE;
            }
            return ::MoveFileEx(srcFilePath, dstFilePath, 
                MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
        }

        /**
         * @brief ��ȡ�ļ�ֻ������
         * @param[in] filePath �ļ�·��
         * @return ���ļ�Ϊֻ��������TRUE�����򷵻�FALSE
         * @see 
         */
        static BOOL IsFileReadOnly(LPCTSTR filePath)
        {
            DWORD dwAttr = ::GetFileAttributes( filePath );
            if (dwAttr == INVALID_FILE_ATTRIBUTES)
            {
                return FALSE;
            }
            if( dwAttr & FILE_ATTRIBUTE_READONLY )
            {
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief ȥ���ļ�ֻ������
         * @param[in] filePath �ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE�����ļ�������ֻ�����ԣ�Ҳ��Ϊ�ɹ�
         * @see 
         */
        static BOOL RemoveReadOnly(LPCTSTR filePath)
        {
            DWORD dwAttr = ::GetFileAttributes( filePath );
            if (dwAttr == INVALID_FILE_ATTRIBUTES)
            {
                return FALSE;
            }
            if( dwAttr & FILE_ATTRIBUTE_READONLY )
            {
                dwAttr = dwAttr ^ FILE_ATTRIBUTE_READONLY;
                return ::SetFileAttributes( filePath, dwAttr );
            }
            return TRUE;
        }

        /**
         * @brief �������ļ�
         * @param[in] filePath ·��
         * @param[in] newFileName Ҫ�����������֣������ļ�����׺
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL RenameFile(LPCTSTR filePath, LPCTSTR newFileName)
        {
            if (!::PathFileExists(filePath))
            {
                return FALSE;
            }
            CString destFile(filePath);
            destFile = ZLPath::PathToParentDir(filePath);
            destFile += newFileName;
            if (::PathFileExists(destFile))
            {
                return FALSE;
            }
            return MoveFile(filePath, destFile);
        }

        /**
         * @brief ����һ��δ��ʹ�õ��ļ���������ָ���ļ�������������
         * @param[in] dir ָ�����ĸ��ļ����²���
         * @param[in] prefix �ļ���ǰ�벿�֣���������׺
         * @param[in] format ��ʽ���ַ�������������%s����%d��ʾ����"%s_%d.txt"
         * @param[in] base ����������ʼֵ
         * @return ����ɹ�����δ��ʹ�õ��ļ�·�������ʧ�ܣ����ؿ�
         */
        static CString EnsureFileName(LPCTSTR dir, LPCTSTR prefix, LPCTSTR format = _T("%s_(%d)"), int base = 0)
        {
            if (!dir || !prefix || !format)
            {
                return _T("");
            }

            CString strUnusedPath;
            CString filename;
            CString strDir = ZLPath::PathAddBackslash(dir);
            if (strDir.IsEmpty())
            {
                return _T("");
            }

            for (int i = base; i < INT_MAX; i++)
            {
                filename.Format(format, prefix, i);
                strUnusedPath = strDir + filename;
                if (FALSE == ::PathFileExists(strUnusedPath) && ERROR_FILE_NOT_FOUND == ::GetLastError())
                {
                    return strUnusedPath;
                }
            }

            return _T("");
        }

        /**
         * @brief ɾ���ļ�����ѡ������޷�����ɾ���ɱ��Ϊ������ɾ��
         * @param[in,out] filePath ·��
         * @param[in,out] rebootDelete ���޷�����ɾ�����Ƿ�������ʱɾ��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL DeleteFile(LPCTSTR filePath, BOOL rebootDelete)
        {
            if (::DeleteFile(filePath))
            {
                return TRUE;
            }
            if (rebootDelete)
            {
                RemoveReadOnly(filePath);
                if (!::MoveFileEx(filePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        /**
         * @brief ɾ��ʧ�ܻ��������ļ�����ѡ��ʹ����������ɾ��
         * @param[in,out] filePath ·��
         * @param[in,out] rebootDelete ���޷�����ɾ�����Ƿ�������ʱɾ�������������ʧ�ܣ����᳢�Ա��������ɾ��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL RenameDeleteFile(LPCTSTR filepath, BOOL rebootDelete = TRUE)
        {
            if (!filepath)
            {
                return FALSE;
            }

            if (::DeleteFile(filepath))
            {
                return TRUE;
            }

            CString dir = ZLPath::PathToParentDir(filepath);
            CString oldName = ZLPath::PathToFileName(filepath);
            CString newName = EnsureFileName(dir, oldName);
            if (newName.IsEmpty())
            {
                return FALSE;
            }

            RemoveReadOnly(filepath);
            if (!::MoveFileEx(filepath, newName, MOVEFILE_REPLACE_EXISTING))
            {
                return FALSE;
            }

            filepath = newName;
            if (rebootDelete)
            {
                if (!::MoveFileEx(filepath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
                {
                    return FALSE;
                }
            }
            return TRUE;
        }

        /**
         * @brief ɾ���ļ��м��ļ�.�ᴦ��δ�ɹ�ɾ�����ļ�
         * @param[in] dir Ҫɾ����Ŀ¼
         * @param[in] rebootDelete ���޷�����ɾ�����Ƿ�������ʱɾ�������������ʧ�ܣ����᳢�Ա��������ɾ��
         * @return �޷���ֵ�����в�����Ϊ�ɹ�
         * @see 
         */
        static VOID RecursiveDeleteFileDir(LPCTSTR dir, BOOL rebootDelete)
        {
            if (!::PathFileExists(dir))
            {
                return;
            }
            ZLDirectory::DeleteDirectory(dir);
            ZLFileEnum enumer(dir);
            int fileCount = enumer.GetFileCount();
            for (int n = 0; n < fileCount; ++n)
            {
                CString fileName(enumer.GetFileName(n));
                RenameDeleteFile(fileName, rebootDelete);
            }
        }
    };
}
}
