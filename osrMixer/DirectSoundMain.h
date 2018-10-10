/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: DirectSound Kernel Part
**********************************************************
* DirectSoundMain.h
* DirectSound header
**********************************************************/
#pragma once
#include "stdafx.h"
#include <dsound.h>

class DEngine
{
public:
	DEngine();
	~DEngine() { _RELEASE(pDirectSound); _RELEASE(pDirectBuffer); _RELEASE(pSupportBuffer); }

	OSRCODE CreateDirectEngine(HWND hwnd, WAVEFORMATEX* waveFormat);
	OSRCODE LoadSoundBuffer(LPVOID pData, DWORD dwSize, WAVEFORMATEX* waveFormat);

	LPVOID pBuffer;
	LPDIRECTSOUND pDirectSound;
	LPDIRECTSOUNDBUFFER pDirectBuffer;
	LPDIRECTSOUNDBUFFER pSupportBuffer;
};

class DMixer
{
public:
	DMixer() {};
	DMixer(DEngine customEngine) { engine = customEngine; }
	~DMixer() {};

	OSRCODE InitMixer(HWND hwnd, WAVEFORMATEX waveFormat, LPVOID pData, DWORD dwDataSize);
	OSRCODE PlaySoundData();
	OSRCODE StopSoundData();
	
private:
	DEngine engine;
};