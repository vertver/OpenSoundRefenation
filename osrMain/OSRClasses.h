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

		void CreateMixer(HWND hwnd);
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

#include <Shobjidl.h>

class DLL_API TaskbarValue
{
#ifndef _RELEASE1
#define _RELEASE1(X) if (X) { X->Release(); X = NULL; }
#endif
public:
	TaskbarValue(HWND hwnd)
	{
		if (FAILED(CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pTaskbar))))
		{
			// set here your error message
		}

		currentHwnd = hwnd;

		pTaskbar->SetProgressState(currentHwnd, TBPF_NORMAL);
		pTaskbar->SetProgressValue(currentHwnd, 0, 100);
	}

	VOID SetValue(DWORD Value)
	{
		if (Value == 100)
		{
			pTaskbar->SetProgressState(currentHwnd, TBPF_NOPROGRESS);
		}

		pTaskbar->SetProgressValue(currentHwnd, Value, 100);
	}

	VOID SetError()
	{
		pTaskbar->SetProgressState(currentHwnd, TBPF_ERROR);
	}

	VOID SetPause()
	{
		pTaskbar->SetProgressState(currentHwnd, TBPF_PAUSED);
	}

	VOID SetCompleted()
	{
		pTaskbar->SetProgressState(currentHwnd, TBPF_NOPROGRESS);
	}

	~TaskbarValue()
	{
		_RELEASE1(pTaskbar);
	}

private:
	HWND currentHwnd;
	ITaskbarList3* pTaskbar;
#undef _RELEASE1
};
