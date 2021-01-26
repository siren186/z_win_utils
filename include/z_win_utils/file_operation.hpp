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
            CString destDir(filePath);
            ZLPath::PathRemoveFileSpec(destDir);
            if (!::PathFileExists(destDir))
            {
                if (!ZLDirectory::CreateDeepDirectory(destDir))
                {
                    return FALSE;
                }
            }
            if (!ZLDirectory::IsDirectory(destDir))
            {
                return FALSE;
            }
            return TRUE;
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
            ZLPath::PathRemoveFileSpec(destFile);
            destFile += newFileName;
            if (::PathFileExists(destFile))
            {
                return FALSE;
            }
            return MoveFile(filePath, destFile);
        }

        /**
         * @brief Ѱ��δ��ռ�õ��ļ���
         * @param[in,out] filePath ·��
         * @param[in] nameFormat �������ĸ�ʽ��%sΪ�ļ�����%dΪ���Եĺ�׺���֣�Ĭ��ΪWindows��ʽ
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see 
         */
        static BOOL EnsureFileName(CString& filePath,LPCTSTR nameFormat=L"%s_(%d)")
        {
            CString dir(ZLPath::PathToFolderPath(filePath));
            CString fileNameExt(ZLPath::PathToFileName(filePath));
            CString fileName(ZLPath::FileNameRemoveSuffix(fileNameExt));
            CString ext(ZLPath::PathToSuffix(fileNameExt));
            int count = 1;
            while (::PathFileExists(filePath))
            {
                CString newFileName;
                newFileName.Format(nameFormat,fileName, ++count);
                filePath.Format(L"%s\\%s", dir, newFileName);
                if (!ext.IsEmpty())
                {
                    filePath += L'.';
                    filePath += ext;
                }
                if (count == INT_MAX)
                {
                    return FALSE;
                }
            }
            return TRUE;
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
        static BOOL RenameDeleteFile(CString& filePath, BOOL rebootDelete)
        {
            if (::DeleteFile(filePath))
            {
                return TRUE;
            }
            CString newName(filePath);
            if (!EnsureFileName(newName))
            {
                return FALSE;
            }
            RemoveReadOnly(filePath);
            if (!::MoveFileEx(filePath, newName, MOVEFILE_REPLACE_EXISTING))
            {
                return FALSE;
            }
            filePath = newName;
            if (rebootDelete)
            {
                if (!::MoveFileEx(filePath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
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
            for (int n=0; n<fileCount; ++n)
            {
                CString fileName(enumer.GetFileName(n));
                RenameDeleteFile(fileName,rebootDelete);
            }
        }
    };
}
}
