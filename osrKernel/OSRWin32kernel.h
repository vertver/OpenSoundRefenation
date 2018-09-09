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

DLL_API BOOL IsProcessWithAdminPrivilege();
DLL_API BOOL IsAdminUser();
DLL_API VOID RunWithAdminPrivilege();
DLL_API VOID CreateTempDirectory();
DLL_API LPCWSTR GetTempDirectory();
