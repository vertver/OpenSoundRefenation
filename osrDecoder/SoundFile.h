/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* SoundFile.h
* libsndfile decoder implementation
*********************************************************/
#pragma once
#define ENABLE_SNDFILE_WINDOWS_PROTOTYPES
#include "sndfile.hh"
#include "OSR.h"

#define BUFFER_SIZE 4096

class SndFileReader
{
public:
	DLL_API VOID OpenFileToSoundBuffer(LPCWSTR lpPath, VOID** lpOutData, LPDWORD dwSize, WAVEFORMATEX* pFormat);

private:
	SF_INFO fileInfo;
	SNDFILE* sndFile;
};
