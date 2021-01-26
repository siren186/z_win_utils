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
 * @brief ��Ļ���
 * 
 *      ��Ļ�ֱ������õ�.
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief ��Ļ�ֱ�������
     */
    class ZLScreen
    {
    public:
        /**
         * @brief ��ȡ��Ļ���
         * @param[out] width  ��
         * @param[out] height ��
         * @see GetSystemMetrics
         */
        static void GetScreenResolution(int& width, int& height)
        {
            width = ::GetSystemMetrics(SM_CXSCREEN);
            height = ::GetSystemMetrics(SM_CYSCREEN);
        }
        /**
         * @brief ������Ļ���
         * @param[in] width      ��
         * @param[in] height     ��
         * @param[in] BitsPerPel ÿ���صı�����
         * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
         * @see ChangeDisplaySettings
         */
        static BOOL SetScreenResolution(int width, int height, int BitsPerPel = 32)
        {
            DEVMODE lpDevMode;
            lpDevMode.dmBitsPerPel  = 32;
            lpDevMode.dmPelsWidth   = width;
            lpDevMode.dmPelsHeight  = height;
            lpDevMode.dmSize        = sizeof(lpDevMode);
            lpDevMode.dmFields      = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
            LONG result = ::ChangeDisplaySettings(&lpDevMode,0);
            if (result == DISP_CHANGE_SUCCESSFUL)
            {
                ::ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
                return TRUE;
            }
            else
            {
                ::ChangeDisplaySettings(NULL,0);
                return FALSE;
            }
        }
    };

}
}
