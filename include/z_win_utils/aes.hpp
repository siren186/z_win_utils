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
 * @brief AES�㷨ʵ��
 */

#pragma once
#include <string.h>
#include "rijndael.hpp"

namespace zl
{
namespace WinUtils
{

/**
 * @brief AES(Advanced Encryption Standard)�߼����ܱ�׼
 *
 *  ���ṩ2���ӿ�:Encrypt��Decrypt,�ֱ�����AES���ܺͽ���,
 * @note: ע:���������ݺ��������,������ͬһ���ڴ���.����ʹ��.����Ԥ���㹻��С����������.(�����볤�ȴ�16����)
 * @todo ��:PADDING_PKCS7, CFB, OFB��δʵ��
 */
class ZLAes
{
public:
    enum KeyBits { KEY_BITS_128 = 128, KEY_BITS_192 = 192, KEY_BITS_256 = 256 };
    enum PaddingMode
    {
        // The number of bytes to be padded equals to "8 - numberOfBytes(clearText) mod 8".
        // So 1 to 8 bytes will be padded to the clear text data depending on the length of the clear text data.
        // All padded bytes have the same value - the number of bytes padded.
        // If numberOfBytes(clearText) mod 8 == 7, PM = M + 0x01
        // If numberOfBytes(clearText) mod 8 == 6, PM = M + 0x0202
        // If numberOfBytes(clearText) mod 8 == 5, PM = M + 0x030303
        // ...
        // If numberOfBytes(clearText) mod 8 == 0, PM = M + 0x0808080808080808
        PADDING_PKCS5,
        
        // ����ַ�����һ���ֽ�������ɣ�ÿ���ֽ������ֽ����еĳ���
        // �����ʾ����ʾ��Щģʽ�Ĺ���ԭ���ٶ��鳤��Ϊ 8�����ݳ���Ϊ 9��������ð�λ�ֽ������� 7�����ݵ��� FF FF FF FF FF FF FF FF FF��
        // ����: FF FF FF FF FF FF FF FF FF
        // PKCS7 ���: FF FF FF FF FF FF FF FF FF 07 07 07 07 07 07 07
        PADDING_PKCS7,
    };

    enum BlockMode
    {
        ECB, // Electronic Code Book,��һ�ֻ����ļ��ܷ�ʽ,���ı��ָ�ɷ��鳤����ȵĿ�(���㲹��),Ȼ�󵥶�һ��������,һ��������������.
        CBC, // Cipher Block Chaining,��һ��ѭ��ģʽ,ǰһ����������ĺ͵�ǰ������������������ټ���,��������Ŀ������ǿ�ƽ��Ѷ�.
        CFB, // Cipher Feed Back,ʵ������һ�ַ���ģʽ��Ŀ��Ҳ����ǿ�ƽ���Ѷȡ�
        OFB, // Output Feed Back,ʵ������һ�ַ���ģʽ��Ŀ��Ҳ����ǿ�ƽ���Ѷȡ�
    };

public:
    /**
     * @brief AES����
     * @param[in]  block_mode
     * @param[in]  padding_mode
     * @param[in]  key_bits ��Կ���ȷ�Ϊ 128λ��192λ��256λ
     * @param[in]  key ����
     * @param[in]  src ����������
     * @param[in]  src_size ���������ݴ�С
     * @param[out] dest ���ܽ��������,��Ԥ���㹻��С����������.(�����볤�ȴ�16����)
     * @param[in]  dest_size ���ܽ����������С
     * @return ���ܺ�Ļ�������С
     * @note src��dest������ͬһ������
     */
    static size_t Encrypt(
        const BlockMode      block_mode,
        const PaddingMode    padding_mode,
        const KeyBits        key_bits,
        const unsigned char* key,
        const unsigned char* src,
        const size_t         src_size,
        unsigned char*       dest,
        const size_t         dest_size);

