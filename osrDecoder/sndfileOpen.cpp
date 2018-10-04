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
	BYTE** lpPCMData,
	LPDWORD dwSize
)
{
	LPCSTR lpNewPath = WCSTRToMBCSTR(lpPath);
	CHAR buf[BUFFER_SIZE] = { NULL };

	sndFile = sf_open(lpNewPath, SFM_READ, &fileInfo);
	if (!sndFile) { DEBUG_BREAK; }

	UnloadFile((LPVOID)lpPath);
}
