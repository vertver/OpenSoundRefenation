/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR DirectX
**********************************************************
* osrUI.h
* Main User Interface implementation
*********************************************************/
#pragma once
#include "stdafx.h"

class DLL_API UserInterface
{
public:
	HWND GetCurrentHwnd() { return MainHwnd; }
	OSRCODE CreateMainWindow();

private:
	HWND MainHwnd;
};
