/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSRWin32kernel.h
* WIN32 kernel implementation
*********************************************************/
#pragma once
#include "stdafx.h"

#ifdef WIN32
DLL_API BOOL IsProcessWithAdminPrivilege();
DLL_API BOOL IsAdminUser();
DLL_API VOID RunWithAdminPrivilege();
DLL_API VOID CreateTempDirectory();
DLL_API LPCWSTR GetTempDirectory();
DLL_API VOID GetTimeString(LPCWSTR lpString);
DLL_API LONG CreateMinidump(_EXCEPTION_POINTERS* pExceptionInfo);
DLL_API LONG WINAPI UnhandledFilter(struct _EXCEPTION_POINTERS* pExceptionInfo);
DLL_API VOID WINAPI GetCurrentPeb(VOID** pPeb);
#endif