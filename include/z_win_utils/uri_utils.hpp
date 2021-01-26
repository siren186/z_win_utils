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
 * @brief URI(Uniform Resource Identifier)���õĹ���
 * 
 * ��urlencode/urldecode��
 */


#pragma once

namespace zl
{
namespace WinUtils
{

/**
 * @brief url�������
 * 
 * URLEncode����������� 
 * 1. ���ֺ���ĸ���䡣
 * 2. �ո��Ϊ"+"�š�
 * 3. �����������"%"�������ǵ�ascii��ʮ�����ƣ������������� 
 *    ���硰������ Ascii��ʮ��������B0A1����>%B0%A1(Note:����ÿ���ֽ�ǰ�Ӹ�%)��
 * 4. ���Բ�����ģ�ֻ����ĸ�����֡��̺���(-)���»���(_)����(.)�Ͳ��˺�(~)
 * �������ַ�Ҫ���������������һ���Ե�urlencodeֻ�豣�������ַ������б��롣
 */
class ZLUriUtils
{
public:
    /**
     * @brief url����
     * @param[in] str Ҫ������ַ���
     * @param[in] reserved��Ҫ����������б�����ַ���
     * @return url�������ַ���
     */
    static std::string UrlEncode(const std::string& str, const std::string& reserved);
    static std::string UrlDecode(const std::string& str);

private:
    static unsigned char ZLUriUtils::_ToHex( unsigned char x );
    static unsigned char ZLUriUtils::_FromHex( unsigned char x );

    static const std::string s_sMustBeEncode;
};


__declspec(selectany) const std::string ZLUriUtils::s_sMustBeEncode = "%<>{}|\\\"^`";

inline std::string ZLUriUtils::UrlEncode( const std::string& str, const std::string& reserved )
{
    std::string sEncodedStr;

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        char c = *it;
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_' ||
            c == '.' || c == '~')
        {
            sEncodedStr += c;
        }
        else if (c <= 0x20 || c >= 0x7F || s_sMustBeEncode.find(c) != std::string::npos || reserved.find(c) != std::string::npos)
        {
            sEncodedStr += '%';
            sEncodedStr += _ToHex((unsigned char)c >> 4);
            sEncodedStr += _ToHex((unsigned char)c % 16);
        }
        else sEncodedStr += c;
    }

    return sEncodedStr;
}

inline std::string ZLUriUtils::UrlDecode( const std::string& str )
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            if (i+2 < length)
            {
                unsigned char high = _FromHex((unsigned char)str[++i]);
                unsigned char low  = _FromHex((unsigned char)str[++i]);
                strTemp += high*16 + low;
            }
        }
        else strTemp += str[i];
    }
    return strTemp;
}

inline unsigned char ZLUriUtils::_ToHex( unsigned char x )
{
    return  x > 9 ? x + 55 : x + 48;
}

inline unsigned char ZLUriUtils::_FromHex( unsigned char x )
{
    unsigned char y = 0;
    if (x >= 'A' && x <= 'Z')       y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')  y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')  y = x - '0';
    else y = 0;
    return y;
}

} // end of namespace WinUtils
} // end of namespace zl
