/*********************************************************
* Copyright (C) VERTVER, 2018. All rights reserved.
* OpenSoundRefenation - WINAPI open-source DAW
* MIT-License
**********************************************************
* Module Name: OSR entry-point
**********************************************************
* OSRClasses.h
* Main classes for engine
*********************************************************/
#pragma once
#include "OSR.h"
#include "OSRSystem.h"
#ifdef WIN32
#include "OSRWin32kernel.h"
#endif

#include "XAudioMain.h"
#include "LoopList.h"

namespace OSR
{
	class DLL_API Engine
	{
	public:
		void DecodeFile(LPCWSTR lpPath, LPLOOP_INFO pLoopInfo)
		{
			DWORD SampleNumber = 0;
			loopList.LoadAudioFile(lpPath, USE_WMF, 0, &SampleNumber);
			*pLoopInfo = *(loopList.GetLoopInfo()->pSampleInfo);
		}

	private:
		LoopList loopList;
	};

	class DLL_API Mixer
	{
	public:
		Mixer() { MixerSampleRate = 44100; MixerBufferSize = 2048; }
		Mixer(u32 SampleRate, u32 BufferSize) { MixerSampleRate = SampleRate; MixerBufferSize = BufferSize; }

		void CreateMixer(HWND hwnd, OSRSample Sample);
		void LoadSample(LPCWSTR lpPath);
		void PlaySample();

	private:
		DWORD LoopNumber; 
		OSR::Engine osrEngine;
		XMixer mixer;
		OSRSample CurrentSample;
		u32 MixerSampleRate;
		u32 MixerBufferSize;
	};

	class DLL_API UserInterface
	{
	public:
		HWND GetCurrentHwnd() { return MainHwnd; }
		OSRCODE CreateMainWindow();
		VOID CreateApplicationMenu();

	private:
		HWND MainHwnd;
	};
}; 
