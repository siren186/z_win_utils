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
 * @brief 磁盘驱动器相关
 */


#pragma once
#include "win_utils_header.h"
#include <map>
#include <windows.h>
#include <winioctl.h>
#include <atlcoll.h>
#include <atlstr.h>
#include <atlfile.h>

namespace zl
{
namespace WinUtils
{

/**
 * @brief 物理磁盘操作接口，操作的路径格式："\\\\.\\PhysicalDrive%u"
 */
class ZLPhysicalDrive
{
public:
    // @see PARTITION_INFORMATION_EX
    struct DriveLayoutInfoEx
    {
        PARTITION_STYLE PartitionStyle;
        union
        {
            DRIVE_LAYOUT_INFORMATION_MBR Mbr;
            DRIVE_LAYOUT_INFORMATION_GPT Gpt;
        };
        CAtlArray<PARTITION_INFORMATION_EX> PartitionEntry;
    };

public:
    ZLPhysicalDrive() : m_diskNumber(-1) ,m_bStoragePropertyQueried(FALSE)
    {
    }

    ~ZLPhysicalDrive()
    {
        Close();
    }

    /**
     * @brief 打开磁盘
     * @param[in] dwDiskNumber 指定要打开的磁盘索引号
     * @param[in] dwDesiredAccess 读写权限设置
     * @param[in] dwShareMode 共享读写设置
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT Open(DWORD dwDiskNumber, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE)
    {
        Close();

        m_diskNumber = dwDiskNumber;
        CString diskpath;
        diskpath.Format(_T("\\\\.\\PhysicalDrive%u"), m_diskNumber);
        return m_file.Create(diskpath, dwDesiredAccess, dwShareMode, OPEN_EXISTING);
    }

    /**
     * @brief 打开磁盘
     * @param[in] dwDiskNumber 指定要打开的磁盘索引号
     * @param[in] dwDesiredAccess 读写权限设置
     * @param[in] dwShareMode 共享读写设置
     * @param[in] pnDiskSize 磁盘大小，不需要可填空
     * @param[in] bLockVolume 是否锁定磁盘的所有卷标
     * @param[in] bDismountVolume 是否dismount磁盘的所有卷标，如果为TRUE，则参数bLockVolume无论是否为TRUE都会强制为TRUE
     * @param[in] pDiskGeometry 磁盘Geometry信息，不需要可填空
     * @param[in] pDrvLayout 磁盘Layout信息，不需要可填空
     * @param[in] nSeekOffset 读写指针跳转到指定的偏移位置，默认为0
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT OpenEx(
        DWORD dwDiskNumber,
        DWORD dwDesiredAccess = GENERIC_READ,
        DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
        LONGLONG* pnDiskSize = NULL,
        BOOL bLockVolume = FALSE,
        BOOL bDismountVolume = FALSE,
        DISK_GEOMETRY* pDiskGeometry = NULL,
        DriveLayoutInfoEx* pDrvLayout = NULL,
        LONGLONG nSeekOffset = 0)
    {
        HRESULT hr = Open(dwDiskNumber, dwDesiredAccess, dwShareMode);
        if (FAILED(hr))
        {
            goto Exit0;
        }

        if (bLockVolume)
        {
            hr = LockVolume(-1);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        if (bDismountVolume)
        {
            if (!bLockVolume)
            {
                hr = LockVolume(-1);
                if (FAILED(hr))
                {
                    goto Exit0;
                }
            }

            hr = DismountVolume(-1);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        if (pnDiskSize)
        {
            hr = GetDiskSize(*pnDiskSize);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        if (pDiskGeometry)
        {
            hr = GetDiskGeometry(*pDiskGeometry);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        if (pDrvLayout)
        {
            hr = GetDriveLayoutEx(*pDrvLayout);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        if (0 != nSeekOffset)
        {
            hr = Seek(nSeekOffset, FILE_BEGIN);
            if (FAILED(hr))
            {
                goto Exit0;
            }
        }

        hr = S_OK;

Exit0:
        if (FAILED(hr))
        {
            Close();
        }

        return hr;
    }

    /**
     * @brief 打开磁盘（只读）
     * @param[in] dwDiskNumber 指定要打开的磁盘索引号
     * @param[in] pnDiskSize 磁盘大小，不需要可填空
     * @param[in] bLockVolume 是否锁定磁盘的所有卷标
     * @param[in] bDismountVolume 是否dismount磁盘的所有卷标，如果为TRUE，则参数bLockVolume无论是否为TRUE都会强制为TRUE
     * @param[in] pDiskGeometry 磁盘Geometry信息，不需要可填空
     * @param[in] pDrvLayout 磁盘Layout信息，不需要可填空
     * @param[in] nSeekOffset 读写指针跳转到指定的偏移位置，默认为0
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT OpenReadOnly(
        DWORD dwDiskNumber,
        LONGLONG* pnDiskSize = NULL,
        BOOL bLockVolume = FALSE,
        BOOL bDismountVolume = FALSE,
        DISK_GEOMETRY* pDiskGeometry = NULL,
        DriveLayoutInfoEx* pDrvLayout = NULL,
        LONGLONG nSeekOffset = 0)
    {
        return OpenEx(
            dwDiskNumber,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            pnDiskSize,
            bLockVolume,
            bDismountVolume,
            pDiskGeometry,
            pDrvLayout,
            nSeekOffset);
    }

    /**
     * @brief 打开磁盘（读写）
     * @param[in] dwDiskNumber 指定要打开的磁盘索引号
     * @param[in] pnDiskSize 磁盘大小，不需要可填空
     * @param[in] bLockVolume 是否锁定磁盘的所有卷标
     * @param[in] bDismountVolume 是否dismount磁盘的所有卷标，如果为TRUE，则参数bLockVolume无论是否为TRUE都会强制为TRUE
     * @param[in] pDiskGeometry 磁盘Geometry信息，不需要可填空
     * @param[in] pDrvLayout 磁盘Layout信息，不需要可填空
     * @param[in] nSeekOffset 读写指针跳转到指定的偏移位置，默认为0
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT OpenReadWrite(
        DWORD dwDiskNumber,
        LONGLONG* pnDiskSize = NULL,
        BOOL bLockVolume = FALSE,
        BOOL bDismountVolume = FALSE,
        DISK_GEOMETRY* pDiskGeometry = NULL,
        DriveLayoutInfoEx* pDrvLayout = NULL,
        LONGLONG nSeekOffset = 0)
    {
        return OpenEx(
            dwDiskNumber,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            pnDiskSize,
            bLockVolume,
            bDismountVolume,
            pDiskGeometry,
            pDrvLayout,
            nSeekOffset);
    }

    /**
     * @brief 关闭磁盘
     */
    void Close()
    {
        UnlockVolume(-1);
        m_file.Close();

        // 清空所有成员变量
        m_bStoragePropertyQueried = FALSE;
        m_diskNumber = -1;
        m_diskModel.Empty();
        m_diskSerialNumber.Empty();
        m_diskBusType = BusTypeUnknown;
        m_diskVendorId.Empty();
        m_diskProductId.Empty();
        m_diskProductRevision.Empty();
        m_lockedVolumes.clear();
    }

