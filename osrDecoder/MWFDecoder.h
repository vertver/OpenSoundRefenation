/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* MFWDecoder.cpp
* WMF decoder implementation
*********************************************************/
#pragma once
#include <mfapi.h>
#include <mfidl.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include "OSR.h"
#include "KernCPU.h"

class MWFReader
{
public:
	DLL_API MWFReader() { FAILEDX1(MFStartup(MF_VERSION)); MFWInit(); }
	DLL_API ~MWFReader() 
	{
		FAILEDX1(MFShutdown());
		_RELEASE(pAttribute);
	}

	DLL_API VOID MFWInit();
	DLL_API BOOL IsSupportedByMWF(LPCWSTR lpPath, WAVEFORMATEX** waveFormat);
	DLL_API VOID LoadFileToMediaBuffer(std::vector<BYTE>& lpData, WAVEFORMATEX** waveFormat);
	DLL_API VOID WriteFileFromMediaBufferEx(IMFSourceReader* pSourceReader, HANDLE hFile, std::vector<BYTE>& pData, BYTE** pSecondData, DWORD dwDataSize);

	IMFAttributes* pAttribute;
	IMFSourceReader* pSourceReader;
	IMFMediaType* pMediaType;
	IMFMediaType* pSecondMediaType;

	UINT64 uDuration;
	UINT64 uSamplesLength;
};
