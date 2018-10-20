/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR loop system
**********************************************************
* LoopList.cpp
* loop system kernel implementation
*********************************************************/
#include "stdafx.h"
#include <vector>
#pragma hdrstop

DLL_API AVReader ffReader;
DLL_API WMFReader mfReader;

LoopList::LoopList() : 
	dwMaxCountOfFiles(NULL),
	dwCurrentCountOfFiles(NULL),
	lpFileInfo(NULL)
{
	dwMaxCountOfFiles = 100;
	lpFileInfo = (LPAUDIO_INFO)AdvanceAlloc(dwMaxCountOfFiles * sizeof(AUDIO_INFO), VIRTUAL_MEMORY_ALLOC);
	ASSERT2(lpFileInfo, L"Can't alloc struct");
}

LoopList::LoopList(
	DWORD dwCountOfFiles
) :
	dwCurrentCountOfFiles(NULL),
	lpFileInfo(NULL)
{
	dwMaxCountOfFiles = dwCountOfFiles;
	lpFileInfo = (LPAUDIO_INFO)AdvanceAlloc(dwMaxCountOfFiles * sizeof(AUDIO_INFO), VIRTUAL_MEMORY_ALLOC);
	ASSERT2(lpFileInfo, L"Can't alloc struct");
}

LoopList::~LoopList()
{
	if (lpFileInfo)
	{
		FreePointer(lpFileInfo, dwMaxCountOfFiles * sizeof(AUDIO_INFO), VIRTUAL_MEMORY_ALLOC);
	}
}

VOID
LoopList::LoadAudioFile(
	LPCWSTR lpFilePath,
	DWORD dwDecoderType,
	DWORD dwFormat,				// only for FFmpeg
	LPDWORD pSampleNumber
)
{	
	std::vector<BYTE> audioData;	
	WAVEFORMATEX* waveFormat = nullptr;
	WSTRING_PATH szString = { NULL };
	DWORD dwHeaderSize = NULL;

	lpFileInfo[dwCurrentCountOfFiles].dwSampleCount = NULL;
	lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo = (LPLOOP_INFO)AdvanceAlloc(sizeof(LOOP_INFO), VIRTUAL_MEMORY_ALLOC);

	lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->cbSize = sizeof(LOOP_INFO);
	lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleNumber = NULL;

	switch (dwDecoderType)
	{
	case NULL:
	case USE_WMF:
		if (mfReader.IsSupportedByMWF(lpFilePath, &waveFormat))
		{
			// load file to vector and copy to local buffer
			mfReader.LoadFileToMediaBuffer(audioData, &waveFormat);
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample = (LPBYTE)AdvanceAlloc(audioData.size() + 1, VIRTUAL_MEMORY_ALLOC);
			memcpy(lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample, &audioData[0], audioData.size());

			// set data to structs
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize = (DWORD)(audioData.size() + 1);
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleDuration = mfReader.uDuration;
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleNumber = dwCurrentCountOfFiles;
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat = *waveFormat;
			lpFileInfo[dwCurrentCountOfFiles].dwFileSize = (DWORD)(audioData.size());
			lpFileInfo[dwCurrentCountOfFiles].dwSampleCount = NULL;

			// free COM-pointer
			CoTaskMemFree(waveFormat);
			break;
		}
	case USE_LIBSNDFILE:
	default:
		ReadAudioFileEx(
			lpFilePath,
			(VOID**)&lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample,
			(LONGLONG*)&lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize,
			&dwHeaderSize
		);

		OSRCODE sCode = GetWaveFormatExtented(
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample,
			lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize,
			&lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat
		);

		if (OSRFAILED(sCode))
		{  
			FreePointer(lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample, NULL, HEAP_MEMORY_ALLOC);
			return;
		}

		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleDuration = 
			(lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize - dwHeaderSize) / 
			(waveFormat->nSamplesPerSec + waveFormat->wBitsPerSample + waveFormat->nChannels);	
		
		break;
	case USE_FFMPEG:
		// open file		
		ffReader.OpenFileToBuffer(lpFilePath, (LPCWSTR*)&szString, &lpFileInfo[dwCurrentCountOfFiles].dwFileSize, dwFormat);

		// read big sample
		ReadAudioFileEx(
			szString,
			(VOID**)&lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->lpSample,
			(LONGLONG*)&lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize,
			&dwHeaderSize
		);

		//#TODO: dynamic WAVEFORMATEX
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.cbSize = sizeof(WAVEFORMATEX);
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.nAvgBytesPerSec = ((44100 * 32 * 2) / 8);
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.nBlockAlign = 4;
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.nChannels = 2;
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.nSamplesPerSec = 44100;
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.wBitsPerSample = 32;
		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->waveFormat.wFormatTag = 3;

		lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleDuration =
			(lpFileInfo[dwCurrentCountOfFiles].lpSampleInfo->dwSampleSize - dwHeaderSize) /
			(waveFormat->nSamplesPerSec + waveFormat->wBitsPerSample + waveFormat->nChannels);

		break;
	}

	dwCurrentCountOfFiles++;
	*pSampleNumber = dwCurrentCountOfFiles;
}
