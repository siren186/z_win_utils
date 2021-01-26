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
 * @brief �ļ��汾��Ϣ
 * @todo ����ļ�ע��,�д��޸�
 */


#pragma once
#pragma comment(lib, "Version.lib")

#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief �ṩ�ļ��汾��Ϣ��ز���
     */
    class ZLFileVersion
    {
    public:
        ZLFileVersion()
        {
            Reset();
        }

        virtual ~ZLFileVersion()
        {}

        /**
         * @brief ������ʼ��ָ��ģ���ļ���Ϣ
         * @param[in] hModule ģ������Ĭ��ΪNULL
         * @return �ɹ�����TRUE��ʧ��FALSE
         */
        BOOL Create(HMODULE hModule = NULL)
        {
            CString sPath;

            ::GetModuleFileName(hModule, sPath.GetBuffer(_MAX_PATH), _MAX_PATH);
            sPath.ReleaseBuffer();
            return Create(sPath);
        }

        /**
         * @brief ������ʼ��ָ���ļ���Ϣ
         * @param[in] lpFileName  �ļ�·��
         * @return �ɹ�����TRUE��ʧ��FALSE
         */
        BOOL Create(LPCTSTR lpFileName)
        {
            Reset();
            DWORD dwHandle = 0;
            DWORD dwFileVersionInfoSize = ::GetFileVersionInfoSize((LPTSTR)lpFileName, &dwHandle);

            if (!dwFileVersionInfoSize)
                return FALSE;

            LPVOID lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];
            if (lpData == NULL)
                return FALSE;

            try
            {
                if (!::GetFileVersionInfo((LPTSTR)lpFileName, dwHandle, dwFileVersionInfoSize, lpData))
                    throw FALSE;

                LPVOID lpInfo = NULL;
                UINT unInfoLen = 0;

                if (::VerQueryValue(lpData, L"\\", &lpInfo, &unInfoLen))
                {
                    if (unInfoLen == sizeof(mFileInfo))
                        memcpy(&mFileInfo, lpInfo, unInfoLen);
                }

                ::VerQueryValue(lpData, L"\\VarFileInfo\\Translation", &lpInfo, &unInfoLen);

                DWORD dwLangCode = 0;
                BOOL bRetCode = GetTranslationId(lpInfo, unInfoLen, ::GetUserDefaultLangID(), dwLangCode, FALSE);
                if (bRetCode == FALSE)
                {
                    bRetCode = GetTranslationId(lpInfo, unInfoLen, ::GetUserDefaultLangID(), dwLangCode, TRUE);
                    if (bRetCode == FALSE)
                    {
                        bRetCode = GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, TRUE);
                        if (bRetCode == FALSE)
                        {
                            bRetCode = GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, TRUE);
                            if (bRetCode == FALSE)
                            {
                                dwLangCode = *((DWORD*)lpInfo);
                            }
                        }
                    }
                }

                CString sSubBlock;
                CString strTemp;

                sSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000)>>16);

                strTemp = sSubBlock + L"CompanyName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == FALSE)//����Ƿ����
                    dwLangCode = 0x04b00409;//sogou
                strTemp.ReleaseBuffer();

                sSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000)>>16);

                strTemp = sSubBlock + L"CompanyName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msCompanyName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileDescription";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileDescription = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileVersion";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"InternalName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msInternalName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalCopyright";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalCopyright = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"OriginalFileName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msOriginalFileName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductVersion";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"Comments";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msComments = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalTrademarks";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalTrademarks = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"PrivateBuild";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msPrivateBuild = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"SpecialBuild";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msSpecialBuild = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                delete[] lpData;
            }catch (BOOL)
            {
                delete[] lpData;
                return FALSE;
            }

            return TRUE;
        }

        /**
         * @brief ��ȡ�ļ��汾
         * @param[in] nIndex �±�,��0��ʼ,�ֱ��Ӧ�汾�����Ե�ż���ĸ�����.
         * @return �汾��
         * @note �ļ��汾����4�����,��"2014.7.17.520", ����nIndex��0ʱ,ȡ��2014, ��3ʱ,ȡ��520
         */
        WORD GetFileVersion(int nIndex) const
        {
            if (nIndex == 0)
                return (WORD)(mFileInfo.dwFileVersionLS & 0x0000FFFF);
            else if (nIndex == 1)
                return (WORD)((mFileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
            else if (nIndex == 2)
                return (WORD)(mFileInfo.dwFileVersionMS & 0x0000FFFF);
            else if (nIndex == 3)
                return (WORD)((mFileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
            else
                return 0;
        }

        /**
         * @brief ��ȡ��Ʒ�汾��
         * @param[in] nIndex �±�,��0��ʼ, �ֱ��Ӧ�汾�����Ե�ż���ĸ�����.
         * @return �汾��
         * @note �ļ��汾����4�����,��"9.1.123455.888", ����nIndex��0ʱ,ȡ��9, ��3ʱ,ȡ��888
         */
        WORD GetProductVersion(int nIndex) const
        {
            if (nIndex == 0)
                return (WORD)(mFileInfo.dwProductVersionLS & 0x0000FFFF);
            else if (nIndex == 1)
                return (WORD)((mFileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
            else if (nIndex == 2)
                return (WORD)(mFileInfo.dwProductVersionMS & 0x0000FFFF);
            else if (nIndex == 3)
                return (WORD)((mFileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
            else
                return 0;
        }

        /**
         * @brief �ļ���־λ������,�ο�MSDN��VS_FIXEDFILEINFO�ṹ��
         */
        DWORD GetFileFlagsMask() const
        {
            return mFileInfo.dwFileFlagsMask;
        }

        /**
         * @brief �ļ���־λ,�ο�MSDN��VS_FIXEDFILEINFO�ṹ��
         */
        DWORD GetFileFlags() const
        {
            return mFileInfo.dwFileFlags;
        }

        /**
        * @brief ��ȡ�ļ�ϵͳ
        * @param[in]   nIndex  ����
        * @return FileOS
        */
        DWORD GetFileOs() const
        {
            return mFileInfo.dwFileOS;
        }

        /**
        * @brief ��ȡ�ļ�ͨ������
        * @return FileType
        */
        DWORD GetFileType() const
        {
            return mFileInfo.dwFileType;
        }

        /**
         * @brief ��ȡ�ļ�����
         * @return FileSubtype
         */
        DWORD GetFileSubtype() const
        {
            return mFileInfo.dwFileSubtype;
        }

        /**
        * @brief ��ȡ�ļ�����
        * @return FILETIME data structure 
        */
        FILETIME GetFileDate() const
        {
            FILETIME ft;

            ft.dwLowDateTime = mFileInfo.dwFileDateLS;
            ft.dwHighDateTime = mFileInfo.dwFileDateMS;

            return ft;
        }

        /**
        * @brief ��ȡCompanyName
        * @return  �ַ���������CompanyName
        */
        CString GetCompanyName() const
        {
            return msCompanyName;
        }

        /**
        * @brief ��ȡ�ļ�����
        * @return  �ַ�������ʾ�ļ�����
        */
        CString GetFileDescription() const
        {
            return msFileDescription;
        }

        /**
         * @brief ��ȡ�ļ��汾
         * @return  �ַ���������FileVersion
         */
        CString GetFileVersion() const
        {
            return msFileVersion;
        }

        /**
        * @brief ��ȡInternalName
        * @return  �ַ���������InternalName
        */
        CString GetInternalName() const
        {
            return msInternalName;
        }

        /**
        * @brief ��ȡLegalCopyright
        * @return  �ַ���������LegalCopyright
        */
        CString GetLegalCopyright() const
        {
            return msLegalCopyright;
        }

        /**
        * @brief ��ȡOriginalFileName
        * @return  �ַ���������OriginalFileName
        */
        CString GetOriginalFileName() const
        {
            return msOriginalFileName;
        }

        /**
         * @brief ��ȡProductName
         * @return  �ַ���������ProductName
         */
        CString GetProductName() const
        {
            return msProductName;
        }

        /**
        * @brief ��ȡProductVersion
        * @return  �ַ���������ProductVersion
        */
        CString GetProductVersion() const
        {
            return msProductVersion;
        }

        /**
         * @brief ��ȡComments
         * @return  �ַ���������Comments
         */
        CString GetComments() const
        {
            return msComments;
        }

        /**
        * @brief ��ȡLegalTrademarks
        * @return  �ַ���������LegalTrademarks
        */
        CString GetLegalTrademarks() const
        {
            return msLegalTrademarks;
        }

        /**
        * @brief ��ȡPrivateBuild
        * @return  �ַ���������PrivateBuild
        */
        CString GetPrivateBuild() const
        {
            return msPrivateBuild;
        }

        /**
         * @brief ��ȡSpecialBuild
         * @return  �ַ���������SpecialBuild
         */
        CString GetSpecialBuild() const
        {
            return msSpecialBuild;
        }

    protected:
        virtual void Reset()
        {
            ZeroMemory(&mFileInfo, sizeof(mFileInfo));

            msCompanyName.Empty();
            msFileDescription.Empty();
            msFileVersion.Empty();
            msInternalName.Empty();
            msLegalCopyright.Empty();
            msOriginalFileName.Empty();
            msProductName.Empty();
            msProductVersion.Empty();
            msComments.Empty();
            msLegalTrademarks.Empty();
            msPrivateBuild.Empty();
            msSpecialBuild.Empty();
        }

        /**
         * @brief ��ȡ��ǰ�û��ı������Ա�ʾ
         * @param[in]   lpData Buffer������ָ���ļ��İ汾��Ϣ
         * @param[in]   unBlockSize VS_FIXEDFILEINFO�ṹ��Ĵ�С
         * @param[in]  wLangId ���Ա�ʾ
         * @param[out]  dwId ���Ա�ʾ
         * @param[in]  bPrimaryEnough �Ƿ���е��ֽڲ���
         * @return ����ɹ�������TRUE�����ʧ�ܷ���FALSE
         */
        BOOL GetTranslationId(
            LPVOID lpData, 
            UINT   unBlockSize, 
            WORD   wLangId, 
            DWORD  &dwId, 
            BOOL   bPrimaryEnough = FALSE)
        {
            LPWORD lpwData;

            for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
            {
                if (*lpwData == wLangId)
                {
                    dwId = *((DWORD*)lpwData);
                    return TRUE;
                }
            }

            if (!bPrimaryEnough)
                return FALSE;

            for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
            {
                if (((*lpwData)&0x00FF) == (wLangId&0x00FF))
                {
                    dwId = *((DWORD*)lpwData);
                    return TRUE;
                }
            }

            return FALSE;
        }

    private:
        VS_FIXEDFILEINFO mFileInfo;

        CString msCompanyName;
        CString msFileDescription;
        CString msFileVersion;
        CString msInternalName;
        CString msLegalCopyright;
        CString msOriginalFileName;
        CString msProductName;
        CString msProductVersion;
        CString msComments;
        CString msLegalTrademarks;
        CString msPrivateBuild;
        CString msSpecialBuild;
    };
}
}
