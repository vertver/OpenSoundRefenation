/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: DirectSound Kernel Part
**********************************************************
* DirectSoundMixer.cpp
* DirectSound mixer implementation
**********************************************************/
#include "stdafx.h"
#pragma hdrstop

OSRCODE
DMixer::InitMixer(
	HWND hwnd,
	WAVEFORMATEX waveFormat,
	LPVOID pData,
	DWORD dwDataSize
)
{
	OSRFAIL2(engine.CreateDirectEngine(hwnd, &waveFormat), L"Can't create DirectSound engine");
	OSRFAIL2(engine.LoadSoundBuffer(pData, dwDataSize, &waveFormat), L"Can't load sound buffer");

	return OSR_SUCCESS;
}

OSRCODE
DMixer::PlaySoundData()
{
	if (!engine.pSupportBuffer) { return MXR_OSR_BUFFER_CORRUPT; }

	if (FAILED(engine.pSupportBuffer->SetCurrentPosition(NULL))) { return MXR_OSR_BUFFER_CORRUPT; }
	if (FAILED(engine.pSupportBuffer->SetVolume(NULL))) { return MXR_OSR_BUFFER_CORRUPT; }
	if (FAILED(engine.pSupportBuffer->Play(NULL, NULL, NULL))) { return MXR_OSR_NO_OUT; }

	return OSR_SUCCESS;
}

OSRCODE
DMixer::StopSoundData()
{
	if (!engine.pSupportBuffer) { return MXR_OSR_BUFFER_CORRUPT; }

	if (FAILED(engine.pSupportBuffer->Stop())) { return MXR_OSR_NO_OUT; }

	return OSR_SUCCESS;
}