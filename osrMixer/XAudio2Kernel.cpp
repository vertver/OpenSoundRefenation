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
DLL_API XEngine XAudioEngine;

VOID
XEngine::CreateXEngine()
{
	FAILEDX2(XAudio2Create(&pXAudio));
	FAILEDX2(pXAudio->CreateMasteringVoice(&pMasteringVoice));
}

VOID
XEngine::CreateSourceBuffer()
{
	FAILEDX2(pXAudio->CreateSourceVoice(&sList.pSourceVoice, sList.waveFormat));
	HRESULT hr = sList.pSourceVoice->SubmitSourceBuffer(&sList.audioBuffer);
	FAILEDX2(hr);
}
