/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* win32kernel.cpp
* WIN32 kernel implementation
*********************************************************/
#include "stdafx.h"
#include <DbgHelp.h>

DLL_API UnhandledExceptionFilterType *previous_filter = nullptr;

BOOL
IsAdminUser()
{
	BOOL isAdmin = FALSE;
	DWORD dwTokenSize = NULL;
	DWORD dwSecondSize = NULL;
	HANDLE hProcessToken = NULL;
	HANDLE hSecondToken = NULL;
	BYTE adminSID[SECURITY_MAX_SID_SIZE];
	TOKEN_ELEVATION_TYPE tokenValue = {};
	ZeroMemory(&tokenValue, sizeof(TOKEN_ELEVATION_TYPE));

	ASSERT1(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hProcessToken), L"Can't open process token");
	ASSERT1(GetTokenInformation(hProcessToken, TokenElevationType, &tokenValue, sizeof(TOKEN_ELEVATION_TYPE), &dwTokenSize), L"Can't get token info");

	// if is our token is limited
	if (tokenValue == TokenElevationTypeLimited)
	{
		ASSERT1(GetTokenInformation(hProcessToken, TokenLinkedToken, &hSecondToken, sizeof(HANDLE), &dwTokenSize), L"Can't get token info");
	}

	// if second token is empty - duplicate it from process token
	if (!hSecondToken)
	{
		ASSERT1(DuplicateToken(hProcessToken, SecurityIdentification, &hSecondToken), L"Can't duplicate token from process token");
	}

	dwSecondSize = sizeof(adminSID);

	// check user token
	ASSERT1(CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSecondSize), L"Can't get SID");
	ASSERT1(CheckTokenMembership(hSecondToken, &adminSID, &isAdmin), L"Can't check membership token");

	// clean handles
	CloseHandle(hProcessToken);
	CloseHandle(hSecondToken);
	hProcessToken = NULL;
	hSecondToken = NULL;

	return isAdmin;
}

BOOL
IsProcessWithAdminPrivilege()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	LPVOID pAdministratorsGroup = NULL;
	BOOL bRet = FALSE;

	// init SID to control privileges
	ASSERT1(
		AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup),
		L"Can't init SID"
	);

	// ckeck membership
	ASSERT1(CheckTokenMembership(NULL, pAdministratorsGroup, &bRet), L"Can't check membership token");

	// clean pointer
	if (pAdministratorsGroup) { FreeSid(pAdministratorsGroup); pAdministratorsGroup = nullptr; }

	return bRet;
}

VOID
RunWithAdminPrivilege()
{
	// check for admin user
	if (IsAdminUser())
	{
		// if current user is admin - go to run this application
		if (!IsProcessWithAdminPrivilege())
		{
			WSTRING_PATH szPath = { NULL };
			ASSERT1(GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath)), L"Can't get file module");
			
			SHELLEXECUTEINFOW shellInfo = { sizeof(SHELLEXECUTEINFOW) };
			shellInfo.lpVerb = L"runas";
			shellInfo.lpFile = szPath;
			shellInfo.hwnd = NULL;
			shellInfo.nShow = SW_NORMAL;

			ShellExecuteExW(&shellInfo);
			ExitProcess(GetCurrentProcessId());
		}
	}
	else
	{
		MessageBoxW(NULL, L"Current user can't set 'Administrator' role for this application", L"Warning", MB_ICONWARNING | MB_OK);
	}
}

std::wstring szPathTemp;

LPCWSTR
GetTempDirectory()
{
	return szPathTemp.c_str();
}

VOID
CreateTempDirectory()
{
	WSTRING_PATH szFullPath = { NULL };
	// get programm directory to create 'temp' directory
	ASSERT1(GetCurrentDirectoryW(sizeof(WSTRING_PATH), szFullPath), L"Can't get proccess directory");

	szPathTemp = szFullPath;
	szPathTemp += L"\\Temp";

	DWORD dwGetDir = GetFileAttributesW(szPathTemp.c_str());
	if (dwGetDir == INVALID_FILE_ATTRIBUTES || !(dwGetDir & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (!CreateDirectoryW(szPathTemp.c_str(), NULL))
		{
			DWORD dwError = GetLastError();
			if (!IsProcessWithAdminPrivilege() && dwError == ERROR_ACCESS_DENIED)
			{
				if (THROW4(L"Can't create temp directory because access denied. Re-run application with 'Administrator' privilege?")) 
				{ 
					RunWithAdminPrivilege(); 
				}
			}
			else
			{
				THROW1(L"Can't create temp directory");
			}
		}
	}
}

VOID
GetTimeString(
	LPCWSTR lpString
)
{
	if (!lpString) { return; }

	SYSTEMTIME sysTime = { NULL };
	GetSystemTime(&sysTime);
	
	std::wstring szTime = std::to_wstring(sysTime.wYear) + std::to_wstring(sysTime.wMonth) + std::to_wstring(sysTime.wDay)
		+ std::to_wstring(sysTime.wHour) + std::to_wstring(sysTime.wMinute);

	lpString = szTime.c_str();
}

LONG 
CreateMinidump(
	_EXCEPTION_POINTERS* pExceptionInfo
)
{
	std::wstring szFullPath = { NULL };
	std::wstring szTemp = { NULL };
	WSTRING_PATH szPath = { NULL };

	// get current working directory
	GetCurrentDirectoryW(sizeof(WSTRING_PATH), szPath);
	szTemp = szPath;
	szTemp += L"\\Dump";

	// create new path "dump"
	DWORD dwGetDir = GetFileAttributesW(szTemp.c_str());
	if (dwGetDir == INVALID_FILE_ATTRIBUTES || !(dwGetDir & FILE_ATTRIBUTE_DIRECTORY))
	{
		// we can't create temp directory at kernel paths or "Program files"
		if (!CreateDirectoryW(szTemp.c_str(), NULL))
		{
			DWORD dwError = GetLastError();
			THROW1(L"Can't create temp directory. Please, change working directory");
		}
	}

	WSTRING128 szName = { NULL };
	DWORD dwNameSize = sizeof(WSTRING128);
	GetUserNameW(szName, &dwNameSize);

	WSTRING128 szTime = { NULL };
	GetTimeString(szTime);

	// create minidump file handle
	szFullPath = szTemp + L"\\" + L"OpenSoundRefenation_" + szName + L"_" + szTime + L".mdmp";
	HANDLE hFile = CreateFileW(szFullPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		szFullPath = szPath + std::wstring(L"\\") + L"OpenSoundRefenation_" + szName + L"_" + szTime + L".mdmp";
		CreateFileW(szFullPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo = { NULL };
		MINIDUMP_TYPE dump_flags = MINIDUMP_TYPE(MiniDumpNormal | MiniDumpFilterMemory | MiniDumpScanMemory);

		ExInfo.ThreadId = GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;

		// write the dump
		if (MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, dump_flags, &ExInfo, NULL, NULL))
		{
			szTemp = L"Minidump saved at (" + szFullPath + L")";
			MessageBoxW(NULL, szTemp.c_str(), L"Minidump saved", MB_OK | MB_ICONINFORMATION);
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

LONG 
WINAPI
UnhandledFilter(
	struct _EXCEPTION_POINTERS* pExceptionInfo
)
{
	return CreateMinidump(pExceptionInfo);
}
