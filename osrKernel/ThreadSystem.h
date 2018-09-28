/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* ThreadSystem.h
* thread system implementation
*********************************************************/
#pragma once
#include "stdafx.h"
#include <subauth.h>

typedef enum __THREAD_INFORMATION_CLASS
{
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} ___THREAD_INFORMATION_CLASS, *__PTHREAD_INFORMATION_CLASS;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_LDR_DATA {
	BYTE Reserved1[8];
	PVOID Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef VOID (NTAPI *PPS_POST_PROCESS_INIT_ROUTINE) ( VOID );


typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[21];
	PPEB_LDR_DATA LoaderData;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	BYTE Reserved3[520];
	PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
	BYTE Reserved4[136];
	ULONG SessionId;
} PEB;

typedef struct __TEB {

	NT_TIB                  Tib;
	PVOID                   EnvironmentPointer;
	CLIENT_ID               Cid;
	PVOID                   ActiveRpcInfo;
	PVOID                   ThreadLocalStoragePointer;
	PEB*                    Peb;
	ULONG                   LastErrorValue;
	ULONG                   CountOfOwnedCriticalSections;
	PVOID                   CsrClientThread;
	PVOID                   Win32ThreadInfo;
	ULONG                   Win32ClientInfo[0x1F];
	PVOID                   WOW32Reserved;
	ULONG                   CurrentLocale;
	ULONG                   FpSoftwareStatusRegister;
	PVOID                   SystemReserved1[0x36];
	PVOID                   Spare1;
	ULONG                   ExceptionCode;
	ULONG                   SpareBytes1[0x28];
	PVOID                   SystemReserved2[0xA];
	ULONG                   GdiRgn;
	ULONG                   GdiPen;
	ULONG                   GdiBrush;
	CLIENT_ID               RealClientId;
	PVOID                   GdiCachedProcessHandle;
	ULONG                   GdiClientPID;
	ULONG                   GdiClientTID;
	PVOID                   GdiThreadLocaleInfo;
	PVOID                   UserReserved[5];
	PVOID                   GlDispatchTable[0x118];
	ULONG                   GlReserved1[0x1A];
	PVOID                   GlReserved2;
	PVOID                   GlSectionInfo;
	PVOID                   GlSection;
	PVOID                   GlTable;
	PVOID                   GlCurrentRC;
	PVOID                   GlContext;
	NTSTATUS                LastStatusValue;
	UNICODE_STRING          StaticUnicodeString;
	WCHAR                   StaticUnicodeBuffer[0x105];
	PVOID                   DeallocationStack;
	PVOID                   TlsSlots[0x40];
	LIST_ENTRY              TlsLinks;
	PVOID                   Vdm;
	PVOID                   ReservedForNtRpc;
	PVOID                   DbgSsReserved[0x2];
	ULONG                   HardErrorDisabled;
	PVOID                   Instrumentation[0x10];
	PVOID                   WinSockData;
	ULONG                   GdiBatchCount;
	ULONG                   Spare2;
	ULONG                   Spare3;
	ULONG                   Spare4;
	PVOID                   ReservedForOle;
	ULONG                   WaitingOnLoaderLock;
	PVOID                   StackCommit;
	PVOID                   StackCommitMax;
	PVOID                   StackReserved;
} ___TEB, *__PTEB;

typedef LONG KPRIORITY;

typedef struct _THREAD_BASIC_INFORMATION 
{
	NTSTATUS ExitStatus;
	PVOID TebBaseAddress;
	CLIENT_ID ClientId;
	KAFFINITY AffinityMask;
	KPRIORITY Priority;
	KPRIORITY BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct tagTREAD_NAME
{
	DWORD dwType;
	LPCSTR lpName;
	DWORD dwThreadID;
	DWORD dwFlags;
} THREAD_NAME, PTHREAD_NAME;

using SETTHREADDESCRIPTION = HRESULT(WINAPI *)(HANDLE handle, PCWSTR name);
using GETTHREADDESCRIPTION = HRESULT(WINAPI *)(HANDLE handle, PWSTR* name);
typedef LONG(NTAPI *NTQUERYINFORMATIONTHREAD)(HANDLE, __THREAD_INFORMATION_CLASS, PVOID, ULONG, PULONG);
typedef LONG(NTAPI *NTREADVIRTUALMEMORY)(HANDLE, PVOID, PVOID, ULONG, PULONG);
typedef void ThreadFunc(void *);

typedef struct tagTHREAD_ENTRY
{
	LPVOID pArgs;
	ThreadFunc* pFunction;
	LPCWSTR lpThreadName;
	__TEB* funcTeb;
} THREAD_ENTRY, PTHREAD_ENTRY;

typedef struct tagTHREAD_INFO
{
	DWORD cbSize;
	DWORD dwThreadId;
	DWORD dwStackSize;
	HANDLE hThread;
	LPWSTR lpThreadName;
} THREAD_INFO, *PTHREAD_INFO;

DLL_API VOID WINAPIV UserThreadProc(LPVOID pArgs);

class DLL_API ThreadSystem
{
public:
	ThreadSystem() { dwNumberOfThreads = NULL; }

	DWORD GetThreadsNumber() { return dwNumberOfThreads; }
	DWORD CreateUserThread(PTHREAD_INFO pThreadInfo, ThreadFunc* pFunction, LPVOID pArgs, LPCWSTR pName);
	VOID GetUserThreadName(DWORD dwThreadId, LPWSTR* lpName);
	VOID SetUserThreadName(DWORD dwThreadId, LPWSTR lpName);
	VOID GetThreadInformation(PTHREAD_INFO pThreadInfo, DWORD dwThreadId);

private:
	DWORD dwNumberOfThreads;
};

extern DLL_API ThreadSystem threadSystem;
