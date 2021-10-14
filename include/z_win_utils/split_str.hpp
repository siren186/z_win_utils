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
 * @brief �ַ����и�
 */


#pragma once
#include <string>

namespace zl
{
namespace WinUtils
{

/**
 * @brief �ַ����и�,��"hello,world",��','���и��, �õ�"hello"��"world".
 */
template<class T>
class ZLSplitStrT
{
public:
    typedef typename T::size_type  size_type;
    typedef typename T::value_type value_type;

    /**
     * @brief ���ַ�Ϊ�и��, �и��ַ���
     * @param[in]  sSrc ���и��ַ���
     * @param[in]  chSplitChar ָ����ʲô�ַ�Ϊ�и��
     * @param[out] vecOutput �и��ĸ����Ӵ�
     * @param[in]  bIncludeEmpty �Ƿ�����յ��Ӵ�.
     * @note ��"h,,e", ��bIncludeEmptyΪtrue,����Ϊ{"h", "", "e"}. ������Ϊ{"h", "e"}.
     */
    static void Split(const T& sSrc,  const value_type chSplitChar, std::vector<T>& vecOutput, bool bIncludeEmpty = true);

    /**
     * @brief ���ַ���Ϊ�и��, �и��ַ���
     * @param[in]  sSrc ���и��ַ���
     * @param[in]  sSplitStr ָ����ʲô�ַ���Ϊ�и��
     * @param[out] vecOutput �и��ĸ����Ӵ�
     * @param[in]  bIncludeEmpty �Ƿ�����յ��Ӵ�.
     * @note ��"h,,e", ��bIncludeEmptyΪtrue,����Ϊ{"h", "", "e"}. ������Ϊ{"h", "e"}.
     */
    static void Split(const T& sSrc,const T& sSplitStr, std::vector<T>& vecOutput, bool bIncludeEmpty = true);

    /**
     * @brief �������е��ַ����÷ָ���ƴ����һ��
     * @param[in] container STL��������vector,list��
     * @param[in] str �ַ����ָ���
     * @return ����ƴ�Ӻ�Ľ��
     * @par ʾ��
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ','); // ���ؽ����"hello,world"
     *     std::wstring result2 = ZLSplitStr::Join(lst, "||"); // ���ؽ����L"hello||world"
     * @end_code
     */
    template< typename _ContainerT>
    static typename _ContainerT::value_type Join(const _ContainerT& container, const typename _ContainerT::value_type& str)
    {
        auto itBegin = std::begin(container);
        auto itEnd = std::end(container);

        typename _ContainerT::value_type result;
        if (itBegin != itEnd)
        {
            result.append(*itBegin);
            ++itBegin;
        }

        for (; itBegin != itEnd; ++itBegin)
        {
            result.append(str);
            result.append(*itBegin);
        }

        return result;
    }

    /**
     * @brief �������е��ַ����÷ָ���ƴ����һ��
     * @param[in] container STL��������vector,list��
     * @param[in] ch �ַ��ָ���
     * @return ����ƴ�Ӻ�Ľ��
     * @par ʾ��
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ','); // ���ؽ����"hello,world"
     *     std::wstring result2 = ZLSplitStr::Join(lst, "||"); // ���ؽ����L"hello||world"
     * @end_code
     */
    template< typename _ContainerT>
    static typename _ContainerT::value_type Join(const _ContainerT& container, const typename _ContainerT::value_type::value_type& ch)
    {
        auto itBegin = std::begin(container);
        auto itEnd = std::end(container);

        typename _ContainerT::value_type result;
        if (itBegin != itEnd)
        {
            result.append(*itBegin);
            ++itBegin;
        }

        for (; itBegin != itEnd; ++itBegin)
        {
            result.push_back(ch);
            result.append(*itBegin);
        }

        return result;
    }
    

    /**
     * @brief �������е��ַ����÷ָ���ƴ����һ��
     * @param[in] container STL��������vector,list��
     * @param[in] split �ַ��ָ���
     * @param[in] prefix ÿ���ַ�����ǰ׺�ַ�
     * @param[in] suffix ÿ���ַ����ĺ�׺�ַ�
     * @return ����ƴ�Ӻ�Ľ��
     * @par ʾ��
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ',', '\'', '\''); // ���ؽ����"'hello','world'"
     * @end_code
     */
    template< typename _ContainerT>
    static typename _ContainerT::value_type Join(
        const _ContainerT& container,
        const typename _ContainerT::value_type::value_type& split,
        const typename _ContainerT::value_type::value_type& prefix,
        const typename _ContainerT::value_type::value_type& suffix
    )
    {
        auto itBegin = std::begin(container);
        auto itEnd = std::end(container);

        typename _ContainerT::value_type result;
        if (itBegin != itEnd)
        {
            result.push_back(prefix);
            result.append(*itBegin);
            result.push_back(suffix);
            ++itBegin;
        }

        for (; itBegin != itEnd; ++itBegin)
        {
            result.push_back(split);
            result.push_back(prefix);
            result.append(*itBegin);
            result.push_back(suffix);
        }

        return result;
    }
};

typedef ZLSplitStrT<std::string>  ZLSplitStrA;
typedef ZLSplitStrT<std::wstring> ZLSplitStrW;

#if defined(UNICODE)
typedef ZLSplitStrW ZLSplitStr;
#else
typedef ZLSplitStrA ZLSplitStr;
#endif

// ������ʵ�ֲ���

template<class T>
void ZLSplitStrT<T>::Split( const T& sSrc, const value_type chSplitChar, std::vector<T>& vecOutput, bool bIncludeEmpty )
{
    vecOutput.clear();
    if (sSrc.empty())
    {
        return;
    }

    T tmp_str;
    size_type pos_begin = 0;
    size_type pos_find  = 0;
    while(pos_find != T::npos)
    {
        pos_find = sSrc.find_first_of(chSplitChar, pos_begin);
        if (pos_find != T::npos)
            tmp_str = sSrc.substr(pos_begin, pos_find - pos_begin);
        else
            tmp_str = sSrc.substr(pos_begin);
        if (bIncludeEmpty || tmp_str.size()>0)
            vecOutput.push_back(tmp_str);
        pos_begin = pos_find + 1;
    }
}

template<class T>
inline void ZLSplitStrT<T>::Split( const T& sSrc, const T& sSplitStr, std::vector<T>& vecOutput, bool bIncludeEmpty)
{
    vecOutput.clear();
    if (sSrc.empty())
    {
        return;
    }

    T tmp_str;
    size_type pos_find  = 0;
    size_type pos_begin = 0;
    while(pos_find != T::npos)
    {
        pos_find = sSrc.find(sSplitStr, pos_begin);
        if (pos_find != T::npos)
            tmp_str = sSrc.substr(pos_begin, pos_find - pos_begin);
        else
            tmp_str = sSrc.substr(pos_begin);
        if (bIncludeEmpty || tmp_str.size()>0)
            vecOutput.push_back(tmp_str);
        pos_begin = pos_find + sSplitStr.size();
    }
}

} // end of namespace WinUtils
} ///> end of namespace zl
