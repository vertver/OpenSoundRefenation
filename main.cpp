/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* main.cpp
* entry-point
*********************************************************/
#include <windows.h>
#include "OSRClasses.h"

BOOL
WINAPI
WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	OSR::UserInterface Userinterface;

	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nShowCmd);

	if (strstr(lpCmdLine, "-admin") || strstr(lpCmdLine, "-a") || strstr(lpCmdLine, "-adm")) { RunWithAdminPrivilege(); }

	// critical section
	InitApplication();
	Userinterface.CreateMainWindow();
}
