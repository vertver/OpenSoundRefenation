/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* ThreadSystem.cpp
* thread system implementation
*********************************************************/
#include "stdafx.h"
#include <process.h>
#include "versionhelpers.h"
#pragma hdrstop

DLL_API ThreadSystem threadSystem;

VOID
WINAPIV
UserThreadProc(
	LPVOID pArgs
)
{
	// get info from struct
	THREAD_ENTRY* threadEntry = reinterpret_cast<THREAD_ENTRY*>(pArgs);
	LPWSTR lpNewThreadName = const_cast<LPWSTR>(threadEntry->lpThreadName);
	LPVOID pArglist = reinterpret_cast<LPVOID>(threadEntry->pArgs);
	ThreadFunc* threadFunc = threadEntry->pFunction;

	threadSystem.SetUserThreadName(GetCurrentThreadId(), lpNewThreadName);

	// cast _TEB to custom _TEB type
	_TEB* teb = NtCurrentTeb();
	threadEntry->funcTeb = (__TEB*)teb;
	
	threadFunc(pArglist);
}

static THREAD_ENTRY threadStruct[4096] = { NULL };

DWORD
ThreadSystem::CreateUserThread(
	PTHREAD_INFO pThreadInfo,
	ThreadFunc* pFunction,
	LPVOID pArgs,
	LPCWSTR pName
)
{
	ASSERT2(pFunction, L"No function pointer at CreateUserThread()");
	ASSERT2(pArgs, L"No thread args at CreateUserThread()");

	UINT_PTR hThread = NULL;
	DWORD dwThreadId = NULL;
	LPWSTR lpThreadName = const_cast<LPWSTR>(pName);

	threadStruct[dwNumberOfThreads].lpThreadName = pName;
	threadStruct[dwNumberOfThreads].pArgs = pArgs;
	threadStruct[dwNumberOfThreads].pFunction = pFunction;

	if (pThreadInfo)
	{
		hThread = _beginthread(UserThreadProc, pThreadInfo->dwStackSize, &threadStruct[dwNumberOfThreads]);
		pThreadInfo->hThread = reinterpret_cast<HANDLE>(hThread);
		pThreadInfo->dwThreadId = GetThreadId(pThreadInfo->hThread);
		pThreadInfo->lpThreadName = lpThreadName;
		pThreadInfo->cbSize = sizeof(THREAD_INFO);
		dwThreadId = pThreadInfo->dwThreadId;
	}
	else
	{
		hThread = _beginthread(UserThreadProc, NULL, &threadStruct[dwNumberOfThreads]);
		dwThreadId = GetThreadId(reinterpret_cast<HANDLE>(hThread));
	}

	dwNumberOfThreads++;
	return dwThreadId;
}

VOID
ThreadSystem::SetUserThreadName(
	DWORD dwThreadId,
	LPWSTR lpName
)
{
	ASSERT2(dwThreadId, L"No thread id at SetUserThreadName");
	ASSERT2(lpName, L"No thread name at SetUserThreadName");

	// only from Windows 10.0.1607
	SETTHREADDESCRIPTION pThreadDesc = (SETTHREADDESCRIPTION)GetProcAddress(GetModuleHandleW(L"kernel32"), "SetThreadDescription");

	if (pThreadDesc)
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
		ASSERT2(SUCCEEDED(pThreadDesc(hThread, lpName)), L"Can't set thread name");
	}
	else
	{
		THREAD_NAME threadName = { NULL };
		threadName.dwType = 0x1000; 
		threadName.lpName = WCSTRToMBCSTR(lpName);
		threadName.dwThreadID = dwThreadId;
		threadName.dwFlags = NULL;

		__try { RaiseException(0x406D1388, 0, sizeof(threadName) / sizeof(ULONG_PTR), (ULONG_PTR*)&threadName); }
		__except (EXCEPTION_CONTINUE_EXECUTION) { }

		UnloadFile((LPVOID)threadName.lpName);
	}
}

VOID
ThreadSystem::GetUserThreadName(
	DWORD dwThreadId,
	LPWSTR* lpName
)
{
	ASSERT2(dwThreadId, L"No thread id at GetUserThreadName");

	// only from Windows 10.0.1607
	GETTHREADDESCRIPTION pThreadDesc = (GETTHREADDESCRIPTION)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetThreadDescription");

	if (pThreadDesc)
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
		ASSERT2(SUCCEEDED(pThreadDesc(hThread, lpName)), L"Can't get thread name");
	}
}

VOID
ThreadSystem::GetThreadInformation(
	PTHREAD_INFO pThreadInfo,
	DWORD dwThreadId
)
{
	ASSERT2(dwThreadId, L"No thread id at GetThreadInformation");

	// get handle to thread 
	__THREAD_INFORMATION_CLASS threadInfo = {};
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
	THREAD_BASIC_INFORMATION basicInfo = { NULL };
	NT_TIB tib = { NULL };

	// documented Native-API for check thread info
	NTQUERYINFORMATIONTHREAD pNtQueryInformationThread = (NTQUERYINFORMATIONTHREAD)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtQueryInformationThread");

	// this is undocumented Native-API function (ReadProcessMemory eq.). 
	NTREADVIRTUALMEMORY pNtReadVirtualMemory = (NTREADVIRTUALMEMORY)GetProcAddress(GetModuleHandleW(L"ntdll"), "NtReadVirtualMemory");
	
	// if functions are exists
	if (pNtQueryInformationThread && pNtReadVirtualMemory)
	{
		// read basic info to our struct
		pNtQueryInformationThread(hThread, ThreadBasicInformation, &basicInfo, sizeof(THREAD_BASIC_INFORMATION), NULL);

		// we don't get a struct with info, so we read virtual memory of our process
		pNtReadVirtualMemory(GetModuleHandleW(NULL), basicInfo.TebBaseAddress, &tib, sizeof(NT_TIB), NULL);
	}

	DWORD cbSize = sizeof(THREAD_INFO);

	//#NOTE: the pThreadInfo variable must exist to delete pointer
	if (!pThreadInfo) { pThreadInfo = (PTHREAD_INFO)FastAlloc(sizeof(THREAD_INFO)); }

	LPWSTR* lpName = NULL;
	GetUserThreadName(dwThreadId, lpName);

	pThreadInfo->cbSize = cbSize;
	pThreadInfo->dwStackSize = (DWORD)(*(DWORD64*)(tib.StackBase));	// in 64-bit system - DWORD64
	pThreadInfo->dwThreadId = dwThreadId;
	pThreadInfo->hThread = hThread;

	if (!lpName) { pThreadInfo->lpThreadName = NULL; }
	else { pThreadInfo->lpThreadName = *lpName; }
}
