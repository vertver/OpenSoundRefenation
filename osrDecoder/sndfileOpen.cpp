/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* sndfileOpen.cpp
* libsndfile decoder implementation
*********************************************************/
#include "stdafx.h"

VOID
SndFileReader::OpenFileToSoundBuffer(
	LPCWSTR lpPath,
	VOID** lpOutData,
	LPDWORD dwSize,
	WAVEFORMATEX* pFormat
)
{
	sndFile = sf_wchar_open(lpPath, SFM_READ, &fileInfo);
	
	if (!sndFile)
	{
		ANSI_LOG(sf_strerror(0));
		return;
	}

	pFormat->nChannels = fileInfo.channels;
	pFormat->nSamplesPerSec = fileInfo.samplerate;
	pFormat->cbSize = sizeof(WAVEFORMATEX);
	pFormat->nAvgBytesPerSec = ((pFormat->nSamplesPerSec * 32 * pFormat->nChannels) / 8);	
	pFormat->wBitsPerSample = 32;
	pFormat->nBlockAlign = (pFormat->wBitsPerSample * pFormat->nChannels) / 8;
	pFormat->wFormatTag = 3;

	*dwSize = fileInfo.frames * fileInfo.channels * sizeof(f32);

	*lpOutData = FastAlloc(fileInfo.frames * fileInfo.channels * sizeof(f32));
	sf_readf_float(sndFile, (f32*)*lpOutData, fileInfo.frames);

	sf_close(sndFile);
}