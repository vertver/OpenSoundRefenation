/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* XAudio2Kernel.cpp
* XAudio2 kernel implementation
*********************************************************/
#include "stdafx.h"

DLL_API HANDLE hPlay;

VOID
XEngine::CreateXEngine()
{
	FAILEDX2(XAudio2Create(&pXAudio));
	FAILEDX2(pXAudio->CreateMasteringVoice(&pMasteringVoice));
}

VOID
XEngine::CreateSourceBuffer()
{
	FAILEDX2(pXAudio->CreateSourceVoice(&pSourceVoice, waveFormat));
	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&audioBuffer);
	FAILEDX2(hr);
}
