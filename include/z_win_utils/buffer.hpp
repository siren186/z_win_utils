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
* @brief buffer��
*/

#pragma once

namespace zl
{
namespace WinUtils
{
    /**
     * @brief buffer��
     */
    class ZLBuffer
    {
    public:
        ZLBuffer()
        {
            m_pBuf=NULL;
            m_bufSize=0;
            m_attached = FALSE;
        }
        ~ZLBuffer()
        {
            _free();
        }
        typedef unsigned char byte;
    public:
        /**
         * @brief ����buffer
         * @param[in] size �ֽڴ�С
         * @return buffer��ַ
         */
        byte* Alloc(DWORD size)
        {
            void *pTemp = realloc(m_pBuf,size+1);
            if (pTemp != NULL)
            {
                m_pBuf = (BYTE*)pTemp;
                m_bufSize = size;
                m_pBuf[size]=0;
            }
            return m_pBuf;
        }
        
        /**
         * @brief �ͷ�buffer�������Ϣ
         */
        VOID Clear()
        {
            _free();
        }
        
        /**
         * @brief ��ȡbuffer��ַ
         * @return buffer��ַ
         */
        byte* GetBuffer() const
        {
            return m_pBuf;
        }
        
        /**
         * @brief ��ȡbuffer��С
         * @return buffer�ֽڴ�С
         */
        DWORD GetSize() const
        {
            return m_bufSize;
        }

        /**
         * @brief �������ݵ���ǰbuffer
         * @param[in] src ������ʼ��ַ
         * @param[in] len Ҫ�����������ֽڳ���
         * @return buffer��ַ
         */
        const byte* Copy(const void* src,DWORD len)
        {
            Alloc(len);
            if (m_pBuf)
            {
                ::memcpy(m_pBuf,src,len);
                m_bufSize=len;
            }
            return *this;
        }
        
        /**
         * @brief �������ݵ���ǰbuffer
         * @param[in] src ����Դ
         * @return buffer��ַ
         */
        const byte* Copy(const ZLBuffer& src)
        {
            return Copy(src,src.GetSize());
        }

        /**
         * @brief �й�һ��buffer������������ͷ��ڴ�ʱ�����free
         * @param[in] src ����Դ
         */
        VOID Attach(void* buffer,size_t bufsize)
        {
            assert(buffer);
            if (buffer && bufsize)
            {
                _free();
                m_attached = TRUE;
                m_pBuf = (BYTE*)buffer;
                m_bufSize = (DWORD)bufsize;
            }
        }

        /**
         * @brief ����й�һ��buffer���ᴥ��Clear
         */
        VOID Detach()
        {
            _free();
        }

        /**
         * @brief ������д�뵽�ļ�
         * @param[in] filePath �ļ�·��
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         */
        BOOL WriteToFile(LPCTSTR filePath) const
        {
            FILE* fp=NULL;
            _tfopen_s(&fp,filePath,TEXT("wb"));
            if (fp)
            {
                size_t temp;
                temp = fwrite(GetBuffer(),GetSize(),1,fp);
                assert(temp == 1);
                fclose(fp);
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief ����ֽ����ݣ�����memset
         * @param[in] value ���ֵ
         */
        void Fill(byte value=0)
        {
            ::memset(m_pBuf,value,m_bufSize);
        }
        
        /**
         * @brief ������д�뵽ָ���ڴ�
         * @param[in] p д�����ʼ��ַ
         * @param[in] size д���С
         * @param[in] startPos Ҫд���buffer����ʼƫ��
         * @return ʵ��д������ݴ�С
         */
        DWORD Write(void* p,DWORD size,DWORD startPos=0)
        {
            if (startPos >= m_bufSize || p==NULL || m_pBuf==NULL)
            {
                return 0;
            }
            if (startPos + size >= m_bufSize)
            {
                size = m_bufSize-startPos-1;
            }
            if (size)
            {
                ::memcpy(m_pBuf+startPos,p,size);
            }
            return size;
        }

        operator const BYTE* () const
        {
            return m_pBuf;
        }
    private:
        void operator=(const ZLBuffer &src);
        void _free()
        {
            if (m_attached)
            {
                m_attached = FALSE;
                m_pBuf = NULL;
                m_bufSize = 0;
                return;
            }
            if(m_pBuf)
            {
                free(m_pBuf);
                m_pBuf = NULL;
                m_bufSize = 0;
            }
        }
    protected:
        BYTE* m_pBuf;
        DWORD m_bufSize;
        BOOL m_attached;
    };

    /**
     * @brief ��дbuffer,�̶�size��С
     */
    class ZLReadWriteBuffer : public ZLBuffer
    {
    public:
        ZLReadWriteBuffer()
        {
            m_writePos = 0;
            m_readPos = 0;
        }
        ~ZLReadWriteBuffer(){};
    public:
        /**
         * @brief д������
         * @param[in] pData ��ȡ����ʼ��ַ
         * @param[in] dataSize д���С
         * @return ʵ��д������ݴ�С
         */
        DWORD Append(const void *pData,DWORD dataSize)
        {
            if (m_writePos >= m_bufSize || pData==NULL || m_pBuf==NULL)
            {
                return 0;
            }
            if (m_writePos+dataSize > m_bufSize)
            {
                dataSize = m_bufSize-m_writePos-1;
            }
            if (dataSize)
            {
                ::memcpy(m_pBuf+m_writePos,pData,dataSize);
                m_writePos += dataSize;
            }
            return dataSize;
        }

        /**
         * @brief ��ȡ����
         * @param[in] pOut д�����ʼ��ַ
         * @param[in] bytesToRead д���С
         * @return ʵ�ʶ�ȡ�������ݴ�С
         */
        DWORD Get(void *pOut,DWORD bytesToRead)
        {
            if (m_pBuf==NULL || pOut==NULL || bytesToRead==0)
            {
                return 0;
            }
            if (bytesToRead+m_readPos >= m_writePos)
            {
                bytesToRead = m_writePos-m_readPos;
            }
            if (bytesToRead)
            {
                ::memcpy(pOut,GetBuffer() + m_readPos,bytesToRead);
                m_readPos += bytesToRead;
            }
            return bytesToRead;
        }
        
        /**
         * @brief ��ʼ��д��ָ�뵽��ʼλ��
         */
        void ResetWritePos()
        {
            m_writePos = 0;
        }
        
        /**
         * @brief ��ʼ����ȡָ�뵽��ʼλ��
         */
        void ResetReadPos()
        {
            m_readPos = 0;
        }
    private:
        void operator =(const ZLReadWriteBuffer &src);
        DWORD m_writePos;
        DWORD m_readPos;
    };
}
}