    HRESULT Seek(LONGLONG nOffset, DWORD dwFrom = FILE_BEGIN)
    {
        return m_file.Seek(nOffset, dwFrom);
    }

    HRESULT GetPosition(ULONGLONG& nPos) const
    {
        return m_file.GetPosition(nPos);
    }

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize)
    {
        return m_file.Read(pBuffer, nBufSize);
    }

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
    {
        return m_file.Read(pBuffer, nBufSize, nBytesRead);
    }

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize, LPOVERLAPPED pOverlapped)
    {
        return m_file.Read(pBuffer, nBufSize, pOverlapped);
    }

    HRESULT Read(LPVOID pBuffer, DWORD nBufSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
    {
        return m_file.Read(pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine);
    }

    HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, DWORD* pnBytesWritten = NULL)
    {
        return m_file.Write(pBuffer, nBufSize, pnBytesWritten);
    }

    HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, LPOVERLAPPED pOverlapped)
    {
        return m_file.Write(pBuffer, nBufSize, pOverlapped);
    }

    HRESULT Write(LPCVOID pBuffer, DWORD nBufSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
    {
        return m_file.Write(pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine);
    }

    HRESULT Flush()
    {
        return m_file.Flush();
    }

    HRESULT GetOverlappedResult(LPOVERLAPPED pOverlapped, DWORD& dwBytesTransferred, BOOL bWait)
    {
        return m_file.GetOverlappedResult(pOverlapped, dwBytesTransferred, bWait);
    }

    DWORD GetDiskNumber() const
    {
        return m_diskNumber;
    }

    CString GetDiskPath() const
    {
        CString path;
        path.Format(_T("\\\\.\\PhysicalDrive%u"), GetDiskNumber());
        return path;
    }

    HRESULT GetDiskSize(LONGLONG& size)
    {
        DWORD junk = 0;
        GET_LENGTH_INFORMATION info;
        ZeroMemory(&info, sizeof(GET_LENGTH_INFORMATION));

        HRESULT hr = Ioctl(IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &info, sizeof(info), &junk);
        if (SUCCEEDED(hr))
        {
            size = info.Length.QuadPart;
        }

        return hr;
    }

    HRESULT GetDiskGeometry(DISK_GEOMETRY& geometry)
    {
        DWORD junk = 0;
        return Ioctl(IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof(DISK_GEOMETRY), &junk);
    }

    // 获取磁盘型号，格式："VendorId ProductId ProductRevision"
    HRESULT GetDiskModel(_Out_ CString& model)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            model = m_diskModel;
        }
        return hr;
    }

    HRESULT GetDiskSerialNumber(CString& serialNumber)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            serialNumber = m_diskSerialNumber;
        }
        return hr;
    }

    HRESULT GetDiskVendorId(CString& vendorId)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            vendorId = m_diskVendorId;
        }
        return hr;
    }

    HRESULT GetDiskProductId(CString& productId)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            productId = m_diskProductId;
        }
        return hr;
    }

    HRESULT GetDiskProductRevision(CString& productRevision)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            productRevision = m_diskProductRevision;
        }
        return hr;
    }

    HRESULT GetDiskBusType(STORAGE_BUS_TYPE& type)
    {
        HRESULT hr = StorageQueryProperty();
        if (SUCCEEDED(hr))
        {
            type = m_diskBusType;
        }
        return hr;
    }

    HRESULT GetDiskBusType(CString& busType)
    {
        // 以下项在不同VS版本中可能不存在，故统一使用局部变量代替
        const UINT32 BusTypeVirtual = 14;
        const UINT32 BusTypeFileBackedVirtual = 15;
        const UINT32 BusTypeSpaces = 16;
        const UINT32 BusTypeNvme = 17;
        const UINT32 BusTypeSCM = 18;

        STORAGE_BUS_TYPE type;
        HRESULT hr = GetDiskBusType(type);
        if (FAILED(hr))
        {
            return hr;
        }

        switch (type)
        {
        case BusTypeScsi:
            busType = _T("SCSI");
            break;
        case BusTypeAtapi:
            busType = _T("ATAPI");
            break;
        case BusTypeAta:
            busType = _T("ATA");
            break;
        case BusType1394:
            busType = _T("IEEE-1394");
            break;
        case BusTypeSsa:
            busType = _T("SSA");
            break;
        case BusTypeFibre:
            busType = _T("Fibre Channel bus");
            break;
        case BusTypeUsb:
            busType = _T("USB");
            break;
        case BusTypeRAID:
            busType = _T("RAID");
            break;
        case BusTypeiScsi:
            busType = _T("ISCSI");
            break;
        case BusTypeSas:
            busType = _T("SAS");
            break;
        case BusTypeSata:
            busType = _T("SATA");
            break;
        case BusTypeSd:
            busType = _T("SD");
            break;
        case BusTypeMmc:
            busType = _T("MMC");
            break;
        case BusTypeVirtual:
            busType = _T("Virtual");
            break;
        case BusTypeFileBackedVirtual:
            busType = _T("FileBackedVirtual");
            break;
        case BusTypeSpaces:
            busType = _T("Spaces");
            break;
        case BusTypeNvme:
            busType = _T("NVMe");
            break;
        case BusTypeSCM:
            busType = _T("SCM");
            break;
        default:
            busType = _T("");
            break;
        }

        return S_OK;
    }

    HRESULT GetDiskLetters(CAtlArray<TCHAR>& letters)
    {
        TCHAR buffer[MAX_PATH];
        ZeroMemory(buffer, MAX_PATH * sizeof(TCHAR));

        if (0 == ::GetLogicalDriveStrings(MAX_PATH, buffer))
        {
            return S_OK;
        }

        for (TCHAR* pbuf = buffer; *pbuf != '\x00'; pbuf += (lstrlen(pbuf) + 1))
        {
            // 路径格式："\\\\.\\C:"
            CString filepath = _T("\\\\.\\");
            filepath += pbuf;
            filepath = filepath.Left(filepath.GetLength() - 1);

            CAtlFile file;
            HRESULT hr = file.Create(filepath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
            if (FAILED(hr))
            {
                continue;
            }

            DWORD junk = 0;
            VOLUME_DISK_EXTENTS exts = { 0 };
            if (FALSE == ::DeviceIoControl(file, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &exts, sizeof(VOLUME_DISK_EXTENTS), &junk, NULL))
            {
                continue;
            }

            for (DWORD n = 0; n < exts.NumberOfDiskExtents; ++n)
            {
                if (m_diskNumber == exts.Extents[n].DiskNumber)
                {
                    letters.Add(pbuf[0]);
                }
            }
        }

        return S_OK;
    }

    HRESULT GetDiskLetters(CString& letters)
    {
        CAtlArray<TCHAR> arr;
        HRESULT hr = GetDiskLetters(arr);
        if (SUCCEEDED(hr))
        {
            letters.Empty();
            for (size_t i = 0; i < arr.GetCount(); i++)
            {
                letters.AppendChar(arr[i]);
                letters.Append(_T(": "));
            }
            letters.Trim();
        }
        return hr;
    }

    HRESULT GetDriveLayoutEx(DriveLayoutInfoEx& layout)
    {
        HRESULT hr = E_FAIL;
        DWORD junk = 0;
        const int MAX_PARTITION = 64;
        const int LEN = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + sizeof(PARTITION_INFORMATION_EX) * (MAX_PARTITION - 1);
        DRIVE_LAYOUT_INFORMATION_EX* pLayoutEx = (DRIVE_LAYOUT_INFORMATION_EX*)malloc(LEN);

        if (!pLayoutEx)
        {
            hr = E_OUTOFMEMORY;
            goto Exit0;
        }

        ZeroMemory(pLayoutEx, LEN);
        hr = Ioctl(IOCTL_DISK_GET_DRIVE_LAYOUT_EX, NULL, 0, pLayoutEx, LEN, &junk);
        if(FAILED(hr))
        {
            goto Exit0;
        }

        layout.PartitionStyle = (PARTITION_STYLE)pLayoutEx->PartitionStyle;
        if (PARTITION_STYLE_MBR == layout.PartitionStyle)
        {
            layout.Mbr = pLayoutEx->Mbr;
        }
        else if (PARTITION_STYLE_GPT == layout.PartitionStyle)
        {
            layout.Gpt = pLayoutEx->Gpt;
        }
        else
        {
            // nothing
        }

        layout.PartitionEntry.RemoveAll();
        for (DWORD i = 0; i < pLayoutEx->PartitionCount; i++)
        {
            const PARTITION_INFORMATION_EX& partition = pLayoutEx->PartitionEntry[i];
            if (0 == partition.PartitionNumber && 0 == partition.StartingOffset.QuadPart && 0 == partition.PartitionLength.QuadPart)
            {
                continue; // 有时会多出一个空的不存在分区，不记录
            }

            layout.PartitionEntry.Add(pLayoutEx->PartitionEntry[i]);
        }

        hr = S_OK;

Exit0:
        if (pLayoutEx)
        {
            free(pLayoutEx);
        }

        return hr;
    }

    HRESULT SetDriveLayoutEx(const DriveLayoutInfoEx& layout)
    {
        HRESULT hr = E_FAIL;
        DWORD junk = 0;
        const int LEN = (int)(sizeof(DRIVE_LAYOUT_INFORMATION_EX) + sizeof(PARTITION_INFORMATION_EX) * (layout.PartitionEntry.GetCount() - 1));
        DRIVE_LAYOUT_INFORMATION_EX* pLayoutEx = (DRIVE_LAYOUT_INFORMATION_EX*)malloc(LEN);

        if (!pLayoutEx)
        {
            hr = E_OUTOFMEMORY;
            goto Exit0;
        }

        ZeroMemory(pLayoutEx, LEN);
        pLayoutEx->PartitionStyle = layout.PartitionStyle;
        pLayoutEx->PartitionCount = (DWORD)layout.PartitionEntry.GetCount();

        if (PARTITION_STYLE_MBR == layout.PartitionStyle)
        {
            pLayoutEx->Mbr = layout.Mbr;
        }
        else if (PARTITION_STYLE_GPT == layout.PartitionStyle)
        {
            pLayoutEx->Gpt = layout.Gpt;
        }
        else
        {
            // do nothing
        }

        for (DWORD i = 0; i < pLayoutEx->PartitionCount; i++)
        {
            pLayoutEx->PartitionEntry[i] = layout.PartitionEntry.GetAt(i);
        }

        hr = Ioctl(IOCTL_DISK_SET_DRIVE_LAYOUT_EX, pLayoutEx, LEN, NULL, 0, &junk);

Exit0:
        if (pLayoutEx)
        {
            free(pLayoutEx);
        }
        return hr;
    }

    // 判断磁盘是否可写入
    BOOL IsDiskWritable() const
    {
        DWORD junk = 0;
        HRESULT hr = Ioctl(IOCTL_DISK_IS_WRITABLE, NULL, 0, NULL, 0, &junk);
        if (SUCCEEDED(hr))
        {
            return true;
        }
        return false;
    }

    // 判断磁盘是否为只读盘
    BOOL IsDiskReadonly() const
    {
        return (!IsDiskWritable());
    }

    // 判断磁盘是不是系统盘
    BOOL IsOperationSystemDisk() const
    {
        TCHAR sysDir[MAX_PATH];
        UINT len = ::GetSystemDirectory(sysDir, MAX_PATH);
        if (len < 0)
        {
            return FALSE;
        }

        DWORD diskNumber = 0;
        HRESULT hr = DiskLetterToDiskNumber(sysDir[0], &diskNumber);
        if (FAILED(hr))
        {
            return FALSE;
        }

        return (diskNumber == m_diskNumber);
    }

    /**
     * @brief 获取当前磁盘的所有卷标名称
     * @param[out] volumeNames 获取到的卷标列表，示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[in] bAppendBackblash 是否在卷标尾部后缀反斜杠
     * @attention 若不想打开磁盘，则可直接调用同名的静态函数GetDiskVolumeNames
     * @see 拿到路径后，可调用系统API：GetVolumeInformation获取详细信息，参数bAppendBackblash需要设为TRUE
     */
    VOID GetDiskVolumeNames(CAtlArray<CString>& volumeNames, BOOL bAppendBackblash = TRUE)
    {
        GetDiskVolumeNames(m_diskNumber, volumeNames, bAppendBackblash);
    }

    /**
     * @brief 锁定当前磁盘的指定卷标
     * @param[in] lpVolumeName 卷标名称。示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT LockVolume(LPCTSTR lpVolumeName)
    {
        CString strVolumeName = VolumeNameRemoveBackblash(lpVolumeName);
        if (strVolumeName.IsEmpty())
        {
            return E_INVALIDARG;
        }

        if (m_lockedVolumes.end() != m_lockedVolumes.find(strVolumeName))
        {
            return S_OK; // 已锁定过，则直接返回成功
        }

        CAtlFile hVolume;
        HRESULT hr = hVolume.Create(strVolumeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
        if (FAILED(hr))
        {
            return hr;
        }

        DWORD junk = 0;
        if (FALSE == ::DeviceIoControl(hVolume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL))
        {
            return AtlHresultFromLastError();
        }

        // 记录被锁定的卷标
        m_lockedVolumes[strVolumeName] = hVolume.Detach();
        return S_OK;
    }

    /**
     * @brief 锁定当前磁盘的指定卷标
     * @param[in] volumeIndex 需要锁定的卷标索引，从0开始。填-1时锁定当前磁盘的所有卷标
     */
    HRESULT LockVolume(DWORD volumeIndex = -1)
    {
        CAtlArray<CString> volumes;
        GetDiskVolumeNames(volumes, FALSE);

        if (volumeIndex < volumes.GetCount())
        {
            return LockVolume(volumes[volumeIndex]);
        }

        if (-1 != volumeIndex)
        {
            return E_NOINTERFACE;
        }

        HRESULT hr = E_FAIL;
        CAtlArray<CString> rollbackVolumes; // 记录本次被成功锁定的卷标，在函数失败时，需要回滚解锁本次被锁定的卷标
        for (size_t i = 0; i < volumes.GetCount(); i++)
        {
            if (m_lockedVolumes.end() != m_lockedVolumes.find(volumes[i]))
            {
                continue; // 若已经锁定过，则不需要再锁定
            }

            CAtlFile hVolume;
            hr = hVolume.Create(volumes[i], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
            if (FAILED(hr))
            {
                goto Exit0;
            }

            DWORD junk = 0;
            if (FALSE == ::DeviceIoControl(hVolume, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL))
            {
                hr = AtlHresultFromLastError();
                goto Exit0;
            }

            m_lockedVolumes[volumes[i]] = hVolume.Detach();
            rollbackVolumes.Add(volumes[i]);
        }

        hr = S_OK;

Exit0:
        if (FAILED(hr))
        {
            for (size_t i = 0 ; i < rollbackVolumes.GetCount(); i++)
            {
                UnlockVolume(rollbackVolumes[i]);
            }
        }

        return hr;
    }

    HRESULT UnlockVolume(LPCTSTR lpVolumeName)
    {
        CString strVolumeName = VolumeNameRemoveBackblash(lpVolumeName);
        if (strVolumeName.IsEmpty())
        {
            return E_INVALIDARG;
        }

        std::map<CString, HANDLE>::iterator it = m_lockedVolumes.find(strVolumeName);
        if (m_lockedVolumes.end() == it)
        {
            return E_NOINTERFACE;
        }

        DWORD junk = 0;
        ::DeviceIoControl(it->second, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
        ::CloseHandle(it->second);
        it->second = NULL;

        m_lockedVolumes.erase(it);
        return S_OK;
    }

    HRESULT UnlockVolume(DWORD volumeIndex = -1)
    {
        if (-1 == volumeIndex)
        {
            for (std::map<CString, HANDLE>::iterator it = m_lockedVolumes.begin(); it != m_lockedVolumes.end(); ++it)
            {
                DWORD junk = 0;
                ::DeviceIoControl(it->second, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
                ::CloseHandle(it->second);
                it->second = NULL;
            }
            m_lockedVolumes.clear();
            return S_OK;
        }

        CAtlArray<CString> volumes;
        GetDiskVolumeNames(volumes, FALSE);

        if (volumeIndex < volumes.GetCount())
        {
            return UnlockVolume(volumes[volumeIndex]);
        }
        return E_NOINTERFACE;
    }

    /**
     * @attention 卸载前必须先UnlockVolume
     */
    HRESULT DismountVolume(LPCTSTR lpVolumeName)
    {
        CString strVolumeName = VolumeNameRemoveBackblash(lpVolumeName);
        if (strVolumeName.IsEmpty())
        {
            return E_INVALIDARG;
        }

        std::map<CString, HANDLE>::const_iterator it = m_lockedVolumes.find(strVolumeName);
        if (m_lockedVolumes.end() == it)
        {
            return E_NOINTERFACE;
        }

        DWORD junk = 0;
        if (::DeviceIoControl(it->second, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL))
        {
            return S_OK;
        }

        return AtlHresultFromLastError();
    }

    /**
     * @attention 卸载前必须先UnlockVolume
     */
    HRESULT DismountVolume(DWORD volumeIndex = -1)
    {
        if (-1 == volumeIndex)
        {
            for (std::map<CString, HANDLE>::const_iterator it = m_lockedVolumes.begin(); it != m_lockedVolumes.end(); ++it)
            {
                DWORD junk = 0;
                ::DeviceIoControl(it->second, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
            }
            return S_OK;
        }

        CAtlArray<CString> volumes;
        GetDiskVolumeNames(volumes, FALSE);

        if (volumeIndex < volumes.GetCount())
        {
            return DismountVolume(volumes[volumeIndex]);
        }

        return E_NOINTERFACE;
    }

    /**
     * @brief 获取卷标的分区信息
     * @param[in] lpVolumeName 卷标名称。示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[out] info 卷标对应的分区信息
     * @return 成功返回S_OK，失败返回错误码
     */
    HRESULT GetVolumePartitionInfoEx(LPCTSTR lpVolumeName, PARTITION_INFORMATION_EX& info)
    {
        CString strVolumeName = VolumeNameRemoveBackblash(lpVolumeName);
        if (strVolumeName.IsEmpty())
        {
            return E_INVALIDARG;
        }

        // 先看是不是打开过这个卷标，若打开过，则直接使用其句柄
        std::map<CString, HANDLE>::iterator it = m_lockedVolumes.find(strVolumeName);
        if (m_lockedVolumes.end() != it)
        {
            DWORD junk = 0;
            if (::DeviceIoControl(it->second, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, &info, sizeof(info), &junk, NULL))
            {
                return S_OK;
            }

            return AtlHresultFromLastError();
        }

        return GetHostVolumePartitionInfoEx(lpVolumeName, info);
    }

    HRESULT Ioctl(
        DWORD dwIoControlCode,
        LPVOID lpInBuffer,
        DWORD nInBufferSize,
        LPVOID lpOutBuffer,
        DWORD nOutBufferSize,
        LPDWORD lpBytesReturned,
        LPOVERLAPPED lpOverlapped = NULL) const
    {
        BOOL suc = ::DeviceIoControl(
            m_file,
            dwIoControlCode,
            lpInBuffer,
            nInBufferSize,
            lpOutBuffer,
            nOutBufferSize,
            lpBytesReturned,
            lpOverlapped);

        HRESULT hr = S_OK;
        if (!suc)
        {
            hr = AtlHresultFromLastError();
            if (SUCCEEDED(hr))
            {
                hr = E_FAIL;
            }
        }
        return hr;
    }

    /**
     * @brief 将磁盘路径，转换为磁盘号
     * @param[in] lpFilePath 磁盘路径。示例："\\\\.\\PhysicalDrive0"
     * @return 失败返回-1，成功返回对应的磁盘号
     */
    static DWORD PhysicalDrivePathToDiskNumber(LPCTSTR lpFilePath)
    {
        const CString prefix = L"\\\\.\\PhysicalDrive";
        CString filePath = lpFilePath;
        if (filePath.GetLength() > prefix.GetLength())
        {
            if (0 == prefix.CompareNoCase(filePath.Left(prefix.GetLength())))
            {
                CString szNum = filePath.Mid(prefix.GetLength());
                return (DWORD)StrToInt(szNum);
            }
        }

        return -1;
    }

    /**
     * @brief 根据盘符获取其对应的磁盘索引号、分区号、设备类型
     * @param[in] letter 指定要查询的盘符
     * @param[in] diskNumber 磁盘索引号
     * @param[in] partitionNumber 分区号
     * @param[in] deviceType 设备类型
     * @return 成功返回S_OK
     */
    static HRESULT DiskLetterToDiskNumber(TCHAR letter, DWORD* diskNumber, DWORD* partitionNumber = NULL, DEVICE_TYPE* deviceType = NULL)
    {
        CString path;
        path.Format(_T("\\\\.\\%C:"), letter);

        CAtlFile file;
        HRESULT hr = file.Create(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
        if (FAILED(hr))
        {
            return hr;
        }

        DWORD junk = 0;
        STORAGE_DEVICE_NUMBER number;
        if (FALSE == ::DeviceIoControl(file, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &number, sizeof(number), &junk, NULL))
        {
            return AtlHresultFromLastError();
        }

        if (diskNumber)
        {
            *diskNumber = number.DeviceNumber;
        }
        if (partitionNumber)
        {
            *partitionNumber = number.PartitionNumber;
        }
        if (deviceType)
        {
            *deviceType = number.DeviceType;
        }

        return S_OK;
    }

    /**
     * 获取模块所在的磁盘索引号
     * @param[out] diskNumber 获取到的模块所在磁盘索引号
     * @param[in] hModule 模块句柄
     * @return 成功返回S_OK
     */
    static HRESULT GetModuleDiskNumber(DWORD& diskNumber, HMODULE hModule = NULL)
    {
        TCHAR fullpath[MAX_PATH + 1] = { 0 };
        DWORD dwRet = ::GetModuleFileName(hModule, fullpath, MAX_PATH);
        if (dwRet)
        {
            return DiskLetterToDiskNumber(fullpath[0], &diskNumber);
        }
        return AtlHresultFromLastError();
    }

    /**
     * @brief 获取卷标对应的盘符
     * @param[in] volumeNames 卷标名称，示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[out] diskLetter 获取到的盘符
     * @return 成功返回S_OK，失败返回错误码
     */
    static HRESULT VolumeNameToDiskLetter(LPCTSTR volumeName, TCHAR& diskLetter)
    {
        CONST int MAX_POINT_COUNT = 1024;
        CAutoVectorPtr<TCHAR> mountPointsBuffer(new TCHAR[MAX_POINT_COUNT]);
        ZeroMemory(mountPointsBuffer, MAX_POINT_COUNT * sizeof(TCHAR));

        // 获取卷标的所有挂载点列表
        DWORD realMountPointCount = 0;
        if (FALSE == ::GetVolumePathNamesForVolumeName(volumeName, mountPointsBuffer, MAX_POINT_COUNT, &realMountPointCount))
        {
            DWORD dwLastError = ::GetLastError();
            if (dwLastError == ERROR_MORE_DATA)
            {
                CAutoVectorPtr<TCHAR> tmp(new TCHAR[realMountPointCount]);
                mountPointsBuffer = tmp;
                ZeroMemory(mountPointsBuffer, realMountPointCount * sizeof(TCHAR));
            }
            else
            {
                return HRESULT_FROM_WIN32(dwLastError);
            }
        }

        if (FALSE == ::GetVolumePathNamesForVolumeName(volumeName, mountPointsBuffer, realMountPointCount, &realMountPointCount))
        {
            return AtlHresultFromLastError();
        }

        // 遍历所有挂载点，找到是盘符的
        for (TCHAR *mountPoint = mountPointsBuffer; *mountPoint; mountPoint += lstrlen(mountPoint) + 1)
        {
            // Check if it's a disk letter.
            if (mountPoint[1] == _T(':') && mountPoint[2] == _T('\\'))
            {
                diskLetter = mountPoint[0];
                return S_OK;
            }
        }

        return E_FAIL;
    }

    /**
     * @brief 遍历查找本机所有的卷标
     * @param[out] volumeNames 获取到的卷标列表，示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[in] bAppendBackblash 是否在卷标尾部后缀反斜杠
     */
    static void FindAllVolumeNames(_Out_ CAtlArray<CString>& paths, _In_ BOOL bAppendBackblash = TRUE)
    {
        paths.RemoveAll();

        TCHAR szVolume[MAX_PATH];
        HANDLE handle = ::FindFirstVolume(szVolume, MAX_PATH);
        if (INVALID_HANDLE_VALUE == handle)
        {
            return;
        }

        // 默认Win API枚举到的路径，是有反斜杠的
        if (!bAppendBackblash)
        {
            size_t len = _tcscnlen(szVolume, MAX_PATH);
            if (szVolume[len - 1] == _T('\\'))
            {
                szVolume[len - 1] = _T('\0');
            }
        }
        paths.Add(szVolume);

        while (::FindNextVolume(handle, szVolume, MAX_PATH))
        {
            if (!bAppendBackblash)
            {
                size_t len = _tcscnlen(szVolume, MAX_PATH);
                if (szVolume[len - 1] == _T('\\'))
                {
                    szVolume[len - 1] = _T('\0');
                }
            }
            paths.Add(szVolume);
        }
        ::FindVolumeClose(handle);
    }

    /**
     * @brief 获取磁盘的所有卷标
     * @param[in] diskNumber 磁盘索引号
     * @param[out] volumeNames 获取到的卷标列表，示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[in] bAppendBackblash 是否在卷标尾部后缀反斜杠
     * @attention 若需要获取当前打开的磁盘卷标，可调用非静态的同名函数GetDiskVolumeNames，本函数为静态函数
     * @see 拿到路径后，可调用系统API：GetVolumeInformation获取详细信息，参数bAppendBackblash需要设为TRUE
     */
    static void GetDiskVolumeNames(DWORD diskNumber, CAtlArray<CString>& volumeNames, BOOL bAppendBackblash = TRUE)
    {
        volumeNames.RemoveAll();

        CAtlArray<CString> allVolumes;
        FindAllVolumeNames(allVolumes, FALSE); // 不能有反斜杠结尾，否则无法CreateFile打开

        for (size_t i = 0; i < allVolumes.GetCount(); i++)
        {
            CAtlFile hVolume;
            HRESULT hr = hVolume.Create(allVolumes[i], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
            if (FAILED(hr))
            {
                continue;
            }

            DWORD junk = 0;
            VOLUME_DISK_EXTENTS exts;
            if (FALSE == ::DeviceIoControl(hVolume, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &exts, sizeof(exts), &junk, NULL))
            {
                continue;
            }

            for (DWORD n = 0; n < exts.NumberOfDiskExtents; ++n)
            {
                if (diskNumber == exts.Extents[n].DiskNumber)
                {
                    if (bAppendBackblash)
                    {
                        volumeNames.Add(allVolumes[i] + _T('\\'));
                    }
                    else
                    {
                        volumeNames.Add(allVolumes[i]);
                    }

                    break;
                }
            }
        }
    }

    /**
     * @brief 获取本机卷标的分区信息
     * @param[in] lpVolumeName 卷标名称。示例：\\?\Volume{a66da20f-2550-4187-b761-199ae04170a4}
     * @param[out] info 卷标对应的分区信息
     * @return 成功返回S_OK，失败返回错误码
     * @attention 若要获取当前已打开的磁盘卷标，则调用GetVolumePartitionInfoEx
     */
    static HRESULT GetHostVolumePartitionInfoEx(LPCTSTR lpVolumeName, PARTITION_INFORMATION_EX& info)
    {
        CString strVolumeName = VolumeNameRemoveBackblash(lpVolumeName);
        if (strVolumeName.IsEmpty())
        {
            return E_INVALIDARG;
        }

        CAtlFile hVolume;
        HRESULT hr = hVolume.Create(strVolumeName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
        if (FAILED(hr))
        {
            return hr;
        }

        DWORD junk = 0;
        if (::DeviceIoControl(hVolume, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, &info, sizeof(info), &junk, NULL))
        {
            return S_OK;
        }
        return AtlHresultFromLastError();
    }

private:
    ZLPhysicalDrive& operator=(const ZLPhysicalDrive& rhs); // 禁用赋值语句
    ZLPhysicalDrive(const ZLPhysicalDrive& rhs); // 禁用拷贝构造函数

    HRESULT StorageQueryProperty()
    {
        // 只查询一次
        if (m_bStoragePropertyQueried)
        {
            return S_OK;
        }

        STORAGE_PROPERTY_QUERY stQuery;
        ZeroMemory(&stQuery, sizeof(STORAGE_PROPERTY_QUERY));
        stQuery.PropertyId = StorageDeviceProperty;
        stQuery.QueryType = PropertyStandardQuery;

        STORAGE_DESCRIPTOR_HEADER stHeader;
        ZeroMemory(&stHeader, sizeof(STORAGE_DESCRIPTOR_HEADER));

        // 查询描述信息头的大小
        DWORD junk = 0;
        HRESULT hr = Ioctl(IOCTL_STORAGE_QUERY_PROPERTY, &stQuery, sizeof(stQuery), &stHeader, sizeof(stHeader), &junk);
        if (FAILED(hr))
        {
            return hr;
        }

        // 申请缓存
        CAutoVectorPtr<char> descHeaderBuf(new char[stHeader.Size]);
        ZeroMemory(descHeaderBuf, stHeader.Size);

        // 查询描述信息头
        hr = Ioctl(IOCTL_STORAGE_QUERY_PROPERTY, &stQuery, sizeof(stQuery), descHeaderBuf, stHeader.Size, &junk);
        if (FAILED(hr))
        {
            return hr;
        }

        STORAGE_DEVICE_DESCRIPTOR* pDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)(char*)descHeaderBuf;

        // 获取磁盘序列号
        if (pDescriptor->SerialNumberOffset != 0)
        {
            CStringA tmp = descHeaderBuf + pDescriptor->SerialNumberOffset;
            tmp.Trim(); // 部分序列号取出来之后，前后会有很多空格，把空格去掉
            m_diskSerialNumber = CA2W(tmp);
        }
        else
        {
            m_diskSerialNumber.Empty();
        }

        // 获取磁盘接口类型
        m_diskBusType = pDescriptor->BusType;

        // VendorId
        if (0 != pDescriptor->VendorIdOffset)
        {
            CStringA tmp = descHeaderBuf + pDescriptor->VendorIdOffset;
            tmp.Trim();
            m_diskVendorId = CA2W(tmp);
        }
        else
        {
            m_diskVendorId.Empty();
        }

        // ProductId
        if (0 != pDescriptor->ProductIdOffset)
        {
            CStringA tmp = descHeaderBuf + pDescriptor->ProductIdOffset;
            tmp.Trim();
            m_diskProductId = CA2W(tmp);
        }

        // ProductRevision
        if (0 != pDescriptor->ProductRevisionOffset)
        {
            CStringA tmp = descHeaderBuf + pDescriptor->ProductRevisionOffset;
            tmp.Trim();
            m_diskProductRevision = CA2W(tmp);
        }

        // 拼接磁盘型号
        m_diskModel.Format(_T("%s %s %s"), m_diskVendorId, m_diskProductId, m_diskProductRevision);
        m_diskModel.Trim();

        m_bStoragePropertyQueried = TRUE;
        return S_OK;
    }

    /**
     * @brief 移除卷标尾部的反斜杠，若尾部不存在反斜杠，则不操作
     *    因为CreateFile打开卷标时，传入的路径不能后缀反斜杠，故有了此接口
     */
    static CString VolumeNameRemoveBackblash(LPCTSTR lpVolumeName)
    {
        CString strVolumeName;

        if (!lpVolumeName)
        {
            return strVolumeName;
        }

        size_t len = _tcscnlen(lpVolumeName, MAX_PATH);
        if (lpVolumeName[len - 1] == _T('\\') || lpVolumeName[len - 1] == _T('/'))
        {
            strVolumeName.Append(lpVolumeName, (int)(len - 1));
        }
        else
        {
            strVolumeName = lpVolumeName;
        }

        return strVolumeName;
    }

private:
    CAtlFile            m_file;
    DWORD               m_diskNumber;               //!< 磁盘索引号

    // IOCTL_STORAGE_QUERY_PROPERTY信息
    BOOL                m_bStoragePropertyQueried;  //!< 是否查询过IOCTL_STORAGE_QUERY_PROPERTY，只查询一次
    CString             m_diskModel;                //!< 磁盘型号
    CString             m_diskSerialNumber;         //!< 磁盘序列号
    STORAGE_BUS_TYPE    m_diskBusType;              //!< 总线接口类型
    CString             m_diskVendorId;
    CString             m_diskProductId;
    CString             m_diskProductRevision;

    std::map<CString, HANDLE> m_lockedVolumes; //!< 被锁定的volume, <volumeName, handle>
};

}
}