    /**
     * @brief AES����
     * @param[in]  block_mode
     * @param[in]  padding_mode
     * @param[in]  key_bits ��Կ���ȷ�Ϊ 128λ��192λ��256λ
     * @param[in]  key ����
     * @param[in]  src ����������
     * @param[in]  src_size ���������ݴ�С
     * @param[out] dest ���ܽ��������
     * @param[in]  dest_size ���ܽ����������С
     * @return ���ܺ�Ļ�������С
     * @note src��dest������ͬһ������
     */
    static size_t Decrypt(
        const BlockMode      block_mode,
        const PaddingMode    padding_mode,
        const KeyBits        key_bits,
        const unsigned char* key,
        const unsigned char* src,
        const size_t         src_size,
        unsigned char*       dest,
        const size_t         dest_size);

private:
    // Padding
    static size_t _Padding(const PaddingMode padding_mode, const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);
    static size_t _PaddingPKCS5(const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);

    // De-padding
    static size_t _DePadding(const PaddingMode padding_mode, const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);
    static size_t _DePaddingPKCS5(const unsigned char* src, const size_t src_size, unsigned char* dest, const size_t dest_size);

    // Encrypt
    static bool _Encrypt(BlockMode block_mode, const unsigned char* key, const KeyBits key_bits, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _EcbEncrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _CbcEncrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);

