/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: DirectSound Kernel Part
**********************************************************
* DirectSoundKernel.cpp
* DirectSound system implementation
**********************************************************/
#include "stdafx.h"
#pragma hdrstop

DEngine::DEngine() : 
	pDirectSound(nullptr), 
	pDirectBuffer(nullptr), 
	pSupportBuffer(nullptr),
	pBuffer(nullptr)
{

}

OSRCODE
DEngine::CreateDirectEngine(
	HWND hwnd, 
	WAVEFORMATEX* waveFormat
)
{
	ASSERT2(waveFormat, L"No wave format");
	ASSERT2(hwnd, L"No HWND to set cooperative level");

	DSBUFFERDESC bufferDesc = { NULL };

	// create default direct sound class
	FAILEDX2(DirectSoundCreate(nullptr, &pDirectSound, nullptr));
	if (!pDirectSound) { return MXR_OSR_NO_OUT; }

	// we need to get exclusive mode for HWND 
	// because if the window was minimized, the
	// sound content will stop playing
	if (FAILED(pDirectSound->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE)))
	{
		// if exclusive mode is not supported - get to priority mode
		if (FAILED(pDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
		{
			return MXR_OSR_NO_OUT;
		}
	}

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	waveFormat->cbSize = NULL;	// DirectSound need to set struct size to 0

	// create primary buffer
	if (FAILED(pDirectSound->CreateSoundBuffer(&bufferDesc, &pDirectBuffer, nullptr))) { return MXR_OSR_BUFFER_CORRUPT; }

	// set waveformat to primary buffer
	if (FAILED(pDirectBuffer->SetFormat(waveFormat)))
	{
		// if our format is empty or 
		if (waveFormat->wFormatTag == 1 || !waveFormat->wFormatTag) { return MXR_OSR_BAD_WFX; }
		return MXR_OSR_UNSUPPORTED_FMT;
	}

	return OSR_SUCCESS;
}

OSRCODE
DEngine::LoadSoundBuffer(
	LPVOID pData, 
	DWORD dwSize,
	WAVEFORMATEX* waveFormat
)
{
	ASSERT2(pData, L"No data pointer");
	ASSERT2(dwSize, L"No data size");
	ASSERT2(waveFormat, L"No wave format");

	//#NOTE: Delete data pointer after this function
	DSBUFFERDESC bufferDesc = { NULL };
	DWORD dwBufSize = NULL;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY;
	bufferDesc.dwBufferBytes = dwSize;
	bufferDesc.lpwfxFormat = waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// create secondary buffer for output buffer
	if (FAILED(pDirectSound->CreateSoundBuffer(&bufferDesc, &pSupportBuffer, NULL))) { return MXR_OSR_BUFFER_CORRUPT; }

	// lock buffer to copy data to DirectSound buffer
	if (FAILED(pSupportBuffer->Lock(NULL, dwSize, &pBuffer, &dwBufSize, nullptr, nullptr, NULL))) { return MXR_OSR_BUFFER_CORRUPT; }

	// copy data
	ASSERT2((dwSize <= dwBufSize), L"DirectSound buffer smaller then data buffer");
	memcpy_s(pBuffer, dwBufSize, pData, dwSize);

	// unlock buffer to use this
	if (FAILED(pSupportBuffer->Unlock(pBuffer, dwBufSize, nullptr, NULL))) { return MXR_OSR_BUFFER_CORRUPT; }

	return OSR_SUCCESS;
}
