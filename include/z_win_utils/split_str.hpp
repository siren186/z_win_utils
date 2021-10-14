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
 * @brief 字符串切割
 */


#pragma once
#include <string>

namespace zl
{
namespace WinUtils
{

/**
 * @brief 字符串切割,如"hello,world",以','号切割后, 得到"hello"和"world".
 */
template<class T>
class ZLSplitStrT
{
public:
    typedef typename T::size_type  size_type;
    typedef typename T::value_type value_type;

    /**
     * @brief 以字符为切割点, 切割字符串
     * @param[in]  sSrc 待切割字符串
     * @param[in]  chSplitChar 指定以什么字符为切割点
     * @param[out] vecOutput 切割后的各个子串
     * @param[in]  bIncludeEmpty 是否包含空的子串.
     * @note 如"h,,e", 若bIncludeEmpty为true,则结果为{"h", "", "e"}. 否则结果为{"h", "e"}.
     */
    static void Split(const T& sSrc,  const value_type chSplitChar, std::vector<T>& vecOutput, bool bIncludeEmpty = true);

    /**
     * @brief 以字符串为切割点, 切割字符串
     * @param[in]  sSrc 待切割字符串
     * @param[in]  sSplitStr 指定以什么字符串为切割点
     * @param[out] vecOutput 切割后的各个子串
     * @param[in]  bIncludeEmpty 是否包含空的子串.
     * @note 如"h,,e", 若bIncludeEmpty为true,则结果为{"h", "", "e"}. 否则结果为{"h", "e"}.
     */
    static void Split(const T& sSrc,const T& sSplitStr, std::vector<T>& vecOutput, bool bIncludeEmpty = true);

    /**
     * @brief 将容器中的字符串用分隔符拼接在一起
     * @param[in] container STL容器，如vector,list等
     * @param[in] str 字符串分隔符
     * @return 返回拼接后的结果
     * @par 示例
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ','); // 返回结果："hello,world"
     *     std::wstring result2 = ZLSplitStr::Join(lst, "||"); // 返回结果：L"hello||world"
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
     * @brief 将容器中的字符串用分隔符拼接在一起
     * @param[in] container STL容器，如vector,list等
     * @param[in] ch 字符分隔符
     * @return 返回拼接后的结果
     * @par 示例
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ','); // 返回结果："hello,world"
     *     std::wstring result2 = ZLSplitStr::Join(lst, "||"); // 返回结果：L"hello||world"
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
     * @brief 将容器中的字符串用分隔符拼接在一起
     * @param[in] container STL容器，如vector,list等
     * @param[in] split 字符分隔符
     * @param[in] prefix 每个字符串的前缀字符
     * @param[in] suffix 每个字符串的后缀字符
     * @return 返回拼接后的结果
     * @par 示例
     * @code
     *     std::vector<std::string> vec { "hello", "world" };
     *     std::list<std::wstring> lst { L"hello", L"world" };
     *     std::string result1 = ZLSplitStr::Join(vec, ',', '\'', '\''); // 返回结果："'hello','world'"
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

// 以下是实现部分

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
