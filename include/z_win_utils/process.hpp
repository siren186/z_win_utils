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
 * @brief 进程控制
 */


#pragma once
#include <atlbase.h>
#include "win_utils_header.h"
#include "dos_name.hpp"

#define ProcessBasicInformation 0

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 进程控制相关操作
     */
    class ZLPsapiDllFunction
    {
    public:
        ZLPsapiDllFunction()
            : m_bInited(FALSE)
            , m_hPsapi(NULL)
        {
        }

        ~ZLPsapiDllFunction()
        {
            UnInit();
        }

        BOOL Init()
        {
            if (!m_bInited)
            {
                m_hPsapi = ::LoadLibrary(_T("PSAPI.DLL"));
                if (m_hPsapi != NULL)
                {
                    m_pEnumProcessModules = (PEnumProcessModules)::GetProcAddress(m_hPsapi, "EnumProcessModules");
                    m_pEnumProcesses = (PEnumProcesses)::GetProcAddress(m_hPsapi, "EnumProcesses");
                    m_pGetModuleBaseName = (PGetModuleBaseName)::GetProcAddress(m_hPsapi, "GetModuleBaseNameW");
                    m_pGetModuleFileNameEx = (PGetModuleFileNameEx)::GetProcAddress(m_hPsapi, "GetModuleFileNameExW");
                    m_pGetProcessImageFileName = (PGetProcessImageFileName)::GetProcAddress(m_hPsapi, "GetProcessImageFileNameW");
                    m_bInited = m_pEnumProcesses 
                        && m_pGetModuleBaseName 
                        && m_pEnumProcessModules 
                        && m_pGetModuleFileNameEx
                        && m_pGetProcessImageFileName;
                }
            }
            return m_bInited;
        }

        void UnInit()
        {
            if (!m_bInited)
            {
                return;
            }
            m_pEnumProcesses = NULL;
            m_pGetModuleBaseName = NULL;
            m_pEnumProcessModules = NULL;
            m_pGetModuleFileNameEx = NULL;
            if (m_hPsapi)
            {
                ::FreeLibrary(m_hPsapi);
            }
            m_bInited = FALSE;
        }
        /**
         * @brief 枚举系统中进程的标识符
         * @param[in]  pProcessIds     指向存放进程标识符数组的指针
         * @param[in]  cb              数组大小
         * @param[out] pBytesReturned  返回的进程标识符数组的大小
         * @return 成功返回TRUE，失败返回FALSE
         */
        BOOL EnumProcesses(DWORD* pProcessIds, DWORD cb, DWORD* pBytesReturned)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pEnumProcesses)
            {
                return m_pEnumProcesses(pProcessIds, cb, pBytesReturned);
            }
            return FALSE;
        }
        /**
         * @brief 枚举系统中进程的标识符
         * @param[in]  hProcess    进程句柄
         * @param[in]  hModule     模块句柄
         * @param[out] lpBaseName 指向获取模块base name的缓冲区的指针
         * @param[in]  nSize      缓冲区的大小，以字节为单位
         * @return 成功，返回复制到缓冲区中的字节的长度，失败返回零
         */
        DWORD GetModuleBaseName(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetModuleBaseName)
            {
                return m_pGetModuleBaseName(hProcess, hModule, lpBaseName, nSize);
            }
            return FALSE;
        }
        /**
         * @brief 枚举指定进程每个模块的句柄
         * @param[in]  hProcess    进程句柄
         * @param[out] lphModule   存放模块句柄的数组
         * @param[in]  cb          数组的大小，以字节为单位
         * @param[out] lpcbNeeded 存储所有模块句柄所需的字节数
         * @return 成功返回非零值，失败返回零
         */
        BOOL EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pEnumProcessModules)
            {
                return m_pEnumProcessModules(hProcess, lphModule, cb, lpcbNeeded);
            }
            return FALSE;
        }
        /**
         * @brief 获取包含指定模块的文件的全路径
         * @param[in]  hProcess   包含模块的进程句柄
         * @param[in]  hModule    模块句柄，如果为NULL，返回由进程句柄指定的进程的可执行文件的全路径
         * @param[out] lpFilename 指向获取模块全路径缓冲区的指针
         * @param[in]  nSize      缓冲区的大小，以字符为单位
         * @return 成功返回复制到缓冲区中的字节的长度，失败返回零
         */
        DWORD GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename,DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetModuleFileNameEx)
            {
                return m_pGetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);
            }
            return FALSE;
        }
        /**
         * @brief 获取指定进程的可执行文件的文件名
         * @param[in]  hProcess        进程句柄
         * @param[in]  lpImageFileName 指向获取可执行文件全路径的缓冲区的指针
         * @param[out] nSize           字符缓冲区的大小
         * @return 成功返回复制到缓冲区中的字节的长度，失败返回零
         */
        DWORD GetProcessImageFileName(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize)
        {
            if (m_bInited == FALSE)
            {
                Init();
            }
            if (m_pGetProcessImageFileName)
            {
                return m_pGetProcessImageFileName(hProcess, lpImageFileName, nSize);
            }
            return FALSE;
        }

    protected:
        typedef BOOL (WINAPI *PEnumProcesses)(DWORD* pProcessIds, DWORD cb,	DWORD* pBytesReturned);
        typedef DWORD (WINAPI *PGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName,	DWORD nSize);
        typedef BOOL (WINAPI *PEnumProcessModules)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
        typedef DWORD (WINAPI *PGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename,DWORD nSize);
        typedef BOOL (WINAPI *PGetProcessImageFileName)(HANDLE hProcess,LPTSTR lpImageFileName, DWORD nSize);

        PEnumProcesses m_pEnumProcesses;
        PGetModuleBaseName m_pGetModuleBaseName;
        PEnumProcessModules m_pEnumProcessModules;
        PGetModuleFileNameEx m_pGetModuleFileNameEx;
        PGetProcessImageFileName m_pGetProcessImageFileName;

    private:
        BOOL m_bInited;
        HMODULE m_hPsapi;
    };

    class ZLProcess
    {
    public:
        ZLProcess() {}
        ~ZLProcess() {}

    public:
        typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);

        typedef struct
        {
            USHORT Length;
            USHORT MaximumLength;
            PWSTR  Buffer;
        } ZL_UNICODE_STRING, *ZL_PUNICODE_STRING;

        typedef struct
        {
            ULONG          AllocationSize;
            ULONG          ActualSize;
            ULONG          Flags;
            ULONG          Unknown1;
            ZL_UNICODE_STRING Unknown2;
            HANDLE         InputHandle;
            HANDLE         OutputHandle;
            HANDLE         ErrorHandle;
            ZL_UNICODE_STRING CurrentDirectory;
            HANDLE         CurrentDirectoryHandle;
            ZL_UNICODE_STRING SearchPaths;
            ZL_UNICODE_STRING ApplicationName;
            ZL_UNICODE_STRING CommandLine;
            PVOID          EnvironmentBlock;
            ULONG          Unknown[9];
            ZL_UNICODE_STRING Unknown3;
            ZL_UNICODE_STRING Unknown4;
            ZL_UNICODE_STRING Unknown5;
            ZL_UNICODE_STRING Unknown6;
        } ZL_PROCESS_PARAMETERS, *ZL_PPROCESS_PARAMETERS;

        typedef struct
        {
            ULONG               AllocationSize;
            ULONG               Unknown1;
            HINSTANCE           ProcessHinstance;
            PVOID               ListDlls;
            ZL_PPROCESS_PARAMETERS ProcessParameters;
            ULONG               Unknown2;
            HANDLE              Heap;
        } ZL_PEB, *ZL_PPEB;

        typedef struct
        {
            DWORD ExitStatus;
            ZL_PPEB  PebBaseAddress;
            DWORD AffinityMask;
            DWORD BasePriority;
            ULONG UniqueProcessId;
            ULONG InheritedFromUniqueProcessId;
        } ZL_PROCESS_BASIC_INFORMATION;

        //////////////////////////////////////////////////////////////////////////

        // NtQueryInformationProcess for pure 32 and 64-bit processes
        typedef NTSTATUS(NTAPI *_NtQueryInformationProcess)(
            IN HANDLE ProcessHandle,
            ULONG ProcessInformationClass,
            OUT PVOID ProcessInformation,
            IN ULONG ProcessInformationLength,
            OUT PULONG ReturnLength OPTIONAL
            );

        typedef NTSTATUS(NTAPI *_NtReadVirtualMemory)(
            IN HANDLE ProcessHandle,
            IN PVOID BaseAddress,
            OUT PVOID Buffer,
            IN SIZE_T Size,
            OUT PSIZE_T NumberOfBytesRead);

        // NtQueryInformationProcess for 32-bit process on WOW64
        typedef NTSTATUS(NTAPI *_NtWow64ReadVirtualMemory64)(
            IN HANDLE ProcessHandle,
            IN PVOID64 BaseAddress,
            OUT PVOID Buffer,
            IN ULONG64 Size,
            OUT PULONG64 NumberOfBytesRead);

        // PROCESS_BASIC_INFORMATION for pure 32 and 64-bit processes
        typedef struct _PROCESS_BASIC_INFORMATION {
            PVOID Reserved1;
            PVOID PebBaseAddress;
            PVOID Reserved2[2];
            ULONG_PTR UniqueProcessId;
            PVOID Reserved3;
        } PROCESS_BASIC_INFORMATION;

        // PROCESS_BASIC_INFORMATION for 32-bit process on WOW64
        // The definition is quite funky, as we just lazily doubled sizes to match offsets...
        typedef struct _PROCESS_BASIC_INFORMATION_WOW64 {
            PVOID Reserved1[2];
            PVOID64 PebBaseAddress;
            PVOID Reserved2[4];
            ULONG_PTR UniqueProcessId[2];
            PVOID Reserved3[2];
        } PROCESS_BASIC_INFORMATION_WOW64;

        typedef struct _UNICODE_STRING {
            USHORT Length;
            USHORT MaximumLength;
            PWSTR  Buffer;
        } UNICODE_STRING;

        typedef struct _UNICODE_STRING_WOW64 {
            USHORT Length;
            USHORT MaximumLength;
            PVOID64 Buffer;
        } UNICODE_STRING_WOW64;

        //////////////////////////////////////////////////////////////////////////

    public:
        /**
         * @brief 获取进程ID对应的可执行文件路径
         * @param[in] dwPid 进程ID
         * @return 成功返回路径
         */
        static CString GetProcessPath(DWORD dwPid)
        {
            CString cstrPath;
            HANDLE hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION);//openprocess
            if (hProcess != NULL)
            {
                cstrPath = _GetProcessPath(hProcess);//process路径
                ::CloseHandle(hProcess);
            }
            return cstrPath;
        }
        /**
         * @brief 关闭指定进程
         * @param[in] dwPid 进程ID
         * @return 成功返回TRUE，失败返回FALSE
         */
        static BOOL KillProcess(DWORD dwPid)
        {
            BOOL bReturn = FALSE;
            HANDLE hProcess = _Open(dwPid, PROCESS_TERMINATE);
            if (hProcess != NULL)
            {
                bReturn = ::TerminateProcess(hProcess, 0);
                ::CloseHandle(hProcess);
            }
            return bReturn;
        }
        /**
         * @brief 获取父进程ID
         * @param[in] dwPid 进程ID
         * @return 成功返回TRUE，失败返回FALSE
         */
        static int GetParentProcessID(DWORD dwPid)
        {
            LONG lStatus = 0;
            int nParentPID = -1;
            HANDLE hProcess = 0;
            ZL_PROCESS_BASIC_INFORMATION pbi = {0};

            hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION);
            if (!hProcess)
            {
                return nParentPID;
            }
            PROCNTQSIP NtQueryInformationProcess;
            NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(
                ::GetModuleHandle(L"ntdll"),
                "NtQueryInformationProcess");
            lStatus = NtQueryInformationProcess(hProcess,
                ProcessBasicInformation,
                (PVOID)&pbi,
                sizeof(ZL_PROCESS_BASIC_INFORMATION),
                NULL);
            if (!lStatus)
            {
                nParentPID = pbi.InheritedFromUniqueProcessId;
            }
            if (hProcess != NULL)
            {
                ::CloseHandle(hProcess);
                hProcess = NULL;
            }
            return nParentPID;
        }
        /**
         * @brief 获取进程的命令行参数
         * @param[in] dwPid 进程ID
         * @return 成功返回命令行参数，失败返回空串
         * @see ReadProcessMemory
         */
        static CString GetProcessCmdLine(DWORD dwPid)
        {
            CString cstrCmdLine;
            WCHAR *pszCmdLineBuffer = NULL;
            LONG lStatus = 0;
            HANDLE hProcess = NULL;
            ZL_PROCESS_BASIC_INFORMATION pbi = {0};
            ZL_PEB Peb = {0};
            ZL_PROCESS_PARAMETERS ProcParam = {0};
            DWORD dwDummy = 0;
            DWORD dwSize = 0;
            LPVOID lpAddress = NULL;
            PROCNTQSIP NtQueryInformationProcess = NULL;

            NtQueryInformationProcess = (PROCNTQSIP)::GetProcAddress(::GetModuleHandle(L"ntdll"), "NtQueryInformationProcess");
            if (NtQueryInformationProcess == NULL)
            {
                goto Exit0;
            }

            hProcess = _Open(dwPid, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
            if (!hProcess)
            {
                goto Exit0;
            }

            lStatus = NtQueryInformationProcess(hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(ZL_PROCESS_BASIC_INFORMATION), NULL);
            if (lStatus)
            {
                goto Exit0;
            }
            if (!::ReadProcessMemory(hProcess, pbi.PebBaseAddress, &Peb, sizeof(ZL_PEB),&dwDummy))
            {
                goto Exit0;
            }
            if (!::ReadProcessMemory(hProcess, Peb.ProcessParameters, &ProcParam, sizeof(ZL_PROCESS_PARAMETERS), &dwDummy))
            {
                goto Exit0;
            }

            lpAddress = ProcParam.CommandLine.Buffer;
            dwSize = ProcParam.CommandLine.Length;
            pszCmdLineBuffer = new WCHAR[dwSize + 1];
            if (pszCmdLineBuffer == NULL)
            {
                goto Exit0;
            }
            ::memset(pszCmdLineBuffer, 0, sizeof(WCHAR) * (dwSize + 1));
            if (!::ReadProcessMemory(hProcess, lpAddress, pszCmdLineBuffer, dwSize, &dwDummy))
            {
                goto Exit0;
            }

            cstrCmdLine = pszCmdLineBuffer;
        Exit0:
            if (hProcess)
            {
                ::CloseHandle(hProcess);
            }
            if (pszCmdLineBuffer)
            {
                delete []pszCmdLineBuffer;
            }
            return cstrCmdLine;
        }
		
        static CString GetProcessCmdLineEx(DWORD dwPid)
        {
            CString cmdLine;
            DWORD err = 0;
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
            if (hProcess == NULL)
            {
                return cmdLine;
            }

            // determine if 64 or 32-bit processor
            SYSTEM_INFO si;
            GetNativeSystemInfo(&si);

            // determine if this process is running on WOW64
            BOOL wow;
            IsWow64Process(GetCurrentProcess(), &wow);

            // use WinDbg "dt ntdll!_PEB" command and search for ProcessParameters offset to find the truth out
            DWORD ProcessParametersOffset = si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x20 : 0x10;
            DWORD CommandLineOffset = si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x70 : 0x40;

            // read basic info to get ProcessParameters address, we only need the beginning of PEB
            DWORD pebSize = ProcessParametersOffset + 8;
            CAutoVectorPtr<char> peb(new char[pebSize]);
            ZeroMemory(peb, pebSize);

            // read basic info to get CommandLine address, we only need the beginning of ProcessParameters
            DWORD ppSize = CommandLineOffset + 16;
            CAutoVectorPtr<char> pp(new char[ppSize]);
            ZeroMemory(pp, ppSize);

            if (wow)
            {
                // we're running as a 32-bit process in a 64-bit OS
                PROCESS_BASIC_INFORMATION_WOW64 pbiWow64;
                ZeroMemory(&pbiWow64, sizeof(pbiWow64));

                // get process information from 64-bit world
                _NtQueryInformationProcess query = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64QueryInformationProcess64");
                if (NULL == query)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                err = query(hProcess, 0, &pbiWow64, sizeof(pbiWow64), NULL);
                if (err != 0)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read PEB from 64-bit address space
                _NtWow64ReadVirtualMemory64 read = (_NtWow64ReadVirtualMemory64)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64ReadVirtualMemory64");
                if (NULL == read)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                err = read(hProcess, pbiWow64.PebBaseAddress, peb, pebSize, NULL);
                if (err != 0)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read ProcessParameters from 64-bit address space
                PBYTE* parameters = (PBYTE*)*(LPVOID*)(peb + ProcessParametersOffset); // address in remote process adress space
                err = read(hProcess, parameters, pp, ppSize, NULL);
                if (err != 0)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read CommandLine
                UNICODE_STRING_WOW64* pCommandLine = (UNICODE_STRING_WOW64*)(pp + CommandLineOffset);
                LPTSTR buf = cmdLine.GetBuffer(pCommandLine->MaximumLength + 1);
                err = read(hProcess, pCommandLine->Buffer, buf, pCommandLine->MaximumLength, NULL);
                cmdLine.ReleaseBuffer();
                if (err != 0)
                {
                    CloseHandle(hProcess);
                    cmdLine = "";
                    return cmdLine;
                }
            }
            else
            {
                // we're running as a 32-bit process in a 32-bit OS, or as a 64-bit process in a 64-bit OS
                PROCESS_BASIC_INFORMATION pbiWow32;
                ZeroMemory(&pbiWow32, sizeof(pbiWow32));

                // get process information
                _NtQueryInformationProcess query = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
                if (NULL == query)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                err = query(hProcess, 0, &pbiWow32, sizeof(pbiWow32), NULL);
                if (err != 0)
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read PEB
                if (!ReadProcessMemory(hProcess, pbiWow32.PebBaseAddress, peb, pebSize, NULL))
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read ProcessParameters
                PBYTE* parameters = (PBYTE*)*(LPVOID*)(peb + ProcessParametersOffset); // address in remote process adress space
                if (!ReadProcessMemory(hProcess, parameters, pp, ppSize, NULL))
                {
                    CloseHandle(hProcess);
                    return cmdLine;
                }

                // read CommandLine
                UNICODE_STRING* pCommandLine = (UNICODE_STRING*)(pp + CommandLineOffset);
                LPTSTR buf = cmdLine.GetBuffer(pCommandLine->MaximumLength + 1);
                BOOL bSuc = ReadProcessMemory(hProcess, pCommandLine->Buffer, buf, pCommandLine->MaximumLength, NULL);
                cmdLine.ReleaseBuffer();
                if (!bSuc)
                {
                    CloseHandle(hProcess);
                    cmdLine = "";
                    return cmdLine;
                }
            }

            CloseHandle(hProcess);
            return cmdLine;
        }

        /**
         * @brief 创建进程
         * @param[in] pszPath         可执行文件路径
         * @param[in] pszCmd          命令行
         * @param[in] dwWaitTime      等待时间，默认为零
         * @param[in] bShow           进程是否显示，默认显示
         * @param[in] hToken          Token句柄，默认为NULL
         * @param[in] dwCreationFlags 进程创建标示，默认为零
         * @return 返回进程的退出代码
         * @see CreateProcess CreateProcessAsUser
         */
        static DWORD Run(LPCTSTR pszPath,
            LPCTSTR pszCmd,
            DWORD dwWaitTime = 0,
            BOOL bShow = TRUE,
            HANDLE hToken = NULL,
            DWORD dwCreationFlags = 0)
        {
            DWORD nReturn = 0;
            STARTUPINFO si = {0};
            PROCESS_INFORMATION pi = {0};

            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESHOWWINDOW;
            CString strExePath;
            strExePath.Format(_T("\"%s\" %s"), pszPath, pszCmd);

            if (bShow)
            {
                si.wShowWindow = SW_SHOWDEFAULT;
            }
            else
            {
                si.wShowWindow = SW_HIDE;
            }

            BOOL bRetCode = FALSE;
            if (hToken == NULL)
            {
                bRetCode = ::CreateProcess(NULL,
                    (LPWSTR)(LPCTSTR)strExePath,
                    NULL,
                    NULL,
                    FALSE,
                    dwCreationFlags,
                    NULL,
                    NULL,
                    &si,
                    &pi);
            }
            else
            {
                bRetCode = ::CreateProcessAsUser(hToken,
                    NULL,
                    (LPWSTR)(LPCTSTR)strExePath,
                    NULL,
                    NULL,
                    FALSE,
                    dwCreationFlags,
                    NULL,
                    NULL,
                    &si,
                    &pi);
            }

            if (!bRetCode)
            {
                goto Exit;
            }
            if (0 != dwWaitTime)
            {
                ::WaitForSingleObject(pi.hProcess, dwWaitTime);
            }
            ::GetExitCodeProcess(pi.hProcess, &nReturn);

        Exit:
            if (pi.hProcess != NULL)
            {
                ::CloseHandle(pi.hProcess);
                pi.hProcess = NULL;
            }
            if (pi.hThread != NULL)
            {
                ::CloseHandle(pi.hThread);
                pi.hThread = NULL;
            }
            return nReturn;
        }

    private:
        static HANDLE _Open(DWORD dwProcessId, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS, BOOL bInheritHandle = FALSE)
        {
            return ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
        }

        static CString _GetProcessPath(HANDLE hProcess)
        {
            if (!hProcess)
            {
                return _T("");
            }

            CString cstrPath;
            ZLPsapiDllFunction psapi;
            if (psapi.Init())
            {
                psapi.GetProcessImageFileName(hProcess, cstrPath.GetBuffer(MAX_PATH + 1), MAX_PATH);
                cstrPath.ReleaseBuffer();

                ZLDosName dosname;
                if (dosname.Init())
                {
                    dosname.DevicePathToDosPath(cstrPath);
                }
                dosname.Unit();
            }
            psapi.UnInit();
            return cstrPath;
        }
    };
}
}