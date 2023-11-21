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
* @brief 字符串转换
*/


#pragma once
#include <atlstr.h>

namespace zl
{
namespace WinUtils
{

/**
 * @brief 字符串转换
 */
class ZLStrConv
{
public:
    /**
     * @brief 简单字符串转换
     * @param[in] lpBuf      需要转换的字符串
     * @param[in] uCodePage  Code Page
     * @param[in] nBufLen    字符串长度,如果字符串以0结尾,则此参数可填-1
     * @return 返回转换后的字符串,为空字符串时转换失败.
     */
    static CStringW a2w(LPCSTR  lpBuf, UINT uCodePage = CP_ACP, int nBufLen = -1);
    static CStringA w2a(LPCWSTR lpBuf, UINT uCodePage = CP_ACP, int nBufLen = -1);

    /**
     * @brief 长度很大的字符串转换,如文件内容
     * @param[in] lpSrc      需要转换的字符串
     * @param[in] nSrcLen    字符串长度
     * @param[in] lpDst      用于存储转换后的字符串的缓存区指针,为NULL时,函数返回实际需要的缓存区长度
     * @param[in] nDstLen    缓存区长度
     * @param[in] uCodePage  Code Page
     * @return 成功返回实际转换长度,小于等于0时失败.
     * @see MultiByteToWideChar,WideCharToMultiByte
     */
    static int a2w(LPCSTR  lpSrcA, int nSrcLen, LPWSTR lpDstW, int nDstLen, UINT uCodePage = CP_ACP);
    static int w2a(LPCWSTR lpSrcW, int nSrcLen, LPSTR  lpDstA, int nDstLen, UINT uCodePage = CP_ACP);

    /**
     * @brief 相同宽度字符串之间的编码转换
     * @param[in] lpBuf         需要转换的字符串
     * @param[in] uSrcCodePage  源字符串的编码格式
     * @param[in] uDstCodePage  目标字符串的编码格式
     * @param[in] nBufLen       字符串长度,如果字符串以0结尾,则此参数可填-1
     * @return 返回转换后的字符串,为空字符串时转换失败.
     */
    static CStringA a2a(LPCSTR  lpBuf, UINT uSrcCodePage, UINT uDstCodePage, int nBufLen = -1);
    static CStringW w2w(LPCWSTR lpBuf, UINT uSrcCodePage, UINT uDstCodePage, int nBufLen = -1);
};


#define ZLA2W zl::WinUtils::ZLStrConv::a2w
#define ZLW2A zl::WinUtils::ZLStrConv::w2a
#define ZLA2A zl::WinUtils::ZLStrConv::a2a
#define ZLW2W zl::WinUtils::ZLStrConv::w2w

#define ZL_ANSI_TO_UTF8_A(str) ZLA2A((str), CP_ACP, CP_UTF8)
#define ZL_UTF8_TO_ANSI_A(str) ZLA2A((str), CP_UTF8, CP_ACP)

#define ZL_ANSI_TO_UTF8_W(str) ZLW2W((str), CP_ACP, CP_UTF8)
#define ZL_UTF8_TO_ANSI_W(str) ZLW2W((str), CP_UTF8, CP_ACP)

#ifdef _UNICODE
#define ZL_ANSI_TO_UTF8 ZL_ANSI_TO_UTF8_W
#define ZL_UTF8_TO_ANSI ZL_UTF8_TO_ANSI_W
#else
#define ZL_ANSI_TO_UTF8 ZL_ANSI_TO_UTF8_A
#define ZL_UTF8_TO_ANSI ZL_UTF8_TO_ANSI_A
#endif

// 下面是实现部分

inline CStringW ZLStrConv::a2w(LPCSTR lpBuf, UINT uCodePage, int nBufLen)
{
    CStringW sRetW;
    int nDstLen = ::MultiByteToWideChar(uCodePage, 0, lpBuf, nBufLen, NULL, 0 );
    if (nDstLen > 0)
    {
        wchar_t* pResult = new wchar_t[nDstLen];
        BOOL bSuc = (0 < ::MultiByteToWideChar(uCodePage, 0, lpBuf, nBufLen, pResult, nDstLen));
        if (bSuc)
        {
            sRetW.Append(pResult, nDstLen);
        }
        delete[] pResult;
    }
    return sRetW;
}

inline CStringA ZLStrConv::w2a( LPCWSTR lpBuf, UINT uCodePage, int nBufLen )
{
    CStringA sRetA;
    int nDstLen = ::WideCharToMultiByte(uCodePage, 0, lpBuf, nBufLen, NULL, 0, NULL, NULL );
    if (nDstLen > 0)
    {
        char* pResult = new char[nDstLen];
        BOOL bSuc = (0 < ::WideCharToMultiByte( uCodePage, 0, lpBuf, nBufLen, pResult, nDstLen, NULL, NULL ));
        if (bSuc)
        {
            sRetA.Append(pResult, nDstLen);
        }
        delete[] pResult;
    }
    return sRetA;
}

inline CStringA ZLStrConv::a2a(LPCSTR lpBuf, UINT uSrcCodePage, UINT uDstCodePage, int nBufLen)
{
    CStringW wstr = a2w(lpBuf, uSrcCodePage, nBufLen);
    return w2a(wstr, uDstCodePage, wstr.GetLength() + 1);
}

inline CStringW ZLStrConv::w2w(LPCWSTR lpBuf, UINT uSrcCodePage, UINT uDstCodePage, int nBufLen)
{
    CStringA astr = w2a(lpBuf, uSrcCodePage, nBufLen);
    return a2w(astr, uDstCodePage, astr.GetLength() + 1);
}

inline int ZLStrConv::a2w(LPCSTR lpSrcA, int nSrcLen, LPWSTR lpDstW, int nDstLen, UINT uCodePage )
{
    return ::MultiByteToWideChar(uCodePage, 0, lpSrcA, nSrcLen, lpDstW, nDstLen);
}

inline int ZLStrConv::w2a(LPCWSTR lpSrcW, int nSrcLen, LPSTR lpDstA, int nDstLen, UINT uCodePage )
{
    return ::WideCharToMultiByte(uCodePage, 0, lpSrcW, nSrcLen, lpDstA, nDstLen, NULL, NULL);
}

}
}
