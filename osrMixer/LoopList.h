/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR loop system
**********************************************************
* LoopList.h
* loop system kernel implementation
*********************************************************/
#pragma once
#include "stdafx.h"

typedef struct  
{
	DWORD cbSize;					// size of struct
	DWORD dwSampleSize;				// size of current sample
	DWORD dwSampleNumber;			// if 0 - random sample, else - numeric sample of audio file
	DWORD64 dwSampleDuration;		// duration of sample (in msecs)
	WAVEFORMATEX waveFormat;		// sample format info
	LPBYTE lpSample;				// pointer to data of sample
} LOOP_INFO, *LPLOOP_INFO, *PLOOP_INFO;

typedef struct  
{
	DWORD dwSampleCount;			// count of all samples in 1 file
	DWORD dwFileSize;
	DWORD64 dwFileDuration;			// final file duration (include all samples duration)
	LPLOOP_INFO lpSampleInfo;		// to count size of this struct - sizeof(LOOP_INFO) * dwSampleCount
} AUDIO_INFO, *LPAUDIO_INFO, *PAUDIO_INFO;;

extern DLL_API AVReader ffReader;
extern DLL_API WMFReader mfReader;

class LoopList
{
public:
	LoopList();
	LoopList(DWORD dwCountOfFiles);
	~LoopList();

	DWORD GetFilesCount() { return dwCurrentCountOfFiles; }
	DWORD GetMaxFilesCount() { return dwMaxCountOfFiles; }

	VOID SetMaxFilesCount(DWORD dwFilesCount) { dwMaxCountOfFiles = dwFilesCount; }

	VOID LoadAudioFile(LPCWSTR lpFilePath, DWORD dwDecoderType, DWORD dwFormat, LPDWORD pSampleNumber);
private:
	DWORD dwCurrentCountOfFiles;
	DWORD dwMaxCountOfFiles;
	LPAUDIO_INFO lpFileInfo;
};