    // Decrypt
    static bool _Decrypt(BlockMode block_mode, const unsigned char* key, const KeyBits key_bits, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _EcbDecrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
    static void _CbcDecrypt(const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out);
};

inline size_t ZLAes::Encrypt(
    const BlockMode      block_mode,
    const PaddingMode    padding_mode,
    const KeyBits        key_bits,
    const unsigned char* key,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t ret = 0;

    // Step1: У���������
    if (NULL == key   ||
        NULL == src   ||
        NULL == dest  ||
        src_size <= 0 ||
        (src_size + 16) > dest_size)
    {
        goto Exit0;
    }

    // Step2: Padding
    size_t real_size = _Padding(padding_mode, src, src_size, dest, dest_size);
    if (real_size < src_size ||
        real_size % 16 != 0)
    {
        goto Exit0;
    }

    // Step3: Encrypt
    if (_Encrypt(block_mode, key, key_bits, dest, real_size, dest))
    {
        ret = real_size;
    }

Exit0:
    return ret;
}

inline size_t ZLAes::Decrypt(
    const BlockMode      block_mode,
    const PaddingMode    padding_mode,
    const KeyBits        key_bits,
    const unsigned char* key,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t ret = 0;
    // Step1: У���������
    if (NULL == key   ||
        NULL == src   ||
        NULL == dest  ||
        src_size <= 0 ||
        (src_size > dest_size))
    {
        goto Exit0;
    }

    // Step2: Decrypt
    if (_Decrypt(block_mode, key, key_bits, src, src_size, dest))
    {
        // Step3: De-padding
        size_t real_size = _DePadding(padding_mode, dest, src_size, dest, dest_size);
        ret = real_size;
    }

Exit0:
    return ret;
}

inline size_t ZLAes::_Padding(
    const PaddingMode    padding_mode,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    switch (padding_mode)
    {
    case PADDING_PKCS5:
        return _PaddingPKCS5(src, src_size, dest, dest_size);
    }
    return 0;
}

inline size_t ZLAes::_PaddingPKCS5(
                                   const unsigned char* src,
                                   const size_t         src_size,
                                   unsigned char*       dest,
                                   const size_t         dest_size
                                   )
{
    size_t need_size =  ((src_size + 16) / 16) * 16;
    if (need_size <= dest_size)
    {
        memcpy_s(dest, dest_size, src, src_size);
        size_t value = need_size - src_size;
        for (size_t i = 0; i < value; i++)
        {
            dest[src_size + i] = (unsigned char)value;
        }
    }
    else
    {
        need_size = 0;
    }
    return need_size;
}

inline size_t ZLAes::_DePadding(
    const PaddingMode    padding_mode,
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    switch (padding_mode)
    {
    case PADDING_PKCS5:
        return _DePaddingPKCS5(src, src_size, dest, dest_size);
    }
    return 0;
}

inline size_t ZLAes::_DePaddingPKCS5(
    const unsigned char* src,
    const size_t         src_size,
    unsigned char*       dest,
    const size_t         dest_size
    )
{
    size_t padding_value = *(src + src_size - 1);
    size_t valid_size = src_size - padding_value;

    memcpy_s(dest, dest_size, src, valid_size);
    return valid_size;
}

inline bool ZLAes::_Encrypt(
    BlockMode            block_mode,
    const unsigned char* key,
    const KeyBits        key_bits,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    bool suc = false;
    unsigned long rk[ZLRIJNDAEL_RK_SIZE] = {0};
    int rounds = ZLRijndael::SetupEncrypt(rk, key, (int)key_bits);
    if (0 != rounds)
    {
        suc = true;
        switch (block_mode)
        {
        case ECB:
            _EcbEncrypt(rk, rounds, in, in_size, out);
            break;
        case CBC:
            _CbcEncrypt(rk, rounds, in, in_size, out);
            break;
        default:
            suc = false;
            break;
        }
    }
    return suc;
}

inline bool ZLAes::_Decrypt(
    BlockMode            block_mode,
    const unsigned char* key,
    const KeyBits        key_bits,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    bool suc = false;
    unsigned long rk[ZLRIJNDAEL_RK_SIZE] = {0};
    int rounds = ZLRijndael::SetupDecrypt(rk, key, (int)key_bits);
    if (0 != rounds)
    {
        suc = true;
        switch (block_mode)
        {
        case ECB:
            _EcbDecrypt(rk, rounds, in, in_size, out);
            break;
        case CBC:
            _CbcDecrypt(rk, rounds, in, in_size, out);
            break;
        default:
            suc = false;
            break;
        }
    }
    return suc;
}

inline void ZLAes::_EcbEncrypt(
    const unsigned long* rk,
    const int            rounds,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    size_t block_count = in_size / 16;
    while (block_count)
    {
        ZLRijndael::Encrypt(rk, rounds, in, out);
        in  += 16;
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_CbcEncrypt(
    const unsigned long* rk,
    const int            rounds,
    const unsigned char* in,
    const size_t         in_size,
    unsigned char*       out
    )
{
    unsigned char buffer[16] = {0};
    size_t block_count = in_size / 16;
    while (block_count)
    {
        for (unsigned int pos = 0; pos < 16; ++pos)
            buffer[pos] ^= *in++;
        ZLRijndael::Encrypt(rk, rounds, buffer, out);
        memcpy(buffer, out, 16);
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_EcbDecrypt(
                               const unsigned long* rk,
                               const int            rounds,
                               const unsigned char* in,
                               const size_t         in_size,
                               unsigned char*       out
                               )
{
    size_t block_count = in_size / 16;
    while (block_count)
    {
        ZLRijndael::Decrypt(rk, rounds, in, out);
        in  += 16;
        out += 16;
        block_count -= 1;
    }
}

inline void ZLAes::_CbcDecrypt( const unsigned long *rk, const int rounds, const unsigned char* in, const size_t in_size, unsigned char* out )
{
    size_t block_count = in_size / 16;

    unsigned char buffer[16] = {0};
    memset(buffer, 0, sizeof(buffer));
    ZLRijndael::Decrypt(rk, rounds, in, out);
    for (unsigned int pos = 0; pos < 16; ++pos)
        *out++ ^= buffer[pos];
    in += 16;
    block_count--;

    while (block_count)
    {
        ZLRijndael::Decrypt(rk, rounds, in, out);
        for (unsigned int pos = 0; pos < 16; ++pos)
            *out++ ^= *(in- 16 + pos);
        in += 16;
        --block_count;
    }
}

} // end of WinUtils
} // end of zl