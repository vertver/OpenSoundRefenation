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

std::wstring szPathTemp;

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

VOID
CreateTempDirectory()
{
	WSTRING_PATH szFullPath = { NULL };
	// get programm directory to create 'temp' directory
	ASSERT1(GetCurrentDirectoryW(sizeof(WSTRING_PATH), szFullPath), L"Can't get proccess directory");

	std::wstring szTempPath = szFullPath;
	szTempPath += L"\\Temp";

	szPathTemp = szTempPath;

	DWORD dwGetDir = GetFileAttributesW(szTempPath.c_str());
	if (dwGetDir == INVALID_FILE_ATTRIBUTES || !(dwGetDir & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (!CreateDirectoryW(szTempPath.c_str(), NULL))
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

LPCWSTR 
GetTempDirectory()
{
	return szPathTemp.c_str();
}
