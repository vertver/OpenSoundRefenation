/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* XAudioMain.h
* XAudio2 decoder implementation
*********************************************************/
#pragma once
#include "stdafx.h"

#define SAMPLE_RATE 44100

class SoundList : public MWFReader
{
public:
	DLL_API OSRCODE LoadSoundFile(LPCWSTR lpPath, DWORD dwFlags);
	DLL_API OSRCODE WriteWaveFile(LPCWSTR lpFullpath, std::vector<BYTE> pFile, DWORD dwFlags);

	IXAudio2SourceVoice* pSourceVoice;
	std::vector<BYTE> audioData;
	XAUDIO2_BUFFER audioBuffer;
	WAVEFORMATEX* waveFormat;
	WAVEFORMATEX wavFile;
	DWORD dwDataSize;
	DWORD dwIndex;
	BYTE* lpData;
};

// AuEngine 2.1
class XEngine : public SoundList
{
public:	
	DLL_API XEngine() {};
	DLL_API ~XEngine() {};
	DLL_API VOID CreateXEngine();
	DLL_API VOID CreateSourceBuffer();

	IXAudio2MasteringVoice* pMasteringVoice;
	IXAudio2* pXAudio;
	SoundList sList;
};

class XMixer
{
public:
	DLL_API VOID InitMixer(XEngine engine);
	DLL_API VOID PlaySimpleWave();

private:
	// interfaces
	IXAudio2* pXAudio;
	IXAudio2SubmixVoice* pSubmixVoice;

	// audio engine and source
	XEngine audioEngine;
};
